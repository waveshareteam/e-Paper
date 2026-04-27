use super::AsyncEpd;
use embassy_rp::gpio::{Input, Output};
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 176;
pub const HEIGHT: u32 = 264;

pub struct SSD1680<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> SSD1680<'d, SPI, CS, DC, RST, BUSY>
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
            Timer::after(Duration::from_millis(1)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x22).await?;
        self.send_data(0xF7).await?;
        self.send_command(0x20).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for SSD1680<'d, SPI, CS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    type Error = SPI::Error;

    async fn init(&mut self) -> Result<(), Self::Error> {
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_low();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;

        self.send_command(0x12).await?; // SW Reset
        self.wait_until_idle().await;

        self.send_command(0x01).await?; // Driver output control
        self.send_data(0x07).await?;
        self.send_data(0x01).await?;
        self.send_data(0x00).await?;

        self.send_command(0x11).await?; // Data entry mode
        self.send_data(0x03).await?;

        self.send_command(0x44).await?; // Set RAM X address
        self.send_data(0x00).await?;
        self.send_data(0x15).await?;

        self.send_command(0x45).await?; // Set RAM Y address
        self.send_data(0x00).await?;
        self.send_data(0x00).await?;
        self.send_data(0x07).await?;
        self.send_data(0x01).await?;

        self.send_command(0x3C).await?; // Border waveform
        self.send_data(0x05).await?;

        self.send_command(0x18).await?; // Temp sensor
        self.send_data(0x80).await?;

        self.send_command(0x4E).await?; // Set RAM X pointer
        self.send_data(0x00).await?;
        self.send_command(0x4F).await?; // Set RAM Y pointer
        self.send_data(0x00).await?;
        self.send_data(0x00).await?;
        
        self.wait_until_idle().await;
        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let width_bytes = WIDTH.div_ceil(8);
        self.send_command(0x24).await?;
        for _ in 0..(width_bytes * HEIGHT) {
            self.send_data(0xFF).await?;
        }
        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x10).await?; // Deep sleep
        self.send_data(0x01).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        self.send_command(0x24).await?;
        self.send_data_bytes(buffer).await?;
        self.turn_on_display().await?;
        Ok(())
    }
}
