#![no_std]
#![no_main]

use defmt::*;
use embassy_executor::Spawner;
use embassy_rp::gpio::{Level, Output};
use embassy_time::{Duration, Timer};
use epd_7in5_e_paper_h::config::Hardware;
use epd_7in5_e_paper_h::epd::*;
use epd_7in5_e_paper_h::epd::EPD7in5h;

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());
    info!("Initializing Hardware...");

    let mut hw = Hardware::init(p);
    
    let mut epd = EPD7in5h::new(
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
