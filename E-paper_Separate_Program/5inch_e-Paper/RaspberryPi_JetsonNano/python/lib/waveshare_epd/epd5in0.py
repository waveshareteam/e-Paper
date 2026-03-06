# *****************************************************************************
# * | File        :	  epd5in0.py
# * | Author      :   Waveshare team
# * | Function    :   5inch e-paper 
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2025-11-04
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
import time
from PIL import Image
import io

# Display resolution
EPD_WIDTH       = 960
EPD_HEIGHT      = 552

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
        epdconfig.delay_ms(20) 
        epdconfig.digital_write(self.reset_pin, 0)         # module reset
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
        
    # send a lot of data   
    def send_data2(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte2(data)
        epdconfig.digital_write(self.cs_pin, 1)
        
    def ReadBusy(self):
        logger.debug("e-Paper busy H")
        while(epdconfig.digital_read(self.busy_pin) == 1):
            epdconfig.delay_ms(1)
        logger.debug("e-Paper busy H release")

    def TurnOnDisplay(self):
        self.send_command(0x22)  
        self.send_data(0xF7)     
        self.send_command(0x20)  
        self.ReadBusy()

    def TurnOnDisplay_Fast(self):
        self.send_command(0x22) #Display Update Control
        self.send_data(0xD7)
        self.send_command(0x20) #Activate Display Update Sequence
        self.ReadBusy()

    def TurnOnDisplay_Part(self):
        self.send_command(0x22) #Display Update Control
        self.send_data(0xFF)
        self.send_command(0x20) #Activate Display Update Sequence
        self.ReadBusy()

    def SetWindow(self, x_start, y_start, x_end, y_end):
        self.send_command(0x44) # SET_RAM_X_ADDRESS_START_END_POSITION
        self.send_data(x_start & 0xFF)
        self.send_data((x_start>>8) & 0xFF)
        self.send_data(x_end & 0xFF)
        self.send_data((x_end>>8) & 0xFF)
        
        self.send_command(0x45) # SET_RAM_Y_ADDRESS_START_END_POSITION
        self.send_data(y_start & 0xFF)
        self.send_data((y_start >> 8) & 0xFF)
        self.send_data(y_end & 0xFF)
        self.send_data((y_end >> 8) & 0xFF)

    def SetCursor(self, x, y):
        self.send_command(0x4E) # SET_RAM_X_ADDRESS_COUNTER
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        self.send_data(x & 0xFF)
        self.send_data((x>>8) & 0xFF)
        
        self.send_command(0x4F) # SET_RAM_Y_ADDRESS_COUNTER
        self.send_data(y & 0xFF)
        self.send_data((y >> 8) & 0xFF)
        
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
        self.send_data((EPD_HEIGHT-1)%256)
        self.send_data((EPD_HEIGHT-1)//256)
        self.send_data(0x02)

        self.send_command(0x11)
        self.send_data(0x01)

        self.send_command(0x44)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data((EPD_WIDTH-1)%256)
        self.send_data((EPD_WIDTH-1)//256)

        self.send_command(0x45)
        self.send_data((EPD_HEIGHT-1)%256)
        self.send_data((EPD_HEIGHT-1)//256)
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
        return 0

    def init_Fast(self):
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
        self.send_data((EPD_HEIGHT-1)%256)
        self.send_data((EPD_HEIGHT-1)//256)
        self.send_data(0x02)

        self.send_command(0x11)
        self.send_data(0x01)

        self.send_command(0x44)
        self.send_data(0x00)
        self.send_data(0x00)
        self.send_data((EPD_WIDTH-1)%256)
        self.send_data((EPD_WIDTH-1)//256)

        self.send_command(0x45)
        self.send_data((EPD_HEIGHT-1)%256)
        self.send_data((EPD_HEIGHT-1)//256)
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

        self.TurnOnDisplay_Fast()

    def display_Partial(self, Image):
        
        self.reset()

        self.send_command(0x18)
        self.send_data(0x80)

        self.send_command(0x3C)
        self.send_data(0x80)

        self.SetWindow(0, self.height-1, self.width-1, 0)

        self.SetCursor(0, 0)

        self.send_command(0x24)

        self.send_data2(Image)

        self.TurnOnDisplay_Part()

    def Clear(self):
        self.send_command(0x24)
        self.send_data2([0xFF] * int(self.height) * int(self.width/8))
        self.send_command(0x26)
        self.send_data2([0x00] * int(self.height) * int(self.width/8))

        self.TurnOnDisplay()

    def sleep(self):
        self.send_command(0x10) # DEEP_SLEEP
        self.send_data(0X01)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

