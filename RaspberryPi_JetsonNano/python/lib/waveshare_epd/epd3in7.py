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
GRAY3  = 0x80 #Close to balck
GRAY4  = 0x00 #balck
class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
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

    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      #  0: idle, 1: busy
            epdconfig.delay_ms(10) 
        logging.debug("e-Paper busy release") 


    def init(self, mode):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(300)
        
        epdconfig.send_command(0x46)
        epdconfig.send_data(0xF7)
        self.ReadBusy()
        epdconfig.send_command(0x47)
        epdconfig.send_data(0xF7)
        self.ReadBusy()
        
        epdconfig.send_command(0x01) # setting gaet number
        epdconfig.send_data(0xDF)
        epdconfig.send_data(0x01)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x03) # set gate voltage
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x04) # set source voltage
        epdconfig.send_data(0x41)
        epdconfig.send_data(0xA8)
        epdconfig.send_data(0x32)

        epdconfig.send_command(0x11) # set data entry sequence
        epdconfig.send_data(0x03)

        epdconfig.send_command(0x3C) # set border 
        epdconfig.send_data(0x03)
        
        epdconfig.send_command(0x0C) # set booster strength
        epdconfig.send_data(0xAE)
        epdconfig.send_data(0xC7)
        epdconfig.send_data(0xC3)
        epdconfig.send_data(0xC0)
        epdconfig.send_data(0xC0)

        epdconfig.send_command(0x18) # set internal sensor on
        epdconfig.send_data(0x80)
         
        epdconfig.send_command(0x2C) # set vcom value
        epdconfig.send_data(0x44)
        
        if(mode == 0):   #4Gray
            epdconfig.send_command(0x37) # set display option, these setting turn on previous function
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)  
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)  
        elif(mode == 1):      #1Gray
            epdconfig.send_command(0x37) # set display option, these setting turn on previous function
            epdconfig.send_data(0x00)     #can switch 1 gray or 4 gray
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)  
            epdconfig.send_data(0x4F)
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)
            epdconfig.send_data(0xFF)
        else:
            logging.debug("There is no such mode") 

        epdconfig.send_command(0x44) # setting X direction start/end position of RAM
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x17)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x45) # setting Y direction start/end position of RAM
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0xDF)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x22) # Display Update Control 2
        epdconfig.send_data(0xCF)
        return 0


    def load_lut(self, lut):
        epdconfig.send_command(0x32)
        for i in range(0, 105):
            epdconfig.send_data(lut[i])


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


    def getbuffer_4Gray(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width / 4) * self.height)
        image_monocolor = image.convert('L')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        i=0
        # logging.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Vertical")
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
            logging.debug("Horizontal")
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

        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x24)
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
            epdconfig.send_data(temp3)

        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x26)
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
            epdconfig.send_data(temp3)

        self.load_lut(self.lut_4Gray_GC)
        epdconfig.send_command(0x22)
        epdconfig.send_data(0xC7)
        epdconfig.send_command(0x20)
        self.ReadBusy()   


    def display_1Gray(self, image):
        if (image == None):
            return            

        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   

        self.load_lut(self.lut_1Gray_A2)
        epdconfig.send_command(0x20)
        self.ReadBusy()   
        

    def Clear(self, color, mode):
        epdconfig.send_command(0x4E)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(0xff)   

        if(mode == 0):              #4Gray
            epdconfig.send_command(0x26)
            for j in range(0, self.height):
                for i in range(0, int(self.width / 8)):
                    epdconfig.send_data(0xff) 
            self.load_lut(self.lut_4Gray_GC)
            epdconfig.send_command(0x22)
            epdconfig.send_data(0xC7)
        elif(mode == 1):            #1Gray
            self.load_lut(self.lut_1Gray_DU)
        else:
            logging.debug("There is no such mode") 

        epdconfig.send_command(0x20)
        self.ReadBusy()   


    def sleep(self):
        epdconfig.send_command(0X50) # DEEP_SLEEP_MODE
        epdconfig.send_data(0xf7)
        epdconfig.send_command(0X02) #power off
        epdconfig.send_command(0X07) #deep sleep
        epdconfig.send_data(0xA5)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

