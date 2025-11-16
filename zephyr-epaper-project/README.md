# Waveshare E-Paper Driver for Zephyr OS

This project provides a Zephyr OS driver for the Waveshare 2.9" e-Paper display (EPD 2.9" V2), along with LVGL integration for easy GUI development.

## Features

- **Full Zephyr OS integration** - Native Zephyr driver using SPI and GPIO APIs
- **Multiple refresh modes** - Full, fast, partial, and 4-level grayscale
- **LVGL support** - Manual LVGL integration for rich UI development
- **Multi-board support** - Ready-to-use overlays for nRF52840-DK, nRF52832-DK, and nRF54L15-DK
- **Low power** - Sleep mode support for battery-powered applications

## Hardware

### Supported Displays
- Waveshare 2.9" e-Paper Display V2 (128x296 pixels)
- WEACT Studio ESL 2.9" (compatible with Waveshare 2.9")

### Supported Development Boards
- nRF52840-DK (Board ID: `nrf52840dk/nrf52840/cpuapp`)
- nRF52832-DK (Board ID: `nrf52dk/nrf52832/xxaa`)
- nRF54L15-DK (Board ID: `nrf54l15dk/nrf54l15/cpuapp`)

### Hardware Connections

The e-Paper display connects via SPI with the following pins:

#### nRF52840-DK / nRF52832-DK
| E-Paper | nRF Pin | Arduino Pin |
|---------|---------|-------------|
| MOSI    | P0.22   | D11         |
| SCK     | P0.24   | D13         |
| CS      | P0.25   | D10         |
| RST     | P0.26   | D9          |
| DC      | P0.27   | D8          |
| BUSY    | P0.28   | D7          |
| VCC     | 3.3V    | 3.3V        |
| GND     | GND     | GND         |

#### nRF54L15-DK
| E-Paper | nRF Pin | Arduino Pin |
|---------|---------|-------------|
| MOSI    | P1.11   | D11         |
| SCK     | P1.13   | D13         |
| CS      | P1.10   | D10         |
| RST     | P1.09   | D9          |
| DC      | P1.08   | D8          |
| BUSY    | P1.07   | D7          |
| VCC     | 3.3V    | 3.3V        |
| GND     | GND     | GND         |

## Software Requirements

- Zephyr OS 4.2.1
- Zephyr SDK 0.17.0
- West build tool
- LVGL 9.x (latest version)

## Installation

### 1. Clone the Repository

```bash
cd ~/zephyrproject
git clone <your-repo-url> modules/lib/epaper
```

### 2. Install LVGL

You need to manually add the LVGL library:

```bash
cd modules/lib/epaper/lib/lvgl
git clone https://github.com/lvgl/lvgl.git
```

### 3. Configure Your Application

Add the e-Paper module to your application's `west.yml` or set it as a module:

```yaml
manifest:
  projects:
    - name: epaper
      path: modules/lib/epaper
      url: <your-repo-url>
```

Or set the module path in your CMakeLists.txt:

```cmake
list(APPEND ZEPHYR_EXTRA_MODULES
    /path/to/zephyr-epaper-project
)
```

## Building and Running

### Build the Demo Application

**Note:** Zephyr 4.x uses a new board naming format with slashes.

#### For nRF52840-DK:
```bash
cd zephyr-epaper-project/samples/epaper_demo
west build -b nrf52840dk/nrf52840/cpuapp -p
west flash
```

#### For nRF52832-DK:
```bash
cd zephyr-epaper-project/samples/epaper_demo
west build -b nrf52dk/nrf52832/xxaa -p
west flash
```

#### For nRF54L15-DK:
```bash
cd zephyr-epaper-project/samples/epaper_demo
west build -b nrf54l15dk/nrf54l15/cpuapp -p
west flash
```

### View Serial Output

```bash
minicom -D /dev/ttyACM0 -b 115200
# or
screen /dev/ttyACM0 115200
```

## Usage

### Basic Usage (Without LVGL)

