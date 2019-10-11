# *****************************************************************************
# * | File        :	  epd7in5bc.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V4.0
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

# Display resolution
EPD_WIDTH       = 800
EPD_HEIGHT      = 480

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(4)
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
        logging.debug("e-Paper busy")
        self.send_command(0x71)
        busy = epdconfig.digital_read(self.busy_pin)
        while(busy == 0):
            self.send_command(0x71)
            busy = epdconfig.digital_read(self.busy_pin)
        epdconfig.delay_ms(200)
            
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
            
        self.reset()
        
        self.send_command(0x01);			#POWER SETTING
        self.send_data(0x07);
        self.send_data(0x07);    #VGH=20V,VGL=-20V
        self.send_data(0x3f);		#VDH=15V
        self.send_data(0x3f);		#VDL=-15V

        self.send_command(0x04); #POWER ON
        epdconfig.delay_ms(100);
        self.ReadBusy();

        self.send_command(0X00);			#PANNEL SETTING
        self.send_data(0x0F);   #KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

        self.send_command(0x61);        	#tres
        self.send_data(0x03);		#source 800
        self.send_data(0x20);
        self.send_data(0x01);		#gate 480
        self.send_data(0xE0);

        self.send_command(0X15);
        self.send_data(0x00);

        self.send_command(0X50);			#VCOM AND DATA INTERVAL SETTING
        self.send_data(0x11);
        self.send_data(0x07);

        self.send_command(0X60);			#TCON SETTING
        self.send_data(0x22);
        
        return 0

    def getbuffer(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        logging.debug('imwidth = %d  imheight =  %d ',imwidth, imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, imageblack, imagered):
        self.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            self.send_data(imageblack[i]);
        
        self.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            self.send_data(~imagered[i]);
        
        self.send_command(0x12)
        epdconfig.delay_ms(100)
        self.ReadBusy()
        
    def Clear(self):
        self.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            self.send_data(0xff)
            
        self.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            self.send_data(0x00)
                
        self.send_command(0x12)
        epdconfig.delay_ms(100)
        self.ReadBusy()

    def sleep(self):
        self.send_command(0x02) # POWER_OFF
        self.ReadBusy()
        
        self.send_command(0x07) # DEEP_SLEEP
        self.send_data(0XA5)
        
        epdconfig.module_exit()
### END OF FILE ###

