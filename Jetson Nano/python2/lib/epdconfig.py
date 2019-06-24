# /*****************************************************************************
# * | File        :	  epdconfig.py
# * | Author      :   Waveshare team
# * | Function    :   Hardware underlying interface
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2019-06-06
# * | Info        :   
# ******************************************************************************
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

import Jetson.GPIO as GPIO
import time

import ctypes
# import spidev

# Pin definition
RST_PIN         = 17
DC_PIN          = 25
CS_PIN          = 8
BUSY_PIN        = 24

# SPI device, bus = 0, device = 0
# SPI = spidev.SpiDev(0, 0)

clib = ctypes.cdll.LoadLibrary
spi = clib('../lib/sysfs_software_spi.so')
# gpio2 = clib('../lib/sysfs_gpio.so')

def digital_write(pin, value):
    GPIO.output(pin, value)

def digital_read(pin):
    return GPIO.input(BUSY_PIN)
    # return gpio2.SYSFS_GPIO_Read(15)
    
def delay_ms(delaytime):
    time.sleep(delaytime / 1000.0)

def spi_writebyte(data):
    # SPI.writebytes(data)
    spi.SYSFS_software_spi_transfer(data[0])

def module_init():
    # print("module_init")
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    # print("RST_PIN")
    GPIO.setup(RST_PIN, GPIO.OUT)
    # print("DC_PIN")
    GPIO.setup(DC_PIN, GPIO.OUT)
    # print("CS_PIN")
    GPIO.setup(CS_PIN, GPIO.OUT)
    # print("BUSY_PIN")
    GPIO.setup(BUSY_PIN, GPIO.IN)
    # SPI.max_speed_hz = 2000000
    # SPI.mode = 0b00
    
    # gpio2.SYSFS_GPIO_Export(15)
    # gpio2.SYSFS_GPIO_Direction(15, 0)
    
    spi.SYSFS_software_spi_begin()
    return 0

    
def module_exit():
    print("spi end")
    spi.SYSFS_software_spi_end()
    
    print("close 5V, Module enters 0 power consumption ...")
    GPIO.output(RST_PIN, 0)
    GPIO.output(DC_PIN, 0)
    
    GPIO.cleanup()
    
    

### END OF FILE ###
