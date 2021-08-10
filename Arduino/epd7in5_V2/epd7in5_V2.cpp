/**
 *  @filename   :   epd7in5.cpp
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
#include "epd7in5_V2.h"

unsigned char Voltage_Frame_7IN5_V2[]={
	0x6, 0x3F, 0x3F, 0x11, 0x24, 0x7, 0x17,
};

unsigned char LUT_VCOM_7IN5_V2[]={	
	0x0,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x0,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};						

unsigned char LUT_WW_7IN5_V2[]={	
	0x10,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x20,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

unsigned char LUT_BW_7IN5_V2[]={	
	0x10,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x20,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

unsigned char LUT_WB_7IN5_V2[]={	
	0x80,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x40,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

unsigned char LUT_BB_7IN5_V2[]={	
	0x80,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x40,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

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

    // SendCommand(0x01); 
    // SendData(0x07);
    // SendData(0x07);
    // SendData(0x3f);
    // SendData(0x3f);

    // SendCommand(0x04);
    // DelayMs(100);
    // WaitUntilIdle();
    
    // SendCommand(0X00);			//PANNEL SETTING
    // SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    // SendCommand(0x61);        	//tres
    // SendData(0x03);		//source 800
    // SendData(0x20);
    // SendData(0x01);		//gate 480
    // SendData(0xE0);

    // SendCommand(0X15);
    // SendData(0x00);

    // SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    // SendData(0x10);
    // SendData(0x07);

    // SendCommand(0X60);			//TCON SETTING
    // SendData(0x22);

	SendCommand(0x01);  // power setting
	SendData(0x17);  // 1-0=11: internal power
	SendData(*(Voltage_Frame_7IN5_V2+6));  // VGH&VGL
	SendData(*(Voltage_Frame_7IN5_V2+1));  // VSH
	SendData(*(Voltage_Frame_7IN5_V2+2));  //  VSL
	SendData(*(Voltage_Frame_7IN5_V2+3));  //  VSHR
	
	SendCommand(0x82);  // VCOM DC Setting
	SendData(*(Voltage_Frame_7IN5_V2+4));  // VCOM

	SendCommand(0x06);  // Booster Setting
	SendData(0x27);
	SendData(0x27);
	SendData(0x2F);
	SendData(0x17);
	
	SendCommand(0x30);   // OSC Setting
	SendData(*(Voltage_Frame_7IN5_V2+0));  // 2-0=100: N=4  ; 5-3=111: M=7  ;  3C=50Hz     3A=100HZ

    SendCommand(0x04); //POWER ON
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0X00);			//PANNEL SETTING
    SendData(0x3F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    SendCommand(0x61);        	//tres
    SendData(0x03);		//source 800
    SendData(0x20);
    SendData(0x01);		//gate 480
    SendData(0xE0);

    SendCommand(0X15);
    SendData(0x00);

    SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    SendData(0x10);
    SendData(0x00);

    SendCommand(0X60);			//TCON SETTING
    SendData(0x22);

    SendCommand(0x65);  // Resolution setting
    SendData(0x00);
    SendData(0x00);//800*480
    SendData(0x00);
    SendData(0x00);

    SetLut_by_host(LUT_VCOM_7IN5_V2, LUT_WW_7IN5_V2, LUT_BW_7IN5_V2, LUT_WB_7IN5_V2, LUT_BB_7IN5_V2);

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
    unsigned char busy;
    Serial.print("e-Paper Busy\r\n ");
    do{
        SendCommand(0x71);
        busy = DigitalRead(busy_pin);
    }while(busy == 0);
    Serial.print("e-Paper Busy Release\r\n ");
    DelayMs(20);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20); 
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(4);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
}

void Epd::DisplayFrame(const unsigned char* frame_buffer) {
    
    SendCommand(0x13);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(~frame_buffer[i + j * width/8]);
        }
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::Displaypart(const unsigned char* pbuffer, unsigned long xStart, unsigned long yStart,unsigned long Picture_Width,unsigned long Picture_Height) {
    SendCommand(0x13);
    // xStart = xStart/8;
    // xStart = xStart*8;
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
                SendData(~(pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]))) );
                // SendData(0xff);
            }else {
                SendData(0x00);
            }
        }
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::SetLut_by_host(unsigned char* lut_vcom,  unsigned char* lut_ww, unsigned char* lut_bw, unsigned char* lut_wb, unsigned char* lut_bb)
{
	unsigned char count;

	SendCommand(0x20); //VCOM	
	for(count=0; count<42; count++)
		SendData(lut_vcom[count]);

	SendCommand(0x21); //LUTBW
	for(count=0; count<42; count++)
		SendData(lut_ww[count]);

	SendCommand(0x22); //LUTBW
	for(count=0; count<42; count++)
		SendData(lut_bw[count]);

	SendCommand(0x23); //LUTWB
	for(count=0; count<42; count++)
		SendData(lut_wb[count]);

	SendCommand(0x24); //LUTBB
	for(count=0; count<42; count++)
		SendData(lut_bb[count]);
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
    SendData(0xA5);
}

void Epd::Clear(void) {
    
    // SendCommand(0x10);
    // for(unsigned long i=0; i<height*width; i++) {
    //     SendData(0x00);
    // }
    SendCommand(0x13);
    for(unsigned long i=0; i<height*width; i++)	{
        SendData(0x00);
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}


/* END OF FILE */


