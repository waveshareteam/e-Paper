/*****************************************************************************
* | File      	:   EPD_5in65f.c
* | Author      :   Waveshare team
* | Function    :   5.65inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-07
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
#include "EPD_5in65f.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5IN65F_Reset(void)
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
static void EPD_5IN65F_SendCommand(UBYTE Reg)
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
static void EPD_5IN65F_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}


static void EPD_5IN65F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!(DEV_Digital_Read(EPD_BUSY_PIN)));
}

static void EPD_5IN65F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(DEV_Digital_Read(EPD_BUSY_PIN));
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5IN65F_Init(void)
{
	EPD_5IN65F_Reset();
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x00);
    EPD_5IN65F_SendData(0xEF);
    EPD_5IN65F_SendData(0x08);
    EPD_5IN65F_SendCommand(0x01);
    EPD_5IN65F_SendData(0x37);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendData(0x23);
    EPD_5IN65F_SendData(0x23);
    EPD_5IN65F_SendCommand(0x03);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendCommand(0x06);
    EPD_5IN65F_SendData(0xC7);
    EPD_5IN65F_SendData(0xC7);
    EPD_5IN65F_SendData(0x1D);
    EPD_5IN65F_SendCommand(0x30);
    EPD_5IN65F_SendData(0x3C);
    EPD_5IN65F_SendCommand(0x40);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendCommand(0x50);
    EPD_5IN65F_SendData(0x37);
    EPD_5IN65F_SendCommand(0x60);
    EPD_5IN65F_SendData(0x22);
    EPD_5IN65F_SendCommand(0x61);
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0xE3);
    EPD_5IN65F_SendData(0xAA);
	
	DEV_Delay_ms(100);
    EPD_5IN65F_SendCommand(0x50);
    EPD_5IN65F_SendData(0x37);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5IN65F_Clear(UBYTE color)
{
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);
    for(int i=0; i<EPD_5IN65F_WIDTH/2; i++) {
        for(int j=0; j<EPD_5IN65F_HEIGHT; j++)
            EPD_5IN65F_SendData((color<<4)|color);
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
    DEV_Delay_ms(500);
}

/******************************************************************************
function :	show 7 kind of color block
parameter:
******************************************************************************/
void EPD_5IN65F_Show7Block(void)
{
    unsigned long i,j,k;
    unsigned char const Color_seven[8] =
	{EPD_5IN65F_BLACK,EPD_5IN65F_BLUE,EPD_5IN65F_GREEN,EPD_5IN65F_ORANGE,
	EPD_5IN65F_RED,EPD_5IN65F_YELLOW,EPD_5IN65F_WHITE,EPD_5IN65F_WHITE};
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);

    for(i=0; i<224; i++) {
        for(k = 0 ; k < 4; k ++) {
            for(j = 0 ; j < 75; j ++) {
                EPD_5IN65F_SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }
    for(i=0; i<224; i++) {
        for(k = 4 ; k < 8; k ++) {
            for(j = 0 ; j < 75; j ++) {
                EPD_5IN65F_SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DEV_Delay_ms(200);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN65F_Display(const UBYTE *image)
{
    unsigned long i,j;
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);
    for(i=0; i<EPD_5IN65F_HEIGHT; i++) {
        for(j=0; j<EPD_5IN65F_WIDTH/2; j++)
            EPD_5IN65F_SendData(image[j+((EPD_5IN65F_WIDTH/2)*i)]);
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DEV_Delay_ms(200);
	
}

/******************************************************************************
function :	Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN65F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart, 
								UWORD image_width, UWORD image_heigh)
{
    unsigned long i,j;
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);
    for(i=0; i<EPD_5IN65F_HEIGHT; i++) {
        for(j=0; j< EPD_5IN65F_WIDTH/2; j++) {
						if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
							EPD_5IN65F_SendData(image[(j-xstart/2) + (image_width/2*(i-ystart))]);
						}
						else {
							EPD_5IN65F_SendData(0x11);
						}
				}
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DEV_Delay_ms(200);
	
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5IN65F_Sleep(void)
{
    DEV_Delay_ms(100);
    EPD_5IN65F_SendCommand(0x07);
    EPD_5IN65F_SendData(0xA5);
    DEV_Delay_ms(100);
	DEV_Digital_Write(EPD_RST_PIN, 0); // Reset
}

