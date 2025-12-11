/*****************************************************************************
* | File      	:  	EPD_10in85g.c
* | Author      :   Waveshare team
* | Function    :   10.85inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-19
* | Info        :
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
#include "EPD_10in85g.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_10in85g_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_10in85g_SendCommand_0(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}
static void EPD_10in85g_SendCommand_1(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}
static void EPD_10in85g_SendCommand_ALL(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/

static void EPD_10in85g_SendData_0(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}
static void EPD_10in85g_SendData_1(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

static void EPD_10in85g_SendnData_0(UBYTE *Data, UDOUBLE Len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_Write_nByte(Data, Len);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}
static void EPD_10in85g_SendnData_1(UBYTE *Data, UDOUBLE Len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_Write_nByte(Data, Len);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

static void EPD_10in85g_SendData_ALL(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_10in85g_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    // DEV_Delay_ms(1000);
	while(1)
	{	 //=1 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==1) 
			break;
		DEV_Delay_ms(10);
	}
	DEV_Delay_ms(10);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_10in85g_TurnOnDisplay(void)
{

	EPD_10in85g_SendCommand_ALL(0x12);
    EPD_10in85g_SendData_ALL(0x00);	
	EPD_10in85g_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_10in85g_Init(void)
{
	EPD_10in85g_Reset();
    EPD_10in85g_ReadBusy();

    EPD_10in85g_SendCommand_ALL(0x4D);
    EPD_10in85g_SendData_ALL(0x78);	

    EPD_10in85g_SendCommand_ALL(0x00);	//0x00
    EPD_10in85g_SendData_ALL(0x2F);	
    EPD_10in85g_SendData_ALL(0x29);	

    EPD_10in85g_SendCommand_ALL(0x06);//47uH
    EPD_10in85g_SendData_ALL(0x0d);
    EPD_10in85g_SendData_ALL(0x12);
    EPD_10in85g_SendData_ALL(0x30);
    EPD_10in85g_SendData_ALL(0x20);
    EPD_10in85g_SendData_ALL(0x19);
    EPD_10in85g_SendData_ALL(0x3D);
    EPD_10in85g_SendData_ALL(0x0C);

    EPD_10in85g_SendCommand_ALL(0x06);	//0x06
    EPD_10in85g_SendData_ALL(0x0d);
    EPD_10in85g_SendData_ALL(0x12);
    EPD_10in85g_SendData_ALL(0x30);
    EPD_10in85g_SendData_ALL(0x20);
    EPD_10in85g_SendData_ALL(0x19);
    EPD_10in85g_SendData_ALL(0x3D);
    EPD_10in85g_SendData_ALL(0x0C);

    EPD_10in85g_SendCommand_ALL(0x50);	//0x50
    EPD_10in85g_SendData_ALL(0x37);	

    EPD_10in85g_SendCommand_ALL(0x61);//0x61	
    EPD_10in85g_SendData_ALL(EPD_10in85g_WIDTH/256);	
    EPD_10in85g_SendData_ALL(EPD_10in85g_WIDTH%256);	
    EPD_10in85g_SendData_ALL(EPD_10in85g_HEIGHT/256);	
    EPD_10in85g_SendData_ALL(EPD_10in85g_HEIGHT%256);	

    EPD_10in85g_SendCommand_ALL(0x65);	//0x65
    EPD_10in85g_SendData_ALL(0x00);	
    EPD_10in85g_SendData_ALL(0x00);	
    EPD_10in85g_SendData_ALL(0x00);	
    EPD_10in85g_SendData_ALL(0x00);	

    EPD_10in85g_SendCommand_ALL(0xE0);
    EPD_10in85g_SendData_ALL(0x01);

    EPD_10in85g_SendCommand_ALL(0xE3);
    EPD_10in85g_SendData_ALL(0x08);  

    EPD_10in85g_SendCommand_ALL(0xE5);
    EPD_10in85g_SendData_ALL(0x08); 

    EPD_10in85g_SendCommand_ALL(0xE9);
    EPD_10in85g_SendData_ALL(0x01);   

    EPD_10in85g_SendCommand_ALL(0x04); //Power on
    EPD_10in85g_ReadBusy();          //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_10in85g_Clear(UBYTE color)
{
	UWORD Width, Height;
    Width = (EPD_10in85g_WIDTH % 4 == 0)? (EPD_10in85g_WIDTH / 4 ): (EPD_10in85g_WIDTH / 4 + 1);
    Height = EPD_10in85g_HEIGHT;
    UBYTE Color = (color << 6) | (color << 4) | (color << 2) | color;

    EPD_10in85g_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_0(Color);
        }
    }	

    EPD_10in85g_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_1(Color);
        }
    }	

	EPD_10in85g_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
	Image : Image data
******************************************************************************/
void EPD_10in85g_Display(const UBYTE *Image)
{
	UWORD Width, Height;
    Width = (EPD_10in85g_WIDTH % 4 == 0)? (EPD_10in85g_WIDTH / 4 ): (EPD_10in85g_WIDTH / 4 + 1);
    Height = EPD_10in85g_HEIGHT;
	
    EPD_10in85g_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_0(Image[j*2*Width + i]);
        }
    }	

    EPD_10in85g_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_1(Image[j*2*Width + i + Width]);
        }
    }

	EPD_10in85g_TurnOnDisplay();	
}

