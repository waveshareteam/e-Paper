# *****************************************************************************
# * | File        :	  epd1in54.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * | This version:   V3.1
# * | Date        :   2019-06-20
# # | Info        :   python demo
# -----------------------------------------------------------------------------
# V3.1(2019-06-18):
# 2.remove commands define:
#   #define PANEL_SETTING                               0x00
#   #define POWER_SETTING                               0x01
#   #define POWER_OFF                                   0x02
#   #define POWER_OFF_SEQUENCE_SETTING                  0x03
#   #define POWER_ON                                    0x04
#   #define POWER_ON_MEASURE                            0x05
#   #define BOOSTER_SOFT_START                          0x06
#   #define DEEP_SLEEP                                  0x07
#   #define DATA_START_TRANSMISSION_1                   0x10
#   #define DATA_STOP                                   0x11
#   #define DISPLAY_REFRESH                             0x12
#   #define DATA_START_TRANSMISSION_2                   0x13
#   #define PLL_CONTROL                                 0x30
#   #define TEMPERATURE_SENSOR_COMMAND                  0x40
#   #define TEMPERATURE_SENSOR_CALIBRATION              0x41
#   #define TEMPERATURE_SENSOR_WRITE                    0x42
#   #define TEMPERATURE_SENSOR_READ                     0x43
#   #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#   #define LOW_POWER_DETECTION                         0x51
#   #define TCON_SETTING                                0x60
#   #define TCON_RESOLUTION                             0x61
#   #define SOURCE_AND_GATE_START_SETTING               0x62
#   #define GET_STATUS                                  0x71
#   #define AUTO_MEASURE_VCOM                           0x80
#   #define VCOM_VALUE                                  0x81
#   #define VCM_DC_SETTING_REGISTER                     0x82
#   #define PROGRAM_MODE                                0xA0
#   #define ACTIVE_PROGRAM                              0xA1
#   #define READ_OTP_DATA                               0xA2
# -----------------------------------------------------------------------------
# V3.0(2018-11-01):
# # 1.Remove:
#   digital_write(self, pin, value)
#   digital_read(self, pin)
#   delay_ms(self, delaytime)
#   set_lut(self, lut)
#   self.lut = self.lut_full_update
# * 2.Change:
#   display_frame -> TurnOnDisplay
#   set_memory_area -> SetWindow
#   set_memory_pointer -> SetCursor
# * 3.How to use
#   epd = epd1in54.EPD()
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

import logging
from . import epdconfig

# Display resolution
EPD_WIDTH       = 200
EPD_HEIGHT      = 200

logger = logging.getLogger(__name__)

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
        epdconfig.digital_write(self.reset_pin, 1)
        epdconfig.delay_ms(200) 
        epdconfig.digital_write(self.reset_pin, 0)         # module reset
        epdconfig.delay_ms(5)
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
        logger.debug("e-Paper busy")
        while(epdconfig.digital_read(self.busy_pin) == 1):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)
        logger.debug("e-Paper busy release")

    def TurnOnDisplay(self):
        self.send_command(0x22) # DISPLAY_UPDATE_CONTROL_2
        self.send_data(0xC4)
        self.send_command(0x20) # MASTER_ACTIVATION
        self.send_command(0xFF) # TERMINATE_FRAME_READ_WRITE
        
        self.ReadBusy()

    def SetWindow(self, x_start, y_start, x_end, y_end):
        self.send_command(0x44) # SET_RAM_X_ADDRESS_START_END_POSITION
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        self.send_data((x_start >> 3) & 0xFF)
        self.send_data((x_end >> 3) & 0xFF)
        self.send_command(0x45) # SET_RAM_Y_ADDRESS_START_END_POSITION
        self.send_data(y_start & 0xFF)
        self.send_data((y_start >> 8) & 0xFF)
        self.send_data(y_end & 0xFF)
        self.send_data((y_end >> 8) & 0xFF)

    def SetCursor(self, x, y):
        self.send_command(0x4E) # SET_RAM_X_ADDRESS_COUNTER
        # x point must be the multiple of 8 or the last 3 bits will be ignored
        self.send_data((x >> 3) & 0xFF)
        
        self.send_command(0x4F) # SET_RAM_Y_ADDRESS_COUNTER
        self.send_data(y & 0xFF)
        self.send_data((y >> 8) & 0xFF)
        # self.ReadBusy()
        
    def init(self, lut):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(0x01) # DRIVER_OUTPUT_CONTROL
        self.send_data((EPD_HEIGHT - 1) & 0xFF)
        self.send_data(((EPD_HEIGHT - 1) >> 8) & 0xFF)
        self.send_data(0x00) # GD = 0 SM = 0 TB = 0
        
        self.send_command(0x0C) # BOOSTER_SOFT_START_CONTROL
        self.send_data(0xD7)
        self.send_data(0xD6)
        self.send_data(0x9D)
        
        self.send_command(0x2C) # WRITE_VCOM_REGISTER
        self.send_data(0xA8) # VCOM 7C
        
        self.send_command(0x3A) # SET_DUMMY_LINE_PERIOD
        self.send_data(0x1A) # 4 dummy lines per gate
        
        self.send_command(0x3B) # SET_GATE_TIME
        self.send_data(0x08) # 2us per line
        
        self.send_command(0x11) # DATA_ENTRY_MODE_SETTING
        self.send_data(0x03) # X increment Y increment
        
        # set the look-up table register
        self.send_command(0x32)
        for i in range(0, len(lut)):
            self.send_data(lut[i])
        # EPD hardware init end
        return 0

    def getbuffer(self, image):
        buf = [0xFF] * (int(self.width / 8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        if(imwidth == self.width and imheight == self.height):
            logger.debug("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[int((x + y * self.width) / 8)] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            logger.debug("Vertical")
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
            
        self.SetWindow(0, 0, self.width, self.height)
        for j in range(0, self.height):
            self.SetCursor(0, j)
            self.send_command(0x24)
            for i in range(0, int(self.width / 8)):
                self.send_data(image[i + j * int(self.width / 8)])   
        self.TurnOnDisplay()
        
    def Clear(self, color=0xFF):
        # self.SetWindow(0, 0, self.width - 1, self.height - 1)
        # send the color data
        self.SetWindow(0, 0, self.width, self.height)
        # epdconfig.digital_write(self.dc_pin, 1)
        # epdconfig.digital_write(self.cs_pin, 0)
        for j in range(0, self.height):
            self.SetCursor(0, j)
            self.send_command(0x24)
            for i in range(0, int(self.width / 8)):
                self.send_data(color)
        # epdconfig.digital_write(self.cs_pin, 1)
        self.TurnOnDisplay()

    def sleep(self):
        self.send_command(0x10) # DEEP_SLEEP_MODE
        self.send_data(0x01)
        
        epdconfig.delay_ms(2000)
        epdconfig.module_exit()
### END OF FILE ###

