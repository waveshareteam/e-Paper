# /*****************************************************************************
# * | File        :	  epd12in48.py
# * | Author      :   Waveshare electrices
# * | Function    :   Hardware underlying interface
# * | Info        :
# *----------------
# * |	This version:   V1.0
# * | Date        :   2019-11-01
# * | Info        :   
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
import time
import epdconfig

import PIL
from PIL import Image
import io

EPD_WIDTH       = 1200
EPD_HEIGHT      = 1600

class EPD():
    def __init__(self):
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

        self.BLACK  = 0x000000   #   0000  BGR
        self.WHITE  = 0xffffff   #   0001
        self.YELLOW = 0x00ffff   #   0010
        self.RED    = 0x0000ff   #   0011
        self.BLUE   = 0xff0000   #   0101
        self.GREEN  = 0x00ff00   #   0110
        
        self.EPD_CS_M_PIN  = epdconfig.EPD_CS_M_PIN
        self.EPD_CS_S_PIN  = epdconfig.EPD_CS_S_PIN

        self.EPD_DC_PIN  = epdconfig.EPD_DC_PIN
        self.EPD_RST_PIN  = epdconfig.EPD_RST_PIN
        self.EPD_BUSY_PIN  = epdconfig.EPD_BUSY_PIN
        self.EPD_PWR_PIN  = epdconfig.EPD_PWR_PIN


    
    def Reset(self):
        epdconfig.digital_write(self.EPD_RST_PIN, 1) 
        time.sleep(0.03) 
        epdconfig.digital_write(self.EPD_RST_PIN, 0) 
        time.sleep(0.03) 
        epdconfig.digital_write(self.EPD_RST_PIN, 1) 
        time.sleep(0.03) 
        epdconfig.digital_write(self.EPD_RST_PIN, 0) 
        time.sleep(0.03) 
        epdconfig.digital_write(self.EPD_RST_PIN, 1) 
        time.sleep(0.03) 

    def CS_ALL(self, Value):
        epdconfig.digital_write(self.EPD_CS_M_PIN, Value)
        epdconfig.digital_write(self.EPD_CS_S_PIN, Value)

    def SendCommand(self, Command):
        epdconfig.spi_writebyte(Command)

    def SendData(self, Data):
        epdconfig.spi_writebyte(Data)
    
    def SendData2(self, buf, Len):
        epdconfig.spi_writebyte2(buf, Len)

    def ReadBusyH(self):
        print("e-Paper busy H")
        while(epdconfig.digital_read(self.EPD_BUSY_PIN) == 0):      # 0: busy, 1: idle
            epdconfig.delay_ms(5)
        print("e-Paper busy H release")

    def TurnOnDisplay(self):
        print("Write PON")
        self.CS_ALL(0)
        self.SendCommand(0x04)
        self.CS_ALL(1)
        self.ReadBusyH()

        epdconfig.delay_ms(50)

        print("Write DRF")
        self.CS_ALL(0)
        self.SendCommand(0x12)
        self.SendData(0x00)
        self.CS_ALL(1)
        self.ReadBusyH()

        print("Write POF")
        self.CS_ALL(0)
        self.SendCommand(0x02)
        self.SendData(0x00)
        self.CS_ALL(1)
        print("Display Done!!")

    def Init(self):
        print("EPD init...")
        epdconfig.module_init()
        
        self.Reset() 
        self.ReadBusyH()

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x74)
        self.SendData(0xC0)
        self.SendData(0x1C)
        self.SendData(0x1C)
        self.SendData(0xCC)
        self.SendData(0xCC)
        self.SendData(0xCC)
        self.SendData(0x15)
        self.SendData(0x15)
        self.SendData(0x55)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0xF0)
        self.SendData(0x49)
        self.SendData(0x55)
        self.SendData(0x13)
        self.SendData(0x5D)
        self.SendData(0x05)
        self.SendData(0x10)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0x00)
        self.SendData(0xDF)
        self.SendData(0x69)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0x50)
        self.SendData(0xF7)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0x60)
        self.SendData(0x03)
        self.SendData(0x03)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0x86)
        self.SendData(0x10)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0xE3)
        self.SendData(0x22)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0xE0)
        self.SendData(0x01)
        self.CS_ALL(1)

        self.CS_ALL(0)
        self.SendCommand(0x61)
        self.SendData(0x04)
        self.SendData(0xB0)
        self.SendData(0x03)
        self.SendData(0x20)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x01)
        self.SendData(0x0F)
        self.SendData(0x00)
        self.SendData(0x28)
        self.SendData(0x2C)
        self.SendData(0x28)
        self.SendData(0x38)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0xB6)
        self.SendData(0x07)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x06)
        self.SendData(0xE8)
        self.SendData(0x28)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0xB7)
        self.SendData(0x01)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x05)
        self.SendData(0xE8)
        self.SendData(0x28)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0xB0)
        self.SendData(0x01)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0xB1)
        self.SendData(0x02)
        self.CS_ALL(1)
    
    def getbuffer(self, image):
        # Create a pallette with the 7 colors supported by the panel
        pal_image = Image.new("P", (1,1))
        pal_image.putpalette( (0,0,0,  255,255,255,  255,255,0,  255,0,0,  0,0,0,  0,0,255,  0,255,0) + (0,0,0)*249)
        # pal_image.putpalette( (0,0,0,  255,255,255,  0,255,0,   0,0,255,  255,0,0,  255,255,0, 255,128,0) + (0,0,0)*249)

        # Check if we need to rotate the image
        imwidth, imheight = image.size
        if(imwidth == self.width and imheight == self.height):
            image_temp = image
        elif(imwidth == self.height and imheight == self.width):
            image_temp = image.rotate(90, expand=True)
        else:
            print("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, self.width, self.height))

        # Convert the soruce image to the 7 colors, dithering if needed
        image_7color = image_temp.convert("RGB").quantize(palette=pal_image)
        buf_7color = bytearray(image_7color.tobytes('raw'))

        # PIL does not support 4 bit color, so pack the 4 bits of color
        # into a single byte to transfer to the panel
        buf = [0x00] * int(self.width * self.height / 2)
        idx = 0
        for i in range(0, len(buf_7color), 2):
            buf[idx] = (buf_7color[i] << 4) + buf_7color[i+1]
            idx += 1
            
        return buf
    
    def Clear(self, color=0x11):
        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x10)
        for i in range(self.height):
            self.SendData2([color]* int(self.width/2), int(self.width/2))
        self.CS_ALL(1)
        epdconfig.digital_write(self.EPD_CS_S_PIN, 0)
        self.SendCommand(0x10)
        for i in range(self.height):
            self.SendData2([color]* int(self.width/2), int(self.width/2))
        self.CS_ALL(1)

        self.TurnOnDisplay()

    def display(self, image):
        Width =int(self.width / 4)
        Width1 =int(self.width / 2)

        epdconfig.digital_write(self.EPD_CS_M_PIN, 0)
        self.SendCommand(0x10)
        for i in range(self.height):
            self.SendData2(image[i * Width1 : i * Width1+Width], Width)
        self.CS_ALL(1)

        epdconfig.digital_write(self.EPD_CS_S_PIN, 0)
        self.SendCommand(0x10)
        for i in range(self.height):
            self.SendData2(image[i * Width1+Width : i * Width1+Width1], Width)
        self.CS_ALL(1)

        self.TurnOnDisplay()

    def sleep(self):
        self.CS_ALL(0)
        self.SendCommand(0x07)
        self.SendData(0XA5)
        self.CS_ALL(1)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###