void EPD_10in85g_Display_2(const UBYTE *Image)
{
	UWORD Width, Height;
    Width = (EPD_10in85g_WIDTH % 4 == 0)? (EPD_10in85g_WIDTH / 4 ): (EPD_10in85g_WIDTH / 4 + 1);
    Height = EPD_10in85g_HEIGHT/4;
	
    EPD_10in85g_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_0(Image[j*Width + i]);
        }
    }	
    for (UWORD j = 0; j < Height*3; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_0(0x55);
        }
    }

    EPD_10in85g_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_1(Image[j*Width + i]);
        }
    }
    for (UWORD j = 0; j < Height*3; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_10in85g_SendData_1(0x55);
        }
    }

	EPD_10in85g_TurnOnDisplay();	
}

void EPD_10in85g_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh)
{
    UDOUBLE Width, Height;
    Width = (EPD_10in85g_WIDTH % 4 == 0)? (EPD_10in85g_WIDTH / 4 ): (EPD_10in85g_WIDTH / 4 + 1);
    Height = EPD_10in85g_HEIGHT;
    
    UWORD Xend = ((xstart + image_width)%4 == 0)?((xstart + image_width) / 4 - 1): ((xstart + image_width) / 4 );
    UWORD Yend = ystart + image_heigh-1;
    xstart = xstart / 4;
    
    if(xstart > 170 )
    {
        Xend = Xend - 170;
        xstart = xstart - 170;
        EPD_10in85g_SendCommand_0(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                EPD_10in85g_SendData_0(0x55);
            }
        }

        EPD_10in85g_SendCommand_1(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                if((i<Yend) && (i>=ystart) && (j<Xend) && (j>=xstart)) {
                    EPD_10in85g_SendData_1(Image[(j-xstart) + (image_width/4*(i-ystart))]);
                }
                else
                    EPD_10in85g_SendData_1(0x55);
            }
        }

    }
    else if(Xend < 170 )
    {
        EPD_10in85g_SendCommand_0(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                if((i<Yend) && (i>=ystart) && (j<Xend) && (j>=xstart)) {
                    EPD_10in85g_SendData_0(Image[(j-xstart) + (image_width/4*(i-ystart))]);
                }
                else
                    EPD_10in85g_SendData_0(0x55);
            }
        }
        
        EPD_10in85g_SendCommand_1(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                EPD_10in85g_SendData_1(0x55);
            }
        }
    }
    else
    {
        EPD_10in85g_SendCommand_0(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                if((i<Yend) && (i>=ystart) && (j>=xstart)) {
                    EPD_10in85g_SendData_0(Image[(j-xstart) + (image_width/4*(i-ystart))]);
                }
                else
                    EPD_10in85g_SendData_0(0x55);
            }
        }

        EPD_10in85g_SendCommand_1(0x10);
        for (UDOUBLE i = 0; i < Height; i++) {
            for (UDOUBLE j = 0; j < Width; j++) {
                if((i<Yend) && (i>=ystart) && (j<Xend-170)) {
                    EPD_10in85g_SendData_1(Image[(j+170-xstart) + (image_width/4*(i-ystart))]);
                }
                else
                    EPD_10in85g_SendData_1(0x55);
            }
        }
    }

    EPD_10in85g_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_10in85g_Sleep(void)
{
    EPD_10in85g_SendCommand_ALL(0x02); 
    EPD_10in85g_SendData_ALL(0x00); 
	DEV_Delay_ms(100);  
	EPD_10in85g_ReadBusy();       

	EPD_10in85g_SendCommand_ALL(0X07); //enter deep sleep
	EPD_10in85g_SendData_ALL(0xA5); 
	DEV_Delay_ms(100);
}
