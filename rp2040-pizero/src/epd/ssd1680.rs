use embassy_rp::gpio::{Output, Input};
use embassy_rp::spi::{Spi, Instance, Blocking};
use embassy_time::{Timer, Duration};
use embedded_hal::spi::SpiBus;
use core::result::Result::{self, Ok, Err};
use core::fmt::Debug;
use core::cmp::{PartialEq, Eq};
use core::clone::Clone;
use core::marker::Copy;

pub const EPD_WIDTH: u32 = 176;
pub const EPD_HEIGHT: u32 = 264;

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum EpdError {
    SpiError,
    BusyTimeout,
}

#[allow(async_fn_in_trait)]
pub trait AsyncEpd {
    async fn init(&mut self) -> Result<(), EpdError>;
    async fn init_4grey(&mut self) -> Result<(), EpdError>;
    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), EpdError>;
    async fn display_turbo(&mut self, buffer: &[u8]) -> Result<(), EpdError>;
    async fn display_4grey(&mut self, buffer_b1: &[u8], buffer_b0: &[u8]) -> Result<(), EpdError>;
}

/// 4-grey waveform LUT for the Waveshare 2.7" V2 (SSD1680-compatible).
pub const LUT_4GREY: [u8; 159] = [
    0x40,0x48,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x08,0x48,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x02,0x48,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x20,0x48,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x0A,0x19,0x00,0x03,0x08,0x00,0x00,
    0x14,0x01,0x00,0x14,0x01,0x00,0x03,
    0x0A,0x03,0x00,0x08,0x19,0x00,0x00,
    0x01,0x00,0x00,0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x22,0x22,0x22,0x22,0x22,0x22,0x00,0x00,0x00,
    0x22,0x17,0x41,0x00,0x32,0x10,
];

pub const LUT_TURBO: [u8; 159] = [
    0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x0F,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x22,0x22,0x22,0x22,0x22,0x22,0x00,0x00,0x00,
    0x22,0x17,0x41,0x00,0x32,0x10,
];

pub struct SSD1680<'d, T: Instance> {
    spi: Spi<'d, T, Blocking>,
    cs: Output<'d>,
    dc: Output<'d>,
    rst: Output<'d>,
    busy: Input<'d>,
    lut_initialized: bool,
}

impl<'d, T: Instance> SSD1680<'d, T> {
    pub fn new(spi: Spi<'d, T, Blocking>, cs: Output<'d>, dc: Output<'d>, rst: Output<'d>, busy: Input<'d>) -> Self {
        Self { spi, cs, dc, rst, busy, lut_initialized: false }
    }

    async fn wait_busy(&mut self) -> Result<(), EpdError> {
        let mut timeout = 0;
        while self.busy.is_high() {
            Timer::after(Duration::from_millis(1)).await;
            timeout += 1;
            if timeout > 5000 { return Err(EpdError::BusyTimeout); }
        }
        Timer::after(Duration::from_millis(2)).await;
        Ok(())
    }

    fn send_command(&mut self, command: u8) -> Result<(), EpdError> {
        self.dc.set_low();
        self.cs.set_low();
        let res = self.spi.write(&[command]).map_err(|_| EpdError::SpiError);
        self.cs.set_high();
        res
    }

    fn send_data(&mut self, data: &[u8]) -> Result<(), EpdError> {
        self.dc.set_high();
        self.cs.set_low();
        let res = self.spi.write(data).map_err(|_| EpdError::SpiError);
        self.cs.set_high();
        res
    }

    async fn reset(&mut self) {
        self.rst.set_high();
        let _ = Timer::after(Duration::from_millis(20)).await;
        self.rst.set_low();
        let _ = Timer::after(Duration::from_millis(2)).await;
        self.rst.set_high();
        let _ = Timer::after(Duration::from_millis(20)).await;
    }

    pub fn set_window(&mut self, x_start: u16, y_start: u16, x_end: u16, y_end: u16) -> Result<(), EpdError> {
        let xs = x_start / 8;
        let xe = x_end.div_ceil(8);
        let xe_reg = xe.saturating_sub(1);
        let ye_reg = y_end.saturating_sub(1);

        self.send_command(0x44)?;
        self.send_data(&[xs as u8, xe_reg as u8])?;
        self.send_command(0x45)?;
        self.send_data(&[y_start as u8, (y_start >> 8) as u8, ye_reg as u8, (ye_reg >> 8) as u8])?;
        self.send_command(0x4E)?;
        self.send_data(&[xs as u8])?;
        self.send_command(0x4F)?;
        self.send_data(&[y_start as u8, (y_start >> 8) as u8])?;
        Ok(())
    }
}

impl<'d, T: Instance> AsyncEpd for SSD1680<'d, T> {
    async fn init(&mut self) -> Result<(), EpdError> {
        self.reset().await;
        self.wait_busy().await?;
        self.send_command(0x12)?; // SWRESET
        self.wait_busy().await?;

        self.send_command(0x01)?; // Driver output control
        self.send_data(&[0x07, 0x01, 0x00])?;
        
        self.send_command(0x11)?; // Data entry mode
        self.send_data(&[0x03])?;
        
        self.send_command(0x44)?; // set Ram-X address start/end
        self.send_data(&[0x00, 0x15])?;
        self.send_command(0x45)?; // set Ram-Y address start/end
        self.send_data(&[0x00, 0x00, 0x07, 0x01])?;
        
        self.send_command(0x3C)?; // Border waveform
        self.send_data(&[0x01])?;
        
        self.lut_initialized = false;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), EpdError> {
        self.send_command(0x3C)?;
        self.send_data(&[0x01])?;
        self.send_command(0x4E)?;
        self.send_data(&[0x00])?;
        self.send_command(0x4F)?;
        self.send_data(&[0x00, 0x00])?;
        self.send_command(0x24)?;
        self.send_data(buffer)?;
        self.send_command(0x26)?;
        self.send_data(buffer)?;
        self.send_command(0x22)?;
        self.send_data(&[0xF7])?;
        self.send_command(0x20)?;
        self.wait_busy().await
    }

