use embassy_rp::peripherals::SPI1;
use embassy_rp::gpio::{Input, Level, Output, Pull};
use embassy_rp::spi::{Config as SpiConfig, Spi, Blocking};

#[allow(dead_code)]
pub struct Hardware<'d> {
    pub spi: Spi<'d, SPI1, Blocking>,
    pub cs: Output<'d>,
    pub dc: Output<'d>,
    pub rst: Output<'d>,
    pub busy: Input<'d>,
}

impl<'d> Hardware<'d> {
    pub fn init(p: embassy_rp::Peripherals) -> Self {
        let mut config = SpiConfig::default();
        config.frequency = 10_000_000;
        let spi = Spi::new_blocking(p.SPI1, p.PIN_10, p.PIN_11, p.PIN_12, config);
        
        Self {
            spi,
            cs: Output::new(p.PIN_9, Level::High),
            dc: Output::new(p.PIN_25, Level::Low),
            rst: Output::new(p.PIN_17, Level::High),
            busy: Input::new(p.PIN_24, Pull::None),
        }
    }
}
