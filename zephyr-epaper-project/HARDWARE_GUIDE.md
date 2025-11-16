# Hardware Connection Guide

This guide provides detailed information about connecting the Waveshare/WEACT 2.9" e-Paper display to Nordic development boards.

## Pin Descriptions

### E-Paper Display Pins

| Pin  | Description | Direction |
|------|-------------|-----------|
| VCC  | Power supply 3.3V | Input |
| GND  | Ground | - |
| MOSI | SPI Master Out Slave In | Input |
| SCK  | SPI Clock | Input |
| CS   | Chip Select (active low) | Input |
| RST  | Reset (active low) | Input |
| DC   | Data/Command selection | Input |
| BUSY | Busy status output | Output |

### Pin Functions

- **VCC/GND**: Power supply, must be 3.3V (not 5V tolerant)
- **MOSI**: SPI data from MCU to display
- **SCK**: SPI clock signal
- **CS**: Chip select, pull low to select the display
- **RST**: Hardware reset, active low
- **DC**: Data/Command pin. Low=Command, High=Data
- **BUSY**: Display status. High=Busy, Low=Ready

## Connection Diagrams

### nRF52840-DK / nRF52832-DK

```
┌─────────────────┐         ┌──────────────────┐
│  E-Paper 2.9"   │         │   nRF52840-DK    │
├─────────────────┤         ├──────────────────┤
│                 │         │                  │
│  VCC ●─────────────────●  3.3V               │
│  GND ●─────────────────●  GND                │
│  MOSI●─────────────────●  P0.22 (D11)        │
│  SCK ●─────────────────●  P0.24 (D13)        │
│  CS  ●─────────────────●  P0.25 (D10)        │
│  RST ●─────────────────●  P0.26 (D9)         │
│  DC  ●─────────────────●  P0.27 (D8)         │
│  BUSY●─────────────────●  P0.28 (D7)         │
│                 │         │                  │
└─────────────────┘         └──────────────────┘
```

**Pin Mapping Table:**

| E-Paper | GPIO Pin | Arduino Header | Alternative |
|---------|----------|----------------|-------------|
| VCC     | -        | 3.3V           | -           |
| GND     | -        | GND            | -           |
| MOSI    | P0.22    | D11            | SPI1_MOSI   |
| SCK     | P0.24    | D13            | SPI1_SCK    |
| CS      | P0.25    | D10            | SPI1_CS     |
| RST     | P0.26    | D9             | Any GPIO    |
| DC      | P0.27    | D8             | Any GPIO    |
| BUSY    | P0.28    | D7             | Any GPIO    |

### nRF54L15-DK

```
┌─────────────────┐         ┌──────────────────┐
│  E-Paper 2.9"   │         │   nRF54L15-DK    │
├─────────────────┤         ├──────────────────┤
│                 │         │                  │
│  VCC ●─────────────────●  3.3V               │
│  GND ●─────────────────●  GND                │
│  MOSI●─────────────────●  P1.11 (D11)        │
│  SCK ●─────────────────●  P1.13 (D13)        │
│  CS  ●─────────────────●  P1.10 (D10)        │
│  RST ●─────────────────●  P1.09 (D9)         │
│  DC  ●─────────────────●  P1.08 (D8)         │
│  BUSY●─────────────────●  P1.07 (D7)         │
│                 │         │                  │
└─────────────────┘         └──────────────────┘
```

**Pin Mapping Table:**

| E-Paper | GPIO Pin | Arduino Header | Alternative |
|---------|----------|----------------|-------------|
| VCC     | -        | 3.3V           | -           |
| GND     | -        | GND            | -           |
| MOSI    | P1.11    | D11            | SPI22_MOSI  |
| SCK     | P1.13    | D13            | SPI22_SCK   |
| CS      | P1.10    | D10            | SPI22_CS    |
| RST     | P1.09    | D9             | Any GPIO    |
| DC      | P1.08    | D8             | Any GPIO    |
| BUSY    | P1.07    | D7             | Any GPIO    |

## Connection Tips

### Best Practices

1. **Use short wires** - Keep connections under 15cm to minimize noise
2. **Proper grounding** - Connect GND first, disconnect last
3. **Power sequencing** - Power the board before connecting display
4. **Avoid hot-plugging** - Disconnect power before changing connections

### Recommended Wire Gauge

