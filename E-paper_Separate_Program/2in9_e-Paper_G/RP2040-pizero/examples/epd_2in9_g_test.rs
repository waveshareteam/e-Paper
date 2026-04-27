#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_time::{Duration, Timer};
use embedded_graphics::{
    pixelcolor::BinaryColor,
    prelude::*,
    primitives::{Circle, Line, Rectangle, PrimitiveStyle},
    mono_font::{ascii::FONT_6X10, MonoTextStyle},
    text::Text,
};
use panic_reset as _;
use defmt_rtt as _;

use waveshare_epd_logic::{
    config::Hardware,
    epd::{AsyncEpd, EPD2in9g},
    gui::DisplayBuffer,
};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());
    let hw = Hardware::init(p);

    let mut epd = EPD2in9g::new(hw.spi, hw.cs, hw.dc, hw.rst, hw.busy);
    let mut buffer = DisplayBuffer::new();

    // --- Initial Render ---
    if epd.init().await.is_ok() {
        let _ = epd.clear().await;

        // Draw standard Waveshare-style demo
        let style = PrimitiveStyle::with_stroke(BinaryColor::On, 1);
        
        // 1. Draw Shapes
        let _ = Rectangle::new(Point::new(10, 10), Size::new(50, 50))
            .into_styled(style)
            .draw(&mut buffer);

        let _ = Circle::new(Point::new(70, 10), 50)
            .into_styled(style)
            .draw(&mut buffer);

        let _ = Line::new(Point::new(10, 80), Point::new(110, 130))
            .into_styled(style)
            .draw(&mut buffer);

        // 2. Draw Text
        let text_style = MonoTextStyle::new(&FONT_6X10, BinaryColor::On);
        let _ = Text::new("RP2040-PiZero EPD Demo", Point::new(10, 150), text_style)
            .draw(&mut buffer);
        let _ = Text::new("2.9 inch G", Point::new(10, 165), text_style)
            .draw(&mut buffer);

        // 3. Display
        let _ = epd.display_base(&buffer.buffer).await;
    }

    loop {
        Timer::after(Duration::from_millis(1000)).await;
    }
}
