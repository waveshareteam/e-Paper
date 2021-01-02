/**
 *  @filename   :   epd5in83_V2.h
 *  @brief      :   Header file for e-paper library epd5in83_V2.cpp
 *  @author     :   MyMX from Waveshare
 *  
 *  Copyright (C) Waveshare     Nov 09 2020
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
#include "epd5in83_V2.h"

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

	SendCommand(0x01);			//POWER SETTING
	SendData (0x07);
	SendData (0x07);    //VGH=20V,VGL=-20V
	SendData (0x3f);		//VDH=15V
	SendData (0x3f);		//VDL=-15V

	SendCommand(0x04); //POWER ON
	DelayMs(100);  
	WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

	SendCommand(0X00);			//PANNEL SETTING
	SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	SendCommand(0x61);        	//tres			
	SendData (0x02);		//source 648
	SendData (0x88);
	SendData (0x01);		//gate 480
	SendData (0xE0);

	SendCommand(0X15);		
	SendData(0x00);		

	SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	SendData(0x10);
	SendData(0x07);

	SendCommand(0X60);			//TCON SETTING
	SendData(0x22);

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
	do {
		SendCommand(0x71);
		DelayMs(50);    
	}
	while(!DigitalRead(busy_pin));   
	DelayMs(50);    
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(5);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

void Epd::DisplayFrame(const unsigned char* frame_buffer, char part) {
    /**  
    * Size of a single array cannot be larger than 32K in AVR GCC, therefore 
    * you have to split the image data () into 2 parts 
    */
    unsigned int Width, Height, i, j;
    Width = (width % 8 == 0)? (width / 8 ): (width / 8 + 1);
    Height = height;

	if(part == 1) {
		SendCommand(0x10);
		for(i=0;i<Height;i++) {
			for(j=0; j<Width; j++) {
				SendData(0x00);  
			}
		}
		SendCommand(0x13);
	}
	for(i=0;i<Height/2;i++) {
		for(j=0; j<Width; j++) {
			SendData(~pgm_read_byte(&frame_buffer[i*Width + j]));  
		}
	}
	if(part == 2)
		TurnOnDisplay();
}

void Epd::Clear(void)
{
    unsigned int Width, Height, i;
    Width = (width % 8 == 0)? (width / 8 ): (width / 8 + 1);
    Height = height;

	SendCommand(0x10);
	for(i=0; i<Width*Height; i++) {
		SendData(0x00);  
	}
	SendCommand(0x13);
	for(i=0; i<Width*Height; i++) {
		SendData(0x00);  
	}
	TurnOnDisplay();
}

void Epd::TurnOnDisplay(void)
{
	SendCommand(0x12);
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
    SendCommand(0X02);
    WaitUntilIdle();
    SendCommand(0X07);
    SendData(0xa5);
}

/* END OF FILE */


