# *****************************************************************************
# * | File        :	  epd1in54_V2.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1
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
EPD_WIDTH       = 200
EPD_HEIGHT      = 200

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):
            epdconfig.delay_ms(20)
        logging.debug("e-Paper busy release")

    def TurnOnDisplay(self):
        epdconfig.send_command(0x22) # DISPLAY_UPDATE_CONTROL_2
        epdconfig.send_data(0xF7)
        epdconfig.send_command(0x20) # MASTER_ACTIVATION
        self.ReadBusy()
    
    def TurnOnDisplayPart(self):
        epdconfig.send_command(0x22) # DISPLAY_UPDATE_CONTROL_2
        epdconfig.send_data(0xFF)
        epdconfig.send_command(0x20) # MASTER_ACTIVATION
        self.ReadBusy()

    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
            
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        self.ReadBusy()
        epdconfig.send_command(0x12) # SWRESET
        self.ReadBusy()
        
        epdconfig.send_command(0x01) # DRIVER_OUTPUT_CONTROL
        epdconfig.send_data(0xC7) # (EPD_HEIGHT - 1) & 0xFF
        epdconfig.send_data(0x00) # ((EPD_HEIGHT - 1) >> 8) & 0xFF
        epdconfig.send_data(0x01) # GD = 0 SM = 0 TB = 0
        
        epdconfig.send_command(0x11) # data entry mode
        epdconfig.send_data(0x01)
        
        epdconfig.send_command(0x44) # set Ram-X address start/end position
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x18) # 0x0C-->(18+1)*8=200
        
        epdconfig.send_command(0x45) # set Ram-Y address start/end position
        epdconfig.send_data(0xC7) # 0xC7-->(199+1)=200
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x3C) # BorderWavefrom
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x18)
        epdconfig.send_data(0x80)

        epdconfig.send_command(0x22) # #Load Temperature and waveform setting.
        epdconfig.send_data(0XB1)
        epdconfig.send_command(0x20)

        epdconfig.send_command(0x4E) # set RAM x address count to 0;
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F) # set RAM y address count to 0X199;
        epdconfig.send_data(0xC7)
        epdconfig.send_data(0x00)
        
        self.ReadBusy()
        
    def Clear(self, color):
        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(color)
                
        self.TurnOnDisplay()
        
    def getbuffer(self, image):
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
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

    def display(self, image):
        if (image == None):
            return
            
        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   
        self.TurnOnDisplay()
        
    def displayPartBaseImage(self, image):
        if (image == None):
            return
        
        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])
        
        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])
                
        self.TurnOnDisplayPart()
        
    def displayPart(self, image):
        if (image == None):
            return
            
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(10)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(10)   
        
        epdconfig.send_command(0x3c)
        epdconfig.send_data(0x80)
        
        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])
                
        self.TurnOnDisplayPart()
        
    def sleep(self):
        epdconfig.send_command(0x10) # DEEP_SLEEP_MODE
        epdconfig.send_data(0x01)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

