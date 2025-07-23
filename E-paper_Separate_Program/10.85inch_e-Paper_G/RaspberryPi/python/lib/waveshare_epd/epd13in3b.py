# *****************************************************************************
# * | File        :	  epd13in3b.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2024-04-08
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
EPD_WIDTH       = 960
EPD_HEIGHT      = 680

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
        if (epdconfig.module_init() != 0):
            return -1
    
    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(20) 
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(2)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(20)   

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

    def send_data2(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.SPI.writebytes2(data)
        epdconfig.digital_write(self.cs_pin, 1)

    def ReadBusy(self):
        logger.debug("e-Paper busy")
        busy = epdconfig.digital_read(self.busy_pin)
        while(busy == 1):
            busy = epdconfig.digital_read(self.busy_pin)
            epdconfig.delay_ms(20)
        epdconfig.delay_ms(20)
        logger.debug("e-Paper busy release")

    def TurnOnDisplay(self):
        self.send_command(0x22) #Display Update Control
        self.send_data(0xF7)
        self.send_command(0x20) #Activate Display Update Sequence
        self.ReadBusy()

    def TurnOnDisplay_Part(self):
        self.send_command(0x22) #Display Update Control
        self.send_data(0xFF)
        self.send_command(0x20) #Activate Display Update Sequence
        self.ReadBusy()
        
    def init(self):
        # EPD hardware init start
        self.reset()
        self.ReadBusy()

        self.send_command(0x12) #SWRESET
        self.ReadBusy()

        self.send_command(0x0C) 
        self.send_data(0xAE)
        self.send_data(0xC7)  
        self.send_data(0xC3)
        self.send_data(0xC0)
        self.send_data(0x80)

        self.send_command(0x01) 
        self.send_data(0xA7)
        self.send_data(0x02)  
        self.send_data(0x00)

        self.send_command(0x11) 
        self.send_data(0x03)

        self.send_command(0x44) 
        self.send_data(0x00)
        self.send_data(0x00)  
        self.send_data(0xBF)
        self.send_data(0x03) 
        
        self.send_command(0x45) 
        self.send_data(0x00)
        self.send_data(0x00)  
        self.send_data(0xA7)
        self.send_data(0x02)

        self.send_command(0x3C)
        self.send_data(0x01)

        self.send_command(0x18) 
        self.send_data(0x80)

        self.send_command(0x4E) 
        self.send_data(0x00)
        self.send_data(0x00)

        self.send_command(0x4F) 
        self.send_data(0x00)
        self.send_data(0x00) 
        self.ReadBusy()

        # EPD hardware init end
        return 0


    def getbuffer(self, image):
        # logger.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width / 8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # logger.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if imwidth == self.width and imheight == self.height:
            logger.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif imwidth == self.height and imheight == self.width:
            logger.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy * self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf

    def Clear(self):
        self.send_command(0x24)
        self.send_data2([0xFF] * (int(self.width/8) * self.height))
        self.send_command(0x26)
        self.send_data2([0x00] * (int(self.width/8) * self.height))

        self.TurnOnDisplay()

    def Clear_Base(self):
        self.send_command(0x24)
        self.send_data2([0xFF] * (int(self.width/8) * self.height))
        self.send_command(0x26)
        self.send_data2([0x00] * (int(self.width/8) * self.height))

        self.TurnOnDisplay()
        self.send_command(0x26)
        self.send_data2([0xFF] * (int(self.width/8) * self.height))
    
    def display(self, blackimage, ryimage):
        if(self.width % 8 == 0):
            Width = self.width // 8
        else:
            Width = self.width // 8 +1
        Height = self.height
        if (blackimage != None):
            self.send_command(0x24)
            self.send_data2(blackimage)        
        if (ryimage != None):
            for j in range(Height):
                for i in range(Width):
                    ryimage[i + j * Width] = ~ryimage[i + j * Width]
            self.send_command(0x26)
            self.send_data2(ryimage)

        self.TurnOnDisplay()

    def display_Base(self, blackimage, ryimage):
        if(self.width % 8 == 0):
            Width = self.width // 8
        else:
            Width = self.width // 8 +1
        Height = self.height
        if (blackimage != None):
            self.send_command(0x24)
            self.send_data2(blackimage)        
        if (ryimage != None):
            for j in range(Height):
                for i in range(Width):
                    ryimage[i + j * Width] = ~ryimage[i + j * Width]
            self.send_command(0x26)
            self.send_data2(ryimage)

        self.TurnOnDisplay()

        self.send_command(0x26)
        self.send_data2(blackimage)

    def display_Partial(self, Image, Xstart, Ystart, Xend, Yend):
        if((Xstart % 8 + Xend % 8 == 8 & Xstart % 8 > Xend % 8) | Xstart % 8 + Xend % 8 == 0 | (Xend - Xstart)%8 == 0):
            Xstart = Xstart // 8
            Xend = Xend // 8
        else:
            Xstart = Xstart // 8 
            if Xend % 8 == 0:
                Xend = Xend // 8
            else:
                Xend = Xend // 8 + 1
                
        if(self.width % 8 == 0):
            Width = self.width // 8
        else:
            Width = self.width // 8 +1
        Height = self.height

        Xend -= 1
        Yend -= 1

        self.send_command(0x3C) 
        self.send_data(0x80)
	
        self.send_command(0x44) 
        self.send_data((Xstart*8) & 0xff) 
        self.send_data((Xstart>>5) & 0x01) 
        self.send_data((Xend*8) & 0xff)  
        self.send_data((Xend>>5) & 0x01) 
        self.send_command(0x45)   
        self.send_data(Ystart & 0xff) 
        self.send_data((Ystart>>8) & 0x01)  
        self.send_data(Yend & 0xff) 
        self.send_data((Yend>>8) & 0x01)   

        self.send_command(0x4E) 
        self.send_data((Xstart*8) & 0xff)
        self.send_data((Xstart>>5) & 0x01)  
        self.send_command(0x4F) 
        self.send_data(Ystart & 0xff)
        self.send_data((Ystart>>8) & 0x01)

        self.send_command(0x24) 
        for j in range(Height):
            for i in range(Width):
                if((j > Ystart-1) & (j < (Yend + 1)) & (i > Xstart-1) & (i < (Xend + 1))):
                    self.send_data(Image[i + j * Width])
        self.TurnOnDisplay_Part()

        self.send_command(0x26) 
        for j in range(Height):
            for i in range(Width):
                if((j > Ystart-1) & (j < (Yend + 1)) & (i > Xstart-1) & (i < (Xend + 1))):
                    self.send_data(Image[i + j * Width])

    def sleep(self):
        self.send_command(0x10) # DEEP_SLEEP
        self.send_data(0x03)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###
