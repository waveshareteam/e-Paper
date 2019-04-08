/**
 *  @filename   :   epd7in5b.cpp
 *  @brief      :   Implements for e-paper library
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
#include "epd7in5b.h"

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
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    SendCommand(POWER_SETTING); 
    SendData(0x37);
    SendData(0x00);

    SendCommand(PANEL_SETTING);
    SendData(0xCF);
    SendData(0x08);
    
    SendCommand(BOOSTER_SOFT_START);
    SendData(0xc7);     
    SendData(0xcc);
    SendData(0x28);

    SendCommand(POWER_ON);
    WaitUntilIdle();

    SendCommand(PLL_CONTROL);
    SendData(0x3A);        

    SendCommand(TEMPERATURE_CALIBRATION);
    SendData(0x00);

    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x77);

    SendCommand(TCON_SETTING);
    SendData(0x22);

    SendCommand(TCON_RESOLUTION);
    SendData(0x02);     //source 640
    SendData(0x80);
    SendData(0x01);     //gate 384
    SendData(0x80);

    SendCommand(VCM_DC_SETTING);
    SendData(0x1E);      //decide by LUT file

    SendCommand(0xe5);           //FLASH MODE            
    SendData(0x03);  

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
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(200);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

void Epd::DisplayFrame(const unsigned char** image_data) {
    unsigned char temp1, temp2;
    SendCommand(DATA_START_TRANSMISSION_1);
/**  
  * Size of a single array cannot be larger than 32K in AVR GCC, therefore 
  * you have to split the image data (61440 bytes in total) into 2 parts 
  */
    for (int image_data_part = 0; image_data_part < 2; image_data_part++) {
        for (long i = 0; i < 30720; i++) {   
            temp1 = pgm_read_byte(image_data[image_data_part] + i);
            for (unsigned char j = 0; j < 4; j++) {
                if ((temp1 & 0xC0) == 0xC0) {
                    temp2 = 0x03;                       // white
                } else if ((temp1 & 0xC0) == 0x00) {
                    temp2 = 0x00;                       // black
                } else {
                    temp2 = 0x04;                       // red
                }
                temp2 <<= 4;
                temp1 <<= 2;
                j++;
                if((temp1 & 0xC0) == 0xC0) {
                    temp2 |= 0x03;                      // white
                } else if ((temp1 & 0xC0) == 0x00) {
                    temp2 |= 0x00;                      // black
                } else {
                    temp2 |= 0x04;                      // red
                }
                temp1 <<= 2;
                SendData(temp2); 
            }
        }
    }
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::Clean(void) {
    SendCommand(DATA_START_TRANSMISSION_1);
    for (long i = 0; i < 122880; i++) {    
        SendData(0x33); 
    }
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::DisplayOneQuarterFrame(const unsigned char* image_data) {
    unsigned char temp1, temp2;
    SendCommand(DATA_START_TRANSMISSION_1);


	for (long i = 0; i < 192; i++) {   
			for (long k = 0; k < 80; k++) {
				temp1 = pgm_read_byte(image_data + i*80 + k);
				for (unsigned char j = 0; j < 4; j++) {
					if ((temp1 & 0xC0) == 0xC0) {
						temp2 = 0x03;                       // white
					} else if ((temp1 & 0xC0) == 0x00) {
						temp2 = 0x00;                       // black
					} else {
						temp2 = 0x04;                       // red
					}
					temp2 <<= 4;
					temp1 <<= 2;
					j++;
					if((temp1 & 0xC0) == 0xC0) {
						temp2 |= 0x03;                      // white
					} else if ((temp1 & 0xC0) == 0x00) {
						temp2 |= 0x00;                      // black
					} else {
						temp2 |= 0x04;                      // red
					}
					temp1 <<= 2;
					SendData(temp2); 
				}
			}
			for (long k = 0; k < 160; k++) {                // 1/4 show white
				SendData(0x33); 
			}
		
	}
	for (long i = 0; i < 192; i++) {                    
			for (long k = 0; k < 160; k++) {                 // 1/4 show red 
				SendData(0x44); 
			}
			for (long k = 0; k < 160; k++) {                 // 1/4 show black 
				SendData(0x00); 
			}
	}
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(POWER_OFF);
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);
    SendData(0xa5);
}

/* END OF FILE */


