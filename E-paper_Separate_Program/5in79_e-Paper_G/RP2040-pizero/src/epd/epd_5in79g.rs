use super::AsyncEpd;
use embassy_rp::gpio::{Input, Output};
use embassy_time::{Duration, Timer};
use embedded_hal::digital::OutputPin;

pub const WIDTH: u32 = 792;
pub const HEIGHT: u32 = 272;

pub struct EPD5in79g<'d, SPI, CS, DC, RST, BUSY> {
    spi: SPI,
    cs: CS,
    dc: DC,
    rst: RST,
    busy: BUSY,
    _phantom: core::marker::PhantomData<&'d ()>,
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD5in79g<'d, SPI, CS, DC, RST, BUSY>
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
        Timer::after(Duration::from_millis(100)).await;
    }

    async fn turn_on_display(&mut self) -> Result<(), SPI::Error> {
        self.send_command(0xA2).await?;
        self.send_data(0x00).await?;

        self.send_command(0x12).await?;
        self.send_data(0x00).await?;
        self.wait_until_idle().await;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> AsyncEpd for EPD5in79g<'d, SPI, CS, DC, RST, BUSY>
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
        Timer::after(Duration::from_millis(1)).await;
        let _ = self.rst.set_high();
        Timer::after(Duration::from_millis(100)).await;

        self.send_command(0xA2).await?;
        self.send_data(0x01).await?;
        self.send_command(0x00).await?;
        self.send_data(0x03).await?;
        self.send_data(0x29).await?;

        self.send_command(0xA2).await?;
        self.send_data(0x02).await?;
        self.send_command(0x00).await?;
        self.send_data(0x07).await?;
        self.send_data(0x29).await?;

        self.send_command(0xA2).await?;
        self.send_data(0x00).await?;

        self.send_command(0x50).await?;
        self.send_data(0x97).await?;

        self.send_command(0x61).await?;
        self.send_data(0x01).await?;
        self.send_data(0x8C).await?;
        self.send_data(0x01).await?;
        self.send_data(0x10).await?;

        self.send_command(0x06).await?;
        self.send_data(0x38).await?;
        self.send_data(0x38).await?;
        self.send_data(0x38).await?;
        self.send_data(0x00).await?;

        self.send_command(0xE9).await?;
        self.send_data(0x01).await?;

        self.send_command(0xE0).await?;
        self.send_data(0x01).await?;

        self.send_command(0x04).await?;
        self.wait_until_idle().await;

        Ok(())
    }

    async fn clear(&mut self) -> Result<(), Self::Error> {
        let row_bytes_half = WIDTH.div_ceil(8);
        let color = 0x01; // white
        let data = (color << 6) | (color << 4) | (color << 2) | color;
        let row_buf = [data; 256];

        // Controller 2
        self.send_command(0xA2).await?;
        self.send_data(0x02).await?;
        self.send_command(0x10).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row_buf[0..row_bytes_half as usize]).await?;
        }

        // Controller 1
        self.send_command(0xA2).await?;
        self.send_data(0x01).await?;
        self.send_command(0x10).await?;
        for _ in 0..HEIGHT {
            self.send_data_bytes(&row_buf[0..row_bytes_half as usize]).await?;
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
        let width_1bpp = WIDTH.div_ceil(8);
        let row_bytes_2bpp = WIDTH.div_ceil(4);
        let row_bytes_half = row_bytes_2bpp / 2;
        
        let mut row_buf = [0u8; 512]; // Large buffer for row-reordering

        // Pre-convert 1bpp buffer to 2bpp row-based buffer for easier indexing?
        // Actually, let's just do it on the fly.

        // Controller 2
        self.send_command(0xA2).await?;
        self.send_data(0x02).await?;
        self.send_command(0x10).await?;
        for j in 0..(HEIGHT / 2) {
            // Row j left half
            self.send_row_half(buffer, j, 0, row_bytes_half, width_1bpp).await?;
            // Row HEIGHT-j-1 left half
            self.send_row_half(buffer, HEIGHT - j - 1, 0, row_bytes_half, width_1bpp).await?;
        }

        // Controller 1
        self.send_command(0xA2).await?;
        self.send_data(0x01).await?;
        self.send_command(0x10).await?;
        for j in 0..(HEIGHT / 2) {
            // Row j right half
            self.send_row_half(buffer, j, row_bytes_half, row_bytes_half, width_1bpp).await?;
            // Row HEIGHT-j-1 right half
            self.send_row_half(buffer, HEIGHT - j - 1, row_bytes_half, row_bytes_half, width_1bpp).await?;
        }

        self.turn_on_display().await?;
        Ok(())
    }
}

impl<'d, SPI, CS, DC, RST, BUSY> EPD5in79g<'d, SPI, CS, DC, RST, BUSY>
where
    SPI: embedded_hal::spi::SpiBus,
    CS: OutputPin,
    DC: OutputPin,
{
    async fn send_row_half(&mut self, buffer: &[u8], y: u32, x_byte_start: u32, count: u32, width_1bpp: u32) -> Result<(), SPI::Error> {
        let mut row_buf = [0u8; 128];
        for x_byte in 0..count {
            let real_x_byte = x_byte + x_byte_start;
            let orig_x_byte = real_x_byte / 2;
            let is_high_nibble = real_x_byte % 2 == 0;
            let orig_byte = buffer[(orig_x_byte + y * width_1bpp) as usize];
            
            let mut send_byte = 0u8;
            for i in 0..4 {
                let bit_idx = if is_high_nibble { 7 - i } else { 3 - i };
                let bit = (orig_byte >> bit_idx) & 1;
                let color = if bit == 0 { 0x00 } else { 0x01 };
                send_byte |= color << (6 - (i * 2));
            }
            row_buf[x_byte as usize] = send_byte;
        }
        self.send_data_bytes(&row_buf[0..count as usize]).await
    }
}
