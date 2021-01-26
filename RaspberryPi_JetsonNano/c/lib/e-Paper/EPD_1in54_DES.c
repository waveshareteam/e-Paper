/*****************************************************************************
* | File      	:  	EPD_1in54_DES.c
* | Author      :   Waveshare team
* | Function    :   1.54inch DES e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-01-13
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
#include "EPD_1in54_DES.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN54_DES_Reset(void)
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
static void EPD_1IN54_DES_SendCommand(UBYTE Reg)
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
static void EPD_1IN54_DES_SendData(UBYTE Data)
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
void EPD_1IN54_DES_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	while(1)
	{	 //=1 BUSY
		EPD_1IN54_DES_SendCommand(0x71);
		if(DEV_Digital_Read(EPD_BUSY_PIN)==1) 
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
static void EPD_1IN54_DES_TurnOnDisplay(void)
{
	EPD_1IN54_DES_SendCommand(0x12); //Display Update Control
	EPD_1IN54_DES_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN54_DES_Init(void)
{
	EPD_1IN54_DES_Reset();
	DEV_Delay_ms(100);
 
	EPD_1IN54_DES_SendCommand(0x04);  //SWRESET
	EPD_1IN54_DES_ReadBusy();   

	EPD_1IN54_DES_SendCommand(0x00); //Driver output control      
	EPD_1IN54_DES_SendData(0x1f);
	
	EPD_1IN54_DES_SendCommand(0x50);
	EPD_1IN54_DES_SendData(0x97);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN54_DES_Clear(void)
{
	UWORD i;
	// EPD_1IN54_DES_SendCommand(0x10);
	// for(i=0;i<2888;i++)
	// {
		// EPD_1IN54_DES_SendData(0xff);
	// }
	EPD_1IN54_DES_SendCommand(0x13);
	for(i=0;i<2888;i++)
	{
		EPD_1IN54_DES_SendData(0xff);
	}
	EPD_1IN54_DES_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54_DES_Display(UBYTE *Image)
{
	UWORD i;	
	// EPD_1IN54_DES_SendCommand(0x10);
	// for(i=0;i<2888;i++)
	// {
		// EPD_1IN54_DES_SendData(0xff);
	// }
	EPD_1IN54_DES_SendCommand(0x13);
	for(i=0;i<2888;i++)
	{
		EPD_1IN54_DES_SendData(Image[i]);
	}
	EPD_1IN54_DES_TurnOnDisplay();	
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN54_DES_Sleep(void)
{
	EPD_1IN54_DES_SendCommand(0x50); //enter deep sleep
	EPD_1IN54_DES_SendData(0xf7); 
	EPD_1IN54_DES_SendCommand(0x02);
	EPD_1IN54_DES_ReadBusy();
	EPD_1IN54_DES_SendCommand(0x07);
	EPD_1IN54_DES_SendData(0xa5);
}
