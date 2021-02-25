# *****************************************************************************
# * | File        :	  epd7in5bc.py
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
            
        epdconfig.reset(200, 5, 200)

        epdconfig.send_command(0x01) # POWER_SETTING
        epdconfig.send_data(0x37)
        epdconfig.send_data(0x00)
        
        epdconfig.send_command(0x00) # PANEL_SETTING
        epdconfig.send_data(0xCF)
        epdconfig.send_data(0x08)
        
        epdconfig.send_command(0x30) # PLL_CONTROL
        epdconfig.send_data(0x3A) # PLL:  0-15:0x3C, 15+:0x3A
        
        epdconfig.send_command(0x82) # VCM_DC_SETTING
        epdconfig.send_data(0x28) #all temperature  range

        epdconfig.send_command(0x06) # BOOSTER_SOFT_START
        epdconfig.send_data(0xc7)
        epdconfig.send_data(0xcc)
        epdconfig.send_data(0x15)

        epdconfig.send_command(0x50) # VCOM AND DATA INTERVAL SETTING
        epdconfig.send_data(0x77)

        epdconfig.send_command(0x60) # TCON_SETTING
        epdconfig.send_data(0x22)

        epdconfig.send_command(0x65) # FLASH CONTROL
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x61) # TCON_RESOLUTION
        epdconfig.send_data(self.width >> 8) # source 640
        epdconfig.send_data(self.width & 0xff)
        epdconfig.send_data(self.height >> 8) # gate 384
        epdconfig.send_data(self.height & 0xff)

        epdconfig.send_command(0xe5) # FLASH MODE
        epdconfig.send_data(0x03)
        
        return 0

    def getbuffer(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        logging.debug('imwidth = %d  imheight =  %d ',imwidth, imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, imageblack, imagered):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width / 8 * self.height)):
            temp1 = imageblack[i]
            temp2 = imagered[i]
            j = 0
            while (j < 8):
                if ((temp2 & 0x80) == 0x00):
                    temp3 = 0x04                #red
                elif ((temp1 & 0x80) == 0x00):
                    temp3 = 0x00                #black
                else:
                    temp3 = 0x03                #white
					
                temp3 = (temp3 << 4) & 0xFF
                temp1 = (temp1 << 1) & 0xFF
                temp2 = (temp2 << 1) & 0xFF
                j += 1
                if((temp2 & 0x80) == 0x00):
                    temp3 |= 0x04              #red
                elif ((temp1 & 0x80) == 0x00):
                    temp3 |= 0x00              #black
                else:
                    temp3 |= 0x03              #white
                temp1 = (temp1 << 1) & 0xFF
                temp2 = (temp2 << 1) & 0xFF
                epdconfig.send_data(temp3)
                j += 1
                
        epdconfig.send_command(0x04) # POWER ON
        self.ReadBusy()
        epdconfig.send_command(0x12) # display refresh
        epdconfig.delay_ms(100)
        self.ReadBusy()
        
    def Clear(self):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width / 8 * self.height)):
            epdconfig.send_data(0x33)
            epdconfig.send_data(0x33)
            epdconfig.send_data(0x33)
            epdconfig.send_data(0x33)
            
        epdconfig.send_command(0x04) # POWER ON
        self.ReadBusy()
        epdconfig.send_command(0x12) # display refresh
        epdconfig.delay_ms(100)
        self.ReadBusy()

    def sleep(self):
        epdconfig.send_command(0x02) # POWER_OFF
        self.ReadBusy()
        
        epdconfig.send_command(0x07) # DEEP_SLEEP
        epdconfig.send_data(0XA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

