#! [no_std]
#! [no_main]

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
    epd::{AsyncEpd, EPD1in64g},
    gui::DisplayBuffer,
};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());
    let hw = Hardware::init(p);

    let mut epd = EPD1in64g::new(hw.spi, hw.cs, hw.dc, hw.rst, hw.busy);
    let mut buffer = DisplayBuffer::new();

    // --- Initial Render ---
    if epd.init().await.is_ok() {
        let _ = epd.clear().await;

        // Draw demo
        let style = PrimitiveStyle::with_stroke(BinaryColor::On, 1);
        
        let _ = Rectangle::new(Point::new(10, 10), Size::new(50, 50))
            .into_styled(style)
            .draw(&mut buffer);

        let _ = Circle::new(Point::new(70, 10), 50)
            .into_styled(style)
            .draw(&mut buffer);

        let _ = Line::new(Point::new(10, 80), Point::new(110, 130))
            .into_styled(style)
            .draw(&mut buffer);

        let text_style = MonoTextStyle::new(&FONT_6X10, BinaryColor::On);
        let _ = Text::new("RP2040-PiZero", Point::new(10, 140), text_style)
            .draw(&mut buffer);
        let _ = Text::new("1.64 inch G", Point::new(10, 155), text_style)
            .draw(&mut buffer);

        let _ = epd.display_base(&buffer.buffer).await;
    }

    loop {
        Timer::after(Duration::from_millis(1000)).await;
    }
}
