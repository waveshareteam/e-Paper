# *****************************************************************************
# * | File        :	  epd5in79.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2024-03-05
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
EPD_WIDTH       = 1360      #  WIDTH = 1360/2
EPD_HEIGHT      = 480

logger = logging.getLogger(__name__)

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_m_pin = epdconfig.CS_M_PIN
        self.cs_s_pin = epdconfig.CS_S_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.partFlag0 = 0
        self.partFlag1 = 0

    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(2)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200)   

    def send_command_M(self, command):
        epdconfig.digital_write(self.dc_pin, 0)
        epdconfig.digital_write(self.cs_m_pin, 0)
        epdconfig.spi_writebyte_M([command])
        epdconfig.digital_write(self.cs_m_pin, 1)

    def send_command_S(self, command):
        epdconfig.digital_write(self.dc_pin, 0)
        epdconfig.digital_write(self.cs_s_pin, 0)
        epdconfig.spi_writebyte_S([command])
        epdconfig.digital_write(self.cs_s_pin, 1)

    def send_command_ALL(self, command):
        epdconfig.digital_write(self.dc_pin, 0)
        epdconfig.digital_write(self.cs_m_pin, 0)
        epdconfig.digital_write(self.cs_s_pin, 0)
        epdconfig.spi_writebyte_M([command])
        epdconfig.spi_writebyte_S([command])
        epdconfig.digital_write(self.cs_m_pin, 1)
        epdconfig.digital_write(self.cs_s_pin, 0)

    def send_data_M(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_m_pin, 0)
        epdconfig.spi_writebyte_M([data])
        epdconfig.digital_write(self.cs_m_pin, 1)
    
    def send_data_S(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_s_pin, 0)
        epdconfig.spi_writebyte_S([data])
        epdconfig.digital_write(self.cs_s_pin, 1)

    def send_data_ALL(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_m_pin, 0)
        epdconfig.digital_write(self.cs_s_pin, 0)
        epdconfig.spi_writebyte_M([data])
        epdconfig.spi_writebyte_S([data])
        epdconfig.digital_write(self.cs_m_pin, 1)
        epdconfig.digital_write(self.cs_s_pin, 1)

    # send a lot of data   
    def send_data2_M(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_m_pin, 0)
        epdconfig.spi_writebyte2_M(data)
        epdconfig.digital_write(self.cs_m_pin, 1)

    def send_data2_S(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_s_pin, 0)
        epdconfig.spi_writebyte2_S(data)
        epdconfig.digital_write(self.cs_s_pin, 1)
        
    def ReadBusy(self):
        logger.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 0):      #  0: busy, 1: idle
            epdconfig.delay_ms(200)
        logger.debug("e-Paper busy release")

    def TurnOnDisplay(self):
        self.send_command_ALL(0x12)
        epdconfig.delay_ms(100)	        # The delay here is necessary, 200uS at least!!!     
        self.ReadBusy()                 # waiting for the electronic paper IC to release the idle signal

    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
            
        self.reset()
        self.ReadBusy()           
        self.send_command_ALL(0x4D)  
        self.send_data_ALL(0x55)   

        self.send_command_ALL(0xA6)
        self.send_data_ALL(0x38)

        self.send_command_ALL(0xB4)
        self.send_data_ALL(0x5D)

        self.send_command_ALL(0xB6)
        self.send_data_ALL(0x80)

        self.send_command_ALL(0xB7)
        self.send_data_ALL(0x00)

        self.send_command_ALL(0xF7)
        self.send_data_ALL(0x02)

        self.send_command_ALL(0xAE)
        self.send_data_ALL(0xA0)

        self.send_command_ALL(0xE0)
        self.send_data_ALL(0x01)

        self.send_command_ALL(0x00)
        self.send_data_ALL(0x9F)
        self.send_data_ALL(0x0D)

        self.send_command_ALL(0x06)
        self.send_data_ALL(0x57)
        self.send_data_ALL(0x24)
        self.send_data_ALL(0x28)
        self.send_data_ALL(0x32)
        self.send_data_ALL(0x08)
        self.send_data_ALL(0x48)

        self.send_command_ALL(0x61)
        self.send_data_ALL(0x02)
        self.send_data_ALL(0xA8)
        self.send_data_ALL(0x01)
        self.send_data_ALL(0xE0)

        self.send_command_ALL(0x62)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)

        self.send_command_ALL(0x60)
        self.send_data_ALL(0x31)

        self.send_command_ALL(0x50)
        self.send_data_ALL(0x97)

        self.send_command_ALL(0xE8)
        self.send_data_ALL(0x01)


        self.send_command_ALL(0x04)
        epdconfig.delay_ms(200)	     
        self.ReadBusy()    

        return 0

    def init_Part(self):
        if (epdconfig.module_init() != 0):
            return -1
        
        self.partFlag0 = 1
            
        self.reset()
        self.ReadBusy()           
        self.send_command_ALL(0x4D)  
        self.send_data_ALL(0x55)   

        self.send_command_ALL(0xA6)
        self.send_data_ALL(0x38)

        self.send_command_ALL(0xB4)
        self.send_data_ALL(0x5D)

        self.send_command_ALL(0xB6)
        self.send_data_ALL(0x80)

        self.send_command_ALL(0xB7)
        self.send_data_ALL(0x00)

        self.send_command_ALL(0xF7)
        self.send_data_ALL(0x02)

        self.send_command_ALL(0xAE)
        self.send_data_ALL(0xA0)

        self.send_command_ALL(0xE0)
        self.send_data_ALL(0x01)

        self.send_command_ALL(0x00)
        self.send_data_ALL(0x9F)
        self.send_data_ALL(0x0D)

        self.send_command_ALL(0x06)
        self.send_data_ALL(0x57)
        self.send_data_ALL(0x24)
        self.send_data_ALL(0x28)
        self.send_data_ALL(0x32)
        self.send_data_ALL(0x08)
        self.send_data_ALL(0x48)

        self.send_command_ALL(0x61)
        self.send_data_ALL(0x02)
        self.send_data_ALL(0xA8)
        self.send_data_ALL(0x01)
        self.send_data_ALL(0xE0)

        self.send_command_ALL(0x62)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)
        self.send_data_ALL(0x00)

        self.send_command_ALL(0x82)
        self.send_data_ALL(0x12)

        self.send_command_ALL(0x60)
        self.send_data_ALL(0x31)

        self.send_command_ALL(0x50)
        self.send_data_ALL(0x97)

        self.send_command_ALL(0xE8)
        self.send_data_ALL(0x01)

        self.send_command_ALL(0xE0)
        self.send_data_ALL(0x03)
        self.send_command_ALL(0xE5)
        self.send_data_ALL(0x64)

        self.send_command_ALL(0x04)
        epdconfig.delay_ms(200)	     
        self.ReadBusy()   

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
        self.send_command_M(0x10)
        self.send_data2_M([0xFF] * 40800)
        self.send_command_M(0x13)
        self.send_data2_M([0xFF] * 40800)

        self.send_command_S(0x10)
        self.send_data2_S([0xFF] * 40800)
        self.send_command_S(0x13)
        self.send_data2_S([0xFF] * 40800)

        self.TurnOnDisplay()

    def Clear_Black(self):
        self.send_command_M(0x10)
        self.send_data2_M([0xFF] * 40800)
        self.send_command_M(0x13)
        self.send_data2_M([0x00] * 40800)

        self.send_command_S(0x10)
        self.send_data2_S([0xFF] * 40800)
        self.send_command_S(0x13)
        self.send_data2_S([0x00] * 40800)

        self.TurnOnDisplay()

    def display(self, imageblack):
        Width =int(self.width / 16)

        self.send_command_M(0x10)
        self.send_data2_M([0xFF] * 40800)
        self.send_command_M(0x13)
        for i in range(self.height):
            self.send_data2_M(imageblack[i * Width * 2 : i * Width * 2+Width])

        self.send_command_S(0x10)
        self.send_data2_S([0xFF] * 40800)
        self.send_command_S(0x13)
        for i in range(self.height):
            self.send_data2_S(imageblack[i * Width * 2 + Width : (i+1) * Width * 2])

        self.TurnOnDisplay()

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

        Width =  Xend -  Xstart
        Height = Yend - Ystart
        IMAGE_COUNTER = Width * Height

        Xend -= 1
        Yend -= 1
        
        if((Xend*8)<self.width//2):
            self.send_command_ALL(0x61)
            self.send_data_ALL(((Width*8)>>8) & 0xff)
            self.send_data_ALL((Width*8) & 0xff)
            self.send_data_ALL((Height>>8) & 0xff)
            self.send_data_ALL(Height & 0xff)
            

            self.send_command_ALL(0x62)
            self.send_data_ALL(((Xstart*8)>>8) & 0xff)
            self.send_data_ALL((Xstart*8) & 0xff)
            self.send_data_ALL((Ystart>>8) & 0xff)
            self.send_data_ALL(Ystart & 0xff)
            

            if self.partFlag0 == 0:
                self.partFlag0 = 1
                self.send_command_M(0x00)
                self.send_data2_M([0xFF] * IMAGE_COUNTER)

            self.send_command_M(0x13)
            self.send_data2_M(Image)

            self.TurnOnDisplay()
            self.send_command_M(0x10)
            self.send_data2_M(Image)
    
        elif((Xstart*8)>self.width//2):
            self.send_command_S(0x61)
            self.send_data_S(((Width*8)>>8) & 0xff)
            self.send_data_S((Width*8) & 0xff)
            self.send_data_S((Height>>8) & 0xff)
            self.send_data_S(Height & 0xff)
            
            self.send_command_S(0x62)
            self.send_data_S(((Xstart*8 - self.width//2)>>8) & 0xff)
            self.send_data_S((Xstart*8 - self.width//2) & 0xff)
            self.send_data_S((Ystart>>8) & 0xff)
            self.send_data_S(Ystart & 0xff)
            

            if self.partFlag1 == 0:
                self.partFlag1 = 1
                self.send_command_S(0x00)
                self.send_data2_S([0xFF] * IMAGE_COUNTER)

            self.send_command_S(0x13)
            self.send_data2_S(Image)

            self.TurnOnDisplay()

            self.send_command_S(0x10)
            self.send_data2_S(Image)

        else:
            Width0 = self.width//16 - Xstart
            Width1 = Xend - self.width//16

            self.send_command_M(0x61)
            self.send_data_M(((Width0*8)>>8) & 0xff)
            self.send_data_M((Width0*8) & 0xff)
            self.send_data_M((Height>>8) & 0xff)
            self.send_data_M(Height& 0xff)

            self.send_command_M(0x62)
            self.send_data_M(((Xstart*8)>>8) & 0x01)
            self.send_data_M((Xstart*8) & 0xff)
            self.send_data_M((Ystart>>8) & 0x01)
            self.send_data_M(Ystart & 0xff)

            self.send_command_S(0x61)
            self.send_data_S(((Width1*8)>>8) & 0xff)
            self.send_data_S((Width1*8) & 0xff)
            self.send_data_S((Height>>8) & 0x01)
            self.send_data_S(Height & 0xff)

            self.send_command_S(0x62)
            self.send_data_S(0x00)
            self.send_data_S(0x00)
            self.send_data_S((Ystart>>8) & 0x01)
            self.send_data_S(Ystart & 0xff)
            

            if self.partFlag0 == 0:
                self.partFlag0 = 1
                self.send_command_M(0x00)
                self.send_data2_M([0xFF] * Height * Width1)

            self.send_command_M(0x13)
            for i in range(self.height):
                self.send_data2_M(Image[i*Width : i*Width+Width0])

            if self.partFlag1 == 0:
                self.partFlag1 = 1
                self.send_command_S(0x00)
                self.send_data2_S([0xFF] * Height * Width0)

            self.send_command_S(0x13)
            for i in range(self.height):
                self.send_data2_S(Image[i*Width+Width0 : i*Width+Width])

            self.TurnOnDisplay()

            self.send_command_M(0x10)
            for i in range(self.height):
                self.send_data2_M(Image[i*Width : i*Width+Width0])

            self.send_command_S(0x10)
            for i in range(self.height):
                self.send_data2_S(Image[i*Width+Width0 : i*Width+Width])
       

    def sleep(self):
        self.send_command_ALL(0x02)
        epdconfig.delay_ms(200)	     
        self.ReadBusy()   

        self.send_command_ALL(0X07) # deep sleep
        self.send_data_ALL(0xA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

