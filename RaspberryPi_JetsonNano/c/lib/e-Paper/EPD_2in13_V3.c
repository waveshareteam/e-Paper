/*****************************************************************************
* | File      	:  	EPD_2in13_V3.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V3
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-22
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
#include "EPD_2in13_V3.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2in13_V3_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2in13_V3_SendCommand(UBYTE Reg)
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
static void EPD_2in13_V3_SendData(UBYTE Data)
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
void EPD_2in13_V3_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	while(1)
	{	 //=1 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==0) 
			break;
		DEV_Delay_ms(50);
	}
	DEV_Delay_ms(50);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2in13_V3_TurnOnDisplay(void)
{
	EPD_2in13_V3_SendCommand(0x22); //Display Update Control
	EPD_2in13_V3_SendData(0xF7);
	EPD_2in13_V3_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2in13_V3_ReadBusy();
}

static void EPD_2in13_V3_TurnOnDisplay_Partial(void)
{
	EPD_2in13_V3_SendCommand(0x22); //Display Update Control
	EPD_2in13_V3_SendData(0xFF);   
	EPD_2in13_V3_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2in13_V3_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_2in13_V3_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
	// Ystart = 295 - Ystart;
	
    EPD_2in13_V3_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2in13_V3_SendData((Xstart>>3) & 0xFF);
    EPD_2in13_V3_SendData((Xend>>3) & 0xFF);
	
    EPD_2in13_V3_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2in13_V3_SendData(Yend & 0xFF);
    EPD_2in13_V3_SendData((Yend >> 8) & 0xFF);
    EPD_2in13_V3_SendData(Ystart & 0xFF);
    EPD_2in13_V3_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_2in13_V3_SetCursor(UWORD Xstart, UWORD Ystart)
{
	Ystart = 295 - Ystart;
	
    EPD_2in13_V3_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2in13_V3_SendData(Xstart & 0xFF);

    EPD_2in13_V3_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2in13_V3_SendData(Ystart & 0xFF);
    EPD_2in13_V3_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2in13_V3_Init(void)
{
	EPD_2in13_V3_Reset();
	DEV_Delay_ms(100);

	EPD_2in13_V3_ReadBusy();   
	EPD_2in13_V3_SendCommand(0x12);  //SWRESET
	EPD_2in13_V3_ReadBusy();   

	EPD_2in13_V3_SendCommand(0x01); //Driver output control      
	EPD_2in13_V3_SendData(0x27);
	EPD_2in13_V3_SendData(0x01);
	EPD_2in13_V3_SendData(0x01);

	EPD_2in13_V3_SendCommand(0x11); //data entry mode       
	EPD_2in13_V3_SendData(0x01);

	EPD_2in13_V3_SetWindows(0, 0, EPD_2in13_V3_WIDTH-1, 296-1);
	
	EPD_2in13_V3_SendCommand(0x3C); //BorderWavefrom
	EPD_2in13_V3_SendData(0x05);	

	EPD_2in13_V3_SendCommand(0x21); //  Display update control
	EPD_2in13_V3_SendData(0x00);
	EPD_2in13_V3_SendData(0x80);	

	EPD_2in13_V3_SendCommand(0x18); //Read built-in temperature sensor
	EPD_2in13_V3_SendData(0x80);	
	
	EPD_2in13_V3_SetCursor(0, 0);
	EPD_2in13_V3_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2in13_V3_Clear(void)
{
	UWORD i;
	EPD_2in13_V3_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		EPD_2in13_V3_SendData(0xff);
	}
	EPD_2in13_V3_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2in13_V3_Display(UBYTE *Image)
{
	UWORD i;	
	EPD_2in13_V3_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		EPD_2in13_V3_SendData(Image[i]);
	}
	EPD_2in13_V3_TurnOnDisplay();	
}

void EPD_2in13_V3_Display_Base(UBYTE *Image)
{
	UWORD i;   

	EPD_2in13_V3_SendCommand(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		EPD_2in13_V3_SendData(Image[i]);
	}
	EPD_2in13_V3_SendCommand(0x26);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		EPD_2in13_V3_SendData(Image[i]);
	}
	EPD_2in13_V3_TurnOnDisplay();	
}

void EPD_2in13_V3_Display_Partial(UBYTE *Image)
{
	UWORD i;
	
	//Reset
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(5);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(10);

	EPD_2in13_V3_SendCommand(0x3C); //BorderWavefrom
	EPD_2in13_V3_SendData(0x80);	
	
	EPD_2in13_V3_SetWindows(0, 0, EPD_2in13_V3_WIDTH-1, 296-1);
	EPD_2in13_V3_SetCursor(0, 0);

	EPD_2in13_V3_SendCommand(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{
		EPD_2in13_V3_SendData(Image[i]);
	}
	EPD_2in13_V3_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2in13_V3_Sleep(void)
{
	EPD_2in13_V3_SendCommand(0x10); //enter deep sleep
	EPD_2in13_V3_SendData(0x01); 
	DEV_Delay_ms(100);
}
