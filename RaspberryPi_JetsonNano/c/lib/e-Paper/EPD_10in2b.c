/*****************************************************************************
* | File      	:  	EPD_10in2b.c
* | Author      :   Waveshare team
* | Function    :   10.2inch b e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-01-20
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
#include "EPD_10in2b.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_10IN2b_Reset(void)
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
static void EPD_10IN2b_SendCommand(UBYTE Reg)
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
static void EPD_10IN2b_SendData(UBYTE Data)
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
void EPD_10IN2b_ReadBusy(void)
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
static void EPD_10IN2b_TurnOnDisplay(void)
{
	EPD_10IN2b_SendCommand(0x22); //Display Update Control
	EPD_10IN2b_SendData(0xC7);
	EPD_10IN2b_SendCommand(0x20); //Activate Display Update Sequence
	EPD_10IN2b_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_10IN2b_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_10IN2b_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_10IN2b_SendData(Xstart & 0xFF);
    EPD_10IN2b_SendData((Xstart>>8) & 0x03);
    EPD_10IN2b_SendData(Xend & 0xFF);
    EPD_10IN2b_SendData((Xend>>8) & 0x03);
	
    EPD_10IN2b_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_10IN2b_SendData(Ystart & 0xFF);
    EPD_10IN2b_SendData((Ystart>>8) & 0x03);
    EPD_10IN2b_SendData(Yend & 0xFF);
    EPD_10IN2b_SendData((Yend>>8) & 0x03);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_10IN2b_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_10IN2b_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_10IN2b_SendData(Xstart & 0xFF);
    EPD_10IN2b_SendData((Xstart>>8) & 0x03);

    EPD_10IN2b_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_10IN2b_SendData(Ystart & 0xFF);
    EPD_10IN2b_SendData((Ystart>>8) & 0x03);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_10IN2b_Init(void)
{
	EPD_10IN2b_Reset();
	DEV_Delay_ms(100);

	EPD_10IN2b_ReadBusy();   
	EPD_10IN2b_SendCommand(0x12);  //SWRESET
	EPD_10IN2b_ReadBusy();   

	EPD_10IN2b_SendCommand(0x0C); //set soft start     
	EPD_10IN2b_SendData(0xAE);
	EPD_10IN2b_SendData(0xC7);
	EPD_10IN2b_SendData(0xC3);
	EPD_10IN2b_SendData(0xC0);
	EPD_10IN2b_SendData(0x80);

	EPD_10IN2b_SendCommand(0x01);   //      drive output control    
	EPD_10IN2b_SendData(0xA7); //  Y  
	EPD_10IN2b_SendData(0x02); //  Y 
	EPD_10IN2b_SendData(0x00);

	EPD_10IN2b_SendCommand(0x11);        //    data  entry  mode
	EPD_10IN2b_SendData(0x03);           //       X-mode  x+ y-    

	EPD_10IN2b_SetWindows(0, 0, EPD_10IN2b_WIDTH-1, EPD_10IN2b_HEIGHT-1);

	EPD_10IN2b_SendCommand(0x3C);        // Border       Border setting 
	EPD_10IN2b_SendData(0x01);

	EPD_10IN2b_SendCommand(0x18); // use the internal temperature sensor
	EPD_10IN2b_SendData(0x80);

    EPD_10IN2b_SendCommand(0x22);  
    EPD_10IN2b_SendData(0xB1); 
    EPD_10IN2b_SendCommand(0x20); 
	
	EPD_10IN2b_SetCursor(0, 0);
	
	EPD_10IN2b_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_10IN2b_Clear(void)
{
	UWORD i, j;
	UWORD height = EPD_10IN2b_HEIGHT;
	UWORD width = EPD_10IN2b_WIDTH/8;	
	
	EPD_10IN2b_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_10IN2b_SendData(0xff);
	}
	EPD_10IN2b_SendCommand(0x26);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_10IN2b_SendData(0x00);
	}
	EPD_10IN2b_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_10IN2b_Display(UBYTE *Image, UBYTE *RedImage)
{
	UWORD i, j;
	UWORD height = EPD_10IN2b_HEIGHT;
	UWORD width = EPD_10IN2b_WIDTH/8;
	
	EPD_10IN2b_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_10IN2b_SendData(Image[j + i*width]);
	}
	EPD_10IN2b_SendCommand(0x26);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_10IN2b_SendData(~RedImage[j + i*width]);
	}
	EPD_10IN2b_TurnOnDisplay();	
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_10IN2b_Sleep(void)
{
	EPD_10IN2b_SendCommand(0x10); //enter deep sleep
	EPD_10IN2b_SendData(0x01); 
	DEV_Delay_ms(100);
}
