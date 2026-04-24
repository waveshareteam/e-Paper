#![no_std]
#![no_main]

use embassy_executor::Spawner;
use embassy_futures::select::select4;
use embassy_rp::gpio::{Input, Level, Output, Pull};
use embassy_rp::spi::{Config as SpiConfig, Spi};
use embassy_time::{Duration, Instant, Timer};
use panic_reset as _;
use defmt_rtt as _;

use recipe_reader_logic::{
    app_logic::{AppState, BtnState, UiEvent},
    assets, data, epd, ui,
};

use crate::data::DATA;
use crate::epd::{AsyncEpd, SSD1680};
use crate::ui::Ui;

use embassy_rp::flash::{Blocking, Flash};
use embassy_rp::peripherals::{FLASH, SPI1};

// Persistence constants
const FLASH_SIZE: usize = 16 * 1024 * 1024;
const PERSIST_OFFSET: u32 = 1024 * 1024;

// Input Tuning
const TICK_MS: u64 = 50;
const BTN_TICK_LONG_PRESS: u16 = 20;
const IDLE_TIMEOUT_TICKS: u32 = (5 * 60 * 1000) / TICK_MS as u32; // 5 minutes

defmt::timestamp!("{=u64:us}", Instant::now().as_micros());

struct Hardware<'d> {
    epd: SSD1680<'d, SPI1>,
    flash: Flash<'d, FLASH, Blocking, FLASH_SIZE>,
    buttons: [Button<'d>; 4],
}

struct Button<'d> {
    pin: Input<'d>,
    state: BtnState,
}

impl<'d> Button<'d> {
    fn new(pin: Input<'d>) -> Self {
        Self {
            pin,
            state: BtnState::new(),
        }
    }

    fn update(&mut self, index: usize) -> Option<UiEvent> {
        let is_pressed = self.pin.is_low();
        if is_pressed {
            self.state.press_duration = self.state.press_duration.saturating_add(1);
            if self.state.press_duration == BTN_TICK_LONG_PRESS && !self.state.handled {
                self.state.handled = true;
                return match index {
                    0 => Some(UiEvent::LongBack), // Up long-press
                    3 => Some(UiEvent::ForceRefresh), // Back long-press
                    _ => None,
                };
            }
        } else {
            let duration = self.state.press_duration;
            let was_handled = self.state.handled;
            self.state.press_duration = 0;
            self.state.handled = false;

            if duration > 0 && !was_handled && duration < BTN_TICK_LONG_PRESS {
                return match index {
                    0 => Some(UiEvent::Up),
                    1 => Some(UiEvent::Down),
                    2 => Some(UiEvent::Select),
                    3 => Some(UiEvent::Back),
                    _ => None,
                };
            }
        }
        None
    }
}

fn pick_refresh_limit() -> u16 {
    15 + (Instant::now().as_ticks() % 6) as u16
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());

    // Stabilize power
    Timer::after(Duration::from_millis(200)).await;

    // --- Peripheral Setup ---
    let mut epd_spi_config = SpiConfig::default();
    epd_spi_config.frequency = 20_000_000;
    
    let spi1 = Spi::new_blocking(p.SPI1, p.PIN_10, p.PIN_11, p.PIN_12, epd_spi_config);
    let mut hw = Hardware {
        epd: SSD1680::new(
            spi1,
            Output::new(p.PIN_9, Level::High),
            Output::new(p.PIN_25, Level::Low),
            Output::new(p.PIN_17, Level::High),
            Input::new(p.PIN_24, Pull::None),
        ),
        flash: Flash::new_blocking(p.FLASH),
        buttons: [
            Button::new(Input::new(p.PIN_19, Pull::Up)), // Up
            Button::new(Input::new(p.PIN_13, Pull::Up)), // Down
            Button::new(Input::new(p.PIN_6, Pull::Up)),  // Select
            Button::new(Input::new(p.PIN_15, Pull::Up)), // Back
        ],
    };

    let mut ui = Ui::new();

    // --- Persistence Management ---
    let current_idx = read_gallery_index(&mut hw.flash);
    write_gallery_index(&mut hw.flash, (current_idx + 1) % assets::COVER_COUNT as usize);

    // --- Initial Render ---
    let _ = hw.epd.init_4grey().await;
    let (b1, b0) = assets::COVERS[current_idx];
    let _ = hw.epd.display_4grey(b1, b0).await;

    // --- Application State ---
    let mut app = App {
        state: AppState::CoverImage,
        category_idx: 0,
        item_idx: 0,
        current_idx,
        total_pages: 0,
        partial_count: 0,
        partial_limit: pick_refresh_limit(),
        idle_ticks: 0,
        needs_refresh: false,
    };

    loop {
        if DATA.is_empty() {
            Timer::after(Duration::from_millis(500)).await;
            continue;
        }

        if app.needs_refresh {
            render_frame(&mut app, &mut ui, &mut hw).await;
            app.needs_refresh = false;
        }

        // Process Input
        let mut events: heapless::Vec<UiEvent, 4> = heapless::Vec::new();
        for (i, btn) in hw.buttons.iter_mut().enumerate() {
            if let Some(event) = btn.update(i) {
                let _ = events.push(event);
            }
        }

        for event in events {
            app.idle_ticks = 0;
            handle_event(&mut app, event, &mut ui, &mut hw).await;
        }

        // Idle Management
        app.idle_ticks = app.idle_ticks.saturating_add(1);
        if app.idle_ticks >= IDLE_TIMEOUT_TICKS {
            let _ = hw.epd.sleep().await;
            let [b0, b1, b2, b3] = &mut hw.buttons;
            let _ = select4(
                b0.pin.wait_for_low(),
                b1.pin.wait_for_low(),
                b2.pin.wait_for_low(),
                b3.pin.wait_for_low(),
            ).await;
            let _ = hw.epd.init().await;
            app.idle_ticks = 0;
            app.needs_refresh = true;
        }

        Timer::after(Duration::from_millis(TICK_MS)).await;
    }
}

