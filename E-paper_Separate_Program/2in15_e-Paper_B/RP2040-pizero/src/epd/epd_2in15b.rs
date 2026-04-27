use super::AsyncEpd;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 122;
pub const HEIGHT: u32 = 250;

pub struct EPD2in15b<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD2in15b<'d, SPI, CS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    pub fn new(spi: SPI, cs: CS, dc: DC, rst: RST, busy: BUSY) -> Self {
        Self {
            spi,
            cs,
            dc,
            rst,
            busy,
            _phantom: core::marker::PhantomData,
        }
    }

    async fn send_command(&mut self, cmd: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        let _ = self.cs.set_low();
        self.spi.write(&[cmd])?;
        let _ = self.cs.set_high();
        Ok(())
    }

    async fn send_data(&mut self, data: u8) -> Result<(), SPI::Error> {
        self.send_data_bytes(&[data]).await
    }

    async fn send_data_bytes(&mut self, data: &[u8]) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs.set_low();
        let res = self.spi.write(data);
        let _ = self.cs.set_high();
        res
    }

    async fn wait_until_idle(&mut self) {
        while self.busy.is_high().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x20).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD2in15b<'d, SPI, CS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    type Error = SPI::Error;

    async fn init(&mut self) -> Result<(), Self::Error> {
        let _ = self.rst.set_low();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;

        self.send_command(0x12).await?; // SW Reset
        Timer::after(Duration::from_millis(100)).await;
        self.wait_until_idle().await;

        self.send_command(0x11).await?; // Data entry mode
        self.send_data(0x03).await?;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let width_bytes = WIDTH.div_ceil(8);
        self.send_command(0x24).await?;
        let row = [0xFFu8; 16]; // 122 / 8 ceil
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row[0..width_bytes as usize]).await?;
        }
        self.send_command(0x26).await?;
        let row_red = [0x00u8; 16];
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row_red[0..width_bytes as usize]).await?;
        }
        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x10).await?;
        self.send_data(0x01).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        // Assume buffer contains both Black and Red (2 * WIDTH * HEIGHT / 8)
        let width_bytes = WIDTH.div_ceil(8);
        let plane_size = (width_bytes * HEIGHT) as usize;
        
        self.send_command(0x24).await?;
        self.send_data_bytes(&buffer[0..plane_size]).await?;
        
        self.send_command(0x26).await?;
        self.send_data_bytes(&buffer[plane_size..2*plane_size]).await?;
        
        self.turn_on_display().await?;
        Ok(())
    }
}
