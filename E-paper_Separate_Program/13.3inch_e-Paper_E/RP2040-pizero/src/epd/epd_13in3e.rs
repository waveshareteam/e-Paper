use super::AsyncEpd;
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 1200;
pub const HEIGHT: u32 = 825;

pub struct EPD13in3e<'d, SPI, CsM, CsS, DC, RST, BUSY> {
    spi: SPI,
    cs_m: CsM,
    cs_s: CsS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CsM, CsS, DC, RST, BUSY> EPD13in3e<'d, SPI, CsM, CsS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CsM: OutputPin,
    CsS: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    pub fn new(spi: SPI, cs_m: CsM, cs_s: CsS, dc: DC, rst: RST, busy: BUSY) -> Self {
        Self {
            spi,
            cs_m,
            cs_s,
            dc,
            rst,
            busy,
            _phantom: core::marker::PhantomData,
        }
    }

    async fn send_command(&mut self, cmd: u8, master: bool, slave: bool) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        if master { let _ = self.cs_m.set_low(); }
        if slave { let _ = self.cs_s.set_low(); }
        self.spi.write(&[cmd])?;
        if master { let _ = self.cs_m.set_high(); }
        if slave { let _ = self.cs_s.set_high(); }
        Ok(())
    }

    async fn send_data_bytes(&mut self, data: &[u8], master: bool, slave: bool) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        if master { let _ = self.cs_m.set_low(); }
        if slave { let _ = self.cs_s.set_low(); }
        let res = self.spi.write(data);
        if master { let _ = self.cs_m.set_high(); }
        if slave { let _ = self.cs_s.set_high(); }
        res
    }

    async fn wait_until_idle(&mut self) {
        while self.busy.is_high().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0x12, true, true).await?; // DISPLAY_REFRESH
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CsM, CsS, DC, RST, BUSY> AsyncEpd for EPD13in3e<'d, SPI, CsM, CsS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CsM: OutputPin,
    CsS: OutputPin,
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

        self.send_command(0x12, true, true).await?; // SW Reset
        self.wait_until_idle().await;

        self.send_command(0x4C, true, true).await?; // PSR
        self.send_data_bytes(&[0x1F, 0x01], true, true).await?;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let _width_bytes = WIDTH.div_ceil(2);
        let _half_width_bytes = _width_bytes / 2;
        let white_byte = (0x1u8 << 4) | 0x1u8;

        self.send_command(0x10, true, false).await?;
        let row = [white_byte; 300];
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row, true, false).await?;
        }

        self.send_command(0x10, false, true).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row, false, true).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command(0x07, true, true).await?; // DEEP_SLEEP
        self.send_data_bytes(&[0xA5], true, true).await?;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        let width_bytes = WIDTH.div_ceil(2);
        let half_width_bytes = width_bytes / 2;

        self.send_command(0x10, true, false).await?;
        for i in 0..HEIGHT {
            let start = (i * width_bytes) as usize;
            let end = start + half_width_bytes as usize;
            self.send_data_bytes(&buffer[start..end], true, false).await?;
        }

        self.send_command(0x10, false, true).await?;
        for i in 0..HEIGHT {
            let start = (i * width_bytes + half_width_bytes) as usize;
            let end = start + half_width_bytes as usize;
            self.send_data_bytes(&buffer[start..end], false, true).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }
}
