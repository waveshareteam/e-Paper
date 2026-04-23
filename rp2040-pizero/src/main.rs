#![no_std]
#![no_main]

use core::fmt::Write;
use embassy_executor::Spawner;
use embassy_futures::select::select4;
use embassy_rp::gpio::{Input, Level, Output, Pull};
use embassy_rp::spi::{Config as SpiConfig, Spi};
use embassy_time::{Duration, Instant, Timer};
use heapless::String;
use panic_reset as _;
use defmt_rtt as _;

use recipe_reader_logic::{
    app_logic::{AppState, BtnState, UiEvent},
    assets, data, epd, ui,
};

use crate::data::DATA;
use crate::epd::{AsyncEpd, SSD1680};
use crate::ui::Ui;

// Persistence (16MB Flash)
use embassy_rp::bind_interrupts;
use embassy_rp::dma::InterruptHandler as DmaInterruptHandler;
use embassy_rp::flash::{Blocking, Flash};
use embassy_rp::peripherals::DMA_CH0;

bind_interrupts!(struct Irqs {
    DMA_IRQ_0 => DmaInterruptHandler<DMA_CH0>;
});

const FLASH_SIZE: usize = 16 * 1024 * 1024;
const PERSIST_OFFSET: u32 = 1024 * 1024; // 1MB mark (Safe-Zone for 2MB+ chips)

// --- Input Tuning ---
const TICK_MS: u64 = 50;
const BTN_TICK_LONG_PRESS: u16 = 20; // Ticks for returning to main menu

const IDLE_TIMEOUT_TICKS: u32 = (5 * 60 * 1000) / TICK_MS as u32; // 5 minutes

const KNOWN_SETS: &[&str] = &[
    "Apothecary Potions",
    "Infusions of Feathers",
    "Master Medicinal",
    "Tonics of the Deep Forest",
];

defmt::timestamp!("{=u64:us}", Instant::now().as_micros());

/// Simple helper to check if a set is "Known" (featured)
fn is_set_known(name: &str) -> bool {
    for &k in KNOWN_SETS {
        if k == name {
            return true;
        }
    }
    false
}

/// Pick a random refresh limit between 15 and 20 (inclusive)
/// using the low bits of the hardware timer as entropy.
fn pick_refresh_limit() -> u16 {
    let ticks = Instant::now().as_ticks();
    15 + (ticks % 6) as u16 // 15..=20
}

