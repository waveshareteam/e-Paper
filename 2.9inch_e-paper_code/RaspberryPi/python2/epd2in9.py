# /*****************************************************************************
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
#   epd = epd2in9.EPD()
#   epd.init(epd.lut_full_update)
#   image = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)
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
EPD_WIDTH       = 128
EPD_HEIGHT      = 296

# EPD2IN9 commands
DRIVER_OUTPUT_CONTROL                       = 0x01
BOOSTER_SOFT_START_CONTROL                  = 0x0C
GATE_SCAN_START_POSITION                    = 0x0F
DEEP_SLEEP_MODE                             = 0x10
DATA_ENTRY_MODE_SETTING                     = 0x11
SW_RESET                                    = 0x12
TEMPERATURE_SENSOR_CONTROL                  = 0x1A
MASTER_ACTIVATION                           = 0x20
DISPLAY_UPDATE_CONTROL_1                    = 0x21
DISPLAY_UPDATE_CONTROL_2                    = 0x22
WRITE_RAM                                   = 0x24
WRITE_VCOM_REGISTER                         = 0x2C
WRITE_LUT_REGISTER                          = 0x32
SET_DUMMY_LINE_PERIOD                       = 0x3A
SET_GATE_TIME                               = 0x3B
BORDER_WAVEFORM_CONTROL                     = 0x3C
SET_RAM_X_ADDRESS_START_END_POSITION        = 0x44
SET_RAM_Y_ADDRESS_START_END_POSITION        = 0x45
SET_RAM_X_ADDRESS_COUNTER                   = 0x4E
SET_RAM_Y_ADDRESS_COUNTER                   = 0x4F
TERMINATE_FRAME_READ_WRITE                  = 0xFF

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.cs_pin = epdconfig.CS_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    lut_full_update = [
        0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
        0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
        0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
        0x35, 0x51, 0x51, 0x19, 0x01, 0x00
    ]

    lut_partial_update  = [
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
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
        while(epdconfig.digital_read(self.busy_pin) == 1):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    

    def TurnOnDisplay(self):
        self.send_command(DISPLAY_UPDATE_CONTROL_2)
        self.send_data(0xC4)
        self.send_command(MASTER_ACTIVATION)
        self.send_command(TERMINATE_FRAME_READ_WRITE)
        
        self.wait_until_idle()

    def SetWindow(self, x_start, y_start, x_end, y_end):
        self.send_command(SET_RAM_X_ADDRESS_START_END_POSITION)
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        self.send_data((x_start >> 3) & 0xFF)
        self.send_data((x_end >> 3) & 0xFF)
        self.send_command(SET_RAM_Y_ADDRESS_START_END_POSITION)
        self.send_data(y_start & 0xFF)
        self.send_data((y_start >> 8) & 0xFF)
        self.send_data(y_end & 0xFF)
        self.send_data((y_end >> 8) & 0xFF)

    def SetCursor(self, x, y):
        self.send_command(SET_RAM_X_ADDRESS_COUNTER)
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        self.send_data((x >> 3) & 0xFF)
        self.send_command(SET_RAM_Y_ADDRESS_COUNTER)
        self.send_data(y & 0xFF)
        self.send_data((y >> 8) & 0xFF)
        self.wait_until_idle()
        
    def init(self, lut):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(DRIVER_OUTPUT_CONTROL)
        self.send_data((EPD_HEIGHT - 1) & 0xFF)
        self.send_data(((EPD_HEIGHT - 1) >> 8) & 0xFF)
        self.send_data(0x00)                     # GD = 0 SM = 0 TB = 0
        self.send_command(BOOSTER_SOFT_START_CONTROL)
        self.send_data(0xD7)
        self.send_data(0xD6)
        self.send_data(0x9D)
        self.send_command(WRITE_VCOM_REGISTER)
        self.send_data(0xA8)                     # VCOM 7C
        self.send_command(SET_DUMMY_LINE_PERIOD)
        self.send_data(0x1A)                     # 4 dummy lines per gate
        self.send_command(SET_GATE_TIME)
        self.send_data(0x08)                     # 2us per line
        self.send_command(BORDER_WAVEFORM_CONTROL)
        self.send_data(0x03)
        self.send_command(DATA_ENTRY_MODE_SETTING)
        self.send_data(0x03)                     # X increment Y increment
        
        self.send_command(WRITE_LUT_REGISTER)
        for i in range(0, len(lut)):
            self.send_data(lut[i])
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        # print "bufsiz = ",(self.width/8) * self.height
        buf = [0xFF] * ((self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        # print "imwidth = %d, imheight = %d",imwidth,imheight
        if(imwidth == self.width and imheight == self.height):
            print "Vertical"
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[(x + y * self.width) / 8] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            print "Horizontal"
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[(newx + newy*self.width) / 8] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, image):
        if (image == None):
            return            
        self.SetWindow(0, 0, self.width - 1, self.height - 1)
        for j in range(0, self.height):
            self.SetCursor(0, j)
            self.send_command(WRITE_RAM)
            for i in range(0, self.width / 8):
                self.send_data(image[i + j * (self.width / 8)])   
        self.TurnOnDisplay()
        
    def Clear(self, color):
        self.SetWindow(0, 0, self.width - 1, self.height - 1)
        for j in range(0, self.height):
            self.SetCursor(0, j)
            self.send_command(WRITE_RAM)
            for i in range(0, self.width / 8):
                self.send_data(color)   
        self.TurnOnDisplay()

    def sleep(self):
        self.send_command(DEEP_SLEEP_MODE)
        self.send_data(0x01)

### END OF FILE ###

