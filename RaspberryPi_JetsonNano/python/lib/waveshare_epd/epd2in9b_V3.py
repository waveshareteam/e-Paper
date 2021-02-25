# *****************************************************************************
# * | File        :	  epd2in9b_V3.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.1
# * | Date        :   2020-12-03
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
EPD_WIDTH       = 128
EPD_HEIGHT      = 296

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
            
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        epdconfig.send_command(0X71)
        while(epdconfig.digital_read(self.busy_pin) == 0):      #  0: idle, 1: busy
            epdconfig.send_command(0X71)
            epdconfig.delay_ms(200)                
        logging.debug("e-Paper busy release")
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 2, 200)
        
        epdconfig.send_command(0x04)  
        self.ReadBusy()#waiting for the electronic paper IC to release the idle signal

        epdconfig.send_command(0x00)    #panel setting
        epdconfig.send_data(0x0f)   #LUT from OTP,128x296
        epdconfig.send_data(0x89)    #Temperature sensor, boost and other related timing settings

        epdconfig.send_command(0x61)    #resolution setting
        epdconfig.send_data (0x80)  
        epdconfig.send_data (0x01)  
        epdconfig.send_data (0x28)

        epdconfig.send_command(0X50)    #VCOM AND DATA INTERVAL SETTING
        epdconfig.send_data(0x77)   #WBmode:VBDF 17|D7 VBDW 97 VBDB 57
                            # WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
        
        return 0

    def getbuffer(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # logging.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, blackimage, ryimage): # ryimage: red or yellow image
        if (blackimage != None):
            epdconfig.send_command(0X10)
            for i in range(0, int(self.width * self.height / 8)):
                epdconfig.send_data(blackimage[i])        
        if (ryimage != None):
            epdconfig.send_command(0X13)
            for i in range(0, int(self.width * self.height / 8)):
                epdconfig.send_data(ryimage[i])

        epdconfig.send_command(0x12)
        epdconfig.delay_ms(200) 
        self.ReadBusy()
        
    def Clear(self):
        epdconfig.send_command(0X10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xff)
        epdconfig.send_command(0X13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xff)

        epdconfig.send_command(0x12)
        epdconfig.delay_ms(200) 
        self.ReadBusy()
        
    def sleep(self):
        epdconfig.send_command(0X02) # power off
        self.ReadBusy()
        epdconfig.send_command(0X07) # deep sleep
        epdconfig.send_data(0xA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

