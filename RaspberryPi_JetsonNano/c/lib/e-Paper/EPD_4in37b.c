/*****************************************************************************
* | File      	:  	EPD_4in37b.c
* | Author      :   Waveshare team
* | Function    :   4.37inch e-paper b
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-01-07
* | Info        :
* -----------------------------------------------------------------------------
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
#include "EPD_4in37b.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN37B_Reset(void)
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
static void EPD_4IN37B_SendCommand(UBYTE Reg)
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
static void EPD_4IN37B_SendData(UBYTE Data)
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
static void EPD_4IN37B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    DEV_Delay_ms(50);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 0) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(10);
    }
    DEV_Delay_ms(50);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_4IN37B_TurnOnDisplay(void)
{
    EPD_4IN37B_SendCommand(0x12);
    EPD_4IN37B_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_4IN37B_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_4IN37B_SendCommand(0x90); 
    EPD_4IN37B_SendData(Xstart & 0xf8);
    EPD_4IN37B_SendData(Xend | 0x07);
    EPD_4IN37B_SendData((Ystart >> 8) & 0x01);
    EPD_4IN37B_SendData(Ystart & 0xFF);
    EPD_4IN37B_SendData((Yend >> 8) & 0x01);
    EPD_4IN37B_SendData(Yend & 0xFF);
	EPD_4IN37B_SendData(0x01);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN37B_Init(void)
{
    EPD_4IN37B_Reset();
	
    EPD_4IN37B_SendCommand(0x04);//soft  reset
    EPD_4IN37B_ReadBusy();

    EPD_4IN37B_SendCommand(0X00);			//PANNEL SETTING
    EPD_4IN37B_SendData(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN37B_Display(UBYTE *ImageBlack, UBYTE*ImageRed)
{
    UWORD Width, Height;
    Width = (EPD_4IN37B_WIDTH % 8 == 0)? (EPD_4IN37B_WIDTH / 8 ): (EPD_4IN37B_WIDTH / 8 + 1);
    Height = EPD_4IN37B_HEIGHT;

    EPD_4IN37B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(ImageBlack[i + j * Width]);
        }
    }
    EPD_4IN37B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(~ImageRed[i + j * Width]);
        }
    }

    EPD_4IN37B_TurnOnDisplay();
}

void EPD_4IN37B_Display_Part(UBYTE *ImageBlack, UBYTE *ImageRed, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{

    UWORD Width, Height;
	Width = Xend - Xstart;
    Height = Yend - Ystart;
    Width = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);

	if(Xstart % 8 != 0) {
		Xstart += 8;
		if(Xstart > Xend) {
			printf("Xstart must be a multiple of 8 and not more than Xend \r\n");
			return;
		}
	}	
	EPD_4IN37B_SetWindows(Xstart, Ystart, Xend-1, Yend-1);
	
	EPD_4IN37B_SendCommand(0x91);
    EPD_4IN37B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(ImageBlack[i + j * Width]);
        }
    }
	
    EPD_4IN37B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(~ImageRed[i + j * Width]);
        }
    }
	EPD_4IN37B_SendCommand(0x92);
	
    EPD_4IN37B_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN37B_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_4IN37B_WIDTH % 8 == 0)? (EPD_4IN37B_WIDTH / 8 ): (EPD_4IN37B_WIDTH / 8 + 1);
    Height = EPD_4IN37B_HEIGHT;

    EPD_4IN37B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(0xff);
        }
    }
	EPD_4IN37B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN37B_SendData(0x00);
        }
    }
    EPD_4IN37B_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN37B_Sleep(void)
{
	EPD_4IN37B_SendCommand(0x02);
	DEV_Delay_ms(100);
    EPD_4IN37B_SendCommand(0x07);
    EPD_4IN37B_SendData(0xa5); 
}
