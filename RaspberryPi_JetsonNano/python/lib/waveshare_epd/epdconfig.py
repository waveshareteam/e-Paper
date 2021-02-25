# /*****************************************************************************
# * | File        :	  epdconfig.py
# * | Author      :   Waveshare team
# * | Function    :   Hardware underlying interface
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2019-06-21
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

import os
import logging
import sys
import time


class RaspberryPi:
    # Pin definition
    RST_PIN         = 17
    DC_PIN          = 25
    CS_PIN          = 8
    BUSY_PIN        = 24

    def __init__(self):
        import spidev
        import RPi.GPIO

        self.GPIO = RPi.GPIO
        self.SPI = spidev.SpiDev()

    def digital_write(self, pin, value):
        self.GPIO.output(pin, value)

    def digital_read(self, pin):
        return self.GPIO.input(pin)

    def delay_ms(self, delaytime):
        time.sleep(delaytime / 1000.0)

    def module_init(self):
        self.GPIO.setmode(self.GPIO.BCM)
        self.GPIO.setwarnings(False)
        self.GPIO.setup(self.RST_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.DC_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.CS_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.BUSY_PIN, self.GPIO.IN)

        # SPI device, bus = 0, device = 0
        self.SPI.open(0, 0)
        self.SPI.max_speed_hz = 4000000
        self.SPI.mode = 0b00
        return 0

    def module_exit(self):
        logging.debug("spi end")
        self.SPI.close()

        logging.debug("close 5V, Module enters 0 power consumption ...")
        self.GPIO.output(self.RST_PIN, 0)
        self.GPIO.output(self.DC_PIN, 0)

        self.GPIO.cleanup()
    
    def reset(self, delay_a, delay_b, delay_c):
        self.GPIO.output(self.RST_PIN, 1)
        self.delay_ms(delay_a) 
        self.GPIO.output(self.RST_PIN, 0)
        self.delay_ms(delay_b)
        self.GPIO.output(self.RST_PIN, 1)
        self.delay_ms(delay_c)   

    def send_command(self, command):
        self.GPIO.output(self.DC_PIN, 0)
        self.GPIO.output(self.CS_PIN, 0)
        self.SPI.writebytes([command])
        self.GPIO.output(self.CS_PIN, 1)

    def send_data(self, data):
        self.GPIO.output(self.DC_PIN, 1)
        self.GPIO.output(self.CS_PIN, 0)
        self.SPI.writebytes([data])
        self.GPIO.output(self.CS_PIN, 1)
        
    def send_data2(self, data):
        self.GPIO.output(self.DC_PIN, 1)
        self.GPIO.output(self.CS_PIN, 0)
        self.SPI.writebytes2(data)
        self.GPIO.output(self.CS_PIN, 1)

class JetsonNano:
    # Pin definition
    RST_PIN         = 17
    DC_PIN          = 25
    CS_PIN          = 8
    BUSY_PIN        = 24

    def __init__(self):
        import ctypes
        find_dirs = [
            os.path.dirname(os.path.realpath(__file__)),
            '/usr/local/lib',
            '/usr/lib',
        ]
        self.SPI = None
        for find_dir in find_dirs:
            so_filename = os.path.join(find_dir, 'sysfs_software_spi.so')
            if os.path.exists(so_filename):
                self.SPI = ctypes.cdll.LoadLibrary(so_filename)
                break
        if self.SPI is None:
            raise RuntimeError('Cannot find sysfs_software_spi.so')

        import Jetson.GPIO
        self.GPIO = Jetson.GPIO

    def digital_write(self, pin, value):
        self.GPIO.output(pin, value)

    def digital_read(self, pin):
        return self.GPIO.input(self.BUSY_PIN)

    def delay_ms(self, delaytime):
        time.sleep(delaytime / 1000.0)

    def module_init(self):
        self.GPIO.setmode(self.GPIO.BCM)
        self.GPIO.setwarnings(False)
        self.GPIO.setup(self.RST_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.DC_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.CS_PIN, self.GPIO.OUT)
        self.GPIO.setup(self.BUSY_PIN, self.GPIO.IN)
        self.SPI.SYSFS_software_spi_begin()
        return 0

    def module_exit(self):
        logging.debug("spi end")
        self.SPI.SYSFS_software_spi_end()

        logging.debug("close 5V, Module enters 0 power consumption ...")
        self.GPIO.output(self.RST_PIN, 0)
        self.GPIO.output(self.DC_PIN, 0)

        self.GPIO.cleanup()
        
    def reset(self):
        self.GPIO.output(self.reset_pin, 1)
        self.delay_ms(200) 
        self.GPIO.output(self.reset_pin, 0)
        self.delay_ms(2)
        self.GPIO.output(self.reset_pin, 1)
        self.delay_ms(200)   

    def send_command(self, command):
        self.GPIO.output(self.dc_pin, 0)
        self.GPIO.output(self.cs_pin, 0)
        self.SPI.SYSFS_software_spi_transfer(command)
        self.GPIO.output(self.cs_pin, 1)

    def send_data(self, data):
        self.GPIO.output(self.dc_pin, 1)
        self.GPIO.output(self.cs_pin, 0)
        self.SPI.SYSFS_software_spi_transfer(data)
        self.GPIO.output(self.cs_pin, 1)

if os.path.exists('/sys/bus/platform/drivers/gpiomem-bcm2835'):
    implementation = RaspberryPi()
else:
    implementation = JetsonNano()

for func in [x for x in dir(implementation) if not x.startswith('_')]:
    setattr(sys.modules[__name__], func, getattr(implementation, func))


### END OF FILE ###
