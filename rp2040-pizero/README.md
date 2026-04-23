# Waveshare 2.7" V2 (SSD1680) Rust Reference Implementation

This repository provides a high-performance, asynchronous Rust driver and UI framework for the **Waveshare 2.7" E-Paper V2 (SSD1680)**, specifically optimized for the **RP2040** microcontroller using the [Embassy](https://embassy.dev/) framework.

## Key Features

- **"Turbo" Refresh Mode**: A custom differential refresh implementation that achieves sub-200ms updates with zero full-screen flashing. This is significantly faster than the default partial update modes.
- **4-Level Grayscale Support**: Capability to render 2-bit grayscale images using custom hardware Waveform LUTs.
- **Asynchronous I/O**: Built on `embassy-rp` for non-blocking SPI communication and power-efficient operation.
- **Dynamic UI Framework**: Includes a word-wrapping text renderer with pagination and a hierarchical menu system.
- **Embedded-HAL Compatible**: The driver is designed to be easily portable to other `embedded-hal` platforms.

## Hardware Support

- **Display**: Waveshare 2.7" E-Paper Module V2 (176x264 resolution, SSD1680 controller).
- **MCU**: Raspberry Pi RP2040 (tested on Pico / Waveshare RP2040-Zero).
- **Interface**: 4-wire SPI.

### Pinout (Default)

| E-Paper Pin | RP2040 Pin | Function |
|-------------|------------|----------|
| BUSY        | GPIO 24    | Busy     |
| RST         | GPIO 17    | Reset    |
| DC          | GPIO 25    | Data/Cmd |
| CS          | GPIO 9     | Chip Sel |
| CLK         | GPIO 10    | SPI SCK  |
| DIN         | GPIO 11    | SPI TX   |

## Getting Started

1.  **Install Rust**: Ensure you have a working Rust toolchain with `thumbv6m-none-eabi` target.
2.  **Hardware Connection**: Connect the display as per the pinout above.
3.  **Build**:
    ```bash
    cargo build --release
    ```
4.  **Flash**: Use `elf2uf2-rs` or copy the `.uf2` file to your RP2040 in bootloader mode.

## "Turbo" Mode Deep Dive

The "Turbo" mode in `epd.rs` uses a hand-crafted Waveform Look-Up Table (LUT) that bypasses the standard e-ink "shaking" cycles. It leverages the SSD1680's dual RAM banks (0x24 and 0x26) to perform hardware-level differential updates.

To use Turbo mode:
1. Initialize with `init()`.
2. Seed the initial state with `display_base(&buffer)`.
3. Subsequent updates can use `display_turbo(&buffer)` for near-instant transitions.

## Data Pipeline

This project uses a multi-stage data pipeline to synchronize content from the backend API into the embedded Rust application.

### 1. Synchronize from API (`sync_data.py`)
Use this script to fetch the latest potion sets, ingredients, and effects from the EmpireCrafting backend.
- **When to use**: Whenever the database is updated or you need to pull the latest Roleplay/Mechanical effects.
- **What it does**: Clears `library_data/` and populates it with a folder hierarchy representing categories and items.
- **Run**: `python sync_data.py`

### 2. Generate Rust Data (`data_gen.py`)
This script crawls the `library_data/` folder and generates a static Rust data structure in `src/data.rs`.
- **When to use**: After running `sync_data.py` or after making manual edits to files in `library_data/`.
- **What it does**: Creates a highly compressed, `&'static` representation of the library that fits in the RP2040's flash memory.
- **Run**: `python data_gen.py`

### 3. Build and Flash
Once `src/data.rs` is updated, recompiling the project will bake the new data into the binary.
- **Run**: `cargo build --release`

## Project Structure

- `src/epd.rs`: The SSD1680 driver with custom LUTs and refresh logic.
- `src/ui.rs`: Layout and rendering logic (using `embedded-graphics`).
- `src/data.rs`: The data structure for the menu system.
- `src/main.rs`: The main application loop and button handling.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.