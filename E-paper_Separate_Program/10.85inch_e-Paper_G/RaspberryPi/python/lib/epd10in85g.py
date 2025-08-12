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

EPD_WIDTH       = 1360//2
EPD_HEIGHT      = 480

class EPD():
    def __init__(self):
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.BLACK  = 0x000000   #   00  BGR
        self.WHITE  = 0xffffff   #   01
        self.YELLOW = 0x00ffff   #   10
        self.RED    = 0x0000ff   #   11
        
        self.EPD_CS_M_PIN  = epdconfig.EPD_CS_M_PIN
        self.EPD_CS_S_PIN  = epdconfig.EPD_CS_S_PIN

        self.EPD_DC_PIN  = epdconfig.EPD_DC_PIN
        self.EPD_RST_PIN  = epdconfig.EPD_RST_PIN
        self.EPD_BUSY_PIN  = epdconfig.EPD_BUSY_PIN
        self.EPD_PWR_PIN  = epdconfig.EPD_PWR_PIN


    
    def Reset(self):
        epdconfig.digital_write(self.EPD_RST_PIN, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.EPD_RST_PIN, 0)         # module reset
        epdconfig.delay_ms(2)
        epdconfig.digital_write(self.EPD_RST_PIN, 1)
        epdconfig.delay_ms(200)   

    def CS_ALL(self, Value):
        epdconfig.digital_write(self.EPD_CS_M_PIN, Value)
        epdconfig.digital_write(self.EPD_CS_S_PIN, Value)

    def SendCommand(self, Command):
        epdconfig.digital_write(self.EPD_DC_PIN, 0)
        epdconfig.spi_writebyte(Command)

    def SendData(self, Data):
        epdconfig.digital_write(self.EPD_DC_PIN, 1)
        epdconfig.spi_writebyte(Data)
    
    def SendData2(self, buf, Len):
        epdconfig.digital_write(self.EPD_DC_PIN, 1)
        epdconfig.spi_writebyte2(buf, Len)

    def ReadBusyH(self):
        print("e-Paper busy H")
        epdconfig.delay_ms(100)
        while(epdconfig.digital_read(self.EPD_BUSY_PIN) == 0):      # 0: idle, 1: busy
            epdconfig.delay_ms(20)
        print("e-Paper busy release")

    def TurnOnDisplay(self):
        self.CS_ALL(0)
        self.SendCommand(0x12)
        self.SendData(0x00)
        self.CS_ALL(1)
        self.ReadBusyH()

    def Init(self):
        print("EPD init...")
        epdconfig.module_init()
        
        
        self.Reset() 
        self.ReadBusyH()

        self.CS_ALL(0)

        self.SendCommand(0x4D)
        self.SendData(0x78)	
       

        self.SendCommand(0x00)
        self.SendData(0x2F)	
        self.SendData(0x29)	

        self.SendCommand(0x06)
        self.SendData(0x0d)
        self.SendData(0x12)
        self.SendData(0x30)
        self.SendData(0x20)
        self.SendData(0x19)
        self.SendData(0x3D)
        self.SendData(0x0C)

        self.SendCommand(0x06)
        self.SendData(0x0d)
        self.SendData(0x12)
        self.SendData(0x30)
        self.SendData(0x20)
        self.SendData(0x19)
        self.SendData(0x3D)
        self.SendData(0x0C)

        self.SendCommand(0x50)
        self.SendData(0x37)	

        self.SendCommand(0x61)
        self.SendData(int(self.width/256))
        self.SendData(self.width%256)
        self.SendData(int(self.height/256))
        self.SendData(self.height%256)

        self.SendCommand(0x65)
        self.SendData(0x00)	
        self.SendData(0x00)	
        self.SendData(0x00)	
        self.SendData(0x00)	

        self.SendCommand(0xE0)
        self.SendData(0x01)

        self.SendCommand(0xE3)
        self.SendData(0x08)  

        self.SendCommand(0xE5)
        self.SendData(0x08) 

        self.SendCommand(0xE9)
        self.SendData(0x01)   

        self.SendCommand(0x04)
        self.ReadBusyH()
        self.CS_ALL(1)
    
    def getbuffer(self, image):
        # Create a pallette with the 4 colors supported by the panel
        pal_image = Image.new("P", (1,1))
        pal_image.putpalette( (0,0,0,  255,255,255,  255,255,0,   255,0,0) + (0,0,0)*252)

        # Check if we need to rotate the image
        imwidth, imheight = image.size
        if(imwidth == self.width*2 and imheight == self.height):
            image_temp = image
        elif(imwidth == self.height and imheight == self.width*2):
            image_temp = image.rotate(90, expand=True)
        else:
            logger.warning("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, self.width*2, self.height))

        # Convert the soruce image to the 4 colors, dithering if needed
        image_4color = image_temp.convert("RGB").quantize(palette=pal_image)
        buf_4color = bytearray(image_4color.tobytes('raw'))

        # into a single byte to transfer to the panel
        if (self.width*2) % 4 == 0 :
            Width = (self.width*2) // 4
        else :
            Width = (self.width*2) // 4 + 1
        Height = self.height 
        buf = [0x00] * int(Width * Height)
        idx = 0
        for j in range(0, Height):
            for i in range(0, Width):
                    buf[i + j * Width] = (buf_4color[idx] << 6) + (buf_4color[idx+1] << 4) + (buf_4color[idx+2] << 2) + buf_4color[idx+3]
                    idx = idx + 4
        return buf
    
    def Clear(self, color=0x55):
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
        self.SendCommand(0x02) # POWER_OFF
        self.SendData(0X00)
        epdconfig.delay_ms(100)
        
        self.SendCommand(0x07) # DEEP_SLEEP
        self.SendData(0XA5)
        self.CS_ALL(1)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###


