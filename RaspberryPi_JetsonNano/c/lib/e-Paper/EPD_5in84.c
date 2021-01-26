/*****************************************************************************
* | File      	:   EPD_5in84.c
* | Author      :   Waveshare team
* | Function    :   5.84inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-01-08
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
#include "EPD_5in84.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in84_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(3);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_5in84_SendCommand(UBYTE Reg)
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
static void EPD_5in84_SendData(UBYTE Data)
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
static void EPD_5in84_ReadBusy(void)
{
	Debug("e-Paper busy\r\n");
	do {
		EPD_5in84_SendCommand(0x71);
		DEV_Delay_ms(10);    
	}
	while(!DEV_Digital_Read(EPD_BUSY_PIN));   
	Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5in84_TurnOnDisplay(void)
{
	EPD_5in84_SendCommand(0x12);	//DISPLAY REFRESH 	
	DEV_Delay_ms(100);					//!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in84_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5in84_Init(void)
{
    EPD_5in84_Reset();

	EPD_5in84_SendCommand(0x61);  // Resolution setting
	EPD_5in84_SendData(0x03);
	EPD_5in84_SendData(0x00);
	EPD_5in84_SendData(0x01);
	EPD_5in84_SendData(0x00);
	
	EPD_5in84_SendCommand(0x00);
	EPD_5in84_SendData(0xdf); 
	
	EPD_5in84_SendCommand(0xF8);  // Enter FITI Command
	EPD_5in84_SendData(0x60); 
	EPD_5in84_SendData(0xA5); 

	EPD_5in84_SendCommand(0xF8);  // Enter FITI Command
	EPD_5in84_SendData(0x89); 
	EPD_5in84_SendData(0xA5); 

	EPD_5in84_SendCommand(0xF8);  
	EPD_5in84_SendData(0x76);
	EPD_5in84_SendData(0x1F);
				
	EPD_5in84_SendCommand(0xF8);   //boost constant on time
	EPD_5in84_SendData(0x7E);
	EPD_5in84_SendData(0x31); 
	
	EPD_5in84_SendCommand(0xF8);  
	EPD_5in84_SendData(0xB8);  
	EPD_5in84_SendData(0x80);  
	
	EPD_5in84_SendCommand(0xF8);   //vgl=> GND:08, HZ:00[default]
	EPD_5in84_SendData(0x92);  
	EPD_5in84_SendData(0x00);  
	
	EPD_5in84_SendCommand(0xF8);   //VCOM(2frme off)=> GND:01(0x88=06)[default], HZ:11  
	EPD_5in84_SendData(0x87);  
	EPD_5in84_SendData(0x01);
	
	EPD_5in84_SendCommand(0xF8);   //r_vcom_init_sel
	EPD_5in84_SendData(0x88);  
	EPD_5in84_SendData(0x06); 
				
	EPD_5in84_SendCommand(0xF8);   
	EPD_5in84_SendData(0xA8);  
	EPD_5in84_SendData(0x30); 
	
	EPD_5in84_SendCommand(0xF8);   
	EPD_5in84_SendData(0xE8); 
	EPD_5in84_SendData(0x88); 

	EPD_5in84_SendCommand(0x04);  //power on  
	EPD_5in84_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in84_Clear(void)
{
    UWORD Width, Height, i;
    Width = (EPD_5in84_WIDTH % 8 == 0)? (EPD_5in84_WIDTH / 8 ): (EPD_5in84_WIDTH / 8 + 1);
    Height = EPD_5in84_HEIGHT;
	
	// EPD_5in84_SendCommand(0x10);
	// for(i=0; i<Width*Height; i++) {
		// EPD_5in84_SendData(0x00);  
	// }
	EPD_5in84_SendCommand(0x13);
	for(i=0; i<Width*Height; i++) {
		EPD_5in84_SendData(0xff);  
	}
	EPD_5in84_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in84_Display(UBYTE *Image)
{
    UWORD Width, Height, i, j;
    Width = (EPD_5in84_WIDTH % 8 == 0)? (EPD_5in84_WIDTH / 8 ): (EPD_5in84_WIDTH / 8 + 1);
    Height = EPD_5in84_HEIGHT;
	
	// EPD_5in84_SendCommand(0x10);
	// for(i=0;i<Height;i++) {
		// for(j=0; j<Width; j++) {
			// EPD_5in84_SendData(0x00);
		// }
	// }
	EPD_5in84_SendCommand(0x13);
	for(i=0;i<Height;i++) {
		for(j=0; j<Width; j++) {
			EPD_5in84_SendData(Image[i*Width + j]);  
		}
	}
    EPD_5in84_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in84_Sleep(void)
{
	EPD_5in84_SendCommand(0X02);  	//power off
	EPD_5in84_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
	EPD_5in84_SendCommand(0X07);		//deep sleep
	EPD_5in84_SendData(0xA5);
}

