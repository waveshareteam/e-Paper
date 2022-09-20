/**
 *  @filename   :   epd3in7.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2020
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
#include "epd3in7.h"
#include "imagedata.h"

static const UBYTE lut_4Gray_GC[] =
{
    0x2A,0x06,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
    0x28,0x06,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
    0x20,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
    0x14,0x06,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
    0x00,0x02,0x02,0x0A,0x00,0x00,0x00,0x08,0x08,0x02,//6
    0x00,0x02,0x02,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,//7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//8
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//9
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//10
    0x22,0x22,0x22,0x22,0x22
};  

static const UBYTE lut_1Gray_GC[] =
{
    0x2A,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
    0x05,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
    0x2A,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
    0x05,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
    0x00,0x02,0x03,0x0A,0x00,0x02,0x06,0x0A,0x05,0x00,//6
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//8
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//9
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//10
    0x22,0x22,0x22,0x22,0x22
};  

static const UBYTE lut_1Gray_DU[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
    0x01,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x0A,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
    0x00,0x00,0x05,0x05,0x00,0x05,0x03,0x05,0x05,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//7
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//9
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x22,0x22,0x22,0x22,0x22
}; 

static const UBYTE lut_1Gray_A2[] =
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //1
0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //2
0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //3
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //5
0x00,0x00,0x03,0x05,0x00,0x00,0x00,0x00,0x00,0x00, //6
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //7
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //8
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //10
0x22,0x22,0x22,0x22,0x22
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

    SendCommand(0x12);
    DelayMs(300);

    SendCommand(0x46); 
    SendData(0xF7);
    WaitUntilIdle();
    SendCommand(0x47);
    SendData(0xF7);
    WaitUntilIdle(); 

    SendCommand(0x01); // setting gaet number
    SendData(0xDF);
    SendData(0x01);
    SendData(0x00);

    SendCommand(0x03); // set gate voltage
    SendData(0x00);

    SendCommand(0x04); // set source voltage
    SendData(0x41);
    SendData(0xA8);
    SendData(0x32);

    SendCommand(0x11); // set data entry sequence
    SendData(0x03);

    SendCommand(0x3C); // set border 
    SendData(0x00);

    SendCommand(0x0C); // set booster strength
    SendData(0xAE);
    SendData(0xC7);
    SendData(0xC3);
    SendData(0xC0);
    SendData(0xC0);  

    SendCommand(0x18); // set internal sensor on
    SendData(0x80);

    SendCommand(0x2C); // set vcom value
    SendData(0x44);

    SendCommand(0x37); // set display option, these setting turn on previous function
    SendData(0x00);
    SendData(0xff);
    SendData(0xff);
    SendData(0xff);
    SendData(0xff);  
    SendData(0x4f);
    SendData(0xff);
    SendData(0xff);
    SendData(0xff);
    SendData(0xff);  

    SendCommand(0x44); // setting X direction start/end position of RAM
    SendData(0x00);
    SendData(0x00);
    SendData(0x17);
    SendData(0x01);

    SendCommand(0x45); // setting Y direction start/end position of RAM
    SendData(0x00);
    SendData(0x00);
    SendData(0xDF);
    SendData(0x01);

    SendCommand(0x22); // Display Update Control 2
    SendData(0xCF);
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
    Serial.print("e-Paper busy \r\n ");
    UBYTE busy;
    do {
        busy = DigitalRead(busy_pin);
    } while(busy);    
    DelayMs(200);
    Serial.print("e-Paper busy release \r\n ");
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
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
}

/******************************************************************************
function :  Display
******************************************************************************/
void Epd::DisplayFrame(const UBYTE *Image, bool isBase) {
    UWORD i;
    UWORD IMAGE_COUNTER = width * height / 8;

    SendCommand(0x4E);
    SendData(0x00);
    SendData(0x00);
    SendCommand(0x4F);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++) {
        SendData(pgm_read_byte(&Image[i]));
    }
    if(isBase) {
        SendCommand(0x26);
        for (i = 0; i < IMAGE_COUNTER; i++) {
            SendData(pgm_read_byte(&Image[i]));
        }
    }

    Load_LUT(1);
    SendCommand(0x20);
    WaitUntilIdle();  
}

/******************************************************************************
function :  Partial Display
note:
    1. iwidth must be a multiple of 8
    2. The update will clear other parts of the image data
******************************************************************************/
void Epd::DisplayFrame_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD iwidth, UWORD iheight) {
    UWORD i, j;
    SendCommand(0x4E);
    SendData(0x00);
    SendData(0x00);
    SendCommand(0x4F);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x24);
    for(i=0; i<EPD_WIDTH; i++) {
        for(j=0; j<EPD_HEIGHT/8; j++) {
            if(i>=Ystart && i<Ystart+iheight && j>=Xstart/8 && j<(Xstart+iwidth)/8)
                SendData(Image[(j-Xstart/8) + (i-Ystart)*iwidth/8]);
            else
                SendData(0xff);
        }
    }

    Load_LUT(0);
    SendCommand(0x20);
    WaitUntilIdle();   
}

/******************************************************************************
function :  Sends part the image buffer in RAM to e-Paper and displays
notes:
    1. Xstart must be a multiple of 8
    1. isGC: use GC mode to clear base-image
******************************************************************************/
void Epd::DisplayFrame_Part(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, bool isGC)
{
	UWORD i, Width;
	Width = (Xend-Xstart)%8 == 0 ? (Xend-Xstart)/8 : (Xend-Xstart)/8+1;
    UWORD IMAGE_COUNTER = Width * (Yend-Ystart);

	Xend -= 1;
	Yend -= 1;

	SendCommand(0x44);
	SendData(Xstart & 0xff);
	SendData((Xstart>>8) & 0x03);
	SendData(Xend & 0xff);
	SendData((Xend>>8) & 0x03);
	SendCommand(0x45);
	SendData(Ystart & 0xff);
	SendData((Ystart>>8) & 0x03);
	SendData(Yend & 0xff);
	SendData((Yend>>8) & 0x03);

    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
	
	SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++)
	{
	    SendData(Image[i]);
	}
    if(isGC)
        Load_LUT(1);
    else
        Load_LUT(0);
}

void Epd::TurnOnDisplay(void) {
    SendCommand(0x20);
	WaitUntilIdle(); 
}

/******************************************************************************
function :  set the look-up tables
parameter:
******************************************************************************/
void Epd::Load_LUT(UBYTE mode) {
    UWORD i;
    SendCommand(0x32);
    for (i = 0; i < 105; i++) {
        if(mode == 1)
            SendData(lut_1Gray_GC[i]);
        else if(mode == 0)
            SendData(lut_1Gray_A2[i]);
    }   
}

/******************************************************************************
function :  Clear Screen
parameter:
  mode: 0:just partial mode
        1:clear all
******************************************************************************/
void Epd::Clear(UBYTE mode) {
    UWORD i;
    UWORD IMAGE_COUNTER = width * height / 8;

    SendCommand(0x4E);
    SendData(0x00);
    SendData(0x00);
    SendCommand(0x4F);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x24);
    for (i = 0; i < IMAGE_COUNTER; i++)
    {
        SendData(0xff);
    }
    if(mode)
        Load_LUT(1);
    else
        Load_LUT(0); 
    SendCommand(0x20);
    WaitUntilIdle();   
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(0X10);   //deep sleep
    SendData(0x03);
}
/* END OF FILE */
