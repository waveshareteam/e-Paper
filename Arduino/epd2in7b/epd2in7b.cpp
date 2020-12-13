/**
 *  @filename   :   epd2in7b.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
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
#include "epd2in7b.h"

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
     
    SendCommand(POWER_ON);
    WaitUntilIdle();
   
    SendCommand(PANEL_SETTING);
    SendData(0xaf);        //KW-BF   KWR-AF    BWROTP 0f
    
    SendCommand(PLL_CONTROL);
    SendData(0x3a);       //3A 100HZ   29 150Hz 39 200HZ    31 171HZ
    
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
    SendData(0x73);
    SendData(0x41);
    
    SendCommand(VCM_DC_SETTING_REGISTER);
    SendData(0x12);                   
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x87);        // define by OTP
    
    SetLut();
    
    SendCommand(PARTIAL_DISPLAY_REFRESH);
    SendData(0x00);  
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
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
    DigitalWrite(reset_pin, LOW);
    DelayMs(10);
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
void Epd::TransmitPartial(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l) {   
    if (buffer_black != NULL) {
        SendCommand(PARTIAL_DATA_START_TRANSMISSION_1);
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
            SendData(buffer_black[i]);  
        }  
        DelayMs(2);                  
    }
    if (buffer_red != NULL) {
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
            SendData(buffer_red[i]);  
        }  
        DelayMs(2);                  
    }
}

/**
 *  @brief: transmit partial data to the black part of SRAM
 */
void Epd::TransmitPartialBlack(const unsigned char* buffer_black, int x, int y, int w, int l) {
    if (buffer_black != NULL) {
        SendCommand(PARTIAL_DATA_START_TRANSMISSION_1);
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
            SendData(buffer_black[i]);  
        }  
        DelayMs(2);                  
    }
}

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
void Epd::TransmitPartialRed(const unsigned char* buffer_red, int x, int y, int w, int l) {
    if (buffer_red != NULL) {
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
            SendData(buffer_red[i]);  
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
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    SendCommand(TCON_RESOLUTION);
    SendData(width >> 8);
    SendData(width & 0xff);        //176      
    SendData(height >> 8);        
    SendData(height & 0xff);         //264

    if (frame_buffer_black != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);           
        DelayMs(2);
        for(int i = 0; i < width * height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_black[i]));  
        }  
        DelayMs(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(DATA_START_TRANSMISSION_2);
        DelayMs(2);
        for(int i = 0; i < width * height / 8; i++) {
            SendData(pgm_read_byte(&frame_buffer_red[i]));  
        }  
        DelayMs(2);
    }
    SendCommand(DISPLAY_REFRESH); 

    WaitUntilIdle();
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(TCON_RESOLUTION);
    SendData(width >> 8);
    SendData(width & 0xff);        //176      
    SendData(height >> 8);        
    SendData(height & 0xff);         //264

    SendCommand(DATA_START_TRANSMISSION_1);           
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0x00);  
    }  
    DelayMs(2);
    SendCommand(DATA_START_TRANSMISSION_2);           
    DelayMs(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0x00);  
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

const unsigned char lut_vcom_dc[] =
{
0x00, 0x00,
0x00, 0x1A, 0x1A, 0x00, 0x00, 0x01,        
0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,        
0x00, 0x0E, 0x01, 0x0E, 0x01, 0x10,        
0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,        
0x00, 0x04, 0x10, 0x00, 0x00, 0x05,        
0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,        
0x00, 0x23, 0x00, 0x00, 0x00, 0x01    
};

//R21H
const unsigned char lut_ww[] =
{
0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
0x40, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
0x80, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R22H    r
const unsigned char lut_bw[] =
{
0xA0, 0x1A, 0x1A, 0x00, 0x00, 0x01,
0x00, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
0x90, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0xB0, 0x04, 0x10, 0x00, 0x00, 0x05,
0xB0, 0x03, 0x0E, 0x00, 0x00, 0x0A,
0xC0, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R23H    w
const unsigned char lut_bb[] =
{
0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
0x40, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
0x80, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};

//R24H    b
const unsigned char lut_wb[] =
{
0x90, 0x1A, 0x1A, 0x00, 0x00, 0x01,
0x20, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x84, 0x0E, 0x01, 0x0E, 0x01, 0x10,
0x10, 0x0A, 0x0A, 0x00, 0x00, 0x08,
0x00, 0x04, 0x10, 0x00, 0x00, 0x05,
0x00, 0x03, 0x0E, 0x00, 0x00, 0x0A,
0x00, 0x23, 0x00, 0x00, 0x00, 0x01
};



/* END OF FILE */
