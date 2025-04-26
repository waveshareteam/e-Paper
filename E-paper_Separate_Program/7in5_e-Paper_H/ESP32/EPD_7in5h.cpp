/*****************************************************************************
* | File      	:   EPD_7in5h.c
* | Author      :   Waveshare team
* | Function    :   7.5inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
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
#include "EPD_7in5h.h"
#include "Debug.h"
#include "time.h"


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN5H_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_7IN5H_SendCommand(UBYTE Reg)
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
static void EPD_7IN5H_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_7IN5H_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    DEV_Delay_ms(100);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_7IN5H_ReadBusyH(void)
{
    Debug("e-Paper busy H\r\n");
    while(!DEV_Digital_Read(EPD_BUSY_PIN)) {      //LOW: busy, HIGH: idle
        DEV_Delay_ms(5);
    }
    Debug("e-Paper busy H release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5H_TurnOnDisplay(void)
{
    // EPD_7IN5H_SendCommand(0x04);
    // EPD_7IN5H_ReadBusyH();

    EPD_7IN5H_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_7IN5H_SendData(0x00);
    EPD_7IN5H_ReadBusyH();

    // EPD_7IN5H_SendCommand(0x20);
    // EPD_7IN5H_SendData(0x00);
    // EPD_7IN5H_ReadBusyH();

}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN5H_Init(void)
{
    EPD_7IN5H_Reset();

	EPD_7IN5H_SendCommand(0x00);
	EPD_7IN5H_SendData(0x0F);	
	EPD_7IN5H_SendData(0x29);	

	EPD_7IN5H_SendCommand(0x06);
	EPD_7IN5H_SendData(0x0F);	
	EPD_7IN5H_SendData(0x8B);	
	EPD_7IN5H_SendData(0x93);	
	EPD_7IN5H_SendData(0xA1);  // C1

	EPD_7IN5H_SendCommand(0x41);
	EPD_7IN5H_SendData(0x00);	

	EPD_7IN5H_SendCommand(0x50);
	EPD_7IN5H_SendData(0x37);	

	EPD_7IN5H_SendCommand(0x60);
	EPD_7IN5H_SendData(0x02);	
	EPD_7IN5H_SendData(0x02);	

	EPD_7IN5H_SendCommand(0x61);//0x61	
	EPD_7IN5H_SendData(EPD_7IN5H_WIDTH/256);	
	EPD_7IN5H_SendData(EPD_7IN5H_WIDTH%256);	
	EPD_7IN5H_SendData(EPD_7IN5H_HEIGHT/256);	
	EPD_7IN5H_SendData(EPD_7IN5H_HEIGHT%256);	

	EPD_7IN5H_SendCommand(0x62);
	EPD_7IN5H_SendData(0x98); 
	EPD_7IN5H_SendData(0x98);
	EPD_7IN5H_SendData(0x98); 
	EPD_7IN5H_SendData(0x75);
	EPD_7IN5H_SendData(0xCA); 
	EPD_7IN5H_SendData(0xB2);	
	EPD_7IN5H_SendData(0x98); 
	EPD_7IN5H_SendData(0x7E); 

	EPD_7IN5H_SendCommand(0x65);
	EPD_7IN5H_SendData(0x00);	
	EPD_7IN5H_SendData(0x00);	
	EPD_7IN5H_SendData(0x00);	
	EPD_7IN5H_SendData(0x00);	
	
	EPD_7IN5H_SendCommand(0xE7);
	EPD_7IN5H_SendData(0x1C);	

	EPD_7IN5H_SendCommand(0xE3);
	EPD_7IN5H_SendData(0x00);	

	EPD_7IN5H_SendCommand(0xE9);	
	EPD_7IN5H_SendData(0x01);

    EPD_7IN5H_SendCommand(0x30);
    EPD_7IN5H_SendData(0x08); 		

    EPD_7IN5H_SendCommand(0x04);
    EPD_7IN5H_ReadBusyH();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5H_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_7IN5H_WIDTH % 4 == 0)? (EPD_7IN5H_WIDTH / 4 ): (EPD_7IN5H_WIDTH / 4 + 1);
    Height = EPD_7IN5H_HEIGHT;

    EPD_7IN5H_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_7IN5H_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_7IN5H_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5H_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_7IN5H_WIDTH % 4 == 0)? (EPD_7IN5H_WIDTH / 4 ): (EPD_7IN5H_WIDTH / 4 + 1);
    Height = EPD_7IN5H_HEIGHT;

    EPD_7IN5H_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_7IN5H_SendData(Image[i + j * Width]);
        }
    }

    EPD_7IN5H_TurnOnDisplay();
}

void EPD_7IN5H_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh)
{
    unsigned long i, j;
    UWORD Width, Height;
    Width = (EPD_7IN5H_WIDTH % 4 == 0)? (EPD_7IN5H_WIDTH / 4 ): (EPD_7IN5H_WIDTH / 4 + 1);
    Height = EPD_7IN5H_HEIGHT;

    EPD_7IN5H_SendCommand(0x10);
	for(i=0; i<Height; i++) {
		for(j=0; j<Width; j++) {
			if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/4 && j>=xstart/4) {
				EPD_7IN5H_SendData(Image[(j-xstart/4) + (image_width/4*(i-ystart))]);
			}
			else {
				EPD_7IN5H_SendData(0x55);
			}
		}
	}
    EPD_7IN5H_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5H_Sleep(void)
{
    EPD_7IN5H_SendCommand(0x02); // POWER_OFF
    EPD_7IN5H_SendData(0X00);
    EPD_7IN5H_ReadBusyH();
    EPD_7IN5H_SendCommand(0x07); // DEEP_SLEEP
    EPD_7IN5H_SendData(0XA5);
}
