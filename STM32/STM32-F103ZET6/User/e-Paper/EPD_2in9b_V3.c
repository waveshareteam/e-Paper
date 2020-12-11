/*****************************************************************************
* | File      	:   EPD_2in9b_V3.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper b V3
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2020-12-03
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
#include "EPD_2in9b_V3.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN9B_V3_Reset(void)
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
static void EPD_2IN9B_V3_SendCommand(UBYTE Reg)
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
static void EPD_2IN9B_V3_SendData(UBYTE Data)
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
void EPD_2IN9B_V3_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    UBYTE busy;
	do
	{
		EPD_2IN9B_V3_SendCommand(0x71);
		busy = DEV_Digital_Read(EPD_BUSY_PIN);
		busy =!(busy & 0x01);        
	}
	while(busy); 
    Debug("e-Paper busy release\r\n");
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN9B_V3_Init(void)
{
    EPD_2IN9B_V3_Reset();

    EPD_2IN9B_V3_SendCommand(0x04);  
    EPD_2IN9B_V3_ReadBusy();//waiting for the electronic paper IC to release the idle signal

    EPD_2IN9B_V3_SendCommand(0x00);//panel setting
    EPD_2IN9B_V3_SendData(0x0f);//LUT from OTP，128x296
    EPD_2IN9B_V3_SendData(0x89);//Temperature sensor, boost and other related timing settings

    EPD_2IN9B_V3_SendCommand(0x61);//resolution setting
    EPD_2IN9B_V3_SendData (0x80);
    EPD_2IN9B_V3_SendData (0x01);
    EPD_2IN9B_V3_SendData (0x28);

    EPD_2IN9B_V3_SendCommand(0X50);//VCOM AND DATA INTERVAL SETTING			
    EPD_2IN9B_V3_SendData(0x77);//WBmode:VBDF 17|D7 VBDW 97 VBDB 57		
                            //WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN9B_V3_Clear(void)
{
    UWORD Width = (EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V3_HEIGHT;

    //send black data
    EPD_2IN9B_V3_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V3_SendData(0xFF);
        }
    }

    //send red data
    EPD_2IN9B_V3_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V3_SendData(0xFF);
        }
    }
    
    EPD_2IN9B_V3_SendCommand(0x12);
    EPD_2IN9B_V3_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN9B_V3_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1);
    Height = EPD_2IN9B_V3_HEIGHT;

	//send black data
    EPD_2IN9B_V3_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V3_SendData(blackimage[i + j * Width]);
        }
    }
    EPD_2IN9B_V3_SendCommand(0x92);
    
	//send red data
    EPD_2IN9B_V3_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V3_SendData(ryimage[i + j * Width]);
        }
    }
    EPD_2IN9B_V3_SendCommand(0x92);

    EPD_2IN9B_V3_SendCommand(0x12);
    EPD_2IN9B_V3_ReadBusy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN9B_V3_Sleep(void)
{
    EPD_2IN9B_V3_SendCommand(0x02); // POWER_OFF
    EPD_2IN9B_V3_ReadBusy();
    EPD_2IN9B_V3_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN9B_V3_SendData(0xA5); // check code
}
