use embassy_rp::peripherals::SPI1;
use embassy_rp::gpio::{Input, Level, Output, Pull};
use embassy_rp::spi::{Config as SpiConfig, Spi, Blocking};

pub struct Hardware<'d> {
    pub spi: Spi<'d, SPI1, Blocking>,
    pub cs: Output<'d>,
    pub dc: Output<'d>,
    pub rst: Output<'d>,
    pub busy: Input<'d>,
    
    pub btn_up: Input<'d>,
    pub btn_down: Input<'d>,
    pub btn_select: Input<'d>,
    pub btn_back: Input<'d>,
}

impl<'d> Hardware<'d> {
    pub fn init(p: embassy_rp::Peripherals) -> Self {
        let mut epd_spi_config = SpiConfig::default();
        epd_spi_config.frequency = 20_000_000;
        
        let spi = Spi::new_blocking(p.SPI1, p.PIN_10, p.PIN_11, p.PIN_12, epd_spi_config);
        
        Self {
            spi,
            cs: Output::new(p.PIN_9, Level::High),
            dc: Output::new(p.PIN_25, Level::Low),
            rst: Output::new(p.PIN_17, Level::High),
            busy: Input::new(p.PIN_24, Pull::None),
            
            btn_up: Input::new(p.PIN_19, Pull::Up),
            btn_down: Input::new(p.PIN_13, Pull::Up),
            btn_select: Input::new(p.PIN_6, Pull::Up),
            btn_back: Input::new(p.PIN_15, Pull::Up),
        }
    }
}
