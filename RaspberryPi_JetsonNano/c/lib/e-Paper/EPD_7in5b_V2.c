/*****************************************************************************
* | File      	:	EPD_7IN5B_V2.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-30
* | Info        :
******************************************************************************
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files(the "Software"), to deal
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
#include "EPD_7in5b_V2.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN5B_V2_Reset(void)
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
static void EPD_7IN5B_V2_SendCommand(UBYTE Reg)
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
static void EPD_7IN5B_V2_SendData(UBYTE Data)
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
void EPD_7IN5B_V2_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
	do	{
		DEV_Delay_ms(20);   
	}while(!(DEV_Digital_Read(EPD_BUSY_PIN)));
	DEV_Delay_ms(20);      
	Debug("e-Paper busy release\r\n");
}


/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5B_V2_TurnOnDisplay(void)
{
    EPD_7IN5B_V2_SendCommand(0x12);			//DISPLAY REFRESH
    DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!
    EPD_7IN5B_V2_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_7IN5B_V2_Init(void)
{
    EPD_7IN5B_V2_Reset();

    EPD_7IN5B_V2_SendCommand(0x01);			//POWER SETTING
    EPD_7IN5B_V2_SendData(0x07);
    EPD_7IN5B_V2_SendData(0x07);    //VGH=20V,VGL=-20V
    EPD_7IN5B_V2_SendData(0x3f);		//VDH=15V
    EPD_7IN5B_V2_SendData(0x3f);		//VDL=-15V

    EPD_7IN5B_V2_SendCommand(0x04); //POWER ON
    DEV_Delay_ms(100);
    EPD_7IN5B_V2_WaitUntilIdle();

    EPD_7IN5B_V2_SendCommand(0X00);			//PANNEL SETTING
    EPD_7IN5B_V2_SendData(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_7IN5B_V2_SendCommand(0x61);        	//tres
    EPD_7IN5B_V2_SendData(0x03);		//source 800
    EPD_7IN5B_V2_SendData(0x20);
    EPD_7IN5B_V2_SendData(0x01);		//gate 480
    EPD_7IN5B_V2_SendData(0xE0);

    EPD_7IN5B_V2_SendCommand(0X15);
    EPD_7IN5B_V2_SendData(0x00);

    EPD_7IN5B_V2_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    EPD_7IN5B_V2_SendData(0x11);
    EPD_7IN5B_V2_SendData(0x07);

    EPD_7IN5B_V2_SendCommand(0X60);			//TCON SETTING
    EPD_7IN5B_V2_SendData(0x22);

    EPD_7IN5B_V2_SendCommand(0x65);  // Resolution setting
    EPD_7IN5B_V2_SendData(0x00);
    EPD_7IN5B_V2_SendData(0x00);//800*480
    EPD_7IN5B_V2_SendData(0x00);
    EPD_7IN5B_V2_SendData(0x00);

    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Clear(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    EPD_7IN5B_V2_SendCommand(0x10);
    for(i=0; i<Width*Height; i++) {
        EPD_7IN5B_V2_SendData(0xff);

    }
    EPD_7IN5B_V2_SendCommand(0x13);
    for(i=0; i<Width*Height; i++)	{
        EPD_7IN5B_V2_SendData(0x00);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

void EPD_7IN5B_V2_ClearRed(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    EPD_7IN5B_V2_SendCommand(0x10);
    for(i=0; i<Width*Height; i++) {
        EPD_7IN5B_V2_SendData(0xff);

    }
    EPD_7IN5B_V2_SendCommand(0x13);
    for(i=0; i<Width*Height; i++)	{
        EPD_7IN5B_V2_SendData(0xff);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

void EPD_7IN5B_V2_ClearBlack(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

    UWORD i;
    EPD_7IN5B_V2_SendCommand(0x10);
    for(i=0; i<Width*Height; i++) {
        EPD_7IN5B_V2_SendData(0x00);

    }
    EPD_7IN5B_V2_SendCommand(0x13);
    for(i=0; i<Width*Height; i++)	{
        EPD_7IN5B_V2_SendData(0x00);

    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5B_V2_WIDTH % 8 == 0)?(EPD_7IN5B_V2_WIDTH / 8 ):(EPD_7IN5B_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5B_V2_HEIGHT;

 //send black data
    EPD_7IN5B_V2_SendCommand(0x10);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_7IN5B_V2_SendData(blackimage[i + j * Width]);
        }
    }

    //send red data
    EPD_7IN5B_V2_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_7IN5B_V2_SendData(~ryimage[i + j * Width]);
        }
    }
    EPD_7IN5B_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5B_V2_Sleep(void)
{
    EPD_7IN5B_V2_SendCommand(0X02);  	//power off
    EPD_7IN5B_V2_WaitUntilIdle();
    EPD_7IN5B_V2_SendCommand(0X07);  	//deep sleep
    EPD_7IN5B_V2_SendData(0xA5);
}
