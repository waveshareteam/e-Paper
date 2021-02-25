# *****************************************************************************
# * | File        :	  epd7in5.py
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
EPD_WIDTH       = 880
EPD_HEIGHT      = 528

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        busy = epdconfig.digital_read(self.busy_pin)
        while(busy == 1):
            busy = epdconfig.digital_read(self.busy_pin)
        epdconfig.delay_ms(200)
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 2, 200)
        
        self.ReadBusy();
        epdconfig.send_command(0x12);  #SWRESET
        self.ReadBusy();

        epdconfig.send_command(0x46);  # Auto Write Red RAM
        epdconfig.send_data(0xf7);
        self.ReadBusy();
        epdconfig.send_command(0x47);  # Auto Write  B/W RAM
        epdconfig.send_data(0xf7);
        self.ReadBusy();

        epdconfig.send_command(0x0C);  # Soft start setting
        epdconfig.send_data2([0xAE, 0xC7, 0xC3, 0xC0, 0x40])

        epdconfig.send_command(0x01);  # Set MUX as 527
        epdconfig.send_data2([0xAF, 0x02, 0x01])

        epdconfig.send_command(0x11);  # Data entry mode      
        epdconfig.send_data(0x01);

        epdconfig.send_command(0x44); 
        epdconfig.send_data2([0x00, 0x00, 0x6F, 0x03]) # RAM x address start at 0
        epdconfig.send_command(0x45); 
        epdconfig.send_data2([0xAF, 0x02, 0x00, 0x00])

        epdconfig.send_command(0x3C); # VBD
        epdconfig.send_data(0x05); # LUT1, for white

        epdconfig.send_command(0x18);
        epdconfig.send_data(0X80);


        epdconfig.send_command(0x22);
        epdconfig.send_data(0XB1); #Load Temperature and waveform setting.
        epdconfig.send_command(0x20);
        self.ReadBusy();

        epdconfig.send_command(0x4E); # set RAM x address count to 0;
        epdconfig.send_data2([0x00, 0x00])
        epdconfig.send_command(0x4F); 
        epdconfig.send_data2([0x00, 0x00])
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        img = image
        imwidth, imheight = img.size
        if(imwidth == self.width and imheight == self.height):
            img = img.convert('1')
        elif(imwidth == self.height and imheight == self.width):
            img = img.rotate(90, expand=True).convert('1')
        else:
            logging.warning("Wrong image dimensions: must be " + str(self.width) + "x" + str(self.height))
            # return a blank buffer
            return [0xff] * int(self.width * self.height / 8)

        buf = bytearray(img.tobytes('raw'))
        return buf
        
    def display(self, image):
        epdconfig.send_command(0x4F); 
        epdconfig.send_data2([0x00, 0x00])
        epdconfig.send_command(0x24);
        epdconfig.send_data2(image)
        epdconfig.send_command(0x22);
        epdconfig.send_data(0xF7);#Load LUT from MCU(0x32)
        epdconfig.send_command(0x20);
        epdconfig.delay_ms(10);
        self.ReadBusy();
        
    def Clear(self):
        buf = [0xff] * int(self.width * self.height / 8)
        epdconfig.send_command(0x4F); 
        epdconfig.send_data2([0x00, 0x00])
        epdconfig.send_command(0x24)
        epdconfig.send_data2(buf)
            
        epdconfig.send_command(0x26)
        epdconfig.send_data2(buf)
                
        epdconfig.send_command(0x22);
        epdconfig.send_data(0xF7);#Load LUT from MCU(0x32)
        epdconfig.send_command(0x20);
        epdconfig.delay_ms(10);
        self.ReadBusy();

    def sleep(self):
        epdconfig.send_command(0x10);
        epdconfig.send_data(0x01);
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

