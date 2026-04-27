use super::AsyncEpd;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 1360;
pub const HEIGHT: u32 = 480;

pub struct EPD10in85<'d, SPI, Cs0, Cs1, DC, RST, BUSY> {
    spi: SPI,
    cs0: Cs0,
    cs1: Cs1,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, Cs0, Cs1, DC, RST, BUSY> EPD10in85<'d, SPI, Cs0, Cs1, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    Cs0: OutputPin,
    Cs1: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    pub fn new(spi: SPI, cs0: Cs0, cs1: Cs1, dc: DC, rst: RST, busy: BUSY) -> Self {
        Self {
            spi,
            cs0,
            cs1,
            dc,
            rst,
            busy,
            _phantom: core::marker::PhantomData,
        }
    }

    async fn send_command(&mut self, cmd: u8, cs0: bool, cs1: bool) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        if cs0 { let _ = self.cs0.set_low(); }
        if cs1 { let _ = self.cs1.set_low(); }
        self.spi.write(&[cmd])?;
        if cs0 { let _ = self.cs0.set_high(); }
        if cs1 { let _ = self.cs1.set_high(); }
        Ok(())
    }

    async fn send_data_bytes(&mut self, data: &[u8], cs0: bool, cs1: bool) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        if cs0 { let _ = self.cs0.set_low(); }
        if cs1 { let _ = self.cs1.set_low(); }
        let res = self.spi.write(data);
        if cs0 { let _ = self.cs0.set_high(); }
        if cs1 { let _ = self.cs1.set_high(); }
        res
    }

    async fn wait_until_idle(&mut self) {
        while self.busy.is_low().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x12, true, true).await?; // DISPLAY_REFRESH
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, Cs0, Cs1, DC, RST, BUSY> AsyncEpd for EPD10in85<'d, SPI, Cs0, Cs1, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    Cs0: OutputPin,
    Cs1: OutputPin,
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

        self.send_command(0x4D, true, true).await?;
        self.send_data_bytes(&[0x55], true, true).await?;

        self.send_command(0x00, true, true).await?; // Panel setting
        self.send_data_bytes(&[0x9F, 0x0D], true, true).await?;

        self.send_command(0x06, true, true).await?; // Booster
        self.send_data_bytes(&[0x57, 0x24, 0x28, 0x32, 0x08, 0x48], true, true).await?;

        self.send_command(0x61, true, true).await?; // Resolution
        self.send_data_bytes(&[0x02, 0xA8, 0x01, 0xE0], true, true).await?; // 680x480

        self.send_command(0x04, true, true).await?; // Power on
        Timer::after(Duration::from_millis(100)).await;
        self.wait_until_idle().await;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let width_bytes = 85; // 680 / 8
        let row = [0xFFu8; 85];

        self.send_command(0x10, true, true).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row, true, true).await?;
        }

        self.send_command(0x13, true, true).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row, true, true).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x02, true, true).await?; // Power off
        self.wait_until_idle().await;
        self.send_command(0x07, true, true).await?; // Deep sleep
        self.send_data_bytes(&[0xA5], true, true).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        let width_bytes = 85;
        
        self.send_command(0x13, true, false).await?;
        for j in 0..HEIGHT {
            let start = (j * width_bytes * 2) as usize;
            let end = start + width_bytes as usize;
            self.send_data_bytes(&buffer[start..end], true, false).await?;
        }

        self.send_command(0x13, false, true).await?;
        for j in 0..HEIGHT {
            let start = (j * width_bytes * 2 + width_bytes) as usize;
            let end = start + width_bytes as usize;
            self.send_data_bytes(&buffer[start..end], false, true).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }
}
