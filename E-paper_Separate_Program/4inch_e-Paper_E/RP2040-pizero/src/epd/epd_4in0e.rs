use super::AsyncEpd;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 400;
pub const HEIGHT: u32 = 600;

pub struct EPD4in0e<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD4in0e<'d, SPI, CS, DC, RST, BUSY>
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
        while self.busy.is_low().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x04).await?; // Power on
        self.wait_until_idle().await;
        
        self.send_command(0x12).await?; // Display refresh
        self.send_data(0x00).await?;
        self.wait_until_idle().await;

        self.send_command(0x02).await?; // Power off
        self.send_data(0x00).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD4in0e<'d, SPI, CS, DC, RST, BUSY>
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

        self.send_command(0xAA).await?;
        self.send_data_bytes(&[0x49, 0x55, 0x20, 0x08, 0x09, 0x18]).await?;

        self.send_command(0x01).await?;
        self.send_data(0x3F).await?;

        self.send_command(0x00).await?;
        self.send_data_bytes(&[0x5F, 0x69]).await?;

        self.send_command(0x61).await?;
        self.send_data_bytes(&[0x01, 0x90, 0x02, 0x58]).await?;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let width_bytes = WIDTH.div_ceil(2);
        self.send_command(0x10).await?;
        let white_byte = (0x1u8 << 4) | 0x1u8;
        let row = [white_byte; 200];
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row).await?;
        }
        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x07).await?;
        self.send_data(0xA5).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        self.send_command(0x10).await?;
        self.send_data_bytes(buffer).await?;
        self.turn_on_display().await?;
        Ok(())
    }
}
