# *****************************************************************************
# * | File        :	  epd1in54b.py
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
EPD_WIDTH       = 200
EPD_HEIGHT      = 200

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT 
 
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):
            epdconfig.delay_ms(100)    
        logging.debug("e-Paper busy release")
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        self.ReadBusy()   
        epdconfig.send_command(0x12)  #SWRESET
        self.ReadBusy()   

        epdconfig.send_command(0x01) #Driver output control      
        epdconfig.send_data(0xC7)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x11) #data entry mode       
        epdconfig.send_data(0x01)

        epdconfig.send_command(0x44) #set Ram-X address start/end position   
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x18)    #0x18-->(24+1)*8=200

        epdconfig.send_command(0x45) #set Ram-Y address start/end position          
        epdconfig.send_data(0xC7)    #0xC7-->(199+1)=200
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00) 

        epdconfig.send_command(0x3C) #BorderWavefrom
        epdconfig.send_data(0x05)

        epdconfig.send_command(0x18) #Read built-in temperature sensor
        epdconfig.send_data(0x80)

        epdconfig.send_command(0x4E)   # set RAM x address count to 0
        epdconfig.send_data(0x00)
        epdconfig.send_command(0x4F)   # set RAM y address count to 0X199    
        epdconfig.send_data(0xC7)
        epdconfig.send_data(0x00)
        self.ReadBusy()
        return 0

    def getbuffer(self, image):
        buf = [0xFF] * int(self.width * self.height / 8)
        # Set buffer to value of Python Imaging Library image.
        # Image must be in mode 1.
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        if imwidth != self.width or imheight != self.height:
            raise ValueError('Image must be same dimensions as display \
                ({0}x{1}).' .format(self.width, self.height))

        pixels = image_monocolor.load()
        for y in range(self.height):
            for x in range(self.width):
                # Set the bits for the column of pixels at the current position.
                if pixels[x, y] == 0:
                    buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        return buf

    def display(self, blackimage, redimage):
        # send black data
        if (blackimage != None):
            epdconfig.send_command(0x24) # DATA_START_TRANSMISSION_1
            for i in range(0, int(self.width * self.height / 8)):
                epdconfig.send_data(blackimage[i])
                
        # send red data        
        if (redimage != None):
            epdconfig.send_command(0x26) # DATA_START_TRANSMISSION_2
            for i in range(0, int(self.width * self.height / 8)):
                epdconfig.send_data(~redimage[i])  

        epdconfig.send_command(0x22) # DISPLAY_REFRESH
        epdconfig.send_data(0xF7)
        epdconfig.send_command(0x20) # DISPLAY_REFRESH
        self.ReadBusy()

    def Clear(self):
        epdconfig.send_command(0x24) # DATA_START_TRANSMISSION_1
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xFF)
            
        epdconfig.send_command(0x26) # DATA_START_TRANSMISSION_2
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0x00)

        epdconfig.send_command(0x22) # DISPLAY_REFRESH
        epdconfig.send_data(0xF7)
        epdconfig.send_command(0x20) # DISPLAY_REFRESH
        self.ReadBusy()


    def sleep(self):
        epdconfig.send_command(0x10) #enter deep sleep
        epdconfig.send_data(0x01) 

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

