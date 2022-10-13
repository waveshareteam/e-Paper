/*****************************************************************************
* | File      	:   EPD_2in36g.c
* | Author      :   Waveshare team
* | Function    :   2.36inch e-Paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-17
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
#include "EPD_2in36g.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN36G_Reset(void)
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
static void EPD_2IN36G_SendCommand(UBYTE Reg)
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
static void EPD_2IN36G_SendData(UBYTE Data)
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
static void EPD_2IN36G_ReadBusyH(void)
{
    Debug("e-Paper busy H\r\n");
    while(!DEV_Digital_Read(EPD_BUSY_PIN)) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(5);
    }
    Debug("e-Paper busy H release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN36G_TurnOnDisplay(void)
{
    EPD_2IN36G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_2IN36G_SendData(0x01);
    EPD_2IN36G_ReadBusyH();

    EPD_2IN36G_SendCommand(0x02); // POWER_OFF
    EPD_2IN36G_SendData(0X00);
    EPD_2IN36G_ReadBusyH();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN36G_Init(void)
{
    EPD_2IN36G_Reset();

    EPD_2IN36G_SendCommand(0x66);
    EPD_2IN36G_SendData(0x49);
    EPD_2IN36G_SendData(0x55);
    EPD_2IN36G_SendData(0x13);
    EPD_2IN36G_SendData(0x5D);

    EPD_2IN36G_SendCommand(0x66);
    EPD_2IN36G_SendData(0x49);
    EPD_2IN36G_SendData(0x55);

    EPD_2IN36G_SendCommand(0xB0);
    EPD_2IN36G_SendData(0x03);//1 boost 20211113

    EPD_2IN36G_SendCommand(0x00);
    EPD_2IN36G_SendData(0x4F);
    EPD_2IN36G_SendData(0x69);

    EPD_2IN36G_SendCommand(0x03);
    EPD_2IN36G_SendData(0x00);

    EPD_2IN36G_SendCommand(0xF0);
    EPD_2IN36G_SendData(0xF6);
    EPD_2IN36G_SendData(0x0D);
    EPD_2IN36G_SendData(0x00);
    EPD_2IN36G_SendData(0x00);
    EPD_2IN36G_SendData(0x00);

    EPD_2IN36G_SendCommand(0x06); //20211113
    EPD_2IN36G_SendData(0xCF);
    EPD_2IN36G_SendData(0xDE);
    EPD_2IN36G_SendData(0x0F);

    EPD_2IN36G_SendCommand(0x41);
    EPD_2IN36G_SendData(0x00);

    EPD_2IN36G_SendCommand(0x50);
    EPD_2IN36G_SendData(0x30);

    EPD_2IN36G_SendCommand(0x60);
    EPD_2IN36G_SendData(0x0C); 
    EPD_2IN36G_SendData(0x05);

    EPD_2IN36G_SendCommand(0x61);
    EPD_2IN36G_SendData(0xA8);
    EPD_2IN36G_SendData(0x01); 
    EPD_2IN36G_SendData(0x28); 

    EPD_2IN36G_SendCommand(0x84);
    EPD_2IN36G_SendData(0x01);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN36G_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_2IN36G_WIDTH % 4 == 0)? (EPD_2IN36G_WIDTH / 4 ): (EPD_2IN36G_WIDTH / 4 + 1);
    Height = EPD_2IN36G_HEIGHT;

    EPD_2IN36G_SendCommand(0x68);
    EPD_2IN36G_SendData(0x01);
    
    EPD_2IN36G_SendCommand(0x04);
    EPD_2IN36G_ReadBusyH();

    EPD_2IN36G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
                EPD_2IN36G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_2IN36G_SendCommand(0x68);
    EPD_2IN36G_SendData(0x00);

    EPD_2IN36G_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN36G_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN36G_WIDTH % 4 == 0)? (EPD_2IN36G_WIDTH / 4 ): (EPD_2IN36G_WIDTH / 4 + 1);
    Height = EPD_2IN36G_HEIGHT;

    EPD_2IN36G_SendCommand(0x68);
    EPD_2IN36G_SendData(0x01);
    
    EPD_2IN36G_SendCommand(0x04);
    EPD_2IN36G_ReadBusyH();

    EPD_2IN36G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN36G_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_2IN36G_SendCommand(0x68);
    EPD_2IN36G_SendData(0x00);
	
    EPD_2IN36G_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN36G_Sleep(void)
{
    EPD_2IN36G_SendCommand(0x02); // POWER_OFF
    EPD_2IN36G_SendData(0X00);
    EPD_2IN36G_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN36G_SendData(0XA5);
}

