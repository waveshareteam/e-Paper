use super::AsyncEpd;
use embassy_rp::gpio::{Input, Output};
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 680; // per controller
pub const HEIGHT: u32 = 480;

pub struct EPD10in85g<'d, SPI, CS_M, CS_S, DC, RST, BUSY> {
    spi: SPI,
    cs_m: CS_M,
    cs_s: CS_S,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS_M, CS_S, DC, RST, BUSY> EPD10in85g<'d, SPI, CS_M, CS_S, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS_M: OutputPin,
    CS_S: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    pub fn new(spi: SPI, cs_m: CS_M, cs_s: CS_S, dc: DC, rst: RST, busy: BUSY) -> Self {
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

    async fn send_command_0(&mut self, cmd: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        let _ = self.cs_m.set_low();
        self.spi.write(&[cmd])?;
        let _ = self.cs_m.set_high();
        Ok(())
    }

    async fn send_command_1(&mut self, cmd: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        let _ = self.cs_s.set_low();
        self.spi.write(&[cmd])?;
        let _ = self.cs_s.set_high();
        Ok(())
    }

    async fn send_command_all(&mut self, cmd: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_low();
        let _ = self.cs_m.set_low();
        let _ = self.cs_s.set_low();
        self.spi.write(&[cmd])?;
        let _ = self.cs_m.set_high();
        let _ = self.cs_s.set_high();
        Ok(())
    }

    async fn send_data_0(&mut self, data: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs_m.set_low();
        let res = self.spi.write(&[data]);
        let _ = self.cs_m.set_high();
        res
    }

    async fn send_data_1(&mut self, data: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs_s.set_low();
        let res = self.spi.write(&[data]);
        let _ = self.cs_s.set_high();
        res
    }

    async fn send_data_all(&mut self, data: u8) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs_m.set_low();
        let _ = self.cs_s.set_low();
        let res = self.spi.write(&[data]);
        let _ = self.cs_m.set_high();
        let _ = self.cs_s.set_high();
        res
    }

    async fn send_data_bytes_0(&mut self, data: &[u8]) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs_m.set_low();
        let res = self.spi.write(data);
        let _ = self.cs_m.set_high();
        res
    }

    async fn send_data_bytes_1(&mut self, data: &[u8]) -> Result<(), SPI::Error> {
        let _ = self.dc.set_high();
        let _ = self.cs_s.set_low();
        let res = self.spi.write(data);
        let _ = self.cs_s.set_high();
        res
    }

    async fn wait_until_idle(&mut self) {
        while self.busy.is_low().expect("Busy pin read failed") {
            Timer::after(Duration::from_millis(10)).await;
        }
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command_all(0x12).await?;
        self.send_data_all(0x00).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS_M, CS_S, DC, RST, BUSY> AsyncEpd for EPD10in85g<'d, SPI, CS_M, CS_S, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS_M: OutputPin,
    CS_S: OutputPin,
    DC: OutputPin,
    RST: OutputPin,
    BUSY: embedded_hal::digital::InputPin,
{
    type Error = SPI::Error;

    async fn init(&mut self) -> Result<(), Self::Error> {
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;
        let _ = self.rst.set_low();
        Timer::after(Duration::from_millis(10)).await;
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;

        self.wait_until_idle().await;

        self.send_command_all(0x4D).await?;
        self.send_data_all(0x78).await?;

        self.send_command_all(0x00).await?;
        self.send_data_all(0x2F).await?;
        self.send_data_all(0x29).await?;

        for _ in 0..2 {
            self.send_command_all(0x06).await?;
            self.send_data_all(0x0D).await?;
            self.send_data_all(0x12).await?;
            self.send_data_all(0x30).await?;
            self.send_data_all(0x20).await?;
            self.send_data_all(0x19).await?;
            self.send_data_all(0x3D).await?;
            self.send_data_all(0x0C).await?;
        }

        self.send_command_all(0x50).await?;
        self.send_data_all(0x37).await?;

        self.send_command_all(0x61).await?;
        self.send_data_all((WIDTH / 256) as u8).await?;
        self.send_data_all((WIDTH % 256) as u8).await?;
        self.send_data_all((HEIGHT / 256) as u8).await?;
        self.send_data_all((HEIGHT % 256) as u8).await?;

        self.send_command_all(0x65).await?;
        self.send_data_all(0x00).await?;
        self.send_data_all(0x00).await?;
        self.send_data_all(0x00).await?;
        self.send_data_all(0x00).await?;

        self.send_command_all(0xE0).await?;
        self.send_data_all(0x01).await?;

        self.send_command_all(0xE3).await?;
        self.send_data_all(0x08).await?;

        self.send_command_all(0xE5).await?;
        self.send_data_all(0x08).await?;

        self.send_command_all(0xE9).await?;
        self.send_data_all(0x01).await?;

        self.send_command_all(0x04).await?;
        self.wait_until_idle().await;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let row_bytes = WIDTH.div_ceil(4);
        let color = 0x01; // white
        let data = (color << 6) | (color << 4) | (color << 2) | color;
        let row_buf = [data; 256];

        self.send_command_0(0x10).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes_0(&row_buf[0..row_bytes as usize]).await?;
        }

        self.send_command_1(0x10).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes_1(&row_buf[0..row_bytes as usize]).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }

    async fn sleep(&mut self) -> Result<(), Self::Error> {
        self.send_command_all(0x02).await?;
        self.send_data_all(0x00).await?;
        Timer::after(Duration::from_millis(100)).await;
        self.wait_until_idle().await;

        self.send_command_all(0x07).await?;
        self.send_data_all(0xA5).await?;
        Timer::after(Duration::from_millis(100)).await;
        Ok(())
    }

    async fn display_base(&mut self, buffer: &[u8]) -> Result<(), Self::Error> {
        let width_pixels_total = WIDTH * 2;
        let width_1bpp_total = (width_pixels_total + 7) / 8;
        let row_bytes_total_2bpp = (width_pixels_total + 3) / 4;
        let row_bytes_half_2bpp = WIDTH.div_ceil(4);

        let mut row_buf = [0u8; 512];

        // Master half (Left)
        self.send_command_0(0x10).await?;
        for y in 0..HEIGHT {
            for x_byte in 0..row_bytes_half_2bpp {
                let orig_x_byte = x_byte / 2;
                let is_high_nibble = x_byte % 2 == 0;
                let orig_byte = buffer[(orig_x_byte + y * width_1bpp_total) as usize];
                
                let mut send_byte = 0u8;
                for i in 0..4 {
                    let bit_idx = if is_high_nibble { 7 - i } else { 3 - i };
                    let bit = (orig_byte >> bit_idx) & 1;
                    let color = if bit == 0 { 0x00 } else { 0x01 };
                    send_byte |= color << (6 - (i * 2));
                }
                row_buf[x_byte as usize] = send_byte;
            }
            self.send_data_bytes_0(&row_buf[0..row_bytes_half_2bpp as usize]).await?;
        }

        // Slave half (Right)
        self.send_command_1(0x10).await?;
        for y in 0..HEIGHT {
            for x_byte in 0..row_bytes_half_2bpp {
                let real_x_byte = x_byte + row_bytes_half_2bpp;
                let orig_x_byte = real_x_byte / 2;
                let is_high_nibble = real_x_byte % 2 == 0;
                let orig_byte = buffer[(orig_x_byte + y * width_1bpp_total) as usize];
                
                let mut send_byte = 0u8;
                for i in 0..4 {
                    let bit_idx = if is_high_nibble { 7 - i } else { 3 - i };
                    let bit = (orig_byte >> bit_idx) & 1;
                    let color = if bit == 0 { 0x00 } else { 0x01 };
                    send_byte |= color << (6 - (i * 2));
                }
                row_buf[x_byte as usize] = send_byte;
            }
            self.send_data_bytes_1(&row_buf[0..row_bytes_half_2bpp as usize]).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }
}
