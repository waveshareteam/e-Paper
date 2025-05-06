# *****************************************************************************
# * | File        :	  epd7in3f.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2022-10-20
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

logger = logging.getLogger(__name__)

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.BLACK  = 0x000000   #   0000  BGR
        self.WHITE  = 0xffffff   #   0001
        self.YELLOW = 0x00ffff   #   0010
        self.RED    = 0x0000ff   #   0011
        # self.ORANGE = 0x0080ff   #   0100
        self.BLUE   = 0xff0000   #   0101
        self.GREEN  = 0x00ff00   #   0110
        

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

    def send_data(self, *data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte(data)
        epdconfig.digital_write(self.cs_pin, 1)
        
    # send a lot of data   
    def send_data2(self, data):
        epdconfig.digital_write(self.dc_pin, 1)
        epdconfig.digital_write(self.cs_pin, 0)
        epdconfig.spi_writebyte2(data)
        epdconfig.digital_write(self.cs_pin, 1)
        
    def ReadBusyH(self):
        logger.debug("e-Paper busy H")
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: busy, 1: idle
            epdconfig.delay_ms(5)
        logger.debug("e-Paper busy H release")

    def TurnOnDisplay(self):
        self.send_command(0x17) # AUTO: AUTO SEQUENCE
        self.send_data(0XA5)    # A5: PON->DRF->POF / A7: PON->DRF->POF->DSLP
        self.ReadBusyH()

        #self.send_command(0x12) # DRF: DISPLAY_REFRESH
        #self.send_data(0X00)
        #self.ReadBusyH()
        
        #self.send_command(0x02) # POF: POWER_OFF
        #self.send_data(0X00)
        #self.ReadBusyH()

        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        self.ReadBusyH()
        epdconfig.delay_ms(30)

        self.send_command(0xAA)   
        self.send_data(0x49, 0x55, 0x20, 0x08, 0x09, 0x18)

        self.send_command(0x01) # PWR: POWER SETTING (REGISTER)
        self.send_data(0x3F)

        self.send_command(0x00) # PSR: PANEL SETTING (REGISTER)
        self.send_data(0x1F)

        self.send_command(0x03) # PFS: POWER OFF SEQUENCE SETTING
        self.send_data(0x00, 0x54, 0x00, 0x44) 
        
        #self.send_command(0x05)
        #self.send_data(0x40, 0x1F, 0x1F, 0x2C)

        self.send_command(0x06) # BTST: BOOSTER SOFT START
        self.send_data(0x10, 0x10, 0x10)

        #self.send_command(0x08)
        #self.send_data(0x6F, 0x1F, 0x1F, 0x22)

        self.send_command(0x30) # PLL: PLL CONTROL
        self.send_data(0x07)    # 0x03 -> slow, 0x07 -> fast

        self.send_command(0x50) # CDI: VCOM AND DATA INTERVAL SETTING
        self.send_data(0x3F)

        self.send_command(0x60) # TCON: TCON SETTING
        self.send_data(0x02, 0x00)

        self.send_command(0x61)                # TRES: RESOLUTION SETTING
        self.send_data(0x03, 0x20, 0x01, 0xE0) # [0x0320][0x01E0] -> displaysize

        self.send_command(0x84)
        self.send_data(0x01)

        self.send_command(0xE3) # PWS: POWER SAVING
        self.send_data(0x2F)

        #self.send_command(0x04) # PON: POWER ON (really wanted here?)
        self.ReadBusyH()
        return 0

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
            logger.warning("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, self.width, self.height))

        # Convert the soruce image to the 7 colors, dithering if needed
        image_7color = image_temp.convert("RGB").quantize(palette=pal_image)
        buf_7color = bytearray(image_7color.tobytes('raw'))

        # PIL does not support 4 bit color, so pack the 4 bits of color
        # into a single byte to transfer to the panel
        buf = [(buf_7color[i] << 4) + buf_7color[i+1] for i in range(0, len(buf_7color), 2)]
            
        return buf

    def display(self, image):
        self.send_command(0x10)
        self.send_data2(image)

        self.TurnOnDisplay()
        
    def Clear(self, color=0x11):
        self.send_command(0x10)
        self.send_data2([color] * int(self.height) * int(self.width/2))

        self.TurnOnDisplay()

    def sleep(self):
        self.send_command(0x07) # DEEP_SLEEP
        self.send_data(0XA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