```c
#include <drivers/epd_2in9_v2.h>

const struct device *epd_dev = DEVICE_DT_GET(DT_NODELABEL(epd_2in9_v2));

/* Initialize display */
epd_2in9_v2_init(epd_dev, EPD_REFRESH_FULL);

/* Clear display */
epd_2in9_v2_clear(epd_dev);

/* Create image buffer */
uint8_t image[EPD_2IN9_V2_BUFFER_SIZE];
memset(image, 0xFF, EPD_2IN9_V2_BUFFER_SIZE);  /* White */

/* Draw some pixels (black) */
/* ... your drawing code ... */

/* Display image */
epd_2in9_v2_display(epd_dev, image, EPD_REFRESH_FULL);

/* Put display to sleep */
epd_2in9_v2_sleep(epd_dev);
```

### With LVGL

```c
#include "lvgl.h"
#include "lvgl_epaper_driver.h"

/* Initialize LVGL with e-Paper */
lvgl_epaper_init();

/* Create UI elements */
lv_obj_t *label = lv_label_create(lv_scr_act());
lv_label_set_text(label, "Hello e-Paper!");
lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

/* Start LVGL task handler in a thread */
while (1) {
    lvgl_epaper_task_handler();
    k_msleep(10);
}
```

## Configuration Options

Add these to your `prj.conf`:

### E-Paper Driver
```ini
CONFIG_EPAPER_EPD_2IN9_V2=y
CONFIG_EPAPER_EPD_2IN9_V2_PARTIAL_UPDATE=y
CONFIG_EPAPER_EPD_2IN9_V2_FAST_MODE=y
CONFIG_EPAPER_EPD_2IN9_V2_GRAY4_MODE=y
```

### LVGL
```ini
CONFIG_LVGL=y
CONFIG_LVGL_COLOR_DEPTH=1
CONFIG_LVGL_HOR_RES=128
CONFIG_LVGL_VER_RES=296
CONFIG_LVGL_EPAPER_DRIVER=y
```

## API Reference

### Display Initialization
- `epd_2in9_v2_init()` - Initialize display with specified refresh mode
- `epd_2in9_v2_wakeup()` - Wake display from sleep

### Display Operations
- `epd_2in9_v2_clear()` - Clear display to white
- `epd_2in9_v2_display()` - Display image buffer
- `epd_2in9_v2_display_partial()` - Partial screen update
- `epd_2in9_v2_display_gray4()` - Display 4-level grayscale image

### Power Management
- `epd_2in9_v2_sleep()` - Put display into deep sleep mode

### Refresh Modes
- `EPD_REFRESH_FULL` - Full screen refresh (slower, best quality)
- `EPD_REFRESH_FAST` - Fast refresh (faster, some ghosting)
- `EPD_REFRESH_PARTIAL` - Partial update (fastest, for small changes)
- `EPD_REFRESH_GRAY4` - 4-level grayscale mode

## Device Tree Configuration

To use the e-Paper display in your custom board, add this to your overlay:

```dts
&spi1 {
    status = "okay";

    epd_2in9_v2: epd@0 {
        compatible = "waveshare,epd-2in9-v2";
        reg = <0>;
        spi-max-frequency = <4000000>;
        rst-gpios = <&gpio0 26 GPIO_ACTIVE_LOW>;
        dc-gpios = <&gpio0 27 GPIO_ACTIVE_HIGH>;
        busy-gpios = <&gpio0 28 GPIO_ACTIVE_HIGH>;
        width = <128>;
        height = <296>;
    };
};
```

## Troubleshooting

### Display not updating
- Check SPI connections
- Verify GPIO pins are correctly configured
- Ensure BUSY pin is monitored correctly
- Try different refresh modes

### LVGL not displaying
- Make sure LVGL library is cloned to `lib/lvgl/lvgl/`
- Check that `CONFIG_LVGL=y` is set
- Verify heap size is sufficient (`CONFIG_HEAP_MEM_POOL_SIZE`)

### Build errors
- Ensure Zephyr SDK version is 0.17.0
- Verify all dependencies are installed
- Check that the module path is correctly set

## License

This project is licensed under the Apache 2.0 License. The original Waveshare driver code is used under its MIT license.

## Credits

- Original Waveshare e-Paper driver: Waveshare Team
- Zephyr OS port: [Your Name]
- LVGL: LVGL Team

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.

## References

- [Waveshare e-Paper Documentation](https://www.waveshare.com/wiki/2.9inch_e-Paper_Module)
- [Zephyr Project Documentation](https://docs.zephyrproject.org/)
- [LVGL Documentation](https://docs.lvgl.io/)
