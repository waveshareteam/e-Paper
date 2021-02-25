# *****************************************************************************
# * | File        :	  epd2in9_V2.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2020-10-20
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
EPD_WIDTH       = 128
EPD_HEIGHT      = 296

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        
    WF_PARTIAL_2IN9 = [
        0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0A,0x0,0x0,0x0,0x0,0x0,0x2,  
        0x1,0x0,0x0,0x0,0x0,0x0,0x0,
        0x1,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
        0x22,0x17,0x41,0xB0,0x32,0x36,
    ] 
        
    def ReadBusy(self):
        logging.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      #  0: idle, 1: busy
            epdconfig.delay_ms(10) 
        logging.debug("e-Paper busy release")  

    def TurnOnDisplay(self):
        epdconfig.send_command(0x22) # DISPLAY_UPDATE_CONTROL_2
        epdconfig.send_data(0xF7)
        epdconfig.send_command(0x20) # MASTER_ACTIVATION
        self.ReadBusy()

    def TurnOnDisplay_Partial(self):
        epdconfig.send_command(0x22) # DISPLAY_UPDATE_CONTROL_2
        epdconfig.send_data(0x0F)
        epdconfig.send_command(0x20) # MASTER_ACTIVATION
        self.ReadBusy()

    def SendLut(self):
        epdconfig.send_command(0x32)
        for i in range(0, 153):
            epdconfig.send_data(self.WF_PARTIAL_2IN9[i])
        self.ReadBusy()

    def SetWindow(self, x_start, y_start, x_end, y_end):
        epdconfig.send_command(0x44) # SET_RAM_X_ADDRESS_START_END_POSITION
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        epdconfig.send_data((x_start>>3) & 0xFF)
        epdconfig.send_data((x_end>>3) & 0xFF)
        epdconfig.send_command(0x45) # SET_RAM_Y_ADDRESS_START_END_POSITION
        epdconfig.send_data(y_start & 0xFF)
        epdconfig.send_data((y_start >> 8) & 0xFF)
        epdconfig.send_data(y_end & 0xFF)
        epdconfig.send_data((y_end >> 8) & 0xFF)

    def SetCursor(self, x, y):
        epdconfig.send_command(0x4E) # SET_RAM_X_ADDRESS_COUNTER
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        epdconfig.send_data(x & 0xFF)
        
        epdconfig.send_command(0x4F) # SET_RAM_Y_ADDRESS_COUNTER
        epdconfig.send_data(y & 0xFF)
        epdconfig.send_data((y >> 8) & 0xFF)
        self.ReadBusy()
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start     
        epdconfig.reset(50, 2, 50)

        self.ReadBusy();   
        epdconfig.send_command(0x12);  #SWRESET
        self.ReadBusy();   

        epdconfig.send_command(0x01); #Driver output control      
        epdconfig.send_data(0x27);
        epdconfig.send_data(0x01);
        epdconfig.send_data(0x00);
    
        epdconfig.send_command(0x11); #data entry mode       
        epdconfig.send_data(0x03);

        self.SetWindow(0, 0, self.width-1, self.height-1);

        epdconfig.send_command(0x21); #  Display update control
        epdconfig.send_data(0x00);
        epdconfig.send_data(0x80);	
    
        self.SetCursor(0, 0);
        self.ReadBusy();
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # logging.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[int((newx + newy*self.width) / 8)] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, image):
        if (image == None):
            return            
        epdconfig.send_command(0x24) # WRITE_RAM
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   
        self.TurnOnDisplay()

    def display_Base(self, image):
        if (image == None):
            return   
            
        epdconfig.send_command(0x24) # WRITE_RAM
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])
                
        epdconfig.send_command(0x26) # WRITE_RAM
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   
                
        self.TurnOnDisplay()
        
    def display_Partial(self, image):
        if (image == None):
            return
            
        epdconfig.digital_write(self.reset_pin, 0)
        epdconfig.delay_ms(2)
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(2)   
        
        self.SendLut();
        epdconfig.send_command(0x37); 
        epdconfig.send_data(0x00);  
        epdconfig.send_data(0x00);  
        epdconfig.send_data(0x00);  
        epdconfig.send_data(0x00); 
        epdconfig.send_data(0x00);  	
        epdconfig.send_data(0x40);  
        epdconfig.send_data(0x00);  
        epdconfig.send_data(0x00);   
        epdconfig.send_data(0x00);  
        epdconfig.send_data(0x00);

        epdconfig.send_command(0x3C); #BorderWavefrom
        epdconfig.send_data(0x80);

        epdconfig.send_command(0x22); 
        epdconfig.send_data(0xC0);   
        epdconfig.send_command(0x20); 
        self.ReadBusy();

        self.SetWindow(0, 0, self.width - 1, self.height - 1)
        self.SetCursor(0, 0)
        
        epdconfig.send_command(0x24) # WRITE_RAM
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(image[i + j * int(self.width / 8)])   
        self.TurnOnDisplay_Partial()

    def Clear(self, color):
        epdconfig.send_command(0x24) # WRITE_RAM
        for j in range(0, self.height):
            for i in range(0, int(self.width / 8)):
                epdconfig.send_data(color)   
        self.TurnOnDisplay()

    def sleep(self):
        epdconfig.send_command(0x10) # DEEP_SLEEP_MODE
        epdconfig.send_data(0x01)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

