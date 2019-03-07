# /*****************************************************************************
# * | File        :	  EPD_1in54.py
# * | Author      :   Waveshare team
# * | Function    :   Electronic paper driver
# * | Info        :
# *----------------
# * |	This version:   V3.0
# * | Date        :   2018-11-05
# * | Info        :   python2 demo
# * 1.Remove:
#   digital_write(self, pin, value)
#   digital_read(self, pin)
#   delay_ms(self, delaytime)
#   set_rotate(self, rotate)
#   set_pixel(self, frame_buffer, x, y, colored)
#   set_absolute_pixel(self, frame_buffer, x, y, colored)
#   display_string_at(self, frame_buffer, x, y, text, font, colored)
#   draw_line(self, frame_buffer, x0, y0, x1, y1, colored)
#   draw_horizontal_line(self, frame_buffer, x, y, width, colored)
#   draw_vertical_line(self, frame_buffer, x, y, height, colored)
#   draw_rectangle(self, frame_buffer, x0, y0, x1, y1, colored):
#   draw_filled_rectangle(self, frame_buffer, x0, y0, x1, y1, colored):
#   draw_circle(self, frame_buffer, x, y, radius, colored):
#   draw_filled_circle(self, frame_buffer, x, y, radius, colored):
# * 2.Change:
#   get_frame_buffer -> getbuffer
#   display_frame -> display
# * 3.How to use
#   epd = epd1in54b.EPD()
#   epd.init()
#   image = Image.new('1', (epd1in54b.EPD_WIDTH, epd1in54b.EPD_HEIGHT), 255)
#   ...
#   drawing ......
#   ...
#   epd.display(getbuffer(image))
# ******************************************************************************/
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
import Image
import RPi.GPIO as GPIO

# Display resolution
EPD_WIDTH       = 200
EPD_HEIGHT      = 200

# EPD1IN54B commands
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
PLL_CONTROL                                 = 0x30
TEMPERATURE_SENSOR_COMMAND                  = 0x40
TEMPERATURE_SENSOR_CALIBRATION              = 0x41
TEMPERATURE_SENSOR_WRITE                    = 0x42
TEMPERATURE_SENSOR_READ                     = 0x43
VCOM_AND_DATA_INTERVAL_SETTING              = 0x50
LOW_POWER_DETECTION                         = 0x51
TCON_SETTING                                = 0x60
TCON_RESOLUTION                             = 0x61
SOURCE_AND_GATE_START_SETTING               = 0x62
GET_STATUS                                  = 0x71
AUTO_MEASURE_VCOM                           = 0x80
VCOM_VALUE                                  = 0x81
VCM_DC_SETTING_REGISTER                     = 0x82
PROGRAM_MODE                                = 0xA0
ACTIVE_PROGRAM                              = 0xA1
READ_OTP_DATA                               = 0xA2

