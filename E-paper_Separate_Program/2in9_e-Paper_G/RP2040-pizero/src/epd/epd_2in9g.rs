use super::AsyncEpd;
use embassy_rp::gpio::{Input, Output};
use embassy_rp::spi::Spi;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 128;
pub const HEIGHT: u32 = 296;

pub struct EPD2in9g<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD2in9g<'d, SPI, CS, DC, RST, BUSY>
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
        let _ = self.dc.set_high();
        let _ = self.cs.set_low();
        self.spi.write(&[data])?;
        let _ = self.cs.set_high();
        Ok(())
    }

    async fn wait_until_idle(&mut self) {
        Timer::after(Duration::from_millis(100)).await;
        // LOW: idle, HIGH: busy (Wait until LOW)
        // Wait, C code: `while(!DEV_Digital_Read)` -> loop while LOW. So it waits for HIGH.
        while self.busy.is_high().expect("Busy pin read failed") != true {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x12).await?;
        self.send_data(0x00).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD2in9g<'d, SPI, CS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    type Error = SPI::Error;

    async fn init(&mut self) -> Result<(), Self::Error> {
        // HW Reset
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_low();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;

        self.wait_until_idle().await;
        
        self.send_command(0x4D).await?;
        self.send_data(0x78).await?;

        self.send_command(0x00).await?; // PSR
        self.send_data(0x0F).await?;
        self.send_data(0x29).await?;

        self.send_command(0x01).await?; // PWRR
        self.send_data(0x07).await?;
        self.send_data(0x00).await?;

        self.send_command(0x03).await?; // POFS
        self.send_data(0x10).await?;
        self.send_data(0x54).await?;
        self.send_data(0x44).await?;

        self.send_command(0x06).await?; // BTST_P
        self.send_data(0x0F).await?;
        self.send_data(0x0A).await?;
        self.send_data(0x2F).await?;
        self.send_data(0x25).await?;
        self.send_data(0x22).await?;
        self.send_data(0x2E).await?;
        self.send_data(0x21).await?;

        self.send_command(0x41).await?; // TSE
        self.send_data(0x00).await?;

        self.send_command(0x50).await?; // CDI
        self.send_data(0x37).await?;

        self.send_command(0x60).await?; // TCON
        self.send_data(0x02).await?;
        self.send_data(0x02).await?;

        self.send_command(0x61).await?; // TRES
        self.send_data((WIDTH / 256) as u8).await?;
        self.send_data((WIDTH % 256) as u8).await?;
        self.send_data((HEIGHT / 256) as u8).await?;
        self.send_data((HEIGHT % 256) as u8).await?;

        self.send_command(0x65).await?; // GSST
        self.send_data(0x00).await?;
        self.send_data(0x00).await?;
        self.send_data(0x00).await?;
        self.send_data(0x00).await?;

        self.send_command(0xE7).await?;
        self.send_data(0x1C).await?;

        self.send_command(0xE3).await?;
        self.send_data(0x22).await?;

        self.send_command(0xB4).await?;
        self.send_data(0xD0).await?;
        self.send_command(0xB5).await?;
        self.send_data(0x03).await?;

        self.send_command(0xE9).await?;
        self.send_data(0x01).await?;

        self.send_command(0x30).await?;
        self.send_data(0x08).await?;

        self.send_command(0x04).await?;
        Timer::after(Duration::from_millis(500)).await;
        self.wait_until_idle().await;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let width = if WIDTH % 4 == 0 { WIDTH / 4 } else { WIDTH / 4 + 1 };
        let color = 0x01; // white
        let data = (color << 6) | (color << 4) | (color << 2) | color;

        self.send_command(0x10).await?;
        for _ in 0..HEIGHT {
            for _ in 0..width {
                self.send_data(data).await?;
            }
        }
        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x02).await?;
        self.send_data(0x00).await?;
        self.wait_until_idle().await;

        self.send_command(0x07).await?;
        self.send_data(0xA5).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        let width_bytes = if WIDTH % 8 == 0 { WIDTH / 8 } else { WIDTH / 8 + 1 };
        
        self.send_command(0x10).await?;
        for y in 0..HEIGHT {
            for x_byte in 0..(WIDTH / 4) {
                // Two 2bpp bytes needed for every 8 pixels (1 byte of 1bpp buffer)
                let orig_x_byte = x_byte / 2;
                let is_high_nibble = x_byte % 2 == 0;
                
                let orig_byte = buffer[(orig_x_byte + y * width_bytes) as usize];
                
                let mut send_byte = 0u8;
                for i in 0..4 {
                    let bit_idx = if is_high_nibble { 7 - i } else { 3 - i };
                    let bit = (orig_byte >> bit_idx) & 1;
                    
                    // 1bpp: 0 = Black, 1 = White
                    // 2bpp: 0x00 = Black, 0x01 = White
                    let color = if bit == 0 { 0x00 } else { 0x01 };
                    send_byte |= color << (6 - (i * 2));
                }
                self.send_data(send_byte).await?;
            }
        }
        self.turn_on_display().await?;
        Ok(())
    }
}
