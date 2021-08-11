# *****************************************************************************
# * | File        :	  epd3in7.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2020-07-16
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
EPD_WIDTH       = 280
EPD_HEIGHT      = 480

GRAY1  = 0xff #white
GRAY2  = 0xC0 #Close to white
GRAY3  = 0x80 #Close to black
GRAY4  = 0x00 #black

logger = logging.getLogger(__name__)

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.GRAY1  = GRAY1 #white
        self.GRAY2  = GRAY2
        self.GRAY3  = GRAY3 #gray
        self.GRAY4  = GRAY4 #Blackest

    lut_4Gray_GC = [
        0x2A,0x06,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x28,0x06,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x20,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x14,0x06,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x02,0x02,0x0A,0x00,0x00,0x00,0x08,0x08,0x02,
        0x00,0x02,0x02,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x22,0x22,0x22,0x22,0x22
    ]

    lut_1Gray_GC  = [
        0x2A,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x05,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x2A,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x05,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x02,0x03,0x0A,0x00,0x02,0x06,0x0A,0x05,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x22,0x22,0x22,0x22,0x22
    ]

    lut_1Gray_DU  = [
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x01,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x0A,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x05,0x05,0x00,0x05,0x03,0x05,0x05,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x22,0x22,0x22,0x22,0x22
    ]

    lut_1Gray_A2  = [
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x03,0x05,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
        0x22,0x22,0x22,0x22,0x22
    ]
        
    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(5)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200)   


    def send_command(self, command):
        epdconfig.digital_write(self.dc_pin, 0)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte([command])
        epdconfig.digital_write(self.cs_pin, 1)


    def send_data(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte([data])
        epdconfig.digital_write(self.cs_pin, 1)


    def ReadBusy(self):
        logger.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      #  0: idle, 1: busy
            epdconfig.delay_ms(10) 
        logger.debug("e-Paper busy release") 


    def init(self, mode):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(0x12)
        epdconfig.delay_ms(300)
        
        self.send_command(0x46)
        self.send_data(0xF7)
        self.ReadBusy()
        self.send_command(0x47)
        self.send_data(0xF7)
        self.ReadBusy()
        
        self.send_command(0x01) # setting gaet number
        self.send_data(0xDF)
        self.send_data(0x01)
        self.send_data(0x00)

        self.send_command(0x03) # set gate voltage
        self.send_data(0x00)

        self.send_command(0x04) # set source voltage
        self.send_data(0x41)
        self.send_data(0xA8)
        self.send_data(0x32)

        self.send_command(0x11) # set data entry sequence
        self.send_data(0x03)

        self.send_command(0x3C) # set border 
        self.send_data(0x03)
        
        self.send_command(0x0C) # set booster strength
        self.send_data(0xAE)
        self.send_data(0xC7)
        self.send_data(0xC3)
        self.send_data(0xC0)
        self.send_data(0xC0)

        self.send_command(0x18) # set internal sensor on
        self.send_data(0x80)
         
        self.send_command(0x2C) # set vcom value
        self.send_data(0x44)
        
        if(mode == 0):   #4Gray
            self.send_command(0x37) # set display option, these setting turn on previous function
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)  
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)
            self.send_data(0x00)  
        elif(mode == 1):      #1Gray
            self.send_command(0x37) # set display option, these setting turn on previous function
            self.send_data(0x00)     #can switch 1 gray or 4 gray
            self.send_data(0xFF)
            self.send_data(0xFF)
            self.send_data(0xFF)
            self.send_data(0xFF)  
            self.send_data(0x4F)
            self.send_data(0xFF)
            self.send_data(0xFF)
            self.send_data(0xFF)
            self.send_data(0xFF)
        else:
            logger.debug("There is no such mode") 

        self.send_command(0x44) # setting X direction start/end position of RAM
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data(0x17)
        self.send_data(0x01)

        self.send_command(0x45) # setting Y direction start/end position of RAM
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data(0xDF)
        self.send_data(0x01)

        self.send_command(0x22) # Display Update Control 2
        self.send_data(0xCF)
        return 0


    def load_lut(self, lut):
        self.send_command(0x32)
        for i in range(0, 105):
            self.send_data(lut[i])


    def getbuffer(self, image):
        # logger.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # logger.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logger.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logger.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf


    def getbuffer_4Gray(self, image):
        # logger.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width / 4) * self.height)
        image_monocolor = image.convert('L')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        i=0
        # logger.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logger.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if(pixels[x, y] == 0xC0):
                        pixels[x, y] = 0x80
                    elif (pixels[x, y] == 0x80):
                        pixels[x, y] = 0x40
                    i = i + 1
                    if(i%4 == 0):
                        buf[int((x + (y * self.width))/4)] = ((pixels[x-3, y]&0xc0) | (pixels[x-2, y]&0xc0)>>2 | (pixels[x-1, y]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6)
                        
        elif(imwidth == self.height and imheight == self.width):
            logger.debug("Horizontal")
            for x in range(imwidth):
                for y in range(imheight):
                    newx = y
                    newy = imwidth - x - 1
                    if(pixels[x, y] == 0xC0):
                        pixels[x, y] = 0x80
                    elif (pixels[x, y] == 0x80):
                        pixels[x, y] = 0x40
                    i = i + 1
                    if(i%4 == 0):
                        buf[int((newx + (newy * self.width))/4)] = ((pixels[x, y-3]&0xc0) | (pixels[x, y-2]&0xc0)>>2 | (pixels[x, y-1]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6) 
        return buf


    def display_4Gray(self, image):
        if (image == None):
            return            

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F)
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x24)
        for i in range(0, (int)(self.height*(self.width/8))):
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0
                    if(temp2 == 0xC0):
                        temp3 |= 0x01           #white
                    elif(temp2 == 0x00):
                        temp3 |= 0x00           #black
                    elif(temp2 == 0x80):
                        temp3 |= 0x00           #gray1
                    else:                       #0x40
                        temp3 |= 0x01           #gray2
                    temp3 <<= 1
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):          #white
                        temp3 |= 0x01
                    elif(temp2 == 0x00):        #black
                        temp3 |= 0x00
                    elif(temp2 == 0x80):
                        temp3 |= 0x00           #gray1
                    else:                       #0x40
                        temp3 |= 0x01           #gray2
                    if(j!=1 or k!=1):
                        temp3 <<= 1
                    temp1 <<= 2
            self.send_data(temp3)

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F)
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x26)
        for i in range(0, (int)(self.height*(self.width/8))):
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0
                    if(temp2 == 0xC0):
                        temp3 |= 0x01       #white
                    elif(temp2 == 0x00):
                        temp3 |= 0x00       #black
                    elif(temp2 == 0x80):
                        temp3 |= 0x01       #gray1
                    else:                   #0x40
                        temp3 |= 0x00       #gray2
                    temp3 <<= 1
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):      #white
                        temp3 |= 0x01
                    elif(temp2 == 0x00):    #black
                        temp3 |= 0x00
                    elif(temp2 == 0x80):
                        temp3 |= 0x01       #gray1
                    else:                   #0x40
                        temp3 |= 0x00       #gray2
                    if(j!=1 or k!=1):
                        temp3 <<= 1
                    temp1 <<= 2
            self.send_data(temp3)

        self.load_lut(self.lut_4Gray_GC)
        self.send_command(0x22)
        self.send_data(0xC7)
        self.send_command(0x20)
        self.ReadBusy()   


    def display_1Gray(self, image):
        if (image == None):
            return            

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F)
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                self.send_data(image[i + j * int(self.width / 8)])   

        self.load_lut(self.lut_1Gray_A2)
        self.send_command(0x20)
        self.ReadBusy()   
        

    def Clear(self, color, mode):
        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F)
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                self.send_data(0xff)   

        if(mode == 0):              #4Gray
            self.send_command(0x26)
            for j in range(0, self.height):
                for i in range(0, int(self.width / 8)):
                    self.send_data(0xff) 
            self.load_lut(self.lut_4Gray_GC)
            self.send_command(0x22)
            self.send_data(0xC7)
        elif(mode == 1):            #1Gray
            self.load_lut(self.lut_1Gray_DU)
        else:
            logger.debug("There is no such mode") 

        self.send_command(0x20)
        self.ReadBusy()   


    def sleep(self):
        self.send_command(0X50) # DEEP_SLEEP_MODE
        self.send_data(0xf7)
        self.send_command(0X02) #power off
        self.send_command(0X07) #deep sleep
        self.send_data(0xA5)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

