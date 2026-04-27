use super::AsyncEpd;
use embassy_rp::gpio::{Input, Output};
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 200;
pub const HEIGHT: u32 = 200;

pub struct EPD1in54g<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD1in54g<'d, SPI, CS, DC, RST, BUSY>
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
        // Wait until pin goes HIGH (IDLE)
        while self.busy.is_low().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(1)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x12).await?;
        self.send_data(0x00).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD1in54g<'d, SPI, CS, DC, RST, BUSY>
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

        self.send_command(0x4D).await?;
        self.send_data(0x78).await?;

        self.send_command(0x00).await?;
        self.send_data(0x0F).await?;
        self.send_data(0x29).await?;

        self.send_command(0x06).await?;
        self.send_data(0x0D).await?;
        self.send_data(0x12).await?;
        self.send_data(0x30).await?;
        self.send_data(0x20).await?;
        self.send_data(0x19).await?;
        self.send_data(0x2A).await?;
        self.send_data(0x22).await?;

        self.send_command(0x50).await?;
        self.send_data(0x37).await?;

        self.send_command(0x61).await?;
        self.send_data((WIDTH / 256) as u8).await?;
        self.send_data((WIDTH % 256) as u8).await?;
        self.send_data((HEIGHT / 256) as u8).await?;
        self.send_data((HEIGHT % 256) as u8).await?;

        self.send_command(0xE9).await?;
        self.send_data(0x01).await?;

        self.send_command(0x30).await?;
        self.send_data(0x08).await?;

        self.send_command(0x04).await?;
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
        let width_bytes = WIDTH.div_ceil(8);
        let row_bytes = WIDTH / 4;
        let mut row_buf = [0u8; 100]; // Max supported width/4 (e.g. 400/4=100)

        self.send_command(0x10).await?;
        for y in 0..HEIGHT {
            for x_byte in 0..row_bytes {
                let orig_x_byte = x_byte / 2;
                let is_high_nibble = x_byte % 2 == 0;
                let orig_byte = buffer[(orig_x_byte + y * width_bytes) as usize];
                
                let mut send_byte = 0u8;
                for i in 0..4 {
                    let bit_idx = if is_high_nibble { 7 - i } else { 3 - i };
                    let bit = (orig_byte >> bit_idx) & 1;
                    let color = if bit == 0 { 0x00 } else { 0x01 };
                    send_byte |= color << (6 - (i * 2));
                }
                row_buf[x_byte as usize] = send_byte;
            }
            self.send_data_bytes(&row_buf[0..row_bytes as usize]).await?;
        }
        self.turn_on_display().await?;
        Ok(())
    }
}
