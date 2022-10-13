/*****************************************************************************
* | File      	:   EPD_1in64_g.c
* | Author      :   Waveshare team
* | Function    :   1.64inchg e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-07-22
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
#include "EPD_1in64g.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN64G_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_1IN64G_SendCommand(UBYTE Reg)
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
static void EPD_1IN64G_SendData(UBYTE Data)
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
static void EPD_1IN64G_ReadBusyH(void)
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
static void EPD_1IN64G_TurnOnDisplay(void)
{
    EPD_1IN64G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_1IN64G_SendData(0x01);
    EPD_1IN64G_ReadBusyH();

    EPD_1IN64G_SendCommand(0x02); // POWER_OFF
    EPD_1IN64G_SendData(0X00);
    EPD_1IN64G_ReadBusyH();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN64G_Init(void)
{
    EPD_1IN64G_Reset();

    EPD_1IN64G_SendCommand(0x66);
    EPD_1IN64G_SendData(0x49);
    EPD_1IN64G_SendData(0x55);
    EPD_1IN64G_SendData(0x13);
    EPD_1IN64G_SendData(0x5D);

    EPD_1IN64G_SendCommand(0x66);
    EPD_1IN64G_SendData(0x49);
    EPD_1IN64G_SendData(0x55);

    EPD_1IN64G_SendCommand(0xB0);
    EPD_1IN64G_SendData(0x03);//1 boost 20211113


    EPD_1IN64G_SendCommand(0x00);
    EPD_1IN64G_SendData(0x4F);
    EPD_1IN64G_SendData(0x6B);

    EPD_1IN64G_SendCommand(0x03);
    EPD_1IN64G_SendData(0x00);

    EPD_1IN64G_SendCommand(0xF0);
    EPD_1IN64G_SendData(0xF6);
    EPD_1IN64G_SendData(0x0D);
    EPD_1IN64G_SendData(0x00);
    EPD_1IN64G_SendData(0x00);
    EPD_1IN64G_SendData(0x00);

    //20220303
    EPD_1IN64G_SendCommand(0x06);
    EPD_1IN64G_SendData(0xCF);
    EPD_1IN64G_SendData(0xDF);
    EPD_1IN64G_SendData(0x0F);

    EPD_1IN64G_SendCommand(0x41);
    EPD_1IN64G_SendData(0x00);

    EPD_1IN64G_SendCommand(0x50);
    EPD_1IN64G_SendData(0x30);

    EPD_1IN64G_SendCommand(0x60);
    EPD_1IN64G_SendData(0x0C); 
    EPD_1IN64G_SendData(0x05);

    EPD_1IN64G_SendCommand(0x61);
    EPD_1IN64G_SendData(0xA8);
    EPD_1IN64G_SendData(0x00); 
    EPD_1IN64G_SendData(0xA8); 

    EPD_1IN64G_SendCommand(0x84);
    EPD_1IN64G_SendData(0x01);

}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN64G_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_1IN64G_WIDTH % 4 == 0)? (EPD_1IN64G_WIDTH / 4 ): (EPD_1IN64G_WIDTH / 4 + 1);
    Height = EPD_1IN64G_HEIGHT;

    EPD_1IN64G_SendCommand(0x68);
    EPD_1IN64G_SendData(0x01);
    
    EPD_1IN64G_SendCommand(0x04);
    EPD_1IN64G_ReadBusyH();

    EPD_1IN64G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN64G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_1IN64G_SendCommand(0x68);
    EPD_1IN64G_SendData(0x00);

    EPD_1IN64G_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN64G_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_1IN64G_WIDTH % 4 == 0)? (EPD_1IN64G_WIDTH / 4 ): (EPD_1IN64G_WIDTH / 4 + 1);
    Height = EPD_1IN64G_HEIGHT;

    EPD_1IN64G_SendCommand(0x68);
    EPD_1IN64G_SendData(0x01);
    
    EPD_1IN64G_SendCommand(0x04);
    EPD_1IN64G_ReadBusyH();

    EPD_1IN64G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN64G_SendData(Image[i + j * Width]);
        }
    }

    EPD_1IN64G_SendCommand(0x68);
    EPD_1IN64G_SendData(0x00);

    EPD_1IN64G_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN64G_Sleep(void)
{
    EPD_1IN64G_SendCommand(0x02); // POWER_OFF
    EPD_1IN64G_SendData(0X00);
    EPD_1IN64G_SendCommand(0x07); // DEEP_SLEEP
    EPD_1IN64G_SendData(0XA5);
}

