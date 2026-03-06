/*****************************************************************************
* | File      	:   EPD_3in7g.c
* | Author      :   Waveshare team
* | Function    :   3inch7 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-02-26
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
#include "EPD_3in7g.h"
#include "Debug.h"
#include "time.h"


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_3IN7G_Reset(void)
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
static void EPD_3IN7G_SendCommand(UBYTE Reg)
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
static void EPD_3IN7G_SendData(UBYTE Data)
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
void EPD_3IN7G_ReadBusy(void)
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
static void EPD_3IN7G_ReadBusyH(void)
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
static void EPD_3IN7G_TurnOnDisplay(void)
{
    // EPD_3IN7G_SendCommand(0x04);
    // EPD_3IN7G_ReadBusyH();

    EPD_3IN7G_SendCommand(0x12); // DISPLAY_REFRESH
    EPD_3IN7G_SendData(0x00);
    EPD_3IN7G_ReadBusyH();

    // EPD_3IN7G_SendCommand(0x20);
    // EPD_3IN7G_SendData(0x00);
    // EPD_3IN7G_ReadBusyH();

}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN7G_Init(void)
{
    EPD_3IN7G_Reset();
    EPD_3IN7G_ReadBusyH(); 

    EPD_3IN7G_SendCommand(0x00);	//0x00
	EPD_3IN7G_SendData(0x0F);	
	EPD_3IN7G_SendData(0x29);	

	EPD_3IN7G_SendCommand(0x01);	//0x01
	EPD_3IN7G_SendData(0x07);
	EPD_3IN7G_SendData(0x00);
	EPD_3IN7G_SendData(0x22);		//RED 
	EPD_3IN7G_SendData(0x78);
	EPD_3IN7G_SendData(0x0A);		//WHITE
	EPD_3IN7G_SendData(0x22);		//RED 	

	EPD_3IN7G_SendCommand(0x03);	//0x03
	EPD_3IN7G_SendData(0x10);	
	EPD_3IN7G_SendData(0x54);	
	EPD_3IN7G_SendData(0x44);	

	EPD_3IN7G_SendCommand(0x06);	//0x06
	EPD_3IN7G_SendData(0xC0);	
	EPD_3IN7G_SendData(0xC0);	
	EPD_3IN7G_SendData(0xC0);	

	EPD_3IN7G_SendCommand(0x30);	//0x30
	EPD_3IN7G_SendData(0x08);	

	EPD_3IN7G_SendCommand(0x41);	//0x41
	EPD_3IN7G_SendData(0x00);	

	EPD_3IN7G_SendCommand(0x50);	//0x50
	EPD_3IN7G_SendData(0x37);	

	EPD_3IN7G_SendCommand(0x60);	//0x60
	EPD_3IN7G_SendData(0x02);	
	EPD_3IN7G_SendData(0x02);	

	EPD_3IN7G_SendCommand(0x61);//0x61	
	EPD_3IN7G_SendData(EPD_3IN7G_WIDTH/256);	
	EPD_3IN7G_SendData(EPD_3IN7G_WIDTH%256);	
	EPD_3IN7G_SendData(EPD_3IN7G_HEIGHT/256);	
	EPD_3IN7G_SendData(EPD_3IN7G_HEIGHT%256);	

	EPD_3IN7G_SendCommand(0x65);	//0x65
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	


	EPD_3IN7G_SendCommand(0xE7);	//0xE7
	EPD_3IN7G_SendData(0x1C);	

	EPD_3IN7G_SendCommand(0xE3);	//0xE3
	EPD_3IN7G_SendData(0x22);	

	EPD_3IN7G_SendCommand(0xFF);	//Enter
	EPD_3IN7G_SendData(0xA5);	//Enter

	EPD_3IN7G_SendCommand(0xEF);	//PWM
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendData(0x1E);	
	EPD_3IN7G_SendData(0x0A);	
	EPD_3IN7G_SendData(0x1B);	
	EPD_3IN7G_SendData(0x0B);	
	EPD_3IN7G_SendData(0x17);	

	EPD_3IN7G_SendCommand(0xC3);	//PWM
	EPD_3IN7G_SendData(0xFD);	
	EPD_3IN7G_SendCommand(0xDC);	//CPCK
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendCommand(0xDD);	
	EPD_3IN7G_SendData(0x08);	
	EPD_3IN7G_SendCommand(0xDE);	
	EPD_3IN7G_SendData(0x41);	

	EPD_3IN7G_SendCommand(0xFD);	//VDLOS
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendCommand(0xE8);	
	EPD_3IN7G_SendData(0x03);	

	EPD_3IN7G_SendCommand(0xDA);	//DAh
	EPD_3IN7G_SendData(0x07);	

	EPD_3IN7G_SendCommand(0xC9);	//GDR
	EPD_3IN7G_SendData(0x00);	

	EPD_3IN7G_SendCommand(0xA8);	//VDH
	EPD_3IN7G_SendData(0x0F);	

	EPD_3IN7G_SendCommand(0xFF);	
	EPD_3IN7G_SendData(0xE3);	//Exit

	EPD_3IN7G_SendCommand(0xE9);	
	EPD_3IN7G_SendData(0x01);
	
    EPD_3IN7G_SendCommand(0x04); //Power on
    EPD_3IN7G_ReadBusyH();          //waiting for the electronic paper IC to release the idle signal

	EPD_3IN7G_SendCommand(0xFF);
	EPD_3IN7G_SendData(0xA5);

	EPD_3IN7G_SendCommand(0xEF);   
	EPD_3IN7G_SendData(0x03);
	EPD_3IN7G_SendData(0x1E);
	EPD_3IN7G_SendData(0x0A);
	EPD_3IN7G_SendData(0x1B);
	EPD_3IN7G_SendData(0x0E);
	EPD_3IN7G_SendData(0x15);

	EPD_3IN7G_SendCommand(0xDC); 	 
	EPD_3IN7G_SendData(0x01);

	EPD_3IN7G_SendCommand(0xDD); 	 
	EPD_3IN7G_SendData(0x08);

	EPD_3IN7G_SendCommand(0xDE);
	EPD_3IN7G_SendData(0x41);

	EPD_3IN7G_SendCommand(0xFF);
	EPD_3IN7G_SendData(0xE3);
}



void EPD_3IN7G_Init_Fast(void)
{
	EPD_3IN7G_Reset();
	EPD_3IN7G_ReadBusyH();          //waiting for the electronic paper IC to release the idle signal
	
	EPD_3IN7G_SendCommand(0x00);	//0x00
	EPD_3IN7G_SendData(0x0F);	
	EPD_3IN7G_SendData(0x29);	

	EPD_3IN7G_SendCommand(0x01);	//0x01
	EPD_3IN7G_SendData(0x07);
	EPD_3IN7G_SendData(0x00);
	EPD_3IN7G_SendData(0x22);		//RED 
	EPD_3IN7G_SendData(0x78);
	EPD_3IN7G_SendData(0x0A);		//WHITE
	EPD_3IN7G_SendData(0x22);		//RED 	

	EPD_3IN7G_SendCommand(0x03);	//0x03
	EPD_3IN7G_SendData(0x10);	
	EPD_3IN7G_SendData(0x54);	
	EPD_3IN7G_SendData(0x44);	

	EPD_3IN7G_SendCommand(0x06);	//0x06
	EPD_3IN7G_SendData(0xC0);	
	EPD_3IN7G_SendData(0xC0);	
	EPD_3IN7G_SendData(0xC0);	

	EPD_3IN7G_SendCommand(0x30);	//0x30
	EPD_3IN7G_SendData(0x08);	

	EPD_3IN7G_SendCommand(0x41);	//0x41
	EPD_3IN7G_SendData(0x00);	

	EPD_3IN7G_SendCommand(0x50);	//0x50
	EPD_3IN7G_SendData(0x37);	

	EPD_3IN7G_SendCommand(0x60);	//0x60
	EPD_3IN7G_SendData(0x02);	
	EPD_3IN7G_SendData(0x02);	

	EPD_3IN7G_SendCommand(0x61);//0x61	
	EPD_3IN7G_SendData(EPD_3IN7G_WIDTH/256);	
	EPD_3IN7G_SendData(EPD_3IN7G_WIDTH%256);	
	EPD_3IN7G_SendData(EPD_3IN7G_HEIGHT/256);	
	EPD_3IN7G_SendData(EPD_3IN7G_HEIGHT%256);	

	EPD_3IN7G_SendCommand(0x65);	//0x65
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	
	EPD_3IN7G_SendData(0x00);	


	EPD_3IN7G_SendCommand(0xE7);	//0xE7
	EPD_3IN7G_SendData(0x1C);	

	EPD_3IN7G_SendCommand(0xE3);	//0xE3
	EPD_3IN7G_SendData(0x22);	

	EPD_3IN7G_SendCommand(0xFF);	//Enter
	EPD_3IN7G_SendData(0xA5);	//Enter

	EPD_3IN7G_SendCommand(0xEF);	//PWM
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendData(0x1E);	
	EPD_3IN7G_SendData(0x0A);	
	EPD_3IN7G_SendData(0x1B);	
	EPD_3IN7G_SendData(0x0B);	
	EPD_3IN7G_SendData(0x17);	

	EPD_3IN7G_SendCommand(0xC3);	//PWM
	EPD_3IN7G_SendData(0xFD);	
	EPD_3IN7G_SendCommand(0xDC);	//CPCK
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendCommand(0xDD);	
	EPD_3IN7G_SendData(0x08);	
	EPD_3IN7G_SendCommand(0xDE);	
	EPD_3IN7G_SendData(0x41);	

	EPD_3IN7G_SendCommand(0xFD);	//VDLOS
	EPD_3IN7G_SendData(0x01);	
	EPD_3IN7G_SendCommand(0xE8);	
	EPD_3IN7G_SendData(0x03);	

	EPD_3IN7G_SendCommand(0xDA);	//DAh
	EPD_3IN7G_SendData(0x07);	

	EPD_3IN7G_SendCommand(0xC9);	//GDR
	EPD_3IN7G_SendData(0x00);	

	EPD_3IN7G_SendCommand(0xA8);	//VDH
	EPD_3IN7G_SendData(0x0F);	

	EPD_3IN7G_SendCommand(0xFF);	
	EPD_3IN7G_SendData(0xE3);	//Exit

	EPD_3IN7G_SendCommand(0xE9);	
	EPD_3IN7G_SendData(0x01);
	
    EPD_3IN7G_SendCommand(0x04); //Power on
    EPD_3IN7G_ReadBusyH();          //waiting for the electronic paper IC to release the idle signal

	EPD_3IN7G_SendCommand(0xFF);
	EPD_3IN7G_SendData(0xA5);

	EPD_3IN7G_SendCommand(0xEF);   
	EPD_3IN7G_SendData(0x03);
	EPD_3IN7G_SendData(0x1E);
	EPD_3IN7G_SendData(0x0A);
	EPD_3IN7G_SendData(0x1B);
	EPD_3IN7G_SendData(0x0E);
	EPD_3IN7G_SendData(0x15);

	EPD_3IN7G_SendCommand(0xDC); 	 
	EPD_3IN7G_SendData(0x01);

	EPD_3IN7G_SendCommand(0xDD); 	 
	EPD_3IN7G_SendData(0x08);

	EPD_3IN7G_SendCommand(0xDE);
	EPD_3IN7G_SendData(0x41);

	EPD_3IN7G_SendCommand(0xFF);
	EPD_3IN7G_SendData(0xE3);


  //Fast
	EPD_3IN7G_SendCommand(0xE0);
	EPD_3IN7G_SendData(0x02);
		
	EPD_3IN7G_SendCommand(0xE6);
	EPD_3IN7G_SendData(0x5B);
	 
	EPD_3IN7G_SendCommand(0xA5);
	EPD_3IN7G_SendData(0x00);
	EPD_3IN7G_ReadBusyH(); 
}


/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_3IN7G_Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_3IN7G_WIDTH % 4 == 0)? (EPD_3IN7G_WIDTH / 4 ): (EPD_3IN7G_WIDTH / 4 + 1);
    Height = EPD_3IN7G_HEIGHT;

    EPD_3IN7G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7G_SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    EPD_3IN7G_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN7G_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_3IN7G_WIDTH % 4 == 0)? (EPD_3IN7G_WIDTH / 4 ): (EPD_3IN7G_WIDTH / 4 + 1);
    Height = EPD_3IN7G_HEIGHT;

    EPD_3IN7G_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7G_SendData(Image[i + j * Width]);
        }
    }

    EPD_3IN7G_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN7G_Sleep(void)
{
    EPD_3IN7G_SendCommand(0x02); // POWER_OFF
    EPD_3IN7G_SendData(0X00);
    EPD_3IN7G_ReadBusyH();
    EPD_3IN7G_SendCommand(0x07); // DEEP_SLEEP
    EPD_3IN7G_SendData(0XA5);
}
