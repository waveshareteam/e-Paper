# //*****************************************************************************
# * | File        :	  EPD_1in54.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * |	This version:   V3.0
# * | Date        :   2018-11-06
# * | Info        :   python2 demo
# * 1.Remove:
#   digital_write(self, pin, value)
#   digital_read(self, pin)
#   delay_ms(self, delaytime)
#   set_lut(self, lut)
#   self.lut = self.lut_full_update
# * 2.Change:
#   display_frame -> TurnOnDisplay
#   set_memory_area -> SetWindow
#   set_memory_pointer -> SetCursor
#   get_frame_buffer -> getbuffer
#   set_frame_memory -> display
# * 3.How to use
#   epd = epd2in7.EPD()
#   epd.init(epd.lut_full_update)
#   image = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)
#   ...
#   drawing ......
#   ...
#   epd.display(getbuffer(image))
# ******************************************************************************//
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and//or sell
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


import epdconfig
from PIL import Image
import RPi.GPIO as GPIO

# Display resolution
EPD_WIDTH       = 400
EPD_HEIGHT      = 300

# GDEW042T2 commands
PANEL_SETTING                               = 0x00
POWER_SETTING                               = 0x01
POWER_OFF                                   = 0x02
POWER_OFF_SEQUENCE_SETTING                  = 0x03
POWER_ON                                    = 0x04
POWER_ON_MEASURE                            = 0x05
BOOSTER_SOFT_START                          = 0x06
DEEP_SLEEP                                  = 0x07
DATA_START_TRANSMISSION_1                   = 0x10
DATA_STOP                                   = 0x11
DISPLAY_REFRESH                             = 0x12
DATA_START_TRANSMISSION_2                   = 0x13
LUT_FOR_VCOM                                = 0x20
LUT_WHITE_TO_WHITE                          = 0x21
LUT_BLACK_TO_WHITE                          = 0x22
LUT_WHITE_TO_BLACK                          = 0x23
LUT_BLACK_TO_BLACK                          = 0x24
PLL_CONTROL                                 = 0x30
TEMPERATURE_SENSOR_COMMAND                  = 0x40
TEMPERATURE_SENSOR_SELECTION                = 0x41
TEMPERATURE_SENSOR_WRITE                    = 0x42
TEMPERATURE_SENSOR_READ                     = 0x43
VCOM_AND_DATA_INTERVAL_SETTING              = 0x50
LOW_POWER_DETECTION                         = 0x51
TCON_SETTING                                = 0x60
RESOLUTION_SETTING                          = 0x61
GSST_SETTING                                = 0x65
GET_STATUS                                  = 0x71
AUTO_MEASUREMENT_VCOM                       = 0x80
READ_VCOM_VALUE                             = 0x81
VCM_DC_SETTING                              = 0x82
PARTIAL_WINDOW                              = 0x90
PARTIAL_IN                                  = 0x91
PARTIAL_OUT                                 = 0x92
PROGRAM_MODE                                = 0xA0
ACTIVE_PROGRAMMING                          = 0xA1
READ_OTP                                    = 0xA2
POWER_SAVING                                = 0xE3
class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

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
    
    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, GPIO.HIGH)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, GPIO.LOW)         # module reset
        epdconfig.delay_ms(200)
        epdconfig.digital_write(self.reset_pin, GPIO.HIGH)
        epdconfig.delay_ms(200)   

    def send_command(self, command):
        epdconfig.digital_write(self.cs_pin, GPIO.LOW)
        epdconfig.digital_write(self.dc_pin, GPIO.LOW)
        epdconfig.spi_writebyte([command])
        epdconfig.digital_write(self.cs_pin, GPIO.HIGH)

    def send_data(self, data):
        epdconfig.digital_write(self.cs_pin, GPIO.LOW)
        epdconfig.digital_write(self.dc_pin, GPIO.HIGH)
        epdconfig.spi_writebyte([data])
        epdconfig.digital_write(self.cs_pin, GPIO.HIGH)
        
    def wait_until_idle(self):        
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    

    def set_lut(self):
        self.send_command(LUT_FOR_VCOM)               # vcom
        for count in range(0, 44):
            self.send_data(self.lut_vcom0[count])
        self.send_command(LUT_WHITE_TO_WHITE)         # ww --
        for count in range(0, 42):
            self.send_data(self.lut_ww[count])
        self.send_command(LUT_BLACK_TO_WHITE)         # bw r
        for count in range(0, 42):
            self.send_data(self.lut_bw[count])
        self.send_command(LUT_WHITE_TO_BLACK)         # wb w
        for count in range(0, 42):
            self.send_data(self.lut_bb[count])
        self.send_command(LUT_BLACK_TO_BLACK)         # bb b
        for count in range(0, 42):
            self.send_data(self.lut_wb[count])
            
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(POWER_SETTING)
        self.send_data(0x03)                  # VDS_EN, VDG_EN
        self.send_data(0x00)                  # VCOM_HV, VGHL_LV[1], VGHL_LV[0]
        self.send_data(0x2b)                  # VDH
        self.send_data(0x2b)                  # VDL
        self.send_command(BOOSTER_SOFT_START)
        self.send_data(0x17)
        self.send_data(0x17)
        self.send_data(0x17)                 
        self.send_command(POWER_ON)
        self.wait_until_idle()
        self.send_command(PANEL_SETTING)
        self.send_data(0xbf)    # KW-BF   KWR-AF  BWROTP 0f
        self.send_data(0x0d)
        self.send_command(PLL_CONTROL)
        self.send_data(0x3c)        # 3A 100HZ   29 150Hz 39 200HZ  31 171HZ

        self.send_command(0x61);	# resolution setting
        self.send_data(0x01);
        self.send_data(0x90);       # 128
        self.send_data(0x01);		
        self.send_data(0x2c);

        self.send_command(0x82);	# vcom_DC setting
        self.send_data(0x28);

        self.send_command(0X50);	# VCOM AND DATA INTERVAL SETTING
        self.send_data(0x97);		# 97white border 77black border		VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7
    
        self.set_lut()
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        buf = [0xFF] * ((self.width//8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # print "imwidth = %d, imheight = %d",imwidth,imheight
        if(imwidth == self.width and imheight == self.height):
            print("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[(x + y * self.width) // 8] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            print("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[(newx + newy*self.width) // 8] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, image):
        self.send_command(DATA_START_TRANSMISSION_1)
        for i in range(0, self.width * self.height // 8):
            self.send_data(0xFF)
        self.send_command(DATA_START_TRANSMISSION_2)
        for i in range(0, self.width * self.height // 8):
            self.send_data(image[i])
        self.send_command(DISPLAY_REFRESH) 
        self.wait_until_idle()
        
    def Clear(self, color):
        self.send_command(DATA_START_TRANSMISSION_1)
        for i in range(0, self.width * self.height // 8):
            self.send_data(0xFF)
        self.send_command(DATA_START_TRANSMISSION_2)
        for i in range(0, self.width * self.height // 8):
            self.send_data(0xFF)
        self.send_command(DISPLAY_REFRESH) 
        self.wait_until_idle()

    def sleep(self):
        self.send_command(POWER_OFF)
        self.wait_until_idle()
        self.send_command(DEEP_SLEEP)
        self.send_data(0XA5)
### END OF FILE ###

