# Waveshare 2.7" V2 (SSD1680) Rust Reference Implementation

This repository provides a high-performance, asynchronous Rust driver and UI framework for the **Waveshare 2.7" E-Paper V2 (SSD1680)**, specifically optimized for the **RP2040** microcontroller using the [Embassy](https://embassy.dev/) framework.

## Key Features

- **"Turbo" Refresh Mode**: A custom differential refresh implementation that achieves sub-200ms updates with zero full-screen flashing.
- **4-Level Grayscale Support**: Capability to render 2-bit grayscale images using custom hardware Waveform LUTs.
- **Asynchronous I/O**: Built on `embassy-rp` for non-blocking SPI communication and power-efficient operation.
- **Dynamic UI Framework**: Includes a word-wrapping text renderer with pagination and a hierarchical menu system.
- **Offline Data Pipeline**: A unified Python-based pipeline for processing local documentation and gallery images.

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
| **KEY 1**   | GPIO 15    | Back     |
| **KEY 2**   | GPIO 6     | Select   |
| **KEY 3**   | GPIO 13    | Down     |
| **KEY 4**   | GPIO 19    | Up       |

## Getting Started

1.  **Install Rust**: Ensure you have a working Rust toolchain with `thumbv6m-none-eabi` target.
2.  **Install Python Dependencies**:
    ```bash
    pip install -r requirements.txt
    ```
3.  **Prepare Assets**:
    - Add `.png` or `.jpg` images to the `gallery/` folder.
    - Edit documentation in the `library_data/` folder.
4.  **Generate Data**:
    ```bash
    python data_gen.py
    ```
5.  **Build and Flash**:
    ```bash
    cargo run --release
    ```

## Data Pipeline

The project uses a unified offline pipeline to transform local assets into embedded Rust code.

### 1. Offline Generation (`data_gen.py`)
This script handles both visual and textual data:
- **Gallery**: Processes all images in `gallery/`, scaling them to fit (264x176) and converting them to 4-level grayscale buffers in `src/assets.rs`.
- **Documentation**: Crawls `library_data/` to build a static hierarchical menu structure in `src/data.rs`.
- **Escaping**: Automatically handles Rust string escaping for special characters.

### 2. Multi-Target Deployment (`deploy.py`)
Used to propagate firmware across multiple e-paper devices or configurations. It manages target-specific builds and binary distribution.

## Project Structure

- `src/epd.rs`: The SSD1680 driver with custom LUTs and refresh logic.
- `src/ui.rs`: Layout and rendering logic (using `embedded-graphics`).
- `src/assets.rs`: Auto-generated image buffers.
- `src/data.rs`: Auto-generated menu structure.
- `src/main.rs`: The main application loop and button handling.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.