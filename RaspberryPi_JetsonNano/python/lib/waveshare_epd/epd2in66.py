# *****************************************************************************
# * | File        :	  epd2in66.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2020-07-22
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

    WF_PARTIAL = [
        0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x40,0x40,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x0A,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x00,0x00,
        0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x22,0x22,0x22,
        0x00,0x00,0x00,0x22,0x17,0x41,0xB0,0x32,0x36,
    ]

    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      #  0: idle, 1: busy
            epdconfig.delay_ms(200) 
        logging.debug("e-Paper busy release") 


    def init(self, mode):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(300)
        self.ReadBusy()

        epdconfig.send_command(0x11) # setting gaet number
        epdconfig.send_data(0x03)
        epdconfig.send_command(0x44) # set gate voltage
        epdconfig.send_data(0x01)
        epdconfig.send_data(0x13)
        epdconfig.send_command(0x45) # set source voltage
        epdconfig.send_data(0x0)
        epdconfig.send_data(0x0)
        epdconfig.send_data(0x28)
        epdconfig.send_data(0x01)
    
        if(mode == 0):      #full
            epdconfig.send_command(0x3C)
            epdconfig.send_data(0x01)
            
        elif(mode == 1):        #partial
            self.load_lut(self.WF_PARTIAL)
            epdconfig.send_command(0x37) # set display option, these setting turn on previous function
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)  
            epdconfig.send_data(0x40)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)  

            epdconfig.send_command(0x3C)
            epdconfig.send_data(0x80)

            epdconfig.send_command(0x22)
            epdconfig.send_data(0xcf)
            
            epdconfig.send_command(0x20)
            self.ReadBusy()

        else:
            logging.debug("There is no such mode") 

        return 0


    def load_lut(self, lut):
        epdconfig.send_command(0x32)
        for i in range(0, 153):
            epdconfig.send_data(lut[i])


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


    def display(self, image):
        if (image == None):
            return            

        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x01)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x27)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   

        self.turnon_display()
        

    def Clear(self):
        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x01)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x27)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(0xff)   

        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(0xff) 

        self.turnon_display()


    def sleep(self):
        epdconfig.send_command(0X10) # DEEP_SLEEP_MODE
        epdconfig.send_data(0x01)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

