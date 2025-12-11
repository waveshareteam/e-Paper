/*****************************************************************************
* | File      	:	EPD_5in79g.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-07-04
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
#include "EPD_5in79g.h"
#include "Debug.h"
#include <time.h>

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in79g_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_5in79g_SendCommand(UBYTE Reg)
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
static void EPD_5in79g_SendData(UBYTE Data)
{
    
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_5in79g_ReadBus(void)
{
    Debug("e-Paper busy\r\n");
	UBYTE busy;
	do
	{
		busy = DEV_Digital_Read(EPD_BUSY_PIN);
        DEV_Delay_ms(10);   
	}
	while(!busy);   
	DEV_Delay_ms(200);     
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5in79g_TurnOnDisplay(void)
{
    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x00);	
    
    EPD_5in79g_SendCommand(0x12);
    EPD_5in79g_SendData(0x00);
    EPD_5in79g_ReadBus();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_5in79g_Init(void)
{
    EPD_5in79g_Reset();

    //  EPD_5in79g_SendCommand(0x4D);	
    //  EPD_5in79g_SendData(0x78);	

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x01);

    EPD_5in79g_SendCommand(0x00);	//0x00
    EPD_5in79g_SendData(0x03);	
    EPD_5in79g_SendData(0x29);

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x02);	

    EPD_5in79g_SendCommand(0x00);	//0x00
    EPD_5in79g_SendData(0x07);	
    EPD_5in79g_SendData(0x29);

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x00);	

    EPD_5in79g_SendCommand(0x50);	//
    EPD_5in79g_SendData(0x97);	

    EPD_5in79g_SendCommand(0x61); //0x61	
    EPD_5in79g_SendData(0x01);	
    EPD_5in79g_SendData(0x8c);	
    EPD_5in79g_SendData(0x01);	
    EPD_5in79g_SendData(0x10); 	

    EPD_5in79g_SendCommand(0x06);	//0x06
    EPD_5in79g_SendData(0x38);	
    EPD_5in79g_SendData(0x38);
    EPD_5in79g_SendData(0x38);
    EPD_5in79g_SendData(0x00);	//////////////////////////////////////////


    EPD_5in79g_SendCommand(0xE9);	//0xE0
    EPD_5in79g_SendData(0x01);


    EPD_5in79g_SendCommand(0xE0);	//0xE0
    EPD_5in79g_SendData(0x01);

    EPD_5in79g_SendCommand(0x04);
    EPD_5in79g_ReadBus();  //while(1);	


    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in79g_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width =(EPD_5in79G_WIDTH % 8 == 0)?(EPD_5in79G_WIDTH / 8 ):(EPD_5in79G_WIDTH / 8 + 1);
    Height = EPD_5in79G_HEIGHT;

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x02);

    EPD_5in79g_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
                EPD_5in79g_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x01);

    EPD_5in79g_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
                EPD_5in79g_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }


    EPD_5in79g_TurnOnDisplay();
}

void EPD_5in79g_Show(void)
{
    UWORD Width, Height;
    Width =(EPD_5in79G_WIDTH % 8 == 0)?(EPD_5in79G_WIDTH / 8 ):(EPD_5in79G_WIDTH / 8 + 1);
    Height = EPD_5in79G_HEIGHT;
    unsigned char const Color_seven[4] = 
    {0x00, 0x55, 0xAA, 0xFF};

    UBYTE k,o;
    k = 0;
    o = 0;
    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x02);
    EPD_5in79g_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {

        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Color_seven[0]);
        }

        for (UWORD i = 0; i < Width; i++) {
            // EPD_5in79g_SendData(0x55);
            EPD_5in79g_SendData(Color_seven[1]);
        }
    }
    

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x01);
    EPD_5in79g_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Color_seven[2]);
        }

        for (UWORD i = 0; i < Width; i++) {
            // EPD_5in79g_SendData(0x55);
            EPD_5in79g_SendData(Color_seven[3]);
        }
    }


    EPD_5in79g_TurnOnDisplay();
}


/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in79g_Display(const UBYTE *Image)
{
    UWORD Width, Height, Width1;
    Width1 = (EPD_5in79G_WIDTH % 4 == 0)? (EPD_5in79G_WIDTH / 4 ): (EPD_5in79G_WIDTH / 4 + 1);
    Width =(EPD_5in79G_WIDTH % 8 == 0)?(EPD_5in79G_WIDTH / 8 ):(EPD_5in79G_WIDTH / 8 + 1);
    Height = EPD_5in79G_HEIGHT;

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x02);
    EPD_5in79g_SendCommand(0x10);
    
    for (UWORD j = 0; j < Height/2; j++) {

        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Image[i + j * Width1]);
        }

        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Image[i + (Height - j - 1) * Width1]);
        }
    }

    EPD_5in79g_SendCommand(0xA2);	//********************
    EPD_5in79g_SendData(0x01);
    EPD_5in79g_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Image[j * Width1 + i + Width]);
        }

        for (UWORD i = 0; i < Width; i++) {
            EPD_5in79g_SendData(Image[(Height - j - 1) * Width1 + i + Width]);
        }
    }

    EPD_5in79g_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in79g_Sleep(void)
{
    // EPD_5in79g_SendCommand(0x02);  // POWER_OFF
    // EPD_5in79g_SendData(0x00);
    // EPD_5in79g_ReadBus();	

    EPD_5in79g_SendCommand(0x07);
    EPD_5in79g_SendData(0xa5);  
}