class EPD:
    def __init__(self):
        self.reset_pin = epdconfig.RST_PIN
        self.dc_pin = epdconfig.DC_PIN
        self.busy_pin = epdconfig.BUSY_PIN
        self.width = EPD_WIDTH
        self.height = EPD_HEIGHT

    lut_vcom0 = [  
        0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
        0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00   
    ]

    lut_w = [  
        0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
        0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04   
    ]

    lut_b = [  
        0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
        0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04   
    ]

    lut_g1 = [
        0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
        0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04   
    ]

    lut_g2 = [ 
        0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
        0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04   
    ]

    lut_vcom1 = [  
        0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   
    ]

    lut_red0 = [   
        0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   
    ]

    lut_red1 = [   
        0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   
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
        epdconfig.digital_write(self.dc_pin, GPIO.LOW)
        epdconfig.spi_writebyte([command])

    def send_data(self, data):
        epdconfig.digital_write(self.dc_pin, GPIO.HIGH)
        epdconfig.spi_writebyte([data])
        
    def wait_until_idle(self):
        while(epdconfig.digital_read(self.busy_pin) == 0):      # 0: idle, 1: busy
            epdconfig.delay_ms(100)    
      
    def set_lut_bw(self):
        self.send_command(0x20)               # vcom
        for count in range(0, 15):
            self.send_data(self.lut_vcom0[count])
        self.send_command(0x21)         # ww --
        for count in range(0, 15):
            self.send_data(self.lut_w[count])
        self.send_command(0x22)         # bw r
        for count in range(0, 15):
            self.send_data(self.lut_b[count])
        self.send_command(0x23)         # wb w
        for count in range(0, 15):
            self.send_data(self.lut_g1[count])
        self.send_command(0x24)         # bb b
        for count in range(0, 15):
            self.send_data(self.lut_g2[count])

    def set_lut_red(self):
        self.send_command(0x25)
        for count in range(0, 15):
            self.send_data(self.lut_vcom1[count])
        self.send_command(0x26)
        for count in range(0, 15):
            self.send_data(self.lut_red0[count])
        self.send_command(0x27)
        for count in range(0, 15):
            self.send_data(self.lut_red1[count])
            
    def init(self):
        if (epdconfig.module_init() != 0):
            return -1
        # EPD hardware init start
        self.reset()
        
        self.send_command(POWER_SETTING) 
        self.send_data(0x07)
        self.send_data(0x00)
        self.send_data(0x08)
        self.send_data(0x00)
        self.send_command(BOOSTER_SOFT_START)
        self.send_data(0x07)
        self.send_data(0x07)
        self.send_data(0x07)
        self.send_command(POWER_ON)

        self.wait_until_idle()

        self.send_command(PANEL_SETTING)
        self.send_data(0xCF)
        self.send_command(VCOM_AND_DATA_INTERVAL_SETTING)
        self.send_data(0x17)
        self.send_command(PLL_CONTROL)
        self.send_data(0x39)
        self.send_command(TCON_RESOLUTION)
        self.send_data(0xC8)
        self.send_data(0x00)
        self.send_data(0xC8)
        self.send_command(VCM_DC_SETTING_REGISTER)
        self.send_data(0x0E)
        
        self.set_lut_bw()
        self.set_lut_red()
        return 0

    def getbuffer(self, image):
        buf = [0xFF] * ((self.width/8) * self.height)
        image_monocolor = image.convert('1')
        imwidth, imheight = image_monocolor.size
        pixels = image_monocolor.load()
        if(imwidth == self.width and imheight == self.height):
            print("Horizontal")
            for y in range(imheight):
                for x in range(imwidth):
                    # Set the bits for the column of pixels at the current position.
                    if pixels[x, y] == 0:
                        buf[(x + y * self.width) / 8] &= ~(0x80 >> (x % 8))
        elif(imwidth == self.height and imheight == self.width):
            print("Vertical")
            for y in range(imheight):
                for x in range(imwidth):
                    newx = y
                    newy = self.height - x - 1
                    if pixels[x, y] == 0:
                        buf[(newx + newy*self.width) / 8] &= ~(0x80 >> (y % 8))
        return buf

    def display(self, blackimage, redimage):
        if (blackimage != None):
            self.send_command(DATA_START_TRANSMISSION_1)
            for i in range(0, self.width * self.height // 8):
                temp = 0x00
                for bit in range(0, 4):
                    if (blackimage[i] & (0x80 >> bit) != 0):
                        temp |= 0xC0 >> (bit * 2)
                self.send_data(temp)  
                temp = 0x00
                for bit in range(4, 8):
                    if (blackimage[i] & (0x80 >> bit) != 0):
                        temp |= 0xC0 >> ((bit - 4) * 2)
                self.send_data(temp)          
        if (redimage != None):
            self.send_command(DATA_START_TRANSMISSION_2)
            for i in range(0, self.width * self.height // 8):
                self.send_data(redimage[i])  

        self.send_command(DISPLAY_REFRESH)
        self.wait_until_idle()
##
 #  @brief: clear the frame memory with the specified color.
 #          this won't update the display.
 ##
    def Clear(self):
        self.send_command(DATA_START_TRANSMISSION_1)
        for i in range(0, self.width * self.height // 8):
            self.send_data(0xFF)
            self.send_data(0xFF)
            
        self.send_command(DATA_START_TRANSMISSION_2)
        for i in range(0, self.width * self.height // 8):
            self.send_data(0xFF)

        self.send_command(DISPLAY_REFRESH)
        self.wait_until_idle()


    # after this, call epd.init() to awaken the module
    def sleep(self):
        self.send_command(VCOM_AND_DATA_INTERVAL_SETTING)
        self.send_data(0x17)
        self.send_command(VCM_DC_SETTING_REGISTER)         #to solve Vcom drop 
        self.send_data(0x00)        
        self.send_command(POWER_SETTING)         #power setting      
        self.send_data(0x02)        #gate switch to external
        self.send_data(0x00)
        self.send_data(0x00) 
        self.send_data(0x00) 
        self.wait_until_idle()
        self.send_command(POWER_OFF)         #power off

### END OF FILE ###

