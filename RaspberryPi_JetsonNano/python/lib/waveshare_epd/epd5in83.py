# *****************************************************************************
# * | File        :	  epd5in83.py
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
EPD_WIDTH       = 600
EPD_HEIGHT      = 448

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
        epdconfig.reset(200, 2, 200)
        
        epdconfig.send_command(0x01) # POWER_SETTING
        epdconfig.send_data(0x37)
        epdconfig.send_data(0x00)
        
        epdconfig.send_command(0x00) # PANEL_SETTING
        epdconfig.send_data(0xCF)
        epdconfig.send_data(0x08)
        
        epdconfig.send_command(0x06) # BOOSTER_SOFT_START
        epdconfig.send_data(0xc7)
        epdconfig.send_data(0xcc)
        epdconfig.send_data(0x28)
        
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
        epdconfig.send_data(0x02) # source 600
        epdconfig.send_data(0x58)
        epdconfig.send_data(0x01) # gate 448
        epdconfig.send_data(0xC0)
        
        epdconfig.send_command(0x82) # VCM_DC_SETTING
        epdconfig.send_data(0x1E) # decide by LUT file
        
        epdconfig.send_command(0xe5) # FLASH MODE
        epdconfig.send_data(0x03)
        
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        buf = [0x00] * int(self.width * self.height / 4)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        logging.debug('imwidth = %d  imheight =  %d ',imwidth, imheight)
        if(imwidth == self.width and imheight == self.height):
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] < 64:           # black
                        buf[int((x + y * self.width) / 4)] &= ~(0xC0 >> (x % 4 * 2))
                    elif pixels[x, y] < 192:     # convert gray to red
                        buf[int((x + y * self.width) / 4)] &= ~(0xC0 >> (x % 4 * 2))
                        buf[int((x + y * self.width) / 4)] |= 0x40 >> (x % 4 * 2)
                    else:                           # white
                        buf[int((x + y * self.width) / 4)] |= 0xC0 >> (x % 4 * 2)
        elif(imwidth == self.height and imheight == self.width):
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1                    
                    if pixels[x, y] < 64:           # black
                        buf[int((newx + newy*self.width) / 4)] &= ~(0xC0 >> (y % 4 * 2))
                    elif pixels[x, y] < 192:     # convert gray to red
                        buf[int((newx + newy*self.width) / 4)] &= ~(0xC0 >> (y % 4 * 2))
                        buf[int((newx + newy*self.width) / 4)] |= 0x40 >> (y % 4 * 2)
                    else:                           # white
                        buf[int((newx + newy*self.width) / 4)] |= 0xC0 >> (y % 4 * 2)
        return buf

    def display(self, image):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width / 4 * self.height)):
            temp1 = image[i]
            j = 0
            while (j < 4):
                if ((temp1 & 0xC0) == 0xC0):
                    temp2 = 0x03
                elif ((temp1 & 0xC0) == 0x00):
                    temp2 = 0x00
                else:
                    temp2 = 0x04
                temp2 = (temp2 << 4) & 0xFF
                temp1 = (temp1 << 2) & 0xFF
                j += 1
                if((temp1 & 0xC0) == 0xC0):
                    temp2 |= 0x03
                elif ((temp1 & 0xC0) == 0x00):
                    temp2 |= 0x00
                else:
                    temp2 |= 0x04
                temp1 = (temp1 << 2) & 0xFF
                epdconfig.send_data(temp2)
                j += 1
                
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(100)
        self.ReadBusy()
        
    def Clear(self):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width / 4 * self.height)):
            for j in range(0, 4):
                epdconfig.send_data(0x33)
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

