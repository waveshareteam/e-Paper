/*****************************************************************************
* | File      	:   EPD_2in7_V2.c
* | Author      :   Waveshare team
* | Function    :   2.7inch V2 e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-04-22
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
#include "EPD_2in7_V2.h"
#include "Debug.h"

UBYTE WF_PARTIAL_2in7_V2[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xF,0x0,0x0,0x0,0x0,0x0,0x1,
0x1,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x22,0x17,0x41,0xB0,0x32,0x32,
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN7_V2_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN7_V2_SendCommand(UBYTE Reg)
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
static void EPD_2IN7_V2_SendData(UBYTE Data)
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
static void EPD_2IN7_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    do {
        if(DEV_Digital_Read(EPD_BUSY_PIN) == 0)
			break;
    } while(1);
    DEV_Delay_ms(20);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn on display
parameter:
******************************************************************************/
static void EPD_2IN7_V2_TurnOnDisplay(void)
{
	EPD_2IN7_V2_SendCommand(0x20);
    EPD_2IN7_V2_ReadBusy();
}

static void EPD_2IN7_V2_TurnOnDisplay_Partial(void)
{
    EPD_2IN7_V2_SendCommand(0x22);
    EPD_2IN7_V2_SendData(0x0f);
	EPD_2IN7_V2_SendCommand(0x20);
    EPD_2IN7_V2_ReadBusy();
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_2IN7_V2_SetLut(void)
{
    unsigned int count;
    EPD_2IN7_V2_SendCommand(0x32); //vcom
    for(count = 0; count < 153; count++) {
        EPD_2IN7_V2_SendData(WF_PARTIAL_2in7_V2[count]);
    }
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_2IN7_V2_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_2IN7_V2_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2IN7_V2_SendData((Xstart>>3) & 0xFF);
    EPD_2IN7_V2_SendData((Xend>>3) & 0xFF);
	
    EPD_2IN7_V2_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2IN7_V2_SendData(Ystart & 0xFF);
    EPD_2IN7_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_2IN7_V2_SendData(Yend & 0xFF);
    EPD_2IN7_V2_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_2IN7_V2_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_2IN7_V2_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2IN7_V2_SendData(Xstart & 0xFF);

    EPD_2IN7_V2_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2IN7_V2_SendData(Ystart & 0xFF);
    EPD_2IN7_V2_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7_V2_Init(void)
{
    EPD_2IN7_V2_Reset();
	EPD_2IN7_V2_ReadBusy();
	
	EPD_2IN7_V2_SendCommand(0x12);
	EPD_2IN7_V2_ReadBusy();

	EPD_2IN7_V2_SendCommand(0x01); //Driver output control      
	EPD_2IN7_V2_SendData(0x27);
	EPD_2IN7_V2_SendData(0x01);
	EPD_2IN7_V2_SendData(0x00);
	
	EPD_2IN7_V2_SendCommand(0x11); //data entry mode       
	EPD_2IN7_V2_SendData(0x03);
	
	EPD_2IN7_V2_SetWindows(0, 0, EPD_2IN7_V2_WIDTH-1, EPD_2IN7_V2_HEIGHT-1);
	EPD_2IN7_V2_SetCursor(0, 0);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(0XFF);
        }
    }

    EPD_2IN7_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(0X00);
        }
    }
	EPD_2IN7_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN7_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;
	
    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_2IN7_V2_TurnOnDisplay();
}

void EPD_2IN7_V2_Display_Base(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

	EPD_2IN7_V2_SendCommand(0x24);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	EPD_2IN7_V2_SendCommand(0x26);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	EPD_2IN7_V2_TurnOnDisplay();	
}

void EPD_2IN7_V2_Display_Partial(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
	
	EPD_2IN7_V2_ReadBusy();
	EPD_2IN7_V2_SetLut();
	EPD_2IN7_V2_SendCommand(0x37);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x40);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);

	EPD_2IN7_V2_SendCommand(0x3c);
	EPD_2IN7_V2_SendData(0x80);
	
	EPD_2IN7_V2_SendCommand(0x22);
	EPD_2IN7_V2_SendData(0xc0);
	EPD_2IN7_V2_SendCommand(0x20);
	EPD_2IN7_V2_ReadBusy();

    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_2IN7_V2_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7_V2_Sleep(void)
{
    EPD_2IN7_V2_SendCommand(0X10);
    EPD_2IN7_V2_SendData(0x01);
}
