/*****************************************************************************
* | File      	:   EPD_4in2_V2_V2.h
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-02-23
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
#include "EPD_4in2_V2.h"
#include "Debug.h"

UBYTE WF_PARTIAL_4IN2[76] =
{
0X00,0x40,0x00,0x00,0x00,0x00,0x00,	// L0 0-6 ABCD		
0X80,0x80,0x00,0x00,0x00,0x00,0x00,	// L1 0-6 ABCD		
0X40,0x40,0x00,0x00,0x00,0x00,0x00,	// L2 0-6 ABCD		
0X00,0x80,0x00,0x00,0x00,0x00,0x00,	// L3 0-6 ABCD		
0X00,0x00,0x00,0x00,0x00,0x00,0x00,	// VCOM 0-6 ABCD		
0x0A,0x00,0x00,0x00,0x02,//0A 0B 0C 0D R			
0x01,0x00,0x00,0x00,0x00,//1A 0B 0C 0D R			
0x00,0x00,0x00,0x00,0x00,//2A 0B 0C 0D R			
0x00,0x00,0x00,0x00,0x00,//3A 0B 0C 0D R			
0x00,0x00,0x00,0x00,0x00,//4A 0B 0C 0D R			
0x00,0x00,0x00,0x00,0x00,//5A 0B 0C 0D R			
0x00,0x00,0x00,0x00,0x00,//6A 0B 0C 0D R 
0x17,0x41,0x0, 0x32,0x2C,0x0A
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN2_V2_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_4IN2_V2_SendCommand(UBYTE Reg)
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
static void EPD_4IN2_V2_SendData(UBYTE Data)
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
void EPD_4IN2_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(10);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_4IN2_V2_TurnOnDisplay(void)
{
    EPD_4IN2_V2_SendCommand(0x22);
	EPD_4IN2_V2_SendData(0xC7);
    EPD_4IN2_V2_SendCommand(0x20);
    EPD_4IN2_V2_ReadBusy();
}

static void EPD_4IN2_V2_TurnOnDisplay_Partial(void)
{
    EPD_4IN2_V2_SendCommand(0x22);
	EPD_4IN2_V2_SendData(0xCF);
    EPD_4IN2_V2_SendCommand(0x20);
    EPD_4IN2_V2_ReadBusy();
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_4IN2_V2_SetLut(void)
{
    UWORD count;
    EPD_4IN2_V2_SendCommand(0x32);
    for(count=0; count<70; count++) {
        EPD_4IN2_V2_SendData(WF_PARTIAL_4IN2[count]);
    }
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_4IN2_V2_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_4IN2_V2_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData((Xstart>>3) & 0xFF);
    EPD_4IN2_V2_SendData((Xend>>3) & 0xFF);
	
    EPD_4IN2_V2_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_4IN2_V2_SendData(Ystart & 0xFF);
    EPD_4IN2_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_4IN2_V2_SendData(Yend & 0xFF);
    EPD_4IN2_V2_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_4IN2_V2_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_4IN2_V2_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData(Xstart & 0xFF);

    EPD_4IN2_V2_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_4IN2_V2_SendData(Ystart & 0xFF);
    EPD_4IN2_V2_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN2_V2_Init(void)
{
    EPD_4IN2_V2_Reset();

    EPD_4IN2_V2_ReadBusy();   
    EPD_4IN2_V2_SendCommand(0x12);   // soft  reset
    EPD_4IN2_V2_ReadBusy();
	
    EPD_4IN2_V2_SendCommand(0x74);   //	IC analog block control 
    EPD_4IN2_V2_SendData(0x54);
    EPD_4IN2_V2_SendCommand(0x7E);   //	IC digital block control           
    EPD_4IN2_V2_SendData(0x3B);

    EPD_4IN2_V2_SendCommand(0x01);  // drive output control    
    EPD_4IN2_V2_SendData(0x2B);		// Y low byte
    EPD_4IN2_V2_SendData(0x01);		// Y high byte 
    EPD_4IN2_V2_SendData(0x00);
	
    EPD_4IN2_V2_SendCommand(0x11);	// data  entry  mode
    EPD_4IN2_V2_SendData(0x03);		// X-mode   
		
	EPD_4IN2_V2_SetWindows(0, 0, EPD_4IN2_V2_WIDTH-1, EPD_4IN2_V2_HEIGHT-1);
		 
    EPD_4IN2_V2_SendCommand(0x3C);	// Border setting 
    EPD_4IN2_V2_SendData(0x01);
		
    EPD_4IN2_V2_SendCommand(0x18); // use the internal temperature sensor
    EPD_4IN2_V2_SendData(0x80);

    EPD_4IN2_V2_SendCommand(0x22);  
    EPD_4IN2_V2_SendData(0xB1); 
    EPD_4IN2_V2_SendCommand(0x20); 
    EPD_4IN2_V2_ReadBusy();
	 
	EPD_4IN2_V2_SetCursor(0, 0);
	
    EPD_4IN2_V2_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN2_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1);
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(0xFF);
        }
    }

    EPD_4IN2_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(0x00);
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN2_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1);
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(Image[i + j * Width]);
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

// Set Base image for partial refresh
void EPD_4IN2_V2_Display_Base(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1);
    Height = EPD_4IN2_V2_HEIGHT;

    EPD_4IN2_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(Image[i + j * Width]);
        }
    }
    EPD_4IN2_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(Image[i + j * Width]);
        }
    }
    EPD_4IN2_V2_TurnOnDisplay();
}

// Send partial data for partial refresh
void EPD_4IN2_V2_PartialDisplay(UBYTE *Image, UWORD x, UWORD y, UWORD w, UWORD l)
{
    UWORD Width, Height;
    Width = (w % 8 == 0)? (w / 8 ): (w / 8 + 1);
    Height = l;
	
	EPD_4IN2_V2_ReadBusy();
	EPD_4IN2_V2_SetLut();
	
	EPD_4IN2_V2_SendCommand(0x37); 
	EPD_4IN2_V2_SendData(0x00);  
	EPD_4IN2_V2_SendData(0x00);  
	EPD_4IN2_V2_SendData(0x00);  
	EPD_4IN2_V2_SendData(0x00);  
	EPD_4IN2_V2_SendData(0x40);  
	EPD_4IN2_V2_SendData(0x00);  
	EPD_4IN2_V2_SendData(0x00);   

	EPD_4IN2_V2_SendCommand(0x3C); 
	EPD_4IN2_V2_SendData(0x80); 

	EPD_4IN2_V2_SetWindows(x, y, x+w-1, y+l-1);
	EPD_4IN2_V2_SetCursor(x, y);
	
    EPD_4IN2_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_V2_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_4IN2_V2_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN2_V2_Sleep(void)
{
    EPD_4IN2_V2_SendCommand(0x10); // DEEP_SLEEP
    EPD_4IN2_V2_SendData(0x01);
	DEV_Delay_ms(200);
}
