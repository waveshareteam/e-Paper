/*****************************************************************************
* | File      	:  	EPD_2IN13b_V4.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper B V4
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-04-25
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
#include "EPD_2in13b_V4.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13B_V4_Reset(void)
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
static void EPD_2IN13B_V4_SendCommand(UBYTE Reg)
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
static void EPD_2IN13B_V4_SendData(UBYTE Data)
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
void EPD_2IN13B_V4_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	while(1)
	{	 //=1 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==0) 
			break;
		DEV_Delay_ms(20);
	}
	DEV_Delay_ms(20);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN13B_V4_TurnOnDisplay(void)
{
	EPD_2IN13B_V4_SendCommand(0x20); // Activate Display Update Sequence
	EPD_2IN13B_V4_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_2IN13B_V4_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_2IN13B_V4_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2IN13B_V4_SendData((Xstart>>3) & 0xFF);
    EPD_2IN13B_V4_SendData((Xend>>3) & 0xFF);
	
    EPD_2IN13B_V4_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2IN13B_V4_SendData(Ystart & 0xFF);
    EPD_2IN13B_V4_SendData((Ystart >> 8) & 0xFF);
    EPD_2IN13B_V4_SendData(Yend & 0xFF);
    EPD_2IN13B_V4_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_2IN13B_V4_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_2IN13B_V4_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2IN13B_V4_SendData(Xstart & 0xFF);

    EPD_2IN13B_V4_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2IN13B_V4_SendData(Ystart & 0xFF);
    EPD_2IN13B_V4_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13B_V4_Init(void)
{
	EPD_2IN13B_V4_Reset();

	EPD_2IN13B_V4_ReadBusy();   
	EPD_2IN13B_V4_SendCommand(0x12);  //SWRESET
	EPD_2IN13B_V4_ReadBusy();   

	EPD_2IN13B_V4_SendCommand(0x01); //Driver output control      
	EPD_2IN13B_V4_SendData(0xf9);
	EPD_2IN13B_V4_SendData(0x00);
	EPD_2IN13B_V4_SendData(0x00);

	EPD_2IN13B_V4_SendCommand(0x11); //data entry mode       
	EPD_2IN13B_V4_SendData(0x03);

	EPD_2IN13B_V4_SetWindows(0, 0, EPD_2IN13B_V4_WIDTH-1, EPD_2IN13B_V4_HEIGHT-1);
	EPD_2IN13B_V4_SetCursor(0, 0);

	EPD_2IN13B_V4_SendCommand(0x3C); //BorderWavefrom
	EPD_2IN13B_V4_SendData(0x05);	

	EPD_2IN13B_V4_SendCommand(0x18); //Read built-in temperature sensor
	EPD_2IN13B_V4_SendData(0x80);	

	EPD_2IN13B_V4_SendCommand(0x21); //  Display update control
	EPD_2IN13B_V4_SendData(0x80);	
	EPD_2IN13B_V4_SendData(0x80);

	EPD_2IN13B_V4_ReadBusy();

}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13B_V4_Clear(void)
{
	UWORD Width, Height;
    Width = (EPD_2IN13B_V4_WIDTH % 8 == 0)? (EPD_2IN13B_V4_WIDTH / 8 ): (EPD_2IN13B_V4_WIDTH / 8 + 1);
    Height = EPD_2IN13B_V4_HEIGHT;
	
    EPD_2IN13B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13B_V4_SendData(0XFF);
        }
    }	
    EPD_2IN13B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13B_V4_SendData(0XFF);
        }
    }
	EPD_2IN13B_V4_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13B_V4_Display(const UBYTE *blackImage, const UBYTE *redImage)
{
	UWORD Width, Height;
    Width = (EPD_2IN13B_V4_WIDTH % 8 == 0)? (EPD_2IN13B_V4_WIDTH / 8 ): (EPD_2IN13B_V4_WIDTH / 8 + 1);
    Height = EPD_2IN13B_V4_HEIGHT;
	
    EPD_2IN13B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13B_V4_SendData(blackImage[i + j * Width]);
        }
    }	
	EPD_2IN13B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN13B_V4_SendData(redImage[i + j * Width]);
        }
    }	
	EPD_2IN13B_V4_TurnOnDisplay();	
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13B_V4_Sleep(void)
{
	EPD_2IN13B_V4_SendCommand(0x10); //enter deep sleep
	EPD_2IN13B_V4_SendData(0x01); 
	DEV_Delay_ms(100);
}
