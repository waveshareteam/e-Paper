# /*****************************************************************************
# * | File        :	  epdconfig.py
# * | Author      :   Waveshare electrices
# * | Function    :   Hardware underlying interface
# * | Info        :
# *----------------
# * |	This version:   V1.0
# * | Date        :   2019-11-01
# * | Info        :   
# ******************************************************************************/
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
import time
import os
import logging
import sys

from ctypes import *
import ctypes

EPD_SCK_PIN     =11
EPD_MOSI_PIN    =10

EPD_CS_M_PIN    =8
EPD_CS_S_PIN    =7


EPD_DC_PIN      =25
EPD_RST_PIN     =17
EPD_BUSY_PIN    =24
EPD_PWR_PIN     =18
 
find_dirs = [
    os.path.dirname(os.path.realpath(__file__)),
    '/usr/local/lib',
    '/usr/lib',
]
spi = None
for find_dir in find_dirs:
    val = int(os.popen('getconf LONG_BIT').read())
    val_1 = os.popen("cat /proc/cpuinfo | grep 'Raspberry Pi 5'").read()
    if val == 64:
        if val_1 == "":
            so_filename = os.path.join(find_dir, 'DEV_Config_64_b.so')
        else:
            so_filename = os.path.join(find_dir, 'DEV_Config_64_w.so')
    else:
        if val_1 == "":
            so_filename = os.path.join(find_dir, 'DEV_Config_32_b.so')
        else:
            so_filename = os.path.join(find_dir, 'DEV_Config_32_w.so')
    if os.path.exists(so_filename):
        spi = CDLL(so_filename)
        break
if spi is None:
    RuntimeError('Cannot find DEV_Config.so')

def digital_write(pin, value):
    spi.DEV_Digital_Write(pin, value)

def digital_read(pin):
    return spi.DEV_Digital_Read(pin)

def spi_writebyte(value): 
    spi.DEV_SPI_SendData(value)

def spi_writebyte2(buf, len): 
    array_data = (ctypes.c_ubyte * len)(*buf)
    spi.DEV_SPI_SendData_nByte(array_data, ctypes.c_ulong(len))
 
def delay_ms(delaytime):
    time.sleep(delaytime / 1000.0)
        
def module_init():
    spi.DEV_ModuleInit()

def module_exit():
    spi.DEV_ModuleExit()

  