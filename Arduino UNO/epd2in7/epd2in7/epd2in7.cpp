/**
 *  @filename   :   epd2in7.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 18 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <epd2in7.h>

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
    SendCommand(POWER_SETTING);
    SendData(0x03);                  // VDS_EN, VDG_EN
    SendData(0x00);                  // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    SendData(0x2b);                  // VDH
    SendData(0x2b);                  // VDL
    SendData(0x09);                  // VDHR
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x07);
    SendData(0x07);
    SendData(0x17);
    // Power optimization
    SendCommand(0xF8);
    SendData(0x60);
    SendData(0xA5);
    // Power optimization
    SendCommand(0xF8);
    SendData(0x89);
    SendData(0xA5);
    // Power optimization
    SendCommand(0xF8);
    SendData(0x90);
    SendData(0x00);
    // Power optimization
    SendCommand(0xF8);
    SendData(0x93);
    SendData(0x2A);
    // Power optimization
    SendCommand(0xF8);
    SendData(0xA0);
    SendData(0xA5);
    // Power optimization
    SendCommand(0xF8);
    SendData(0xA1);
    SendData(0x00);
    // Power optimization
    SendCommand(0xF8);
    SendData(0x73);
    SendData(0x41);
    SendCommand(PARTIAL_DISPLAY_REFRESH);
    SendData(0x00);
    SendCommand(POWER_ON);
    WaitUntilIdle();

    SendCommand(PANEL_SETTING);
    SendData(0xAF);        //KW-BF   KWR-AF    BWROTP 0f
    SendCommand(PLL_CONTROL);
    SendData(0x3A);       //3A 100HZ   29 150Hz 39 200HZ    31 171HZ
    SendCommand(VCM_DC_SETTING_REGISTER);
    SendData(0x12);
    DelayMs(2);
    SetLut();
    /* EPD hardware init end */
    return 0;

}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void) {
    while(DigitalRead(busy_pin) == 0) {      //0: busy, 1: idle
        DelayMs(100);
    }      
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, LOW);
    DelayMs(200);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
}

/**
 *  @brief: set the look-up tables
 */
void Epd::SetLut(void) {
    unsigned int count;     
    SendCommand(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 44; count++) {
        SendData(lut_vcom_dc[count]);
    }
    
    SendCommand(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 42; count++) {
        SendData(lut_ww[count]);
    }   
    
    SendCommand(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 42; count++) {
        SendData(lut_bw[count]);
    } 

    SendCommand(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 42; count++) {
        SendData(lut_bb[count]);
    } 

    SendCommand(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 42; count++) {
        SendData(lut_wb[count]);
    } 
}

/**
 *  @brief: transmit partial data to the SRAM
 */
void Epd::TransmitPartialData(const unsigned char* buffer, int x, int y, int w, int l) {
    if (buffer != NULL) {
        SendCommand(PARTIAL_DATA_START_TRANSMISSION_2);
        SendData(x >> 8);
        SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
        SendData(y >> 8);        
        SendData(y & 0xff);
        SendData(w >> 8);
        SendData(w & 0xf8);     // w (width) should be the multiple of 8, the last 3 bit will always be ignored
        SendData(l >> 8);        
        SendData(l & 0xff);
        DelayMs(2);
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer[i]);  
        }  
        DelayMs(2);                  
    }
}

/**
 * @brief: refreshes a specific part of the display
 */
void Epd::RefreshPartial(int x, int y, int w, int l) {
    SendCommand(PARTIAL_DISPLAY_REFRESH); 
    SendData(x >> 8);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(y >> 8);        
    SendData(y & 0xff);
    SendData(w >> 8);
    SendData(w & 0xf8);     // w (width) should be the multiple of 8, the last 3 bit will always be ignored
    SendData(l >> 8);        
    SendData(l & 0xff);
   
    WaitUntilIdle();
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer) {
    if (frame_buffer != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);           
        DelayMs(2);
        for(int i = 0; i < this->width / 8 * this->height; i++) {
            SendData(0xFF);
        }  
        DelayMs(2);                  
        SendCommand(DATA_START_TRANSMISSION_2);
        DelayMs(2);
        for(int i = 0; i < this->width / 8 * this->height; i++) {
            SendData(pgm_read_byte(&frame_buffer[i]));  
        }  
        DelayMs(2);
        SendCommand(DISPLAY_REFRESH); 
        WaitUntilIdle();
    }
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(DATA_START_TRANSMISSION_1);           
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }  
    DelayMs(2);
    SendCommand(DATA_START_TRANSMISSION_2);           
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }  
    DelayMs(2);
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    SendCommand(DISPLAY_REFRESH); 
    WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
  SendCommand(DEEP_SLEEP);
  SendData(0xa5);
}

const unsigned char lut_vcom_dc[] = {
    0x00, 0x00, 
    0x00, 0x0F, 0x0F, 0x00, 0x00, 0x05,      
    0x00, 0x32, 0x32, 0x00, 0x00, 0x02,      
    0x00, 0x0F, 0x0F, 0x00, 0x00, 0x05,      
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//R21H
const unsigned char lut_ww[] = {
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//R22H    r
const unsigned char lut_bw[] =
{
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//R24H    b
const unsigned char lut_bb[] =
{
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//R23H    w
const unsigned char lut_wb[] =
{
    0xA0, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x60, 0x32, 0x32, 0x00, 0x00, 0x02,
    0x50, 0x0F, 0x0F, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};



/* END OF FILE */


