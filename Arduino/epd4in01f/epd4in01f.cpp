/*****************************************************************************
* | File      	:   EPD_4in01f.c
* | Author      :   Waveshare team
* | Function    :   4.01inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-25
* | Info        :
* -----------------------------------------------------------------------------
#
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
******************************************************************************/

#include <stdlib.h>
#include "epd4in01f.h"
#include "imagedata.h"

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

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
int Epd::Init(void) {
	if (IfInit() != 0) {
	return -1;
	}
	Reset();
	EPD_4IN01F_BusyHigh();
	SendCommand(0x00);
	SendData(0x2f);
	SendData(0x00);
	SendCommand(0x01);
	SendData(0x37);
	SendData(0x00);
	SendData(0x05);
	SendData(0x05);
	SendCommand(0x03);
	SendData(0x00);
	SendCommand(0x06);
	SendData(0xC7);
	SendData(0xC7);
	SendData(0x1D);
	SendCommand(0x41);
	SendData(0x00);
	SendCommand(0x50);
	SendData(0x37);
	SendCommand(0x60);
	SendData(0x22);
	SendCommand(0x61);
	SendData(0x02);
	SendData(0x80);
	SendData(0x01);
	SendData(0x90);
	SendCommand(0xE3);
	SendData(0xAA);
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

void Epd::EPD_4IN01F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!(DigitalRead(BUSY_PIN)));
}

void Epd::EPD_4IN01F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(DigitalRead(BUSY_PIN));
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
    DelayMs(1);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::EPD_4IN01F_Display(const UBYTE *image) {
    unsigned long i,j;
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x80);
    SendData(0x01);
    SendData(0x90);
    SendCommand(0x10);
    for(i=0; i<height; i++) {
        for(j=0; j<width/2; j++) {
			SendData(image[j+((width/2)*i)]);
		}
    }
    SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	DelayMs(200);
}

/******************************************************************************
function :  Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::EPD_4IN01F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart, 
                                        UWORD image_width, UWORD image_heigh)
{
    unsigned long i,j;
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x80);
    SendData(0x01);
    SendData(0x90);
    SendCommand(0x10);
    for(i=0; i<height; i++) {
        for(j=0; j< width/2; j++) {
            if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
              SendData(pgm_read_byte(&image[(j-xstart/2) + (image_width/2*(i-ystart))]));
            }
			else {
				SendData(0x11);
			}
		}
    }
    SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	DelayMs(200);
}

/******************************************************************************
function : 
      Clear screen
******************************************************************************/
void Epd::Clear(UBYTE color) {
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x80);
    SendData(0x01);
    SendData(0x90);
    SendCommand(0x10);
    for(int i=0; i<width/2; i++) {
        for(int j=0; j<height; j++) {
			SendData((color<<4)|color);
		}
    }
    SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
    DelayMs(500);
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    DelayMs(100);
    SendCommand(0x07);
    SendData(0xA5);
    DelayMs(100);
	DigitalWrite(RST_PIN, 0); // Reset
}



/* END OF FILE */
