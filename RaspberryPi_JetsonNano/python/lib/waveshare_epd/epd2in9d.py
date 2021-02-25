# *****************************************************************************
# * | File        :	  epd2in9d.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V2.0
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
from PIL import Image
import RPi.GPIO as GPIO

# Display resolution
EPD_WIDTH       = 128
EPD_HEIGHT      = 296

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    lut_vcomDC = [  
        0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
        0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
        0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
    ]

    lut_ww = [  
        0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
        0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
        0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_bw = [  
        0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x0F, 0x0F, 0x00, 0x00, 0x03,
        0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
        0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_wb = [
        0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
        0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
        0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_bb = [ 
        0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
        0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
        0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
        0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    
    lut_vcom1 = [  
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
    ]

    lut_ww1 = [  
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_bw1 = [  
        0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_wb1 = [
        0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]

    lut_bb1 = [ 
        0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ] 
        
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.send_command(0x71)
            epdconfig.delay_ms(100)  
        logging.debug("e-Paper busy release")
    def TurnOnDisplay(self):
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(10)
        self.ReadBusy()
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x01)	# POWER SETTING
        epdconfig.send_data(0x03)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x2b)
        epdconfig.send_data(0x2b)
        epdconfig.send_data(0x03)

        epdconfig.send_command(0x06)	# boost soft start
        epdconfig.send_data(0x17) # A
        epdconfig.send_data(0x17) # B
        epdconfig.send_data(0x17) # C

        epdconfig.send_command(0x04)
        self.ReadBusy()

        epdconfig.send_command(0x00)	# panel setting
        epdconfig.send_data(0xbf) # LUT from OTP,128x296
        epdconfig.send_data(0x0d) # VCOM to 0V fast

        epdconfig.send_command(0x30)	#PLL setting
        epdconfig.send_data(0x3a)     # 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

        epdconfig.send_command(0x61)	# resolution setting
        epdconfig.send_data(self.width)
        epdconfig.send_data((self.height >> 8) & 0xff)
        epdconfig.send_data(self.height& 0xff)

        epdconfig.send_command(0x82)	# vcom_DC setting
        epdconfig.send_data(0x28)
        return 0
        
    def SetFullReg(self):
        epdconfig.send_command(0x82)
        epdconfig.send_data(0x00)
        epdconfig.send_command(0X50)
        epdconfig.send_data(0x97)
        
        epdconfig.send_command(0x20)         # vcom
        for count in range(0, 44):
            epdconfig.send_data(self.lut_vcomDC[count])
        epdconfig.send_command(0x21)         # ww --
        for count in range(0, 42):
            epdconfig.send_data(self.lut_ww[count])
        epdconfig.send_command(0x22)         # bw r
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bw[count])
        epdconfig.send_command(0x23)         # wb w
        for count in range(0, 42):
            epdconfig.send_data(self.lut_wb[count])
        epdconfig.send_command(0x24)         # bb b
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bb[count])
    
    def SetPartReg(self):
        epdconfig.send_command(0x82)
        epdconfig.send_data(0x03)
        epdconfig.send_command(0X50)
        epdconfig.send_data(0x47)
        
        epdconfig.send_command(0x20)         # vcom
        for count in range(0, 44):
            epdconfig.send_data(self.lut_vcom1[count])
        epdconfig.send_command(0x21)         # ww --
        for count in range(0, 42):
            epdconfig.send_data(self.lut_ww1[count])
        epdconfig.send_command(0x22)         # bw r
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bw1[count])
        epdconfig.send_command(0x23)         # wb w
        for count in range(0, 42):
            epdconfig.send_data(self.lut_wb1[count])
        epdconfig.send_command(0x24)         # bb b
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bb1[count])

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
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0x00)
        epdconfig.delay_ms(10)
        
        epdconfig.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(image[i])
        epdconfig.delay_ms(10)
        
        self.SetFullReg()
        self.TurnOnDisplay()
        
    def DisplayPartial(self, image):
        self.SetPartReg()
        epdconfig.send_command(0x91)
        epdconfig.send_command(0x90)
        epdconfig.send_data(0)
        epdconfig.send_data(self.width - 1)

        epdconfig.send_data(0)
        epdconfig.send_data(0)
        epdconfig.send_data(int(self.height / 256))
        epdconfig.send_data(self.height % 256 - 1)
        epdconfig.send_data(0x28)
            
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(image[i])
        epdconfig.delay_ms(10)
        
        epdconfig.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(~image[i])
        epdconfig.delay_ms(10)
          
        self.TurnOnDisplay()
        
    def Clear(self, color):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0x00)
        epdconfig.delay_ms(10)
        
        epdconfig.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xFF)
        epdconfig.delay_ms(10)
        
        self.SetFullReg()
        self.TurnOnDisplay()

    def sleep(self):
        epdconfig.send_command(0X50)
        epdconfig.send_data(0xf7)
        epdconfig.send_command(0X02)         #power off
        epdconfig.send_command(0X07)         #deep sleep  
        epdconfig.send_data(0xA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

