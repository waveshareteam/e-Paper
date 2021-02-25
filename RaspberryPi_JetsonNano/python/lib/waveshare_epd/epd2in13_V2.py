# *****************************************************************************
# * | File        :	  epd2in13_V2.py
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
import numpy as np

# Display resolution
EPD_WIDTH       = 122
EPD_HEIGHT      = 250

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        
    FULL_UPDATE = 0
    PART_UPDATE = 1
    lut_full_update= [
        0x80,0x60,0x40,0x00,0x00,0x00,0x00,             #LUT0: BB:     VS 0 ~7
        0x10,0x60,0x20,0x00,0x00,0x00,0x00,             #LUT1: BW:     VS 0 ~7
        0x80,0x60,0x40,0x00,0x00,0x00,0x00,             #LUT2: WB:     VS 0 ~7
        0x10,0x60,0x20,0x00,0x00,0x00,0x00,             #LUT3: WW:     VS 0 ~7
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT4: VCOM:   VS 0 ~7

        0x03,0x03,0x00,0x00,0x02,                       # TP0 A~D RP0
        0x09,0x09,0x00,0x00,0x02,                       # TP1 A~D RP1
        0x03,0x03,0x00,0x00,0x02,                       # TP2 A~D RP2
        0x00,0x00,0x00,0x00,0x00,                       # TP3 A~D RP3
        0x00,0x00,0x00,0x00,0x00,                       # TP4 A~D RP4
        0x00,0x00,0x00,0x00,0x00,                       # TP5 A~D RP5
        0x00,0x00,0x00,0x00,0x00,                       # TP6 A~D RP6

        0x15,0x41,0xA8,0x32,0x30,0x0A,
    ]

    lut_partial_update = [ #20 bytes
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT0: BB:     VS 0 ~7
        0x80,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT1: BW:     VS 0 ~7
        0x40,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT2: WB:     VS 0 ~7
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT3: WW:     VS 0 ~7
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,             #LUT4: VCOM:   VS 0 ~7

        0x0A,0x00,0x00,0x00,0x00,                       # TP0 A~D RP0
        0x00,0x00,0x00,0x00,0x00,                       # TP1 A~D RP1
        0x00,0x00,0x00,0x00,0x00,                       # TP2 A~D RP2
        0x00,0x00,0x00,0x00,0x00,                       # TP3 A~D RP3
        0x00,0x00,0x00,0x00,0x00,                       # TP4 A~D RP4
        0x00,0x00,0x00,0x00,0x00,                       # TP5 A~D RP5
        0x00,0x00,0x00,0x00,0x00,                       # TP6 A~D RP6

        0x15,0x41,0xA8,0x32,0x30,0x0A,
    ]  
        
    def ReadBusy(self):
        while(epdconfig.digital_read(self.busy_pin) == 1):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    

    def TurnOnDisplay(self):
        epdconfig.send_command(0x22)
        epdconfig.send_data(0xC7)
        epdconfig.send_command(0x20)        
        self.ReadBusy()
        
    def TurnOnDisplayPart(self):
        epdconfig.send_command(0x22)
        epdconfig.send_data(0x0c)
        epdconfig.send_command(0x20)        
        self.ReadBusy()
        
    def init(self, update):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        if(update == self.FULL_UPDATE):
            self.ReadBusy()
            epdconfig.send_command(0x12) # soft reset
            self.ReadBusy()

            epdconfig.send_command(0x74) #set analog block control
            epdconfig.send_data(0x54)
            epdconfig.send_command(0x7E) #set digital block control
            epdconfig.send_data(0x3B)

            epdconfig.send_command(0x01) #Driver output control
            epdconfig.send_data(0xF9)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)

            epdconfig.send_command(0x11) #data entry mode
            epdconfig.send_data(0x01)

            epdconfig.send_command(0x44) #set Ram-X address start/end position
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x0F)    #0x0C-->(15+1)*8=128

            epdconfig.send_command(0x45) #set Ram-Y address start/end position
            epdconfig.send_data(0xF9)   #0xF9-->(249+1)=250
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            
            epdconfig.send_command(0x3C) #BorderWavefrom
            epdconfig.send_data(0x03)

            epdconfig.send_command(0x2C)     #VCOM Voltage
            epdconfig.send_data(0x55)    #

            epdconfig.send_command(0x03)
            epdconfig.send_data(self.lut_full_update[70])

            epdconfig.send_command(0x04) #
            epdconfig.send_data(self.lut_full_update[71])
            epdconfig.send_data(self.lut_full_update[72])
            epdconfig.send_data(self.lut_full_update[73])

            epdconfig.send_command(0x3A)     #Dummy Line
            epdconfig.send_data(self.lut_full_update[74])
            epdconfig.send_command(0x3B)     #Gate time
            epdconfig.send_data(self.lut_full_update[75])

            epdconfig.send_command(0x32)
            for count in range(70):
                epdconfig.send_data(self.lut_full_update[count])

            epdconfig.send_command(0x4E)   # set RAM x address count to 0
            epdconfig.send_data(0x00)
            epdconfig.send_command(0x4F)   # set RAM y address count to 0X127
            epdconfig.send_data(0xF9)
            epdconfig.send_data(0x00)
            self.ReadBusy()
        else:
            epdconfig.send_command(0x2C)     #VCOM Voltage
            epdconfig.send_data(0x26)

            self.ReadBusy()

            epdconfig.send_command(0x32)
            for count in range(70):
                epdconfig.send_data(self.lut_partial_update[count])

            epdconfig.send_command(0x37)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x40)
            epdconfig.send_data(0x00)
            epdconfig.send_data(0x00)

            epdconfig.send_command(0x22)
            epdconfig.send_data(0xC0)
            epdconfig.send_command(0x20)
            self.ReadBusy()

            epdconfig.send_command(0x3C) #BorderWavefrom
            epdconfig.send_data(0x01)
        return 0

    def getbuffer(self, image):
        if self.width%8 == 0:
            linewidth = int(self.width/8)
        else:
            linewidth = int(self.width/8) + 1
         
        buf = [0xFF] * (linewidth * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):                    
                    if pixels[x, y] == 0:
                        x = imwidth - x
                        buf[int(x / 8) + y * linewidth] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        newy = imwidth - newy - 1
                        buf[int(newx / 8) + newy*linewidth] &= ~(0x80 >> (y % 8))
        return buf   
        
        
    def display(self, image):
        if self.width%8 == 0:
            linewidth = int(self.width/8)
        else:
            linewidth = int(self.width/8) + 1

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(image[i + j * linewidth])   
        self.TurnOnDisplay()
        
    def displayPartial(self, image):
        if self.width%8 == 0:
            linewidth = int(self.width/8)
        else:
            linewidth = int(self.width/8) + 1

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(image[i + j * linewidth])   
                
                
        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(~image[i + j * linewidth])  
        self.TurnOnDisplayPart()

    def displayPartBaseImage(self, image):
        if self.width%8 == 0:
            linewidth = int(self.width/8)
        else:
            linewidth = int(self.width/8) + 1

        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(image[i + j * linewidth])   
                
                
        epdconfig.send_command(0x26)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(image[i + j * linewidth])  
        self.TurnOnDisplay()
    
    def Clear(self, color):
        if self.width%8 == 0:
            linewidth = int(self.width/8)
        else:
            linewidth = int(self.width/8) + 1
        # logging.debug(linewidth)
        
        epdconfig.send_command(0x24)
        for j in range(0, self.height):
            for i in range(0, linewidth):
                epdconfig.send_data(color)
                
        # epdconfig.send_command(0x26)
        # for j in range(0, self.height):
            # for i in range(0, linewidth):
                # epdconfig.send_data(color)   
                
        self.TurnOnDisplay()

    def sleep(self):
        # epdconfig.send_command(0x22) #POWER OFF
        # epdconfig.send_data(0xC3)
        # epdconfig.send_command(0x20)

        epdconfig.send_command(0x10) #enter deep sleep
        epdconfig.send_data(0x03)
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()

### END OF FILE ###

