# Quick Start Guide

Get your e-Paper display running with Zephyr OS in minutes!

## Prerequisites

1. **Zephyr OS 4.2.1** installed
2. **Zephyr SDK 0.17.0** installed
3. **West** build tool
4. One of the supported boards: nRF52840-DK, nRF52832-DK, or nRF54L15-DK
5. Waveshare 2.9" e-Paper display V2 or WEACT ESL 2.9"

## Step 1: Hardware Setup

Connect your e-Paper display to the development board:

### For nRF52840-DK or nRF52832-DK:
```
E-Paper  →  nRF Board
-------------------------
VCC      →  3.3V
GND      →  GND
MOSI     →  P0.22 (Arduino D11)
SCK      →  P0.24 (Arduino D13)
CS       →  P0.25 (Arduino D10)
RST      →  P0.26 (Arduino D9)
DC       →  P0.27 (Arduino D8)
BUSY     →  P0.28 (Arduino D7)
```

### For nRF54L15-DK:
```
E-Paper  →  nRF Board
-------------------------
VCC      →  3.3V
GND      →  GND
MOSI     →  P1.11 (Arduino D11)
SCK      →  P1.13 (Arduino D13)
CS       →  P1.10 (Arduino D10)
RST      →  P1.09 (Arduino D9)
DC       →  P1.08 (Arduino D8)
BUSY     →  P1.07 (Arduino D7)
```

## Step 2: Setup Software

### Option A: Using as a Zephyr Module

1. Copy this project to your Zephyr modules directory:
```bash
cd ~/zephyrproject
cp -r /path/to/zephyr-epaper-project modules/lib/epaper
```

2. Add to your application's CMakeLists.txt:
```cmake
list(APPEND ZEPHYR_EXTRA_MODULES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/lib/epaper
)
```

### Option B: Standalone Build

1. Navigate to the project directory:
```bash
cd /path/to/zephyr-epaper-project
```

2. Set Zephyr environment:
```bash
source ~/zephyrproject/zephyr/zephyr-env.sh
```

## Step 3: Install LVGL

Clone the latest LVGL library:

```bash
cd lib/lvgl
git clone https://github.com/lvgl/lvgl.git
cd ../../
```

## Step 4: Build and Flash

**Important:** Zephyr 4.x uses new board naming with slashes.

### For nRF52840-DK:
```bash
cd samples/epaper_demo
west build -b nrf52840dk/nrf52840 -p
west flash
```

### For nRF52832-DK:
```bash
cd samples/epaper_demo
west build -b nrf52dk/nrf52832 -p
west flash
```

### For nRF54L15-DK:
```bash
cd samples/epaper_demo
west build -b nrf54l15dk/nrf54l15/cpuapp -p
west flash
```

## Step 5: View Output

Connect to serial console:
```bash
minicom -D /dev/ttyACM0 -b 115200
```

Or:
```bash
screen /dev/ttyACM0 115200
```

## What to Expect

1. The e-Paper display will clear to white
2. A demo UI will appear showing:
   - "Zephyr OS e-Paper Demo" title
   - A bordered box with "WEACT ESL 2.9""
   - Display specifications
   - Board information
3. Serial console will show initialization logs

## Common Issues

### Display stays blank
- **Check connections** - Verify all pins are connected correctly
- **Check power** - Ensure 3.3V is connected
- **Check BUSY pin** - Must be connected for proper operation

### Build fails with "LVGL not found"
- **Install LVGL** - Run: `cd lib/lvgl && git clone https://github.com/lvgl/lvgl.git`

### "Device not ready" error
- **Check device tree** - Ensure overlay file exists for your board
- **Verify SPI** - Make sure SPI peripheral is enabled

### Display shows garbage
- **Check SPI frequency** - Try reducing from 4MHz to 2MHz in overlay
- **Check wiring** - Especially MOSI and SCK pins

## Next Steps

- Modify the demo UI in `samples/epaper_demo/src/main.c`
- Try different refresh modes (fast, partial, grayscale)
- Create your own application using the driver
- Explore LVGL widgets and features

## Need Help?

- Check the main README.md for detailed documentation
- Review the API reference
- Look at the example code in samples/epaper_demo
- Check Zephyr and LVGL documentation

## Performance Tips

1. **Use partial refresh** for small updates
2. **Minimize refresh frequency** - e-Paper has limited refresh cycles
3. **Use sleep mode** when not updating to save power
4. **Batch updates** instead of multiple small refreshes

Happy coding with e-Paper displays! 🎉
