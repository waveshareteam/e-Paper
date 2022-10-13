/*****************************************************************************
* | File      	:   EPD_7in3g.c
* | Author      :   Waveshare team
* | Function    :   7.3inch e-paper (G)
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
#include "EPD_7in3g.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN3G_Reset(void)
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
static void EPD_7IN3G_SendCommand(UBYTE Reg)
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
static void EPD_7IN3G_SendData(UBYTE Data)
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
static void EPD_7IN3G_ReadBusyH(void)
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
static void EPD_7IN3G_TurnOnDisplay(void)
{
    EPD_7IN3G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_7IN3G_SendData(0x00);
    EPD_7IN3G_ReadBusyH();

    EPD_7IN3G_SendCommand(0x02); // POWER_OFF
    EPD_7IN3G_SendData(0X00);
    EPD_7IN3G_ReadBusyH();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN3G_Init(void)
{
    EPD_7IN3G_Reset();
    EPD_7IN3G_ReadBusyH();
    DEV_Delay_ms(30);

    EPD_7IN3G_SendCommand(0xAA);
    EPD_7IN3G_SendData(0x49);
    EPD_7IN3G_SendData(0x55);
    EPD_7IN3G_SendData(0x20);
    EPD_7IN3G_SendData(0x08);
    EPD_7IN3G_SendData(0x09);
    EPD_7IN3G_SendData(0x18);

    EPD_7IN3G_SendCommand(0x01);
    EPD_7IN3G_SendData(0x3F);

    EPD_7IN3G_SendCommand(0x00);
    EPD_7IN3G_SendData(0x4F);
    EPD_7IN3G_SendData(0x69);

    EPD_7IN3G_SendCommand(0x05);
    EPD_7IN3G_SendData(0x40);
    EPD_7IN3G_SendData(0x1F);
    EPD_7IN3G_SendData(0x1F);
    EPD_7IN3G_SendData(0x2C);

    EPD_7IN3G_SendCommand(0x08);
    EPD_7IN3G_SendData(0x6F);
    EPD_7IN3G_SendData(0x1F);
    EPD_7IN3G_SendData(0x1F);
    EPD_7IN3G_SendData(0x22);

    //===================
    //20211212
    //First setting
    EPD_7IN3G_SendCommand(0x06);
    EPD_7IN3G_SendData(0x6F);
    EPD_7IN3G_SendData(0x1F);
    EPD_7IN3G_SendData(0x14);
    EPD_7IN3G_SendData(0x14);
    //===================

    EPD_7IN3G_SendCommand(0x03);
    EPD_7IN3G_SendData(0x00);
    EPD_7IN3G_SendData(0x54);
    EPD_7IN3G_SendData(0x00);
    EPD_7IN3G_SendData(0x44);

    EPD_7IN3G_SendCommand(0x60);
    EPD_7IN3G_SendData(0x02);
    EPD_7IN3G_SendData(0x00);
    //Please notice that PLL must be set for version 2 IC
    EPD_7IN3G_SendCommand(0x30);
    EPD_7IN3G_SendData(0x08);

    EPD_7IN3G_SendCommand(0x50);
    EPD_7IN3G_SendData(0x3F);

    EPD_7IN3G_SendCommand(0x61);
    EPD_7IN3G_SendData(0x03);
    EPD_7IN3G_SendData(0x20);
    EPD_7IN3G_SendData(0x01); 
    EPD_7IN3G_SendData(0xE0); 

    EPD_7IN3G_SendCommand(0xE3);
    EPD_7IN3G_SendData(0x2F);

    EPD_7IN3G_SendCommand(0x84);
    EPD_7IN3G_SendData(0x01);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN3G_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_7IN3G_WIDTH % 4 == 0)? (EPD_7IN3G_WIDTH / 4 ): (EPD_7IN3G_WIDTH / 4 + 1);
    Height = EPD_7IN3G_HEIGHT;
    
    EPD_7IN3G_SendCommand(0x04);
    EPD_7IN3G_ReadBusyH();

    EPD_7IN3G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_7IN3G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_7IN3G_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN3G_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_7IN3G_WIDTH % 4 == 0)? (EPD_7IN3G_WIDTH / 4 ): (EPD_7IN3G_WIDTH / 4 + 1);
    Height = EPD_7IN3G_HEIGHT;
    
    EPD_7IN3G_SendCommand(0x04);
    EPD_7IN3G_ReadBusyH();

    EPD_7IN3G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_7IN3G_SendData(Image[i + j * Width]);
        }
    }
    EPD_7IN3G_TurnOnDisplay();
}

void EPD_7IN3G_Display_part(UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_height)
{
    UWORD Width, Height;
    Width = (EPD_7IN3G_WIDTH % 4 == 0)? (EPD_7IN3G_WIDTH / 4 ): (EPD_7IN3G_WIDTH / 4 + 1);
    Height = EPD_7IN3G_HEIGHT;
    
    EPD_7IN3G_SendCommand(0x04);
    EPD_7IN3G_ReadBusyH();

    EPD_7IN3G_SendCommand(0x10);
    for (UWORD i = 0; i < Height; i++) {
        for (UWORD j = 0; j < Width; j++) {
					if(i<image_height+ystart && i>=ystart && j<(image_width+xstart)/4 && j>=xstart/4) {
							EPD_7IN3G_SendData(Image[(j-xstart/4) + (image_width/4*(i-ystart))]);
						}
						else
            EPD_7IN3G_SendData(0x55);
        }
    }
    EPD_7IN3G_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN3G_Sleep(void)
{
    EPD_7IN3G_SendCommand(0x02); // POWER_OFF
    EPD_7IN3G_SendData(0X00);
    EPD_7IN3G_SendCommand(0x07); // DEEP_SLEEP
    EPD_7IN3G_SendData(0XA5);
}

