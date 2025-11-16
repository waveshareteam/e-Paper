# Build Instructions for Zephyr 4.2.1

This guide provides the correct build commands for Zephyr OS 4.2.1 with the updated board naming convention.

## Important: Board Names in Zephyr 4.x

Zephyr 4.x uses a new board naming format with slashes instead of underscores:

| Old Format (Zephyr 3.x) | New Format (Zephyr 4.x) |
|-------------------------|-------------------------|
| nrf52840dk_nrf52840 | nrf52840dk/nrf52840 |
| nrf52dk_nrf52832 | nrf52dk/nrf52832 |
| nrf54l15dk_nrf54l15_cpuapp | nrf54l15dk/nrf54l15/cpuapp |

## Step-by-Step Build Guide

### 1. Install LVGL Library

First, clone the LVGL library:

```bash
cd zephyr-epaper-project
./setup_lvgl.sh
```

Or manually:

```bash
cd zephyr-epaper-project/lib/lvgl
git clone https://github.com/lvgl/lvgl.git
```

### 2. Navigate to Sample Directory

```bash
cd zephyr-epaper-project/samples/epaper_demo
```

### 3. Build for Your Board

**Important:** Since the sample is inside the module directory, you must specify the module path using `-DZEPHYR_EXTRA_MODULES=../..`

#### For nRF52840-DK:
```bash
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../..
west flash
```

#### For nRF52832-DK:
```bash
west build -b nrf52dk/nrf52832 -p -- -DZEPHYR_EXTRA_MODULES=../..
west flash
```

#### For nRF54L15-DK:
```bash
west build -b nrf54l15dk/nrf54l15/cpuapp -p -- -DZEPHYR_EXTRA_MODULES=../..
west flash
```

### 4. View Serial Output

```bash
# Linux/macOS
minicom -D /dev/ttyACM0 -b 115200

# Or using screen
screen /dev/ttyACM0 115200

# Windows (using PuTTY or similar)
# Connect to COMx at 115200 baud
```

## Build Options

### Clean Build (Recommended)

Use `-p` or `--pristine` to ensure a clean build:

```bash
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../..
```

### Build Without LVGL

To build without LVGL support, modify `prj.conf`:

```ini
# Comment out LVGL
# CONFIG_LVGL=y
```

Then rebuild:

```bash
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../..
```

### Custom Configuration

Create a custom `.conf` file and use:

```bash
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../.. -DEXTRA_CONF_FILE=my_config.conf
```

## Troubleshooting Build Issues

### Issue: "No board named 'nrf52840dk_nrf52840' found"

**Solution:** Use the new board naming format with slashes:
```bash
# Wrong (old format)
west build -b nrf52840dk_nrf52840

# Correct (new format)
west build -b nrf52840dk/nrf52840
```

### Issue: "Kconfig warnings: undefined symbol LVGL_XXX"

**Solution:** Make sure you've added the module path in CMakeLists.txt:
```cmake
list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
```

This is already fixed in the latest version.

### Issue: "LVGL library not found"

**Solution:** Clone LVGL to the correct location:
```bash
cd zephyr-epaper-project/lib/lvgl
git clone https://github.com/lvgl/lvgl.git
```

### Issue: "Device not ready" at runtime

**Solution:** Check your hardware connections and device tree overlay:
1. Verify all pins are connected correctly
2. Check that the correct overlay file exists for your board
3. Review pin mappings in the HARDWARE_GUIDE.md

### Issue: SPI or GPIO errors

**Solution:** Make sure these are enabled in prj.conf:
```ini
CONFIG_SPI=y
CONFIG_GPIO=y
```

## Build System Details

### Module Registration

The e-Paper driver is registered as a Zephyr module through:
1. `zephyr/module.yml` - Module metadata
2. CMakeLists.txt ZEPHYR_EXTRA_MODULES - Module path
3. Kconfig - Configuration options

### File Naming Convention

Board-specific files use the new naming:
- Overlays: `boards/<board_identifier>.overlay`
  - Example: `nrf52840dk_nrf52840_cpuapp.overlay`
- Configs: `boards/<board_identifier>.conf`
  - Example: `nrf52840dk_nrf52840_cpuapp.conf`

## Advanced Build Options

### Optimizing for Size

Add to your `.conf` file:
```ini
CONFIG_SIZE_OPTIMIZATIONS=y
CONFIG_COMPILER_OPTIMIZATIONS_SIZE=y
```

### Enabling Debug Symbols

```ini
CONFIG_DEBUG=y
CONFIG_DEBUG_OPTIMIZATIONS=y
```

### Adjusting Log Levels

```ini
CONFIG_LOG_DEFAULT_LEVEL=4  # 0=OFF, 1=ERR, 2=WRN, 3=INF, 4=DBG
```

## Building for Custom Hardware

1. Create a custom board directory or overlay
2. Define SPI and GPIO pins for your hardware
3. Create a `.overlay` file:

```dts
&spi1 {
    status = "okay";

    epd_2in9_v2: epd@0 {
        compatible = "waveshare,epd-2in9-v2";
        reg = <0>;
        spi-max-frequency = <4000000>;
        rst-gpios = <&gpio0 XX GPIO_ACTIVE_LOW>;
        dc-gpios = <&gpio0 YY GPIO_ACTIVE_HIGH>;
        busy-gpios = <&gpio0 ZZ GPIO_ACTIVE_HIGH>;
        width = <128>;
        height = <296>;
    };
};
```

4. Build with your custom overlay:

```bash
west build -b <your_board> -p -- -DDTC_OVERLAY_FILE=path/to/your.overlay
```

## Continuous Integration

For CI/CD pipelines:

```bash
# Install dependencies
pip install west

# Initialize workspace
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.2.1
west update

# Build
cd path/to/epaper_demo
west build -b nrf52840dk/nrf52840/cpuapp -p
```

## Getting Help

If you encounter build issues:

1. Check this guide for common solutions
2. Review the main README.md
3. Verify Zephyr and SDK versions:
   ```bash
   west --version
   cmake --version
   ```
4. Check the Zephyr Project documentation
5. Open an issue with:
   - Full build log
   - Zephyr version
   - SDK version
   - Board being used

## Quick Reference

### Build Commands Summary

```bash
# nRF52840-DK
west build -b nrf52840dk/nrf52840 -p && west flash

# nRF52832-DK
west build -b nrf52dk/nrf52832 -p && west flash

# nRF54L15-DK
west build -b nrf54l15dk/nrf54l15/cpuapp -p && west flash
```

### Common West Commands

```bash
west build -b <board> -p          # Pristine build
west flash                        # Flash to board
west debug                        # Start debugger
west clean                        # Clean build files
west boards                       # List available boards
```

---

**Last Updated:** 2025-01-16
**Zephyr Version:** 4.2.1
**SDK Version:** 0.17.0
