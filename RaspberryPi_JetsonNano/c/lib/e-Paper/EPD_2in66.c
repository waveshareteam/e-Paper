/*****************************************************************************
* | File      	:  	EPD_2in66.c
* | Author      :   Waveshare team
* | Function    :   2.66inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-21
* | Info        :
* -----------------------------------------------------------------------------
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
#include "EPD_2in66.h"
#include "Debug.h"

const unsigned char WF_PARTIAL[159] ={
0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x40,0x40,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x0A,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x22,0x22,0x22,
0x00,0x00,0x00,0x22,0x17,0x41,0xB0,0x32,0x36,
};


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN66_Reset(void)
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
static void EPD_2IN66_SendCommand(UBYTE Reg)
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
static void EPD_2IN66_SendData(UBYTE Data)
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
static void EPD_2IN66_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    DEV_Delay_ms(200);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(5);
    }
    DEV_Delay_ms(100);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Send LUT
parameter:
******************************************************************************/
static void EPD_2IN66_SetLUA(void)
{
    UWORD count;
    EPD_2IN66_SendCommand(0x32);
    for(count=0;count<153;count++){
        EPD_2IN66_SendData(WF_PARTIAL[count]);
    }    
    EPD_2IN66_ReadBusy();
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN66_TurnOnDisplay(void)
{
    EPD_2IN66_SendCommand(0x20);
    EPD_2IN66_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN66_Init(void)
{
    EPD_2IN66_Reset();
    EPD_2IN66_ReadBusy();
    EPD_2IN66_SendCommand(0x12);//soft  reset
    EPD_2IN66_ReadBusy();
	
	/* Y increment, X increment */
    EPD_2IN66_SendCommand(0x11);
    EPD_2IN66_SendData(0x03);
    /*	Set RamX-address Start/End position	*/
    EPD_2IN66_SendCommand(0x44);
    EPD_2IN66_SendData(0x01); 
    EPD_2IN66_SendData((EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1) );
    /*	Set RamY-address Start/End position	*/
    EPD_2IN66_SendCommand(0x45);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0xff));
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0x100)>>8);

    EPD_2IN66_SendCommand(0x3C);        // Border  设置  黑白一般设置为跟随白波形即 0x01        Border setting 
    EPD_2IN66_SendData(0x01);
	
}

/******************************************************************************
function :	Initialize the e-Paper register (Partial display)
parameter:
******************************************************************************/
void EPD_2IN66_Init_partial(void)
{
    EPD_2IN66_Reset();
    EPD_2IN66_ReadBusy();
    EPD_2IN66_SendCommand(0x12);//soft  reset
    EPD_2IN66_ReadBusy();
   
    EPD_2IN66_SetLUA();
    EPD_2IN66_SendCommand(0x37); 
    EPD_2IN66_SendData(0x00);  
    EPD_2IN66_SendData(0x00);  
    EPD_2IN66_SendData(0x00);  
    EPD_2IN66_SendData(0x00); 
    EPD_2IN66_SendData(0x00);  	
    EPD_2IN66_SendData(0x40);  
    EPD_2IN66_SendData(0x00);  
    EPD_2IN66_SendData(0x00);   
    EPD_2IN66_SendData(0x00);  
    EPD_2IN66_SendData(0x00);

	/* Y increment, X increment */
    EPD_2IN66_SendCommand(0x11);
    EPD_2IN66_SendData(0x03);
    /*	Set RamX-address Start/End position	*/
    EPD_2IN66_SendCommand(0x44);
    EPD_2IN66_SendData(0x01); 
    EPD_2IN66_SendData((EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1) );
    /*	Set RamY-address Start/End position	*/
    EPD_2IN66_SendCommand(0x45);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData(0);
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0xff));
    EPD_2IN66_SendData((EPD_2IN66_HEIGHT&0x100)>>8);

    EPD_2IN66_SendCommand(0x3C); 
    EPD_2IN66_SendData(0x80);   

    EPD_2IN66_SendCommand(0x22); 
    EPD_2IN66_SendData(0xcf); 
    EPD_2IN66_SendCommand(0x20); 
    EPD_2IN66_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN66_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1);
    Height = EPD_2IN66_HEIGHT;
    
    EPD_2IN66_SendCommand(0x4E);     
    EPD_2IN66_SendData(0x01);

    EPD_2IN66_SendCommand(0x4F);       
    EPD_2IN66_SendData(0x27);
    EPD_2IN66_SendData(0x01);
    
    EPD_2IN66_ReadBusy();
    EPD_2IN66_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66_SendData(Image[i + j * Width]);
        }
    }

    EPD_2IN66_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN66_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN66_WIDTH % 8 == 0)? (EPD_2IN66_WIDTH / 8 ): (EPD_2IN66_WIDTH / 8 + 1);
    Height = EPD_2IN66_HEIGHT;

    EPD_2IN66_SendCommand(0x4E);     
    EPD_2IN66_SendData(0x01);

    EPD_2IN66_SendCommand(0x4F);       
    EPD_2IN66_SendData(0x27);
    EPD_2IN66_SendData(0x01);
    
    EPD_2IN66_ReadBusy();
    EPD_2IN66_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66_SendData(0xff);
        }
    }
	EPD_2IN66_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66_SendData(0xff);
        }
    }
    EPD_2IN66_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN66_Sleep(void)
{
    EPD_2IN66_SendCommand(0x10);
    EPD_2IN66_SendData(0x01); 
}
