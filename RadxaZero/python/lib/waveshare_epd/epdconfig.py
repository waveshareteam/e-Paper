# epdconfig.py
# Waveshare 4.2" G (B/W/R) for Radxa Zero
# Uses RADXAZero.GPIO + hardware SPI

import os
import time
import logging
import spidev
import RADXAZero.GPIO as GPIO

logger = logging.getLogger(__name__)

# =============================================================================
# SPI CONFIGURATION
# =============================================================================
SPI_DEV = os.getenv("EPD_SPI_DEV", "/dev/spidev1.0")
SPI_HZ = int(os.getenv("EPD_SPI_HZ", "1000000"))


# =============================================================================
# GPIO CONFIGURATION (BOARD numbering)
# =============================================================================
PIN_CS   = 32
PIN_DC   = 7
PIN_RST  = 11
PIN_BUSY = 10

# these are connected but not sure if they are used
# DIN = 19
# CLK = 23 

BUSY_ACTIVE_LOW = os.getenv("EPD_BUSY_ACTIVE_LOW", "0") == "1"

# Backward compatibility
DC_PIN   = PIN_DC
RST_PIN  = PIN_RST
BUSY_PIN = PIN_BUSY
PWR_PIN  = -1
CS_PIN   = PIN_CS

# =============================================================================
# INTERNAL STATE
# =============================================================================
_spi = None
_is_setup = False

# =============================================================================
# GPIO HELPERS (RPi.GPIO compatible)
# =============================================================================
def digital_write(pin, value):
    if pin < 0:
        return
    print(f"GPIO {pin} <- {'H' if value else 'L'}")
    GPIO.output(pin, GPIO.HIGH if value else GPIO.LOW)


def digital_read(pin):
    if pin < 0:
        return 0

    return GPIO.input(pin)


def is_busy():
    if PIN_BUSY < 0:
        return False

    raw = GPIO.input(PIN_BUSY)
    if BUSY_ACTIVE_LOW:
        return raw == GPIO.LOW
    return raw == GPIO.HIGH


def delay_ms(ms):
    time.sleep(ms / 1000.0)

# =============================================================================
# SPI HELPERS
# =============================================================================
def _to_bytes(data):
    if isinstance(data, (bytes, bytearray)):
        return list(data)
    return [int(x) & 0xFF for x in data]

def spi_writebyte(data):
    tx = _to_bytes(data)
    for i in range(0, len(tx), 4096):
        # first argument is data, second argument is speed in HZ, third argument is chunk-to-chunk delay in microseconds
        _spi.xfer3(tx[i:i + 4096], 4000000, 0)




def spi_writebyte2(data):
    spi_writebyte(data)


# =============================================================================
# EPD CONTROL
def epd_reset():
    digital_write(PIN_RST, 0)
    delay_ms(200)
    digital_write(PIN_RST, 1)
    delay_ms(200)

def wait_until_idle(timeout=30):
    start = time.time()
    while is_busy():
        if time.time() - start > timeout:
            raise TimeoutError("EPD busy timeout")
        time.sleep(0.05)

# =============================================================================
# INIT / EXIT
# =============================================================================
def module_init(cleanup=False):
    global _spi, _is_setup

    if _is_setup:
        return 0

    logger.info("Initializing Waveshare 4.2 V2.2 Monochrome EPD")

    # GPIO setup
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(PIN_CS,   GPIO.OUT)   # <<< ADD THIS
    GPIO.output(PIN_CS, GPIO.HIGH)   # CS idle HIGH
    GPIO.setup(PIN_DC,   GPIO.OUT)
    GPIO.setup(PIN_RST,  GPIO.OUT)
    GPIO.setup(PIN_BUSY, GPIO.IN)

    # if PIN_PWR >= 0:
    #     GPIO.setup(PIN_PWR, GPIO.OUT)
    #     GPIO.output(PIN_PWR, GPIO.HIGH)
    #     delay_ms(10)

    # SPI
    _spi = spidev.SpiDev()
    _spi.open(1, 0)           # spidev1.0 → SPI1 CS0
    _spi.mode = 0
    _spi.max_speed_hz = SPI_HZ
    _spi.bits_per_word = 8


    # Hardware reset
    epd_reset()
    wait_until_idle()

    _is_setup = True
    logger.info("EPD initialized successfully")
    return 0

def module_exit(cleanup=False):
    global _spi, _is_setup

    logger.info("Cleaning up EPD")

    if _spi:
        _spi.close()
        _spi = None

    GPIO.cleanup()
    _is_setup = False
    logger.info("Cleanup complete")