    async fn display_turbo(&mut self, buffer: &[u8]) -> Result<(), EpdError> {
        self.reset().await;
        self.send_command(0x3C)?;
        self.send_data(&[0x01])?;
        self.send_command(0x44)?;
        self.send_data(&[0x00, (EPD_WIDTH / 8 - 1) as u8])?;
        self.send_command(0x45)?;
        self.send_data(&[0x00, 0x00, ((EPD_HEIGHT - 1) & 0xFF) as u8, (((EPD_HEIGHT - 1) >> 8) & 0x01) as u8])?;
        self.send_command(0x4E)?;
        self.send_data(&[0x00])?;
        self.send_command(0x4F)?;
        self.send_data(&[0x00, 0x00])?;
        self.send_command(0x24)?;
        self.send_data(buffer)?;
        self.send_command(0x22)?;
        self.send_data(&[0xFF])?;
        self.send_command(0x20)?;
        self.wait_busy().await
    }

    async fn init_4grey(&mut self) -> Result<(), EpdError> {
        self.reset().await;
        self.wait_busy().await?;
        self.send_command(0x12)?;
        self.wait_busy().await?;

        self.send_command(0x01)?; 
        self.send_data(&[0x07, 0x01, 0x00])?;
        self.send_command(0x11)?; 
        self.send_data(&[0x03])?;
        self.send_command(0x44)?; 
        self.send_data(&[0x00, 0x15])?;
        self.send_command(0x45)?;
        self.send_data(&[0x00, 0x00, 0x07, 0x01])?;
        
        self.send_command(0x2C)?; 
        self.send_data(&[LUT_4GREY[158]])?;
        self.send_command(0x3F)?;
        self.send_data(&[LUT_4GREY[153]])?;
        self.send_command(0x03)?;
        self.send_data(&[LUT_4GREY[154]])?;
        self.send_command(0x04)?;
        self.send_data(&[LUT_4GREY[155], LUT_4GREY[156], LUT_4GREY[157]])?;
        self.send_command(0x32)?;
        self.send_data(&LUT_4GREY[..153])?;
        
        self.wait_busy().await?;
        self.lut_initialized = true;
        Ok(())
    }

    async fn display_4grey(&mut self, buffer_b1: &[u8], buffer_b0: &[u8]) -> Result<(), EpdError> {
        self.send_command(0x3C)?;
        self.send_data(&[0x01])?;
        self.send_command(0x4E)?;
        self.send_data(&[0x00])?;
        self.send_command(0x4F)?;
        self.send_data(&[0x00, 0x00])?;
        self.send_command(0x24)?;
        self.send_data(buffer_b1)?;
        self.send_command(0x26)?;
        self.send_data(buffer_b0)?;
        self.send_command(0x22)?;
        self.send_data(&[0xC7])?;
        self.send_command(0x20)?;
        self.wait_busy().await
    }
}

impl<'d, T: Instance> SSD1680<'d, T> {
    pub async fn init_turbo(&mut self) -> Result<(), EpdError> {
        self.reset().await;
        self.wait_busy().await?;
        self.send_command(0x12)?;
        self.wait_busy().await?;

        self.send_command(0x01)?; 
        self.send_data(&[0x07, 0x01, 0x00])?;
        self.send_command(0x11)?; 
        self.send_data(&[0x03])?;
        self.send_command(0x44)?; 
        self.send_data(&[0x00, 0x15])?;
        self.send_command(0x45)?;
        self.send_data(&[0x00, 0x00, 0x07, 0x01])?;

        self.send_command(0x2C)?; 
        self.send_data(&[LUT_TURBO[158]])?;
        self.send_command(0x3F)?;
        self.send_data(&[LUT_TURBO[153]])?;
        self.send_command(0x03)?;
        self.send_data(&[LUT_TURBO[154]])?;
        self.send_command(0x04)?;
        self.send_data(&[LUT_TURBO[155], LUT_TURBO[156], LUT_TURBO[157]])?;
        self.send_command(0x32)?;
        self.send_data(&LUT_TURBO[..153])?;
        
        self.wait_busy().await?;
        self.lut_initialized = true;
        Ok(())
    }

    pub async fn sleep(&mut self) -> Result<(), EpdError> {
        self.send_command(0x10)?;
        self.send_data(&[0x01])?;
        let _ = Timer::after(Duration::from_millis(100)).await;
        Ok(())
    }

    pub async fn clear(&mut self) -> Result<(), EpdError> {
        let size = (EPD_WIDTH * EPD_HEIGHT / 8) as usize;
        self.send_command(0x24)?;
        let chunk = [0xFFu8; 128];
        for _ in 0..(size / 128) { self.send_data(&chunk)?; }
        let remainder = size % 128;
        if remainder > 0 { self.send_data(&chunk[0..remainder])?; }
        self.send_command(0x22)?;
        self.send_data(&[0xF7])?;
        self.send_command(0x20)?;
        self.wait_busy().await?;
        self.lut_initialized = false;
        Ok(())
    }
}
