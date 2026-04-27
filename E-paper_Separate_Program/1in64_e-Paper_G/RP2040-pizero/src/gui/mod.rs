use embedded_graphics::{
    pixelcolor::BinaryColor,
    prelude::*,
};
use crate::epd::epd_1in64g::{WIDTH as EPD_WIDTH, HEIGHT as EPD_HEIGHT};

pub struct DisplayBuffer {
    pub buffer: [u8; ((EPD_WIDTH + 7) / 8 * EPD_HEIGHT) as usize],
}

impl Default for DisplayBuffer {
    fn default() -> Self {
        Self {
            buffer: [0xFF; ((EPD_WIDTH + 7) / 8 * EPD_HEIGHT) as usize],
        }
    }
}

impl DisplayBuffer {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn clear(&mut self, color: BinaryColor) {
        let byte = if color == BinaryColor::On { 0x00 } else { 0xFF };
        self.buffer.fill(byte);
    }
}

impl DrawTarget for DisplayBuffer {
    type Color = BinaryColor;
    type Error = core::convert::Infallible;

    fn draw_iter<I>(&mut self, pixels: I) -> Result<(), Self::Error>
    where
        I: IntoIterator<Item = Pixel<Self::Color>>,
    {
        for Pixel(point, color) in pixels.into_iter() {
            // Landscape mode (rotated 90 degrees)
            if point.x >= 0 && point.x < EPD_HEIGHT as i32 && point.y >= 0 && point.y < EPD_WIDTH as i32 {
                let phys_x = (EPD_WIDTH as i32 - 1 - point.y) as usize;
                let phys_y = point.x as usize;
                let width_bytes = ((EPD_WIDTH + 7) / 8) as usize;
                let idx = (phys_x / 8) + (phys_y * width_bytes);
                let bit = 7 - (phys_x % 8);
                if color == BinaryColor::On { // On = Black (0)
                    self.buffer[idx] &= !(1 << bit);
                } else { // Off = White (1)
                    self.buffer[idx] |= 1 << bit;
                }
            }
        }
        Ok(())
    }
}

impl OriginDimensions for DisplayBuffer {
    fn size(&self) -> Size {
        Size::new(EPD_HEIGHT, EPD_WIDTH) // Landscape
    }
}
