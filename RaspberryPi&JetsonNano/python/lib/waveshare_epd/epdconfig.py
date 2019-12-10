#!/usr/bin/env python
"""

"""

import spidev
import R64.GPIO as GPIO
import time

__author__ = 'Brian Yiu'
__credits__ = ['Martin Cerveny']
__version__ = '0.1.0'
__maintainer__ = 'Brian Yiu'
__status__ = 'Development'

# Pin definitions
CS_PIN 		= 26  # Orange
DC_PIN 		= 15  # Green
RST_PIN 	= 16  # White
BUSY_PIN 	= 18  # Purple

# SPI device, bus = 32766, device = 1 on ROCK64
SPI = spidev.SpiDev(32766, 1)

def digital_write(pin, value):
	GPIO.output(pin, value)

def digital_read(pin):
	return GPIO.input(pin)

def delay_ms(delaytime):
	time.sleep(delaytime / 1000.0)

def spi_writebyte(data):
	# manual CS on ROCK64
	GPIO.output(CS_PIN, GPIO.LOW)
	SPI.writebytes(data)
	GPIO.output(CS_PIN, GPIO.HIGH)

def module_init():
	GPIO.setmode(GPIO.BOARD)
	GPIO.setwarnings(False)
	GPIO.setup(RST_PIN, GPIO.OUT)
	GPIO.setup(DC_PIN, GPIO.OUT)
	GPIO.setup(CS_PIN, GPIO.OUT, initial=GPIO.HIGH)
	GPIO.setup(BUSY_PIN, GPIO.IN)
	SPI.max_speed_hz = 2000000
	SPI.mode = 0b00
	return 0

def module_exit():
	SPI.close()
	GPIO.output(RST_PIN, GPIO.LOW)
	GPIO.output(DC_PIN, GPIO.LOW)
	GPIO.cleanup()
