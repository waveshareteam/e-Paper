use super::AsyncEpd;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 480;
pub const HEIGHT: u32 = 800;

pub struct EPD7in5h<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD7in5h<'d, SPI, CS, DC, RST, BUSY>
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

    async fn wait_until_idle_high(&mut self) {
        while self.busy.is_low().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x12).await?; // DISPLAY_REFRESH
        self.send_data(0x00).await?;
        self.wait_until_idle_high().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD7in5h<'d, SPI, CS, DC, RST, BUSY>
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

        self.send_command(0x01).await?; // Power Setting
        self.send_data_bytes(&[0x07, 0x07, 0x3F, 0x3F]).await?;

        self.send_command(0x04).await?; // Power ON
        self.wait_until_idle_high().await;

        self.send_command(0x00).await?; // Panel Setting
        self.send_data(0x1F).await?;

        self.send_command(0x61).await?; // Resolution
        self.send_data_bytes(&[0x01, 0xE0, 0x03, 0x20]).await?; // 480x800

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x10).await?;
        let _width_bytes = WIDTH.div_ceil(4);
        let white_byte = 0x55u8; // 01 01 01 01
        let row = [white_byte; 120];
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row).await?;
        }
        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x02).await?; // Power OFF
        self.wait_until_idle_high().await;
        self.send_command(0x07).await?; // Deep Sleep
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
