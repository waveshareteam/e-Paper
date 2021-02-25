# *****************************************************************************
# * | File        :	  epd4in2.py
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
from PIL import Image
import RPi.GPIO as GPIO

# Display resolution
EPD_WIDTH       = 400
EPD_HEIGHT      = 300

GRAY1  = 0xff #white
GRAY2  = 0xC0
GRAY3  = 0x80 #gray
GRAY4  = 0x00 #Blackest

class EPD:
    def __init__(self):
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
        self.GRAY1  = GRAY1 #white
        self.GRAY2  = GRAY2
        self.GRAY3  = GRAY3 #gray
        self.GRAY4  = GRAY4 #Blackest

    lut_vcom0 = [
    0x00, 0x17, 0x00, 0x00, 0x00, 0x02,        
    0x00, 0x17, 0x17, 0x00, 0x00, 0x02,        
    0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,        
    0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,        
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    lut_ww = [
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    lut_bw = [
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    lut_wb = [
    0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    lut_bb = [
    0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
    0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    ]
    #******************************partial screen update LUT*********************************/
    EPD_4IN2_Partial_lut_vcom1 =[
    0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00,	]

    EPD_4IN2_Partial_lut_ww1 =[
    0x00	,0x19	,0x01	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,]

    EPD_4IN2_Partial_lut_bw1 =[
    0x80	,0x19	,0x01	,0x00	,0x00	,0x01,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	]

    EPD_4IN2_Partial_lut_wb1 =[
    0x40	,0x19	,0x01	,0x00	,0x00	,0x01,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	]

    EPD_4IN2_Partial_lut_bb1 =[
    0x00	,0x19	,0x01	,0x00	,0x00	,0x01,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,	]

    #******************************gray*********************************/
    #0~3 gray
    EPD_4IN2_4Gray_lut_vcom =[
    0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
    0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
    0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
    0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00
    ]
    #R21
    EPD_4IN2_4Gray_lut_ww =[
    0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
    0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
    0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
    0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    ]
    #R22H	r
    EPD_4IN2_4Gray_lut_bw =[
    0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
    0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
    0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
    0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    ]
    #R23H	w
    EPD_4IN2_4Gray_lut_wb =[
    0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
    0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
    0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
    0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    ]
    #R24H	b
    EPD_4IN2_4Gray_lut_bb =[
    0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
    0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
    0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
    0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    ]
        
    def ReadBusy(self):
        epdconfig.send_command(0x71)
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.send_command(0x71)
            epdconfig.delay_ms(100)    

    def set_lut(self):
        epdconfig.send_command(0x20)               # vcom
        for count in range(0, 44):
            epdconfig.send_data(self.lut_vcom0[count])
            
        epdconfig.send_command(0x21)         # ww --
        for count in range(0, 42):
            epdconfig.send_data(self.lut_ww[count])
            
        epdconfig.send_command(0x22)         # bw r
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bw[count])
            
        epdconfig.send_command(0x23)         # wb w
        for count in range(0, 42):
            epdconfig.send_data(self.lut_bb[count])
            
        epdconfig.send_command(0x24)         # bb b
        for count in range(0, 42):
            epdconfig.send_data(self.lut_wb[count])


    def Partial_SetLut(self):
        epdconfig.send_command(0x20);
        for count in range(0, 44):	     
            epdconfig.send_data(self.EPD_4IN2_Partial_lut_vcom1[count])

        epdconfig.send_command(0x21);
        for count in range(0, 42):	     
            epdconfig.send_data(self.EPD_4IN2_Partial_lut_ww1[count])
        
        epdconfig.send_command(0x22);
        for count in range(0, 42):     
            epdconfig.send_data(self.EPD_4IN2_Partial_lut_bw1[count])

        epdconfig.send_command(0x23);
        for count in range(0, 42):	     
            epdconfig.send_data(self.EPD_4IN2_Partial_lut_wb1[count])

        epdconfig.send_command(0x24);
        for count in range(0, 42):	     
            epdconfig.send_data(self.EPD_4IN2_Partial_lut_bb1[count])


       
    def Gray_SetLut(self):
        epdconfig.send_command(0x20)						#vcom
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_vcom[count]) 

        epdconfig.send_command(0x21)						#red not use
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_ww[count]) 

        epdconfig.send_command(0x22)							#bw r
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_bw[count]) 

        epdconfig.send_command(0x23)							#wb w
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_wb[count]) 

        epdconfig.send_command(0x24)                          #bb b
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_bb[count]) 

        epdconfig.send_command(0x25)						#vcom
        for count in range(0, 42):
            epdconfig.send_data(self.EPD_4IN2_4Gray_lut_ww[count])
      
    
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x01) # POWER SETTING
        epdconfig.send_data(0x03) # VDS_EN, VDG_EN
        epdconfig.send_data(0x00) # VCOM_HV, VGHL_LV[1], VGHL_LV[0]
        epdconfig.send_data(0x2b) # VDH
        epdconfig.send_data(0x2b) # VDL
        
        epdconfig.send_command(0x06) # boost soft start
        epdconfig.send_data(0x17)
        epdconfig.send_data(0x17)
        epdconfig.send_data(0x17)
        
        epdconfig.send_command(0x04) # POWER_ON
        self.ReadBusy()
        
        epdconfig.send_command(0x00) # panel setting
        epdconfig.send_data(0xbf) # KW-BF   KWR-AF  BWROTP 0f
        epdconfig.send_data(0x0d)
        
        epdconfig.send_command(0x30) # PLL setting
        epdconfig.send_data(0x3c) # 3A 100HZ   29 150Hz 39 200HZ  31 171HZ

        epdconfig.send_command(0x61)	# resolution setting
        epdconfig.send_data(0x01)
        epdconfig.send_data(0x90) # 128
        epdconfig.send_data(0x01)		
        epdconfig.send_data(0x2c)

        epdconfig.send_command(0x82)	# vcom_DC setting
        epdconfig.send_data(0x28)

        epdconfig.send_command(0X50)	# VCOM AND DATA INTERVAL SETTING
        epdconfig.send_data(0x97) # 97white border 77black border		VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7
    
        self.set_lut()
        # EPD hardware init end
        return 0
        
    def Init_4Gray(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        epdconfig.reset(200, 5, 200)
        
        epdconfig.send_command(0x01)			#POWER SETTING
        epdconfig.send_data (0x03)
        epdconfig.send_data (0x00)       #VGH=20V,VGL=-20V
        epdconfig.send_data (0x2b)		#VDH=15V															 
        epdconfig.send_data (0x2b)		#VDL=-15V
        epdconfig.send_data (0x13)

        epdconfig.send_command(0x06)         #booster soft start
        epdconfig.send_data (0x17)		#A
        epdconfig.send_data (0x17)		#B
        epdconfig.send_data (0x17)		#C 

        epdconfig.send_command(0x04)
        self.ReadBusy()

        epdconfig.send_command(0x00)			#panel setting
        epdconfig.send_data(0x3f)		#KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

        epdconfig.send_command(0x30)			#PLL setting
        epdconfig.send_data (0x3c)      	#100hz 

        epdconfig.send_command(0x61)			#resolution setting
        epdconfig.send_data (0x01)		#400
        epdconfig.send_data (0x90)     	 
        epdconfig.send_data (0x01)		#300
        epdconfig.send_data (0x2c)

        epdconfig.send_command(0x82)			#vcom_DC setting
        epdconfig.send_data (0x12)

        epdconfig.send_command(0X50)			#VCOM AND DATA INTERVAL SETTING			
        epdconfig.send_data(0x97)

    def getbuffer(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # logging.debug("imwidth = %d, imheight = %d",imwidth,imheight)
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
        
    def getbuffer_4Gray(self, image):
        # logging.debug("bufsiz = ",int(self.width/8) * self.height)
        buf = [0xFF] * (int(self.width / 4) * self.height)
        image_monocolor = image.convert('L')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        i=0
        # logging.debug("imwidth = %d, imheight = %d",imwidth,imheight)
        if(imwidth == self.width and imheight == self.height):
            logging.debug("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if(pixels[x, y] == 0xC0):
                        pixels[x, y] = 0x80
                    elif (pixels[x, y] == 0x80):
                        pixels[x, y] = 0x40
                    i= i+1
                    if(i%4 == 0):
                        buf[int((x + (y * self.width))/4)] = ((pixels[x-3, y]&0xc0) | (pixels[x-2, y]&0xc0)>>2 | (pixels[x-1, y]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6)
                        
        elif(imwidth == self.height and imheight == self.width):
            logging.debug("Horizontal")
            for x in range(imwidth):
                for y in range(imheight):
                    newx = y
                    newy = x
                    if(pixels[x, y] == 0xC0):
                        pixels[x, y] = 0x80
                    elif (pixels[x, y] == 0x80):
                        pixels[x, y] = 0x40
                    i= i+1
                    if(i%4 == 0):
                        buf[int((newx + (newy * self.width))/4)] = ((pixels[x, y-3]&0xc0) | (pixels[x, y-2]&0xc0)>>2 | (pixels[x, y-1]&0xc0)>>4 | (pixels[x, y]&0xc0)>>6) 
        
        return buf

    def display(self, image):
        epdconfig.send_command(0x92);	
        self.set_lut();
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xFF)
            
        epdconfig.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(image[i])
            
        epdconfig.send_command(0x12) 
        self.ReadBusy()

    def EPD_4IN2_PartialDisplay(self, X_start, Y_start, X_end, Y_end, Image):
        # EPD_WIDTH       = 400
        # EPD_HEIGHT      = 300
        if(EPD_WIDTH % 8 != 0):
           Width = int(EPD_WIDTH / 8) + 1;
        else:
            Width = int(EPD_WIDTH / 8);
        Height = EPD_HEIGHT;
        
        if(X_start % 8 != 0):
            X_start = int(X_start/8)*8+8
        if(X_end % 8 != 0):
            X_end = int(X_end/8)*8+8
        
        self.Partial_SetLut();
        epdconfig.send_command(0x91);		#This command makes the display enter partial mode
        epdconfig.send_command(0x90);		#resolution setting
        epdconfig.send_data (int(X_start/256));
        epdconfig.send_data (int(X_start%256));   #x-start    
        
        epdconfig.send_data (int(X_end /256));		
        epdconfig.send_data (int(X_end %256)-1);  #x-end

        epdconfig.send_data (int(Y_start/256));
        epdconfig.send_data (int(Y_start%256));   #y-start    
        

        epdconfig.send_data (int(Y_end/256));		
        epdconfig.send_data (int(Y_end%256)-1);  #y-end
        epdconfig.send_data (0x28);	

        epdconfig.send_command(0x10);	       #writes Old data to SRAM for programming
        for j in range(0, int(Y_end - Y_start)):
            for i in range(0, int(X_end/8) - int(X_start/8)):
                epdconfig.send_data(Image[(Y_start + j)*Width + int(X_start/8) + i]);
            
        epdconfig.send_command(0x13);				 #writes New data to SRAM.
        for j in range(0, int(Y_end - Y_start)):
            for i in range(0, int(X_end/8) - int(X_start/8)):
                epdconfig.send_data(~Image[(Y_start + j)*Width + int(X_start/8) + i]);
            
        epdconfig.send_command(0x12);		 #DISPLAY REFRESH 		             
        epdconfig.delay_ms(200)    #The delay here is necessary, 200uS at least!!!     
        self.ReadBusy()


    def display_4Gray(self, image):
        epdconfig.send_command(0x92);	
        self.set_lut();
        epdconfig.send_command(0x10)
        for i in range(0, int(EPD_WIDTH * EPD_HEIGHT / 8)):                   # EPD_WIDTH * EPD_HEIGHT / 4
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):
                        temp3 |= 0x01#white
                    elif(temp2 == 0x00):
                        temp3 |= 0x00  #black
                    elif(temp2 == 0x80): 
                        temp3 |= 0x01  #gray1
                    else: #0x40
                        temp3 |= 0x00 #gray2
                    temp3 <<= 1	
                    
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):  #white
                        temp3 |= 0x01
                    elif(temp2 == 0x00): #black
                        temp3 |= 0x00
                    elif(temp2 == 0x80):
                        temp3 |= 0x01 #gray1
                    else :   #0x40
                            temp3 |= 0x00	#gray2	
                    if(j!=1 or k!=1):				
                        temp3 <<= 1
                    temp1 <<= 2
            epdconfig.send_data(temp3)
            
        epdconfig.send_command(0x13)	    
               
        for i in range(0, int(EPD_WIDTH * EPD_HEIGHT / 8)):                #5808*4  46464
            temp3=0
            for j in range(0, 2):
                temp1 = image[i*2+j]
                for k in range(0, 2):
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):
                        temp3 |= 0x01#white
                    elif(temp2 == 0x00):
                        temp3 |= 0x00  #black
                    elif(temp2 == 0x80):
                        temp3 |= 0x00  #gray1
                    else: #0x40
                        temp3 |= 0x01 #gray2
                    temp3 <<= 1	
                    
                    temp1 <<= 2
                    temp2 = temp1&0xC0 
                    if(temp2 == 0xC0):  #white
                        temp3 |= 0x01
                    elif(temp2 == 0x00): #black
                        temp3 |= 0x00
                    elif(temp2 == 0x80):
                        temp3 |= 0x00 #gray1
                    else:    #0x40
                            temp3 |= 0x01	#gray2
                    if(j!=1 or k!=1):					
                        temp3 <<= 1
                    temp1 <<= 2
            epdconfig.send_data(temp3)
        
        self.Gray_SetLut()
        epdconfig.send_command(0x12)
        epdconfig.delay_ms(200)
        self.ReadBusy()
        # pass
    
    def Clear(self):
        epdconfig.send_command(0x10)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xFF)
            
        epdconfig.send_command(0x13)
        for i in range(0, int(self.width * self.height / 8)):
            epdconfig.send_data(0xFF)
            
        epdconfig.send_command(0x12) 
        self.ReadBusy()

    def sleep(self):
        epdconfig.send_command(0x02) # POWER_OFF
        self.ReadBusy()
        epdconfig.send_command(0x07) # DEEP_SLEEP
        epdconfig.send_data(0XA5)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
        
### END OF FILE ###

