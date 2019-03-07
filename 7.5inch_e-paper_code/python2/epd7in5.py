# /*****************************************************************************
# * | File        :	  epd7in5.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * |	This version:   V3.0
# * | Date        :   2018-11-12
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
#   image = Image.new('1', (epd7in5.EPD_WIDTH, epd7in5.EPD_HEIGHT), 255)
#   ...
#   drawing ......
#   ...
#   epd.display(getbuffer(image))
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


import epdconfig
from PIL import Image
import RPi.GPIO as GPIO

# Display resolution
EPD_WIDTH       = 640
EPD_HEIGHT      = 384

# EPD7IN5 commands
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
IMAGE_PROCESS                               = 0x13
LUT_FOR_VCOM                                = 0x20
LUT_BLUE                                    = 0x21
LUT_WHITE                                   = 0x22
LUT_GRAY_1                                  = 0x23
LUT_GRAY_2                                  = 0x24
LUT_RED_0                                   = 0x25
LUT_RED_1                                   = 0x26
LUT_RED_2                                   = 0x27
LUT_RED_3                                   = 0x28
LUT_XON                                     = 0x29
PLL_CONTROL                                 = 0x30
TEMPERATURE_SENSOR_COMMAND                  = 0x40
TEMPERATURE_CALIBRATION                     = 0x41
TEMPERATURE_SENSOR_WRITE                    = 0x42
TEMPERATURE_SENSOR_READ                     = 0x43
VCOM_AND_DATA_INTERVAL_SETTING              = 0x50
LOW_POWER_DETECTION                         = 0x51
TCON_SETTING                                = 0x60
TCON_RESOLUTION                             = 0x61
SPI_FLASH_CONTROL                           = 0x65
REVISION                                    = 0x70
GET_STATUS                                  = 0x71
AUTO_MEASUREMENT_VCOM                       = 0x80
READ_VCOM_VALUE                             = 0x81
VCM_DC_SETTING                              = 0x82

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT
    
    # Hardware reset
    def reset(self):
        epdconfig.digital_write(self.reset_pin, GPIO.HIGH)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, GPIO.LOW)         # module reset
        epdconfig.delay_ms(200)
        epdconfig.digital_write(self.reset_pin, GPIO.HIGH)
        epdconfig.delay_ms(200)   

    def send_command(self, command):
        epdconfig.digital_write(self.dc_pin, GPIO.LOW)
        epdconfig.spi_writebyte([command])

    def send_data(self, data):
        epdconfig.digital_write(self.dc_pin, GPIO.HIGH)
        epdconfig.spi_writebyte([data])
        
    def wait_until_idle(self):
        print("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    
        print("e-Paper busy release")
        
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(POWER_SETTING)
        self.send_data(0x37)
        self.send_data(0x00)
        self.send_command(PANEL_SETTING)
        self.send_data(0xCF)
        self.send_data(0x08)
        self.send_command(BOOSTER_SOFT_START)
        self.send_data(0xc7)
        self.send_data(0xcc)
        self.send_data(0x28)
        self.send_command(POWER_ON)
        self.wait_until_idle()
        self.send_command(PLL_CONTROL)
        self.send_data(0x3c)
        self.send_command(TEMPERATURE_CALIBRATION)
        self.send_data(0x00)
        self.send_command(VCOM_AND_DATA_INTERVAL_SETTING)
        self.send_data(0x77)
        self.send_command(TCON_SETTING)
        self.send_data(0x22)
        self.send_command(TCON_RESOLUTION)
        self.send_data(EPD_WIDTH >> 8)     #source 640
        self.send_data(EPD_WIDTH & 0xff)
        self.send_data(EPD_HEIGHT >> 8)     #gate 384
        self.send_data(EPD_HEIGHT & 0xff)
        self.send_command(VCM_DC_SETTING)
        self.send_data(0x1E)      #decide by LUT file
        self.send_command(0xe5)           #FLASH MODE
        self.send_data(0x03)
        
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        buf = [0x00] * (self.width * self.height / 4)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        print("imwidth = ", imwidth, "imheight = ", imheight)
        if(imwidth == self.width and imheight == self.height):
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] < 64:           # black
                        buf[(x + y * self.width) / 4] &= ~(0xC0 >> (x % 4 * 2))
                    elif pixels[x, y] < 192:     # convert gray to red
                        buf[(x + y * self.width) / 4] &= ~(0xC0 >> (x % 4 * 2))
                        buf[(x + y * self.width) / 4] |= 0x40 >> (x % 4 * 2)
                    else:                           # white
                        buf[(x + y * self.width) / 4] |= 0xC0 >> (x % 4 * 2)
        elif(imwidth == self.height and imheight == self.width):
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1                    
                    if pixels[x, y] < 64:           # black
                        buf[(newx + newy*self.width) / 4] &= ~(0xC0 >> (y % 4 * 2))
                    elif pixels[x, y] < 192:     # convert gray to red
                        buf[(newx + newy*self.width) / 4] &= ~(0xC0 >> (y % 4 * 2))
                        buf[(newx + newy*self.width) / 4] |= 0x40 >> (y % 4 * 2)
                    else:                           # white
                        buf[(newx + newy*self.width) / 4] |= 0xC0 >> (y % 4 * 2)
        return buf    
        
    def display(self, image):
        self.send_command(DATA_START_TRANSMISSION_1)
        for i in range(0, self.width / 4 * self.height):
            temp1 = image[i]
            j = 0
            while (j < 4):
                if ((temp1 & 0xC0) == 0xC0):
                    temp2 = 0x03
                elif ((temp1 & 0xC0) == 0x00):
                    temp2 = 0x00
                else:
                    temp2 = 0x04
                temp2 = (temp2 << 4) & 0xFF
                temp1 = (temp1 << 2) & 0xFF
                j += 1
                if((temp1 & 0xC0) == 0xC0):
                    temp2 |= 0x03
                elif ((temp1 & 0xC0) == 0x00):
                    temp2 |= 0x00
                else:
                    temp2 |= 0x04
                temp1 = (temp1 << 2) & 0xFF
                self.send_data(temp2)
                j += 1
        self.send_command(DISPLAY_REFRESH)
        epdconfig.delay_ms(100)
        self.wait_until_idle()
        
    def Clear(self, color):
        self.send_command(DATA_START_TRANSMISSION_1)
        for i in range(0, self.width / 4 * self.height):
            for j in range(0, 4):
                self.send_data(0x33)
        self.send_command(DISPLAY_REFRESH)
        self.wait_until_idle()

    def sleep(self):
        self.send_command(POWER_OFF)
        self.wait_until_idle()
        self.send_command(DEEP_SLEEP)
        self.send_data(0XA5)
### END OF FILE ###

