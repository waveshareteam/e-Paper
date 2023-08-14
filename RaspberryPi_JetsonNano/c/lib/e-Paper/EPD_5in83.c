/*****************************************************************************
* | File      	:   EPD_5in83_V2.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-23
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
#include "EPD_5in83_V2.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in83_V2_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(5);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_5in83_V2_SendCommand(UBYTE Reg)
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
static void EPD_5in83_V2_SendData(UBYTE Data)
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
static void EPD_5in83_V2_ReadBusy(void)
{
	Debug("e-Paper busy\r\n");
	do {
		EPD_5in83_V2_SendCommand(0x71);
		DEV_Delay_ms(10);    
	}
	while(!DEV_Digital_Read(EPD_BUSY_PIN));   
	Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5in83_V2_TurnOnDisplay(void)
{
	EPD_5in83_V2_SendCommand(0x12);	//DISPLAY REFRESH 	
	DEV_Delay_ms(100);					//!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in83_V2_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5in83_V2_Init(void)
{
    EPD_5in83_V2_Reset();

	EPD_5in83_V2_SendCommand(0x01);			//POWER SETTING
	EPD_5in83_V2_SendData (0x07);
	EPD_5in83_V2_SendData (0x07);    //VGH=20V,VGL=-20V
	EPD_5in83_V2_SendData (0x3f);		//VDH=15V
	EPD_5in83_V2_SendData (0x3f);		//VDL=-15V

	EPD_5in83_V2_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);  
	EPD_5in83_V2_ReadBusy();        //waiting for the electronic paper IC to release the idle signal

	EPD_5in83_V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_5in83_V2_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	EPD_5in83_V2_SendCommand(0x61);        	//tres			
	EPD_5in83_V2_SendData (0x02);		//source 648
	EPD_5in83_V2_SendData (0x88);
	EPD_5in83_V2_SendData (0x01);		//gate 480
	EPD_5in83_V2_SendData (0xE0);

	EPD_5in83_V2_SendCommand(0X15);		
	EPD_5in83_V2_SendData(0x00);		

	EPD_5in83_V2_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_5in83_V2_SendData(0x10);
	EPD_5in83_V2_SendData(0x07);

	EPD_5in83_V2_SendCommand(0X60);			//TCON SETTING
	EPD_5in83_V2_SendData(0x22);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in83_V2_Clear(void)
{
    UWORD Width, Height, i;
    Width = (EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1);
    Height = EPD_5in83_V2_HEIGHT;

	EPD_5in83_V2_SendCommand(0x10);
	for(i=0; i<Width*Height; i++) {
		EPD_5in83_V2_SendData(0x00);  
	}
	EPD_5in83_V2_SendCommand(0x13);
	for(i=0; i<Width*Height; i++) {
		EPD_5in83_V2_SendData(0x00);  
	}
	EPD_5in83_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in83_V2_Display(UBYTE *Image)
{
    UWORD Width, Height, i, j;
    Width = (EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1);
    Height = EPD_5in83_V2_HEIGHT;

	EPD_5in83_V2_SendCommand(0x10);
	for(i=0;i<Height;i++) {
		for(j=0; j<Width; j++) {
			EPD_5in83_V2_SendData(0x00);  
		}
	}
	EPD_5in83_V2_SendCommand(0x13);
	for(i=0;i<Height;i++) {
		for(j=0; j<Width; j++) {
			EPD_5in83_V2_SendData(~Image[i*Width + j]);  
		}
	}
    EPD_5in83_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in83_V2_Sleep(void)
{
	EPD_5in83_V2_SendCommand(0X02);  	//power off
	EPD_5in83_V2_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
	EPD_5in83_V2_SendCommand(0X07);		//deep sleep
	EPD_5in83_V2_SendData(0xA5);
}

