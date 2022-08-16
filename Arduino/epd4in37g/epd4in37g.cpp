/**
 *  @filename   :   epd4in37g.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     2022/08/16
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
#include "epd4in37g.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    WIDTH = EPD_WIDTH;
    HEIGHT = EPD_HEIGHT;
};

int Epd::Init() {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    SendCommand(0xAA);
    SendData(0x49);
    SendData(0x55);
    SendData(0x20);
	SendData(0x08);
	SendData(0x09);
    SendData(0x18);

    SendCommand(0x01);
    SendData(0x3F);

    SendCommand(0x00);
    SendData(0x4F);
    SendData(0x69);

    SendCommand(0x05);
    SendData(0x40);
    SendData(0x1F);
    SendData(0x1F);
    SendData(0x2C);

    SendCommand(0x08);
    SendData(0x6F);
    SendData(0x1F);
    SendData(0x1F);
    SendData(0x22);

	//===================
	//20211212
	//First setting
    SendCommand(0x06);
    SendData(0x6F);
    SendData(0x1F);
    SendData(0x17);
    SendData(0x17);
	//===================
	
    SendCommand(0x03);
    SendData(0x00);
    SendData(0x54);
    SendData(0x00);
    SendData(0x44); 

    SendCommand(0x50);
    SendData(0x3F);

    SendCommand(0x60);
    SendData(0x02);
    SendData(0x00);

	//Please notice that PLL must be set for version 2 IC
    SendCommand(0x30);
    SendData(0x08);
	
    SendCommand(0x61);
    SendData(0x02);
    SendData(0x00);
    SendData(0x01); 
    SendData(0x70); 

    SendCommand(0xE3);
    SendData(0x2F);

    SendCommand(0x84);
    SendData(0x01);
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
 *  @brief: Wait until the busy_pin goes LOW
 */
void Epd::ReadBusyH(void) {
    Serial.print("e-Paper busy H\r\n ");
    while(DigitalRead(busy_pin) == LOW) {      //LOW: busy, HIGH: idle
        DelayMs(5);
    } 
    Serial.print("e-Paper busy release H\r\n ");    
}

void Epd::ReadBusyL(void) {
    Serial.print("e-Paper busy L\r\n ");
    while(DigitalRead(busy_pin) == HIGH) {      //LOW: idle, HIGH: busy
        DelayMs(5);
    }      
    Serial.print("e-Paper busy release L\r\n ");
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
function :	Turn On Display
parameter:
******************************************************************************/
void Epd::TurnOnDisplay(void)
{
    SendCommand(0x12); // DISPLAY_REFRESH
    SendData(0x00);
    ReadBusyH();

    SendCommand(0x02); // POWER_OFF
    SendData(0X00);
    ReadBusyH();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (WIDTH % 4 == 0)? (WIDTH / 4 ): (WIDTH / 4 + 1);
    Height = HEIGHT;
    
    SendCommand(0x04);
    ReadBusyH();

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            SendData((color<<6) | (color<<4) | (color<<2) | color);
        }
    }
    TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (WIDTH % 4 == 0)? (WIDTH / 4 ): (WIDTH / 4 + 1);
    Height = HEIGHT;
    
    SendCommand(0x04);
    ReadBusyH();

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            SendData(pgm_read_byte(&Image[i + j * Width]));
        }
    }
    TurnOnDisplay();
}

void Epd::Display_part(UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_height)
{
    UWORD Width, Height, i, j;
    Width = (WIDTH % 4 == 0)? (WIDTH / 4 ): (WIDTH / 4 + 1);
    Height = HEIGHT;
    
    SendCommand(0x04);
    ReadBusyH();

    SendCommand(0x10);
    for(i=0; i<Height; i++) {
        for(j=0; j< Width; j++) {
            if(i<image_height+ystart && i>=ystart && j<(image_width+xstart)/4 && j>=xstart/4) {
              SendData(pgm_read_byte(&Image[(j-xstart/4) + (image_width/4*(i-ystart))]));
            }
			else {
				SendData(0x55);
			}
		}
    }
    TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep(void)
{
    SendCommand(0x02); // POWER_OFF
    SendData(0X00);
    SendCommand(0x07); // DEEP_SLEEP
    SendData(0XA5);
}

/* END OF FILE */