#[embassy_executor::main]
async fn main(_spawner: Spawner) {
    let p = embassy_rp::init(Default::default());

    // 1. Initial 200ms Delay for power & hardware stabilization
    Timer::after(Duration::from_millis(200)).await;

    // --- E-Paper ---
    let mut epd_spi_config = SpiConfig::default();
    epd_spi_config.frequency = 20_000_000;
    let spi1 = Spi::new_blocking(p.SPI1, p.PIN_10, p.PIN_11, p.PIN_12, epd_spi_config);
    let epd_cs = Output::new(p.PIN_9, Level::High);
    let epd_dc = Output::new(p.PIN_25, Level::Low);
    let epd_rst = Output::new(p.PIN_17, Level::High);
    let epd_busy = Input::new(p.PIN_24, Pull::None);
    let mut epd = SSD1680::new(spi1, epd_cs, epd_dc, epd_rst, epd_busy);
    let mut ui = Ui::new();

    // --- Persistence (Gallery Index) - BLOCKING MODE ---
    let mut flash = Flash::<_, Blocking, FLASH_SIZE>::new_blocking(p.FLASH);
    let mut index_buf = [0u8; 4];
    flash
        .blocking_read(PERSIST_OFFSET, &mut index_buf)
        .expect("Initial index read failed");
    
    let mut current_idx = u32::from_le_bytes(index_buf) as usize;

    // Safety check for first boot or corruption
    if current_idx >= assets::COVER_COUNT {
        current_idx = 0;
    }

    // --- DIRECT INDEX UPDATE ---
    let next_idx = ((current_idx + 1) % assets::COVER_COUNT) as u32;
    let needs_write = true; // In this specific "vibe" logic, we always increment on boot.
    
    if needs_write {
        // Only erase and write if we actually have a new value or need to fix corruption
        flash
            .blocking_erase(PERSIST_OFFSET, PERSIST_OFFSET + 4096)
            .expect("Flash erase failed");

        // Wait for flash controller to stabilize
        Timer::after(Duration::from_millis(10)).await;

        flash
            .blocking_write(PERSIST_OFFSET, &next_idx.to_le_bytes())
            .expect("Flash write failed");

        let mut v_buf = [0u8; 4];
        flash
            .blocking_read(PERSIST_OFFSET, &mut v_buf)
            .expect("Final verify read failed");
        if v_buf != next_idx.to_le_bytes() {
            defmt::error!("Flash sync failed! Expected {:?}, got {:?}", next_idx.to_le_bytes(), v_buf);
        }
    }

    // Now proceed to the slow display process
    epd.init_4grey().await.ok();
    let (b1, b0) = assets::COVERS[current_idx];
    epd.display_4grey(b1, b0).await.ok();

    // --- Buttons ---
    let mut btn_up_pin = Input::new(p.PIN_19, Pull::Up);
    let mut btn_down_pin = Input::new(p.PIN_13, Pull::Up);
    let mut btn_sel_pin = Input::new(p.PIN_6, Pull::Up);
    let mut btn_back_pin = Input::new(p.PIN_15, Pull::Up);

    // --- State ---
    let mut state = AppState::CoverImage;
    let mut category_idx: usize = 0;
    let mut item_idx: usize = 0;
    let mut needs_refresh = false; // Initial refresh done at startup
    let mut total_pages: usize = 0;
    let mut partial_count: u16 = 0;
    let mut partial_limit: u16 = pick_refresh_limit();
    let mut idle_ticks: u32 = 0;

    // Sorting: Known first
    let mut display_to_real_cat = [0usize; 32];
    let mut known_count = 0;
    for (i, cat) in DATA.iter().enumerate().take(32) {
        if is_set_known(cat.name) {
            display_to_real_cat[known_count] = i;
            known_count += 1;
        }
    }
    let mut total_count = known_count;
    for (i, cat) in DATA.iter().enumerate().take(32) {
        if !is_set_known(cat.name) {
            display_to_real_cat[total_count] = i;
            total_count += 1;
        }
    }

    let mut up = BtnState::new();
    let mut down = BtnState::new();
    let mut sel = BtnState::new();
    let mut back = BtnState::new();

    loop {
        if total_count == 0 {
            Timer::after(Duration::from_millis(500)).await;
            continue;
        }

        if needs_refresh {
            match state {
                AppState::SelectingCategory => {
                    let mut items = [""; 32];
                    let mut names: [String<32>; 32] = Default::default();
                    for i in 0..total_count {
                        let real_idx = display_to_real_cat[i];
                        let name = DATA[real_idx].name;
                        if i < known_count {
                            let _ = write!(names[i], "[K] {}", name);
                        } else {
                            let _ = write!(names[i], "{}", name);
                        }
                    }
                    for i in 0..total_count {
                        items[i] = names[i].as_str();
                    }
                    ui.render_menu("Potion Library", &items[..total_count], category_idx);
                }
                AppState::SelectingItem => {
                    let real_cat_idx = display_to_real_cat[category_idx];
                    let mut items = [""; 32];
                    items[0] = "#About this Set";
                    for (i, p) in DATA[real_cat_idx].items.iter().enumerate().take(31) {
                        items[i + 1] = p.name;
                    }
                    ui.render_menu(
                        DATA[real_cat_idx].name,
                        &items[..DATA[real_cat_idx].items.len() + 1],
                        item_idx,
                    );
                }
                AppState::ViewingCategoryDescription { page } => {
                    let real_cat_idx = display_to_real_cat[category_idx];
                    let cat = &DATA[real_cat_idx];
                    total_pages = ui.render_paged_text(cat.name, &[], cat.description, page);
                }
                AppState::ViewingItem { page } => {
                    let real_cat_idx = display_to_real_cat[category_idx];
                    let set_name = DATA[real_cat_idx].name;
                    let item = &DATA[real_cat_idx].items[item_idx - 1];
                    let known = is_set_known(set_name);

                    let (top_details, body_with_effects) = if known {
                        // Known set: Show everything
                        let top = if item.details.len() >= 2 {
                            &item.details[..2]
                        } else {
                            item.details
                        };

                        let mut body: String<4096> = String::new();
                        let _ = write!(body, "#Info\n{}", item.body);

                        // Show all effects starting from index 2 (RP, Mechanical, etc)
                        for i in 2..item.details.len() {
                            let effects = &item.details[i];
                            let _ = write!(body, "\n~\n#{}:\n{}", effects.label, effects.value);
                        }
                        (top, body)
                    } else {
                        // Unknown set: Hide mechanics (Ingredients, Effects)
                        let mut body: String<4096> = String::new();
                        let _ = write!(body, "#Info\n{}", item.body);
                        (Default::default(), body)
                    };

                    total_pages = ui.render_paged_text(
                        item.name,
                        top_details,
                        body_with_effects.as_str(),
                        page,
                    );
                }
                AppState::CoverImage => {
                    // No dynamic UI to render, static image handles it in the EPD layer
                }
            }
            if state == AppState::CoverImage {
                if epd.init_4grey().await.is_ok() {
                    let (b1, b0) = assets::COVERS[current_idx];
                    let _ = epd.display_4grey(b1, b0).await;
                }
            } else {
                if partial_count >= partial_limit {
                    if epd.display_base(&ui.buffer.buffer).await.is_ok() {
                        partial_count = 0;
                        partial_limit = pick_refresh_limit();
                        epd.init_turbo().await.ok(); // Prepared for subsequent partial updates
                    }
                } else {
                    if epd.display_turbo(&ui.buffer.buffer).await.is_ok() {
                        partial_count += 1;
                    }
                }
            }
            needs_refresh = false;
        }

        let phys = [
            !btn_up_pin.is_high(),
            !btn_down_pin.is_high(),
            !btn_sel_pin.is_high(),
            !btn_back_pin.is_high(),
        ];
        let mut btns = [&mut up, &mut down, &mut sel, &mut back];

        let mut events: heapless::Vec<UiEvent, 4> = heapless::Vec::new();

        for (i, b) in btns.iter_mut().enumerate() {
            if phys[i] {
                b.press_duration += 1;
                
                // --- LONG PRESS TRIGGERS (Immediate upon threshold) ---
                // Button 4 (Top Right / Up) -> Escape to Cover
                if i == 0 && b.press_duration == BTN_TICK_LONG_PRESS && !b.handled {
                    let _ = events.push(UiEvent::LongBack); 
                    b.handled = true;
                }
                
                // Key1 (Bottom Right / Back) -> Force Refresh
                if i == 3 && b.press_duration == BTN_TICK_LONG_PRESS && !b.handled {
                    let _ = events.push(UiEvent::ForceRefresh);
                    b.handled = true;
                }

                // Note: No immediate or repeat actions here to prevent interference with long press
            } else {
                // --- RELEASE ACTIONS (Short Press Only) ---
                if !b.handled && b.press_duration > 0 {
                    // Only trigger if pressed for less than long press threshold
                    if b.press_duration < BTN_TICK_LONG_PRESS {
                        match i {
                            0 => { let _ = events.push(UiEvent::Up); }
                            1 => { let _ = events.push(UiEvent::Down); }
                            2 => { let _ = events.push(UiEvent::Select); }
                            3 => { let _ = events.push(UiEvent::Back); }
                            _ => {}
                        }
                    }
                }
                b.press_duration = 0;
                b.handled = false;
            }
        }

        for event in events {
            let real_cat_idx = display_to_real_cat[category_idx];
            let items_len = if state == AppState::SelectingItem {
                DATA[real_cat_idx].items.len() + 1
            } else {
                0
            };

            // Custom logic for returning to cover (cycling)
            if state == AppState::SelectingCategory && event == UiEvent::Back && category_idx == 0 {
                current_idx = (current_idx + 1) % assets::COVER_COUNT;
                let save_idx = ((current_idx + 1) % assets::COVER_COUNT) as u32;
                flash.blocking_erase(PERSIST_OFFSET, PERSIST_OFFSET + 4096).ok();
                flash.blocking_write(PERSIST_OFFSET, &save_idx.to_le_bytes()).ok();
                state = AppState::CoverImage;
                needs_refresh = true;
                continue;
            }

            if let Some(new_state) = state.next_state(event, &mut category_idx, &mut item_idx, total_count, items_len, total_pages) {
                if event == UiEvent::ForceRefresh {
                    partial_count = partial_limit;
                    epd.init().await.ok(); // Explicit re-init for full refresh
                }
                if state == AppState::CoverImage && new_state != AppState::CoverImage {
                    epd.init().await.ok();
                    partial_count = partial_limit; // Force full refresh
                }
                state = new_state;
                needs_refresh = true;
            } else {
                // If next_state returned None but we have an event, it might have modified category_idx/item_idx
                needs_refresh = true;
            }
        }

        if phys.iter().any(|&p| p) {
            idle_ticks = 0;
        } else {
            idle_ticks += 1;
        }

        if idle_ticks >= IDLE_TIMEOUT_TICKS {
            let _ = epd.sleep().await;
            let _ = select4(
                btn_up_pin.wait_for_low(),
                btn_down_pin.wait_for_low(),
                btn_sel_pin.wait_for_low(),
                btn_back_pin.wait_for_low(),
            )
            .await;
            let _ = epd.init().await;
            idle_ticks = 0;
        }

        Timer::after(Duration::from_millis(TICK_MS)).await;
    }
}
