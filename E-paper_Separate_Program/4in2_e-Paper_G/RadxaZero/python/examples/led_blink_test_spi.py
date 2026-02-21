#!/usr/bin/env python3
"""
SPI1 MOSI test for Radxa Zero

This script tests SPI1 by sending 100,000 bytes of 0xFF followed by
100,000 bytes of 0x00 repeatedly. At 5MHz SPI clock, this should blink
an LED connected to MOSI at approximately 6.3 times per second.

SPI1 MOSI is on GPIOH_4 (gpio-532, offset 20) - Pin 19 on the 40-pin header.

To test this connect LED +ve to SPI PIN 19 and -ve to GND (pin 6) of the radxa zero amlogic board. 
which is also labeled GPIO447
"""

import time
import spidev

# SPI1 configuration
SPI_BUS = 1      # SPI1
SPI_DEVICE = 0   # CS0 -> /dev/spidev1.0
SPI_SPEED = 5000000  # 5 MHz
SPI_MODE = 0b00  # SPI mode 0

# Number of bytes to send per pattern
BYTES_PER_PATTERN = 100000

def main():
    print("SPI1 MOSI Test for Radxa Zero")
    print("=" * 50)
    print(f"SPI Device: /dev/spidev{SPI_BUS}.{SPI_DEVICE}")
    print(f"Clock Speed: {SPI_SPEED / 1000000:.1f} MHz")
    print(f"Bytes per pattern: {BYTES_PER_PATTERN:,}")
    print("=" * 50)
    
    # Initialize SPI
    spi = spidev.SpiDev()
    
    try:
        # Open SPI device
        print(f"\nOpening SPI bus {SPI_BUS}, device {SPI_DEVICE}...")
        spi.open(SPI_BUS, SPI_DEVICE)
        
        # Configure SPI
        spi.max_speed_hz = SPI_SPEED
        spi.mode = SPI_MODE
        spi.bits_per_word = 8
        
        print(f"SPI configured: {SPI_SPEED} Hz, mode {SPI_MODE}")
        print("\nStarting SPI transmission...")
        print("Pattern: 100,000 bytes of 0xFF, then 100,000 bytes of 0x00")
        print("Press Ctrl+C to stop\n")
        
        # Prepare data buffers
        data_high = bytes([0xFF] * BYTES_PER_PATTERN)
        data_low = bytes([0x00] * BYTES_PER_PATTERN)
        
        cycle_count = 0
        
        try:
            while True:
                cycle_count += 1
                start_time = time.time()
                
                # Send 0xFF bytes
                print(f"Cycle {cycle_count}: Sending {BYTES_PER_PATTERN:,} bytes of 0xFF...", end="", flush=True)
                spi.writebytes2(data_high)
                high_time = time.time() - start_time
                
                # Send 0x00 bytes
                print(f" done ({high_time:.2f}s). Sending {BYTES_PER_PATTERN:,} bytes of 0x00...", end="", flush=True)
                low_start = time.time()
                spi.writebytes2(data_low)
                low_time = time.time() - low_start
                
                total_time = time.time() - start_time
                frequency = 1.0 / total_time
                
                print(f" done ({low_time:.2f}s). Total: {total_time:.2f}s ({frequency:.2f} Hz)")
                
        except KeyboardInterrupt:
            print(f"\n\nStopped after {cycle_count} cycles")

    except Exception as e:
        raise e
    finally:
        try:
            spi.close()
            print("\nSPI device closed.")
        except:
            pass
    
    return 0

if __name__ == '__main__':
    exit(main())
