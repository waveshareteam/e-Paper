#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use embassy_rp::gpio::{Level, Output};
use embassy_time::{Duration, Timer};
use epd_2in7_v2_e_paper::config::Hardware;
use epd_2in7_v2_e_paper::epd::*;
use epd_2in7_v2_e_paper::epd::Ssd1680;

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());
    info!("Initializing Hardware...");

    let mut hw = Hardware::init(p);
    
    let mut epd = Ssd1680::new(
        hw.spi,
        hw.cs,
        hw.dc,
        hw.rst,
        hw.busy,
    );

    info!("Initializing EPD...");
    epd.init().await.expect("EPD init failed");

    info!("Clearing EPD...");
    epd.clear().await.expect("EPD clear failed");

    info!("EPD Ready!");

    loop {
        Timer::after(Duration::from_millis(1000)).await;
    }
}
