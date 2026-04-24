#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_time::{Duration, Timer};
use panic_reset as _;
use defmt_rtt as _;

use recipe_reader_logic::{
    config::Hardware,
    epd::{AsyncEpd, SSD1680},
    gui::DisplayBuffer,
};

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());
    let hw = Hardware::init(p);

    let mut epd = SSD1680::new(hw.spi, hw.cs, hw.dc, hw.rst, hw.busy);
    let mut _buffer = DisplayBuffer::new();

    // --- Initial Render ---
    if epd.init().await.is_ok() {
        let _ = epd.clear().await;
    }

    loop {
        Timer::after(Duration::from_millis(1000)).await;
    }
}
