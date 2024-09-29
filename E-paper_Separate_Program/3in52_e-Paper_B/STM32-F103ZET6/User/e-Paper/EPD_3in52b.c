/*****************************************************************************
* | File      	:   EPD_3IN52B.C
* | Author      :   Waveshare team
* | Function    :   3.52inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-09-27
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
#include "EPD_3in52b.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void EPD_3IN52B_Reset(void)
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
void EPD_3IN52B_SendCommand(UBYTE Reg)
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
void EPD_3IN52B_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Read Busy
parameter:
******************************************************************************/
void EPD_3IN52B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(!DEV_Digital_Read(EPD_BUSY_PIN))
    {
        DEV_Delay_ms(20);
    }
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}


/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_3IN52B_TurnOnDisplay(void)
{
    EPD_3IN52B_SendCommand(0x12); // DISPLAY_REFRESH
    // EPD_3IN52B_SendData(0xA5);	
    EPD_3IN52B_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN52B_Init(void)
{
    EPD_3IN52B_Reset();

    EPD_3IN52B_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);
    EPD_3IN52B_ReadBusy();

    EPD_3IN52B_SendCommand(0x00);	 
    EPD_3IN52B_SendData(0x03);	
    EPD_3IN52B_SendData(0x0D);	

    EPD_3IN52B_SendCommand(0x61);	 	
	EPD_3IN52B_SendData(0xF0);	
	EPD_3IN52B_SendData(0x01);	
	EPD_3IN52B_SendData(0x68);
	
	EPD_3IN52B_SendCommand(0x06);	 	
	EPD_3IN52B_SendData(0x2F);	
	EPD_3IN52B_SendData(0x2F);	
    EPD_3IN52B_SendData(0x2E);

    EPD_3IN52B_ReadBusy();

}

void EPD_3IN52B_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_3IN52B_WIDTH % 8 == 0)? (EPD_3IN52B_WIDTH / 8 ): (EPD_3IN52B_WIDTH / 8 + 1);
    Height = EPD_3IN52B_HEIGHT;

    EPD_3IN52B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN52B_SendData(~blackimage[i + j * Width]);
        }
    }

    EPD_3IN52B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN52B_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_3IN52B_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_3IN52B_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_3IN52B_WIDTH % 8 == 0)? (EPD_3IN52B_WIDTH / 8 ): (EPD_3IN52B_WIDTH / 8 + 1);
    Height = EPD_3IN52B_HEIGHT;

    EPD_3IN52B_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN52B_SendData(0x00);
        }
    }

    EPD_3IN52B_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN52B_SendData(0x00);
        }
    }

    EPD_3IN52B_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN52B_sleep(void)
{
    EPD_3IN52B_SendCommand(0X07);  	//deep sleep
    EPD_3IN52B_SendData(0xA5);
}



