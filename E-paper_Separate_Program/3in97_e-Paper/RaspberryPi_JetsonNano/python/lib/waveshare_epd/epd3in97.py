# *****************************************************************************
# * | File        :	  epd2in15g.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2024-08-07
# # | Info        :   python demo
# -----------------------------------------------------------------------------
# ******************************************************************************/
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

import PIL
from PIL import Image
import io

# Display resolution
EPD_WIDTH       = 800
EPD_HEIGHT      = 480

GRAY1  = 0xff #white
GRAY2  = 0xC0
GRAY3  = 0x80 #gray
GRAY4  = 0x00 #Blackest

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

        
    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, 0)         # module reset
        epdconfig.delay_ms(2)
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

    # send a lot of data   
    def send_data2(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte2(data)
        epdconfig.digital_write(self.cs_pin, 1)
        
    def ReadBusy(self):
        logger.debug("e-Paper busy H")
        epdconfig.delay_ms(100)
        while(epdconfig.digital_read(self.busy_pin) == 1):      # 0: idle, 1: busy
            epdconfig.delay_ms(5)
        logger.debug("e-Paper busy release")
        
    def TurnOnDisplay(self):
        self.send_command(0x22)
        self.send_data(0xF7)
        self.send_command(0x20)
        self.ReadBusy()

    def TurnOnDisplay_Fast(self):
        self.send_command(0x22)
        self.send_data(0xD7)
        self.send_command(0x20)
        self.ReadBusy()

    def TurnOnDisplay_4GRAY(self):
        self.send_command(0x22)
        self.send_data(0xD7)
        self.send_command(0x20)
        self.ReadBusy()

    def TurnOnDisplay_Partial(self):
        self.send_command(0x22)
        self.send_data(0xFF)
        self.send_command(0x20)
        self.ReadBusy()
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()

        self.ReadBusy()
        self.send_command(0x12) 
        self.ReadBusy()

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x0C)
        self.send_data(0xAE)
        self.send_data(0xC7)
        self.send_data(0xC3)
        self.send_data(0xC0)
        self.send_data(0x80)

        self.send_command(0x01)
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x02)

        self.send_command(0x3C)
        self.send_data(0x01)

        self.send_command(0x11)     
        self.send_data(0x01)

        self.send_command(0x44) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data(int((self.width-1)%256))    
        self.send_data(int((self.width-1)/256))

        self.send_command(0x45) 
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.ReadBusy()

    def init_Fast(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()

        self.ReadBusy()
        self.send_command(0x12) 
        self.ReadBusy()

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x0C)
        self.send_data(0xAE)
        self.send_data(0xC7)
        self.send_data(0xC3)
        self.send_data(0xC0)
        self.send_data(0x80)

        self.send_command(0x01)
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x02)

        self.send_command(0x11)     
        self.send_data(0x01)

        self.send_command(0x44) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data(int((self.width-1)%256))    
        self.send_data(int((self.width-1)/256))

        self.send_command(0x45) 
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.ReadBusy()

        self.send_command(0x3C)
        self.send_data(0x01)

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x1A)
        self.send_data(0x6A)

    def init_4GRAY(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()

        self.ReadBusy()
        self.send_command(0x12) 
        self.ReadBusy()

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x0C)
        self.send_data(0xAE)
        self.send_data(0xC7)
        self.send_data(0xC3)
        self.send_data(0xC0)
        self.send_data(0x80)

        self.send_command(0x01)
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x02)

        self.send_command(0x11)     
        self.send_data(0x01)

        self.send_command(0x44) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data(int((self.width-1)%256))    
        self.send_data(int((self.width-1)/256))

        self.send_command(0x45) 
        self.send_data(int((self.height-1)%256))
        self.send_data(int((self.height-1)/256))
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x4E)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_command(0x4F) 
        self.send_data(0x00)
        self.send_data(0x00)
        self.ReadBusy()

        self.send_command(0x3C)
        self.send_data(0x01)

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x1A)
        self.send_data(0x5A)


    def getbuffer(self, image):
        img = image
        imwidth, imheight = img.size
        if(imwidth == self.width and imheight == self.height):
            img = img.convert('1')
        elif(imwidth == self.height and imheight == self.width):
            # image has correct dimensions, but needs to be rotated
            img = img.rotate(90, expand=True).convert('1')
        else:
            logger.warning("Wrong image dimensions: must be " + str(self.width) + "x" + str(self.height))
            # return a blank buffer
            return [0x00] * (int(self.width/8) * self.height)

        buf = bytearray(img.tobytes('raw'))
        # The bytes need to be inverted, because in the PIL world 0=black and 1=white, but
        # in the e-paper world 0=white and 1=black.
        # for i in range(len(buf)):
        #     buf[i] ^= 0xFF
        return buf
    
    def getbuffer_Part(self, image, width, height):
        img = image
        imwidth, imheight = img.size
        if(imwidth == width and imheight == height):
            img = img.convert('1')
        elif(imwidth == height and imheight == width):
            # image has correct dimensions, but needs to be rotated
            img = img.rotate(90, expand=True).convert('1')
        else:
            logger.warning("Wrong image dimensions: must be " + str(width) + "x" + str(height))
            # return a blank buffer
            return [0x00] * (int(width/8) * height)

        buf = bytearray(img.tobytes('raw'))
        # The bytes need to be inverted, because in the PIL world 0=black and 1=white, but
        # in the e-paper world 0=white and 1=black.
        for i in range(len(buf)):
            buf[i] ^= 0xFF
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
                    i= i+1
                    if(i%4 == 0):
                        buf[int((x + (y * self.width))/4)] = ((pixels[x-3, y]&0xc0) | (pixels[x-2, y]&0xc0)>>2 | (pixels[x-1, y]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6)
                        
        elif(imwidth == self.height and imheight == self.width):
            logger.debug("Horizontal")
            for x in range(imwidth):
                for y in range(imheight):
                    newx = y
                    newy = self.height - x - 1
                    if(pixels[x, y] == 0xC0):
                        pixels[x, y] = 0x80
                    elif (pixels[x, y] == 0x80):
                        pixels[x, y] = 0x40
                    i= i+1
                    if(i%4 == 0):
                        buf[int((newx + (newy * self.width))/4)] = ((pixels[x, y-3]&0xc0) | (pixels[x, y-2]&0xc0)>>2 | (pixels[x, y-1]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6) 
        return buf

    def display(self, image):
        self.send_command(0x24)
        self.send_data2(image)

        self.TurnOnDisplay()

    def display_Base(self, image):
        self.send_command(0x24)
        self.send_data2(image)

        self.send_command(0x26)
        self.send_data2(image)

        self.TurnOnDisplay()

    def display_Fast(self, image):
        self.send_command(0x24)
        self.send_data2(image)

        self.TurnOnDisplay()

    def display_Fast_Base(self, image):
        self.send_command(0x24)
        self.send_data2(image)

        self.send_command(0x26)
        self.send_data2(image)

        self.TurnOnDisplay()


    def Clear(self):
        self.send_command(0x24)
        self.send_data2([0xFF] * int(self.width * self.height / 8))
        self.send_command(0x26)
        self.send_data2([0xFF] * int(self.width * self.height / 8))

        self.TurnOnDisplay()

    def display_Partial(self, Image, Xstart, Ystart, Xend, Yend):
        if((Xstart % 8 + Xend % 8 == 8 & Xstart % 8 > Xend % 8) | Xstart % 8 + Xend % 8 == 0 | (Xend - Xstart)%8 == 0):
            Xstart = Xstart // 8 * 8
            Xend = Xend // 8 * 8
        else:
            Xstart = Xstart // 8 * 8
            if Xend % 8 == 0:
                Xend = Xend // 8 * 8
            else:
                Xend = Xend // 8 * 8 + 1
                
        Width = (Xend - Xstart) // 8
        Height = Yend - Ystart

        self.reset()

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x3C)
        self.send_data(0x80)

        self.send_command(0x44)
        self.send_data(Xstart%256)  
        self.send_data(Xstart//256)
        self.send_data((Xend-1)%256)
        self.send_data((Xend-1)//256)		

        self.send_command(0x45)
        self.send_data(Ystart%256) 
        self.send_data(Ystart//256)
        self.send_data((Yend-1)%256)
        self.send_data((Yend-1)//256)		

        self.send_command(0x4E)
        self.send_data(Xstart%256)  
        self.send_data(Xstart//256) 

        self.send_command(0x4F)
        self.send_data(Ystart%256) 
        self.send_data(Ystart//256) 

        self.send_command(0x24)
        self.send_data2(Image)

        self.TurnOnDisplay_Partial()

    def display_4GRAY(self, image):
        IMAGE_COUNTER = (int(self.width/8) * self.height)
        self.send_command(0x24)
        for i in range(0, IMAGE_COUNTER):
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):
                        temp3 |= 0x00
                    elif(temp2 == 0x00):
                        temp3 |= 0x01  
                    elif(temp2 == 0x80): 
                        temp3 |= 0x01 
                    else: #0x40
                        temp3 |= 0x00 
                    temp3 <<= 1	
                    
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0): 
                        temp3 |= 0x00
                    elif(temp2 == 0x00): 
                        temp3 |= 0x01
                    elif(temp2 == 0x80):
                        temp3 |= 0x01
                    else :   #0x40
                        temp3 |= 0x00	
                    if(j!=1 or k!=1):				
                        temp3 <<= 1
                    temp1 <<= 2
            self.send_data(temp3)
            
        self.send_command(0x26)	       
        for i in range(0, IMAGE_COUNTER):
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):
                        temp3 |= 0x00
                    elif(temp2 == 0x00):
                        temp3 |= 0x01
                    elif(temp2 == 0x80):
                        temp3 |= 0x00
                    else: #0x40
                        temp3 |= 0x01 
                    temp3 <<= 1	
                    
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0): 
                        temp3 |= 0x00
                    elif(temp2 == 0x00): 
                        temp3 |= 0x01
                    elif(temp2 == 0x80):
                        temp3 |= 0x00 
                    else:    #0x40
                            temp3 |= 0x01	
                    if(j!=1 or k!=1):					
                        temp3 <<= 1
                    temp1 <<= 2
            self.send_data(temp3)
        
        self.TurnOnDisplay_4GRAY()

    def sleep(self):   
        self.send_command(0x10) # DEEP_SLEEP
        self.send_data(0x01)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

