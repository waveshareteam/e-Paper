# E-Paper Display Demo Application

This sample demonstrates the use of the Waveshare 2.9" e-Paper display driver with LVGL on Zephyr OS.

## Description

The demo application shows:
- Display initialization
- LVGL integration with e-Paper
- Creating UI elements (labels, boxes, borders)
- Full screen refresh
- Proper display management

## Building

**Note:** This sample is inside the module directory. You must specify the module path when building.

### Method 1: Using Command Line (Recommended)

Navigate to the sample directory, then specify the module path via CMake variable:

```bash
cd zephyr-epaper-project/samples/epaper_demo

# For nRF52840-DK
west build -b nrf52840dk/nrf52840 -p -- -DZEPHYR_EXTRA_MODULES=../..

# For nRF52832-DK
west build -b nrf52dk/nrf52832 -p -- -DZEPHYR_EXTRA_MODULES=../..

# For nRF54L15-DK
west build -b nrf54l15dk/nrf54l15/cpuapp -p -- -DZEPHYR_EXTRA_MODULES=../..
```

### Method 2: Using Environment Variable

```bash
export ZEPHYR_EXTRA_MODULES=/path/to/zephyr-epaper-project
cd zephyr-epaper-project/samples/epaper_demo
west build -b nrf52840dk/nrf52840 -p
```

### Flash the Application

```bash
west flash
```

## Requirements

- One of the supported development boards
- Waveshare 2.9" e-Paper display V2 or WEACT ESL 2.9"
- Jumper wires for connections
- USB cable for programming and power

## Hardware Setup

Connect the e-Paper display to your development board using the pinout specified in the main README.

## Expected Output

The display will show:
1. Initial clear (all white)
2. Demo UI with:
   - Title: "Zephyr OS e-Paper Demo"
   - Box with "WEACT ESL 2.9"" text
   - Display specifications
   - Board information

## Serial Console Output

```
*** Booting Zephyr OS build v4.2.1 ***
[00:00:00.000,000] <inf> epaper_demo: === E-Paper Display Demo ===
[00:00:00.000,000] <inf> epaper_demo: Board: nrf52840dk_nrf52840
[00:00:00.000,000] <inf> epaper_demo: E-Paper device is ready
[00:00:00.100,000] <inf> epd_2in9_v2: Initializing EPD 2.9" V2 display
[00:00:01.000,000] <inf> epd_2in9_v2: EPD 2.9" V2 initialized successfully
[00:00:01.000,000] <inf> lvgl_epaper: LVGL initialized successfully for e-Paper display
[00:00:01.000,000] <inf> epaper_demo: LVGL initialized successfully
[00:00:01.200,000] <inf> epaper_demo: LVGL task started
[00:00:01.200,000] <inf> epaper_demo: Demo UI created and displayed
```

## Customization

You can customize the demo by modifying `src/main.c`:
- Change UI elements in `create_demo_ui()`
- Adjust refresh rates
- Add button handling
- Create animations (note: e-Paper has slow refresh)

## Power Consumption

The e-Paper display only consumes power during refresh. When not updating, it retains the image without power consumption, making it ideal for battery-powered applications.

## Notes

- E-Paper displays have slower refresh rates (1-5 seconds)
- Avoid frequent updates to extend display lifetime
- The display retains content even when powered off
- Use partial refresh mode for better performance when updating small areas