struct App {
    state: AppState,
    category_idx: usize,
    item_idx: usize,
    current_idx: usize,
    total_pages: usize,
    partial_count: u16,
    partial_limit: u16,
    idle_ticks: u32,
    needs_refresh: bool,
}

fn read_gallery_index(flash: &mut Flash<FLASH, Blocking, FLASH_SIZE>) -> usize {
    let mut buf = [0u8; 4];
    flash.blocking_read(PERSIST_OFFSET, &mut buf).expect("Flash read failed");
    let idx = u32::from_le_bytes(buf) as usize;
    if idx >= assets::COVER_COUNT { 0 } else { idx }
}

fn write_gallery_index(flash: &mut Flash<FLASH, Blocking, FLASH_SIZE>, index: usize) {
    let val = index as u32;
    flash.blocking_erase(PERSIST_OFFSET, PERSIST_OFFSET + 4096).ok();
    flash.blocking_write(PERSIST_OFFSET, &val.to_le_bytes()).ok();
}

async fn render_frame(app: &mut App, ui: &mut Ui, hw: &mut Hardware<'_>) {
    match app.state {
        AppState::SelectingCategory => {
            let mut items = [""; 32];
            for (i, cat) in DATA.iter().enumerate().take(32) {
                items[i] = cat.name;
            }
            ui.render_menu("Documentation", &items[..DATA.len()], app.category_idx);
        }
        AppState::SelectingItem => {
            let mut items = [""; 32];
            items[0] = "#Description";
            for (i, p) in DATA[app.category_idx].items.iter().enumerate().take(31) {
                items[i + 1] = p.name;
            }
            ui.render_menu(
                DATA[app.category_idx].name,
                &items[..DATA[app.category_idx].items.len() + 1],
                app.item_idx,
            );
        }
        AppState::ViewingCategoryDescription { page } => {
            let cat = &DATA[app.category_idx];
            app.total_pages = ui.render_paged_text(cat.name, &[], cat.description, page);
        }
        AppState::ViewingItem { page } => {
            let item = &DATA[app.category_idx].items[app.item_idx.saturating_sub(1)];
            app.total_pages = ui.render_paged_text(item.name, item.details, item.body, page);
        }
        AppState::CoverImage => {}
    }

    if app.state == AppState::CoverImage {
        if hw.epd.init_4grey().await.is_ok() {
            let (b1, b0) = assets::COVERS[app.current_idx];
            let _ = hw.epd.display_4grey(b1, b0).await;
        }
    } else {
        if app.partial_count >= app.partial_limit {
            if hw.epd.display_base(&ui.buffer.buffer).await.is_ok() {
                app.partial_count = 0;
                app.partial_limit = pick_refresh_limit();
                let _ = hw.epd.init_turbo().await;
            }
        } else if hw.epd.display_turbo(&ui.buffer.buffer).await.is_ok() {
            app.partial_count += 1;
        }
    }
}

async fn handle_event(app: &mut App, event: UiEvent, _ui: &mut Ui, hw: &mut Hardware<'_>) {
    let items_len = if app.state == AppState::SelectingItem {
        DATA[app.category_idx].items.len() + 1
    } else {
        0
    };

    // Gallery cycle escape
    if app.state == AppState::SelectingCategory && event == UiEvent::Back && app.category_idx == 0 {
        app.current_idx = (app.current_idx + 1) % assets::COVER_COUNT;
        write_gallery_index(&mut hw.flash, (app.current_idx + 1) % assets::COVER_COUNT);
        app.state = AppState::CoverImage;
        app.needs_refresh = true;
        return;
    }

    if let Some(new_state) = app.state.next_state(
        event,
        &mut app.category_idx,
        &mut app.item_idx,
        DATA.len(),
        items_len,
        app.total_pages,
    ) {
        if event == UiEvent::ForceRefresh {
            app.partial_count = app.partial_limit;
            let _ = hw.epd.init().await;
        }
        if app.state == AppState::CoverImage && new_state != AppState::CoverImage {
            let _ = hw.epd.init().await;
            app.partial_count = app.partial_limit;
        }
        app.state = new_state;
        app.needs_refresh = true;
    } else {
        app.needs_refresh = true;
    }
}
