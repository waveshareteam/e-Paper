# *****************************************************************************
# * | File        :	  epd7in5.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V4.0
# * | Date        :   2019-06-20
# # | Info        :   python demo
# -----------------------------------------------------------------------------
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


import logging
from . import epdconfig

# Display resolution
EPD_WIDTH       = 640
EPD_HEIGHT      = 384

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    
        logging.debug("e-Paper busy release")
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x01) # POWER_SETTING
        epdconfig.send_data2([0x37, 0x00])
        
        epdconfig.send_command(0x00) # PANEL_SETTING
        epdconfig.send_data2([0xCF, 0x08])
        
        epdconfig.send_command(0x06) # BOOSTER_SOFT_START
        epdconfig.send_data2([0xc7, 0xcc, 0x28])
        
        epdconfig.send_command(0x04) # POWER_ON
        self.ReadBusy()
        
        epdconfig.send_command(0x30) # PLL_CONTROL
        epdconfig.send_data(0x3c)
        
        epdconfig.send_command(0x41) # TEMPERATURE_CALIBRATION
        epdconfig.send_data(0x00)
        
        epdconfig.send_command(0x50) # VCOM_AND_DATA_INTERVAL_SETTING
        epdconfig.send_data(0x77)
        
        epdconfig.send_command(0x60) # TCON_SETTING
        epdconfig.send_data(0x22)
        
        epdconfig.send_command(0x61) # TCON_RESOLUTION
        epdconfig.send_data(EPD_WIDTH >> 8)     #source 640
        epdconfig.send_data(EPD_WIDTH & 0xff)
        epdconfig.send_data(EPD_HEIGHT >> 8)     #gate 384
        epdconfig.send_data(EPD_HEIGHT & 0xff)
        
        epdconfig.send_command(0x82) # VCM_DC_SETTING
        epdconfig.send_data(0x1E) # decide by LUT file
        
        epdconfig.send_command(0xe5) # FLASH MODE
        epdconfig.send_data(0x03)
        
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        img = image
        imwidth, imheight = img.size
        halfwidth = int(self.width / 2)
        buf = [0x33] * halfwidth * self.height
        
        if(imwidth == self.width and imheight == self.height):
            img = img.convert('1')
        elif(imwidth == self.height and imheight == self.width):
            img = img.rotate(90, expand=True).convert('1')
            imwidth, imheight = img.size
        else:
            logging.warning("Wrong image dimensions: must be " + str(self.width) + "x" + str(self.height))
            # return a blank buffer
            return buf
        
        pixels = img.load()

        for y in range(imheight):
            offset = y * halfwidth
            for x in range(1, imwidth, 2):
                i = offset + x // 2
                if(pixels[x-1, y] > 191):
                    if(pixels[x, y] > 191):
                        buf[i] = 0x33
                    else:
                        buf[i] = 0x30
                else:
                    if(pixels[x, y] > 191):
                        buf[i] = 0x03
                    else:
                        buf[i] = 0x00
        return buf
        
    def display(self, image):
        epdconfig.send_command(0x10)
        epdconfig.send_data2(image)
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(100)
        self.ReadBusy()
        
    def Clear(self):
        buf = [0x33] * int(self.width * self.height / 2)
        epdconfig.send_command(0x10)
        epdconfig.send_data2(buf)
        epdconfig.send_command(0x12)
        self.ReadBusy()

    def sleep(self):
        epdconfig.send_command(0x02) # POWER_OFF
        self.ReadBusy()
        
        epdconfig.send_command(0x07) # DEEP_SLEEP
        epdconfig.send_data(0XA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

