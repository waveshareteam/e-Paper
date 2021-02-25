# *****************************************************************************
# * | File        :	  epd1in54.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V1.0
# * | Date        :   2019-06-20
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

# Display resolution
EPD_WIDTH       = 80
EPD_HEIGHT      = 128

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
    
    #full screen update LUT

    lut_w1 =[
    0x60,  0x5A,  0x5A,  0x00,  0x00,  0x01,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    ]
    
    lut_b1 =[
    0x90,  0x5A,  0x5A,  0x00,  0x00,  0x01,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    ]

    # partial screen update LUT
    lut_w = [
    0x60,  0x01,  0x01,  0x00,  0x00,  0x01,  
    0x80,  0x1f,  0x00,  0x00,  0x00,  0x01,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    ]
    
    lut_b = [
    0x90,  0x01,  0x01,  0x00,  0x00,  0x01,  
    0x40,  0x1f,  0x00,  0x00,  0x00,  0x01,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
    ]

    def ReadBusy(self):
        logging.debug("e-Paper busy")
        epdconfig.send_command(0x71)
        busy = epdconfig.digital_read(self.busy_pin)
        busy =not(busy & 0x01)
        while(busy):
            epdconfig.send_command(0x71)
            busy = epdconfig.digital_read(self.busy_pin)
            busy =not(busy & 0x01)
        epdconfig.delay_ms(800)
        logging.debug("e-Paper busy release")        

    def TurnOnDisplay(self):
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(10)
        self.ReadBusy()

    def SetFulltReg(self):
        epdconfig.send_command(0x23)
        for count in range(0, 42):
            epdconfig.send_data(self.lut_w1[count]) 
        
        epdconfig.send_command(0x24)
        for count in range(0, 42):
            epdconfig.send_data(self.lut_b1[count])     

    def SetPartReg(self):
        epdconfig.send_command(0x23)
        for count in range(0, 42):
            epdconfig.send_data(self.lut_w[count]) 
        
        epdconfig.send_command(0x24)
        for count in range(0, 42):
            epdconfig.send_data(self.lut_b[count])     

    def Init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 2, 200)
        
        epdconfig.send_command(0xD2)			
        epdconfig.send_data(0x3F)

        epdconfig.send_command(0x00)  			
        epdconfig.send_data (0x6F)  #from outside

        epdconfig.send_command(0x01)  #power setting
        epdconfig.send_data (0x03)	    
        epdconfig.send_data (0x00)
        epdconfig.send_data (0x2b)		
        epdconfig.send_data (0x2b) 

        epdconfig.send_command(0x06)  #Configuring the charge pump
        epdconfig.send_data(0x3f)

        epdconfig.send_command(0x2A)  #Setting XON and the options of LUT
        epdconfig.send_data(0x00) 
        epdconfig.send_data(0x00) 

        epdconfig.send_command(0x30)  #Set the clock frequency
        epdconfig.send_data(0x17) #50Hz

        epdconfig.send_command(0x50)  #Set VCOM and data output interval
        epdconfig.send_data(0x57)			

        epdconfig.send_command(0x60)  #Set The non-overlapping period of Gate and Source.
        epdconfig.send_data(0x22)

        epdconfig.send_command(0x61)  #resolution setting
        epdconfig.send_data (0x50)    #source 128 	 
        epdconfig.send_data (0x80)       

        epdconfig.send_command(0x82)  #sets VCOM_DC value
        epdconfig.send_data(0x12)  #-1v

        epdconfig.send_command(0xe3)#Set POWER SAVING
        epdconfig.send_data(0x33)
        self.SetFulltReg()	
        epdconfig.send_command(0x04)     		#power on
        self.ReadBusy()
        # EPD hardware init end
        return 0
    
    def Partial_Init(self):
        epdconfig.reset(200, 2, 200)
        
        epdconfig.send_command(0xD2)
        epdconfig.send_data(0x3F)

        epdconfig.send_command(0x00)
        epdconfig.send_data (0x6F)  #from outside

        epdconfig.send_command(0x01)  #power setting
        epdconfig.send_data (0x03)
        epdconfig.send_data (0x00)
        epdconfig.send_data (0x2b)
        epdconfig.send_data (0x2b)

        epdconfig.send_command(0x06)  #Configuring the charge pump
        epdconfig.send_data(0x3f)

        epdconfig.send_command(0x2A)  #Setting XON and the options of LUT
        epdconfig.send_data(0x00)
        epdconfig.send_data(0x00)

        epdconfig.send_command(0x30)  #Set the clock frequency
        epdconfig.send_data(0x17)

        epdconfig.send_command(0x50)  #Set VCOM and data output interval
        epdconfig.send_data(0xf2)

        epdconfig.send_command(0x60)  #Set The non-overlapping period of Gate and Source.
        epdconfig.send_data(0x22)

        epdconfig.send_command(0x82)  #Set VCOM_DC value
        epdconfig.send_data(0x12)#-1v

        epdconfig.send_command(0xe3)#Set POWER SAVING
        epdconfig.send_data(0x33)

        self.SetPartReg()	

        epdconfig.send_command(0x04)#Set POWER SAVING	
        self.ReadBusy()
        # EPD hardware init end
        return 0
    
    def getbuffer(self, image):
        buf = [0xFF] * (int(self.width / 8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
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

    def Display(self, image):
        if (image == None):
            return
        # Width = (self.width % 8 == 0)? (self.width / 8 ): (self.width / 8 + 1)
        if(self.width % 8 == 0):
            Width = self.width / 8
        else:
            Width = self.width / 8 + 1
            
        epdconfig.send_command(0x10)
        for j in range(0, self.height):
            for i in range(0, int(Width)):
                epdconfig.send_data(0xff)  
        
        epdconfig.send_command(0x13)
        for j in range(0, self.height):
            for i in range(0, int(Width)):
                epdconfig.send_data(image[i + j * int(Width)])  
        self.TurnOnDisplay()
        
    def Clear(self):
        # Width = (self.width % 8 == 0)? (self.width / 8 ): (self.width / 8 + 1)
        if(self.width % 8 == 0):
            Width = self.width / 8
        else:
            Width = self.width / 8 + 1
            
        Height = self.height
        
        epdconfig.send_command(0x10)
        for j in range(0, Height):
            for i in range(0, int(Width)):
                epdconfig.send_data(0x00)  
        
        epdconfig.send_command(0x13)
        for j in range(0, Height):
            for i in range(0, int(Width)):
                epdconfig.send_data(0xff)  
        self.TurnOnDisplay()

    def DisplayPartial(self, old_Image, Image):

        # Set partial Windows */
        epdconfig.send_command(0x91)		#This command makes the display enter partial mode
        epdconfig.send_command(0x90)		#resolution setting
        epdconfig.send_data(0)           #x-start
        epdconfig.send_data(79)       #x-end

        epdconfig.send_data(0)
        epdconfig.send_data(127)  #y-end
        epdconfig.send_data(0x00)
       
        # Width = (self.width % 8 == 0)? (self.width / 8 ): (self.width / 8 + 1)
        if(self.width % 8 == 0):
            Width = self.width / 8
        else:
            Width = self.width / 8 + 1
            
        Height = self.height
        # send data
        epdconfig.send_command(0x10)
        for j in range(0, Height):
            for i in range(0, int(Width)):
                epdconfig.send_data(old_Image[i + j * int(Width)])

        epdconfig.send_command(0x13)
        for j in range(0, Height):
            for i in range(0, int(Width)):
                epdconfig.send_data(Image[i + j * int(Width)])

        # Set partial refresh
        self.TurnOnDisplay()

    def Sleep(self):
        epdconfig.send_command(0x50)
        epdconfig.send_data(0xf7)
        epdconfig.send_command(0x02)
        self.ReadBusy()
        epdconfig.send_command(0x07)
        epdconfig.send_data(0xA5)
        epdconfig.delay_ms(200)

        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

