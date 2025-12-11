/*****************************************************************************
* | File        :   EPD_3in6e.c
* | Author      :   Waveshare team
* | Function    :   3.6inch e-Paper (E) Driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2025-12-04
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
#include "EPD_3in6e.h"
#include "Debug.h"
#include <time.h> 

/******************************************************************************
function :  Software reset
parameter:
******************************************************************************/
static void EPD_3IN6E_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :  send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_3IN6E_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_3IN6E_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :  Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_3IN6E_ReadBusyH(void)
{
    Debug("e-Paper busy H\r\n");
    while(!DEV_Digital_Read(EPD_BUSY_PIN)) {      //LOW: busy, HIGH: idle
        DEV_Delay_ms(10);
    }
    DEV_Delay_ms(100);
    Debug("e-Paper busy H release\r\n");
}

/******************************************************************************
function :  Turn On Display
parameter:
******************************************************************************/
static void EPD_3IN6E_TurnOnDisplay(void)
{
    
    EPD_3IN6E_SendCommand(0x04); // POWER_ON
    EPD_3IN6E_ReadBusyH();
    DEV_Delay_ms(200);

    //Second setting 
    EPD_3IN6E_SendCommand(0x06);
    EPD_3IN6E_SendData(0x6F);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x16);
    EPD_3IN6E_SendData(0x29);
    DEV_Delay_ms(200);

    EPD_3IN6E_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_3IN6E_SendData(0x00);
    EPD_3IN6E_ReadBusyH();

    EPD_3IN6E_SendCommand(0x02); // POWER_OFF
    EPD_3IN6E_SendData(0X00);
    EPD_3IN6E_ReadBusyH();

}

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN6E_Init(void)
{
    EPD_3IN6E_Reset();
    EPD_3IN6E_ReadBusyH();
    DEV_Delay_ms(30);


    //20211212
    EPD_3IN6E_SendCommand(0xAA);
    EPD_3IN6E_SendData(0x49);
    EPD_3IN6E_SendData(0x55);
    EPD_3IN6E_SendData(0x20);
    EPD_3IN6E_SendData(0x08);
    EPD_3IN6E_SendData(0x09);
    EPD_3IN6E_SendData(0x18);

    EPD_3IN6E_SendCommand(0x01);
    EPD_3IN6E_SendData(0x3F);
    
    EPD_3IN6E_SendCommand(0x00);
    EPD_3IN6E_SendData(0x5F);
    EPD_3IN6E_SendData(0x69);
    
    
    EPD_3IN6E_SendCommand(0x05);
    EPD_3IN6E_SendData(0x40);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x2C);
    
    EPD_3IN6E_SendCommand(0x08);
    EPD_3IN6E_SendData(0x6F);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x22);
    
    //===================
    //20211212
    //First setting
    EPD_3IN6E_SendCommand(0x06);
    EPD_3IN6E_SendData(0x6F);
    EPD_3IN6E_SendData(0x1F);
    EPD_3IN6E_SendData(0x17);
    EPD_3IN6E_SendData(0x17);
    //===================
    
    EPD_3IN6E_SendCommand(0x03);
    EPD_3IN6E_SendData(0x00);
    EPD_3IN6E_SendData(0x54);
    EPD_3IN6E_SendData(0x00);
    EPD_3IN6E_SendData(0x44);
    
    EPD_3IN6E_SendCommand(0x60);
    EPD_3IN6E_SendData(0x02);
    EPD_3IN6E_SendData(0x00);
    //Please notice that PLL must be set for version 2 IC
    EPD_3IN6E_SendCommand(0x30);
    EPD_3IN6E_SendData(0x08);

    EPD_3IN6E_SendCommand(0x50);
    EPD_3IN6E_SendData(0x3F);
    
    EPD_3IN6E_SendCommand(0x61);
    EPD_3IN6E_SendData(0x01);
    EPD_3IN6E_SendData(0x90);
    EPD_3IN6E_SendData(0x02); 
    EPD_3IN6E_SendData(0x58); 
    
    EPD_3IN6E_SendCommand(0xE3);
    EPD_3IN6E_SendData(0x2F);
    
    EPD_3IN6E_SendCommand(0x84);
    EPD_3IN6E_SendData(0x01);
    EPD_3IN6E_ReadBusyH();
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void EPD_3IN6E_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1);
    Height = EPD_3IN6E_HEIGHT;

    EPD_3IN6E_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN6E_SendData((color<<4)|color);
        }
    }

    EPD_3IN6E_TurnOnDisplay();
}