- For prototyping: 22-26 AWG jumper wires
- For permanent installations: Use proper connectors and shielded cables

### Signal Integrity

- **SPI signals** (MOSI, SCK, CS) should use twisted pairs if possible
- Keep BUSY signal separate from high-speed signals
- Route power and ground with adequate current capacity

## Troubleshooting Hardware Issues

### Display not responding

1. **Check power**
   - Measure VCC pin - should be 3.0-3.3V
   - Check current draw - ~25mA during refresh

2. **Verify connections**
   ```bash
   # Use multimeter in continuity mode
   # Check each connection from MCU to display
   ```

3. **Test BUSY signal**
   - Should be HIGH during display refresh
   - Should go LOW when ready

4. **Check SPI signals**
   - Use oscilloscope or logic analyzer
   - SCK should show clock pulses during transfer
   - MOSI should show data

### Ghosting or artifacts

1. **Check power stability**
   - Add 10µF capacitor near display VCC/GND
   - Ensure stable 3.3V supply

2. **Reduce SPI speed**
   - Try 2MHz instead of 4MHz
   - Modify in device tree overlay

3. **Check signal integrity**
   - Shorten wire lengths
   - Add pull-up resistors on CS, DC, RST (4.7kΩ)

### Partial display not working

1. **Verify BUSY connection**
   - Must be properly connected
   - Check it goes HIGH/LOW during operation

2. **Check timing**
   - Some displays need longer delays
   - Modify delay values in driver if needed

## Custom Hardware Modifications

### Using Different Pins

To use different GPIO pins, modify the board overlay file:

```dts
&spi1 {
    epd_2in9_v2: epd@0 {
        compatible = "waveshare,epd-2in9-v2";
        reg = <0>;
        spi-max-frequency = <4000000>;
        rst-gpios = <&gpio0 XX GPIO_ACTIVE_LOW>;   /* Change XX */
        dc-gpios = <&gpio0 YY GPIO_ACTIVE_HIGH>;   /* Change YY */
        busy-gpios = <&gpio0 ZZ GPIO_ACTIVE_HIGH>; /* Change ZZ */
    };
};
```

### Power Optimization

For battery-powered applications:

1. **Use sleep mode** - Call `epd_2in9_v2_sleep()` when not updating
2. **Minimize refreshes** - E-paper retains image without power
3. **Use partial refresh** - Updates only changed regions
4. **Consider VCC switching** - Add MOSFET to completely power off display

### Adding External Buttons

Connect buttons to unused GPIO pins for user interaction:

```dts
/ {
    buttons {
        compatible = "gpio-keys";
        button0: button_0 {
            gpios = <&gpio0 11 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
            label = "Push button 0";
        };
    };
};
```

## Safety Considerations

⚠️ **Important Safety Notes**

1. **Voltage**: E-paper displays are 3.3V only - do NOT use 5V
2. **ESD**: Handle displays with ESD precautions
3. **Current**: Display can draw up to 30mA during refresh
4. **Temperature**: Operating range typically 0-50°C
5. **Humidity**: Avoid high humidity environments

## Display Specifications

### Electrical Characteristics

| Parameter | Min | Typ | Max | Unit |
|-----------|-----|-----|-----|------|
| Supply Voltage | 3.0 | 3.3 | 3.6 | V |
| Logic High | 2.2 | 3.3 | 3.6 | V |
| Logic Low | 0 | - | 0.4 | V |
| Current (refresh) | - | 25 | 30 | mA |
| Current (sleep) | - | <0.1 | 1 | µA |

### Timing Characteristics

| Parameter | Value | Unit |
|-----------|-------|------|
| SPI Clock | 4 | MHz (max) |
| Full Refresh Time | 2-5 | seconds |
| Partial Refresh Time | 0.5-2 | seconds |
| Sleep Entry Time | 100 | milliseconds |

## Additional Resources

- [Waveshare Wiki](https://www.waveshare.com/wiki/2.9inch_e-Paper_Module)
- [Nordic Semiconductor Documentation](https://infocenter.nordicsemi.com/)
- [SPI Protocol Specification](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)

## Support

For hardware-related issues:
1. Check this guide
2. Verify connections with multimeter
3. Test with example code
4. Check Waveshare/WEACT documentation
5. Contact manufacturer support if hardware is faulty
