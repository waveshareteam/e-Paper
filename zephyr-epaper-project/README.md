# Waveshare E-Paper Driver for Zephyr OS

A Zephyr OS 4.2.1 driver module for Waveshare 2.9" V2 e-Paper displays (EPD), compatible with WEACT Studio ESL 2.9" displays.

## Features

- Full driver support for Waveshare EPD 2.9" V2 display (128x296 resolution)
- Multiple refresh modes: Full, Partial, Fast, and 4-level Grayscale
- SPI interface with Zephyr HAL
- Device tree integration
- Support for Nordic nRF52 and nRF54 boards
- Optional LVGL integration for GUI applications
- Sample application included

## Module Structure (Zephyr 4.2.1 Compliant)

```
zephyr-epaper-project/
├── zephyr/
│   ├── module.yml              # Module metadata
│   └── CMakeLists.txt          # Module build logic
├── drivers/epaper/             # Driver implementation
├── include/drivers/            # Public API headers
├── dts/                        # Device tree bindings & overlays
├── samples/epaper_demo/        # Sample application
├── Kconfig                     # Driver configuration
└── README.md
```

## Quick Start

```bash
cd zephyr-epaper-project/samples/epaper_demo
west build -b nrf52840dk/nrf52840 -p
west flash
```

See BUILD_INSTRUCTIONS.md for detailed build information.