/******************************************************************************
function :  show 7 kind of color block
parameter:
******************************************************************************/
void EPD_3IN6E_Show7Block(void)
{
    unsigned long j, k;
    unsigned char const Color_seven[6] = 
    {EPD_3IN6E_BLACK, EPD_3IN6E_YELLOW, EPD_3IN6E_RED, EPD_3IN6E_BLUE, EPD_3IN6E_GREEN, EPD_3IN6E_WHITE};

    EPD_3IN6E_SendCommand(0x10);
    for(k = 0 ; k < 6; k ++) {
        for(j = 0 ; j < 20000; j ++) {
            EPD_3IN6E_SendData((Color_seven[k]<<4) |Color_seven[k]);
        }
    }
    EPD_3IN6E_TurnOnDisplay();
}

void EPD_3IN6E_Show(void)
{
    unsigned long k,o;
    unsigned char const Color_seven[6] = 
    {EPD_3IN6E_BLACK, EPD_3IN6E_YELLOW, EPD_3IN6E_RED, EPD_3IN6E_BLUE, EPD_3IN6E_GREEN, EPD_3IN6E_WHITE};

    UWORD Width, Height;
    Width = (EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1);
    Height = EPD_3IN6E_HEIGHT;
    k = 0;
    o = 0;

    EPD_3IN6E_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        if((j > 10) && (j<50))
        for (UWORD i = 0; i < Width; i++) {
                EPD_3IN6E_SendData((Color_seven[0]<<4) |Color_seven[0]);
            }
        else if(o < Height/2)
        for (UWORD i = 0; i < Width; i++) {
                EPD_3IN6E_SendData((Color_seven[0]<<4) |Color_seven[0]);
            }
        
        else
        {
            for (UWORD i = 0; i < Width; i++) {
                EPD_3IN6E_SendData((Color_seven[k]<<4) |Color_seven[k]);
                
            }
            k++ ;
            if(k >= 6)
                k = 0;
        }
            
        o++ ;
        if(o >= Height)
            o = 0;
    }
    EPD_3IN6E_TurnOnDisplay();
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN6E_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1);
    Height = EPD_3IN6E_HEIGHT;

    EPD_3IN6E_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN6E_SendData(Image[i + j * Width]);
        }
    }
    EPD_3IN6E_TurnOnDisplay();
}

void EPD_3IN6E_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh)
{
    unsigned long i, j;
	UWORD Width, Height;
	Width = (EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1);
	Height = EPD_3IN6E_HEIGHT;
	
	EPD_3IN6E_SendCommand(0x10);
	for(i=0; i<Height; i++) {
		for(j=0; j<Width; j++) {
			if((i<(image_heigh+ystart)) && (i>=ystart) && (j<((image_width+xstart)/2)) && (j>=(xstart/2))) {
				EPD_3IN6E_SendData(Image[(j-xstart/2) + (image_width/2*(i-ystart))]);
			}
			else {
				EPD_3IN6E_SendData(0x11);
			}
		}
	}
	EPD_3IN6E_TurnOnDisplay();
}

/******************************************************************************
function :  Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN6E_Sleep(void)
{
    EPD_3IN6E_SendCommand(0x07); // DEEP_SLEEP
    EPD_3IN6E_SendData(0XA5);
    // EPD_3IN6E_ReadBusyH();
}

