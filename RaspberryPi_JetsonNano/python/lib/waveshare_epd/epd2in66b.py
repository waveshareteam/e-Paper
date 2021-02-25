# *****************************************************************************
# * | File        :	  epd2in66b.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2020-12-01
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
EPD_WIDTH       = 152
EPD_HEIGHT      = 296

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      #  0: idle, 1: busy
            epdconfig.delay_ms(20) 
        logging.debug("e-Paper busy release") 


    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(30)
        self.ReadBusy()

        epdconfig.send_command(0x11) # setting gaet number
        epdconfig.send_data(0x03)
        
        self.setWindows(0, 0, self.width-1, self.height-1)
        
        epdconfig.send_command(0x21)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x80)
        
        self.setCursor(0, 0)
        self.ReadBusy()

        return 0

    def setWindows(self, Xstart, Ystart, Xend, Yend):
        epdconfig.send_command(0x44); # SET_RAM_X_ADDRESS_START_END_POSITION
        epdconfig.send_data((Xstart>>3) & 0x1F);
        epdconfig.send_data((Xend>>3) & 0x1F);
        
        epdconfig.send_command(0x45); # SET_RAM_Y_ADDRESS_START_END_POSITION
        epdconfig.send_data(Ystart & 0xFF);
        epdconfig.send_data((Ystart >> 8) & 0x01);
        epdconfig.send_data(Yend & 0xFF);
        epdconfig.send_data((Yend >> 8) & 0x01);

    def setCursor(self, Xstart, Ystart):
        epdconfig.send_command(0x4E); # SET_RAM_X_ADDRESS_COUNTER
        epdconfig.send_data(Xstart & 0x1F);

        epdconfig.send_command(0x4F); # SET_RAM_Y_ADDRESS_COUNTER
        epdconfig.send_data(Ystart & 0xFF);
        epdconfig.send_data((Ystart >> 8) & 0x01);
        
    def turnon_display(self):
        epdconfig.send_command(0x20)
        self.ReadBusy()

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

    def display(self, Blackimage, Redimage):
        if (Blackimage == None or Redimage == None):
            return            

        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x01)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x27)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(Blackimage[i + j * int(self.width / 8)])   

        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(~Redimage[i + j * int(self.width / 8)]) 
                
        self.turnon_display()
        

    def Clear(self):

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(0xff)   

        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(0x00) 

        self.turnon_display()


    def sleep(self):
        epdconfig.send_command(0X10) # DEEP_SLEEP_MODE
        epdconfig.send_data(0x01)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

