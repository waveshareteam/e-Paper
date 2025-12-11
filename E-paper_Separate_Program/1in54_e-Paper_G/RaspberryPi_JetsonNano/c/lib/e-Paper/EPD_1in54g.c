/*****************************************************************************
* | File      	:   EPD_1in54g.c
* | Author      :   Waveshare team
* | Function    :   1inch54 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
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
#include "EPD_1in54g.h"
#include "Debug.h"
#include "time.h"


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN54G_Reset(void)
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
static void EPD_1IN54G_SendCommand(UBYTE Reg)
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
static void EPD_1IN54G_SendData(UBYTE Data)
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
void EPD_1IN54G_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    DEV_Delay_ms(100);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_1IN54G_ReadBusyH(void)
{
    Debug("e-Paper busy H\r\n");
    DEV_Delay_ms(100);
    while(!DEV_Digital_Read(EPD_BUSY_PIN)) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(5);
    }
    Debug("e-Paper busy H release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_1IN54G_TurnOnDisplay(void)
{
    // EPD_1IN54G_SendCommand(0x04);
    // EPD_1IN54G_ReadBusyH();


    EPD_1IN54G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_1IN54G_SendData(0x00);
    EPD_1IN54G_ReadBusyH();

    // EPD_1IN54G_SendCommand(0x20);
    // EPD_1IN54G_SendData(0x00);
    // EPD_1IN54G_ReadBusyH();

}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN54G_Init(void)
{
    EPD_1IN54G_Reset();

    EPD_1IN54G_SendCommand(0x4D);
    EPD_1IN54G_SendData(0x78);

    EPD_1IN54G_SendCommand(0x00);	//PSR
    EPD_1IN54G_SendData(0x0F);
    EPD_1IN54G_SendData(0x29);

    EPD_1IN54G_SendCommand(0X06); //BTST_P
    EPD_1IN54G_SendData(0x0D);
    EPD_1IN54G_SendData(0x12);
    EPD_1IN54G_SendData(0x30);
    EPD_1IN54G_SendData(0x20);
    EPD_1IN54G_SendData(0x19);
    EPD_1IN54G_SendData(0x2A);
    EPD_1IN54G_SendData(0x22);

    EPD_1IN54G_SendCommand(0x50);	//CDI
    EPD_1IN54G_SendData(0x37);

    EPD_1IN54G_SendCommand(0x61); //TRES
    EPD_1IN54G_SendData(EPD_1IN54G_WIDTH/256);
    EPD_1IN54G_SendData(EPD_1IN54G_WIDTH%256);
    EPD_1IN54G_SendData(EPD_1IN54G_HEIGHT/256);	
    EPD_1IN54G_SendData(EPD_1IN54G_HEIGHT%256); 

    EPD_1IN54G_SendCommand(0xE9);
    EPD_1IN54G_SendData(0x01); 

    EPD_1IN54G_SendCommand(0x30);
    EPD_1IN54G_SendData(0x08); 

    EPD_1IN54G_SendCommand(0x04);
    EPD_1IN54G_ReadBusyH();
}

void EPD_1IN54G_Init_Fast(void)
{
    EPD_1IN54G_Reset();

    EPD_1IN54G_SendCommand(0x4D);
    EPD_1IN54G_SendData(0x78);

    EPD_1IN54G_SendCommand(0x00);	//PSR
    EPD_1IN54G_SendData(0x0F);
    EPD_1IN54G_SendData(0x29);

    EPD_1IN54G_SendCommand(0X06); //BTST_P
    EPD_1IN54G_SendData(0x0D); 
    EPD_1IN54G_SendData(0x12);
    EPD_1IN54G_SendData(0x30);
    EPD_1IN54G_SendData(0x20);
    EPD_1IN54G_SendData(0x19);
    EPD_1IN54G_SendData(0x2A);
    EPD_1IN54G_SendData(0x22);

    EPD_1IN54G_SendCommand(0x50);	//CDI
    EPD_1IN54G_SendData(0x37);

    EPD_1IN54G_SendCommand(0x61); //TRES
    EPD_1IN54G_SendData(EPD_1IN54G_WIDTH/256);		
    EPD_1IN54G_SendData(EPD_1IN54G_WIDTH%256);	
    EPD_1IN54G_SendData(EPD_1IN54G_HEIGHT/256);	
    EPD_1IN54G_SendData(EPD_1IN54G_HEIGHT%256); 

    EPD_1IN54G_SendCommand(0xE9);
    EPD_1IN54G_SendData(0x01); 

    EPD_1IN54G_SendCommand(0x30);
    EPD_1IN54G_SendData(0x08);  

    EPD_1IN54G_SendCommand(0x04);
    EPD_1IN54G_ReadBusyH(); 

    EPD_1IN54G_SendCommand(0xE0);
    EPD_1IN54G_SendData(0x02);

    EPD_1IN54G_SendCommand(0xE6);
    EPD_1IN54G_SendData(0x5D);

    EPD_1IN54G_SendCommand(0xA5);
    EPD_1IN54G_SendData(0x00);
    EPD_1IN54G_ReadBusyH();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN54G_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_1IN54G_WIDTH % 4 == 0)? (EPD_1IN54G_WIDTH / 4 ): (EPD_1IN54G_WIDTH / 4 + 1);
    Height = EPD_1IN54G_HEIGHT;

    EPD_1IN54G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_1IN54G_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54G_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_1IN54G_WIDTH % 4 == 0)? (EPD_1IN54G_WIDTH / 4 ): (EPD_1IN54G_WIDTH / 4 + 1);
    Height = EPD_1IN54G_HEIGHT;

    EPD_1IN54G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54G_SendData(Image[i + j * Width]);
        }
    }

    EPD_1IN54G_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN54G_Sleep(void)
{
    EPD_1IN54G_SendCommand(0x02); // POWER_OFF
    EPD_1IN54G_SendData(0X00);
    EPD_1IN54G_ReadBusyH();
    EPD_1IN54G_SendCommand(0x07); // DEEP_SLEEP
    EPD_1IN54G_SendData(0XA5);
}
