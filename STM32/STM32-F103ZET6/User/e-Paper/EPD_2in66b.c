/*****************************************************************************
* | File      	:  	EPD_2in66b.c
* | Author      :   Waveshare team
* | Function    :   2.66inch e-paper b
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-02
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
#include "EPD_2in66b.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN66B_Reset(void)
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
static void EPD_2IN66B_SendCommand(UBYTE Reg)
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
static void EPD_2IN66B_SendData(UBYTE Data)
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
static void EPD_2IN66B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    DEV_Delay_ms(50);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(10);
    }
    DEV_Delay_ms(50);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN66B_TurnOnDisplay(void)
{
    EPD_2IN66B_SendCommand(0x20);
    EPD_2IN66B_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_2IN66B_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_2IN66B_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2IN66B_SendData((Xstart>>3) & 0x1F);
    EPD_2IN66B_SendData((Xend>>3) & 0x1F);
	
    EPD_2IN66B_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2IN66B_SendData(Ystart & 0xFF);
    EPD_2IN66B_SendData((Ystart >> 8) & 0x01);
    EPD_2IN66B_SendData(Yend & 0xFF);
    EPD_2IN66B_SendData((Yend >> 8) & 0x01);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_2IN66B_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_2IN66B_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2IN66B_SendData(Xstart & 0x1F);

    EPD_2IN66B_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2IN66B_SendData(Ystart & 0xFF);
    EPD_2IN66B_SendData((Ystart >> 8) & 0x01);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN66B_Init(void)
{
    EPD_2IN66B_Reset();
    EPD_2IN66B_ReadBusy();
    EPD_2IN66B_SendCommand(0x12);//soft  reset
    EPD_2IN66B_ReadBusy();

	EPD_2IN66B_SendCommand(0x11); //data entry mode       
	EPD_2IN66B_SendData(0x03);

	EPD_2IN66B_SetWindows(0, 0, EPD_2IN66B_WIDTH-1, EPD_2IN66B_HEIGHT-1);
	
	EPD_2IN66B_SendCommand(0x21); //  Display update control
	EPD_2IN66B_SendData(0x00);
	EPD_2IN66B_SendData(0x80);	

	EPD_2IN66B_SetCursor(0, 0);
	EPD_2IN66B_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN66B_Display(UBYTE *ImageBlack, UBYTE*ImageRed)
{
    UWORD Width, Height;
    Width = (EPD_2IN66B_WIDTH % 8 == 0)? (EPD_2IN66B_WIDTH / 8 ): (EPD_2IN66B_WIDTH / 8 + 1);
    Height = EPD_2IN66B_HEIGHT;

    EPD_2IN66B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66B_SendData(ImageBlack[i + j * Width]);
        }
    }
	
    EPD_2IN66B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66B_SendData(~ImageRed[i + j * Width]);
        }
    }

    EPD_2IN66B_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN66B_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN66B_WIDTH % 8 == 0)? (EPD_2IN66B_WIDTH / 8 ): (EPD_2IN66B_WIDTH / 8 + 1);
    Height = EPD_2IN66B_HEIGHT;

    EPD_2IN66B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66B_SendData(0xff);
        }
    }
	EPD_2IN66B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66B_SendData(0x00);
        }
    }
    EPD_2IN66B_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN66B_Sleep(void)
{
    EPD_2IN66B_SendCommand(0x10);
    EPD_2IN66B_SendData(0x01); 
}
