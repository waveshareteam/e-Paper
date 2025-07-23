/*****************************************************************************
* | File      	:   EPD_4IN01F.c
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
#include "EPD_4in01f.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN01F_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_4IN01F_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_4IN01F_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}


static void EPD_4IN01F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!(DEV_Digital_Read(EPD_BUSY_PIN)));
}

static void EPD_4IN01F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(DEV_Digital_Read(EPD_BUSY_PIN));
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN01F_Init(void)
{
	EPD_4IN01F_Reset();
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x00);
    EPD_4IN01F_SendData(0x2f);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x01);
    EPD_4IN01F_SendData(0x37);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendData(0x05);
    EPD_4IN01F_SendData(0x05);
    EPD_4IN01F_SendCommand(0x03);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x06);
    EPD_4IN01F_SendData(0xC7);
    EPD_4IN01F_SendData(0xC7);
    EPD_4IN01F_SendData(0x1D);
    EPD_4IN01F_SendCommand(0x41);
    EPD_4IN01F_SendData(0x00);
    EPD_4IN01F_SendCommand(0x50);
    EPD_4IN01F_SendData(0x37);
    EPD_4IN01F_SendCommand(0x60);
    EPD_4IN01F_SendData(0x22);
    EPD_4IN01F_SendCommand(0x61);
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0xE3);
    EPD_4IN01F_SendData(0xAA);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN01F_Clear(UBYTE color)
{
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(int i=0; i<EPD_4IN01F_WIDTH/2; i++) {
        for(int j=0; j<EPD_4IN01F_HEIGHT; j++)
            EPD_4IN01F_SendData((color<<4)|color);
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
    DEV_Delay_ms(500);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN01F_Display(const UBYTE *image)
{
    unsigned long i,j;
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(i=0; i<EPD_4IN01F_HEIGHT; i++) {
        for(j=0; j<EPD_4IN01F_WIDTH/2; j++)
            EPD_4IN01F_SendData(image[j+((EPD_4IN01F_WIDTH/2)*i)]);
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	DEV_Delay_ms(200);
	
}

/******************************************************************************
function :	Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN01F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart, 
								UWORD image_width, UWORD image_heigh)
{
    unsigned long i,j;
    EPD_4IN01F_SendCommand(0x61);//Set Resolution setting
    EPD_4IN01F_SendData(0x02);
    EPD_4IN01F_SendData(0x80);
    EPD_4IN01F_SendData(0x01);
    EPD_4IN01F_SendData(0x90);
    EPD_4IN01F_SendCommand(0x10);
    for(i=0; i<EPD_4IN01F_HEIGHT; i++) {
        for(j=0; j< EPD_4IN01F_WIDTH/2; j++) {
			if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
				EPD_4IN01F_SendData(image[(j-xstart/2) + (image_width/2*(i-ystart))]);
			}
			else {
				EPD_4IN01F_SendData(0x11);
			}
		}
    }
    EPD_4IN01F_SendCommand(0x04);//0x04
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x12);//0x12
    EPD_4IN01F_BusyHigh();
    EPD_4IN01F_SendCommand(0x02);  //0x02
    EPD_4IN01F_BusyLow();
	DEV_Delay_ms(200);
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN01F_Sleep(void)
{
    DEV_Delay_ms(100);
    EPD_4IN01F_SendCommand(0x07);
    EPD_4IN01F_SendData(0xA5);
}

