/*****************************************************************************
* | File      	:	EPD_7in5_HD.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-25
* | Info        :
*****************************************************************************
#
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
#include "EPD_7in5_HD.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN5_HD_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function : send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_7IN5_HD_SendCommand(UBYTE Reg)
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
static void EPD_7IN5_HD_SendData(UBYTE Data)
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
static void EPD_7IN5_HD_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
    do{
        DEV_Delay_ms(10);
    }while(DEV_Digital_Read(EPD_BUSY_PIN) == 1);   
    DEV_Delay_ms(200);      
    Debug("e-Paper busy release\r\n");
    
}


/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_7IN5_HD_Init(void)
{
    EPD_7IN5_HD_Reset();
    
    EPD_7IN5_HD_WaitUntilIdle();
    EPD_7IN5_HD_SendCommand(0x12);  //SWRESET
    EPD_7IN5_HD_WaitUntilIdle();
    
    EPD_7IN5_HD_SendCommand(0x46);  // Auto Write Red RAM
    EPD_7IN5_HD_SendData(0xf7);
    EPD_7IN5_HD_WaitUntilIdle();
    EPD_7IN5_HD_SendCommand(0x47);  // Auto Write  B/W RAM
    EPD_7IN5_HD_SendData(0xf7);
    EPD_7IN5_HD_WaitUntilIdle();


    EPD_7IN5_HD_SendCommand(0x0C);  // Soft start setting
    EPD_7IN5_HD_SendData(0xAE);
    EPD_7IN5_HD_SendData(0xC7);
    EPD_7IN5_HD_SendData(0xC3);
    EPD_7IN5_HD_SendData(0xC0);
    EPD_7IN5_HD_SendData(0x40); 


    EPD_7IN5_HD_SendCommand(0x01);  // Set MUX as 527
    EPD_7IN5_HD_SendData(0xAF);
    EPD_7IN5_HD_SendData(0x02);
    EPD_7IN5_HD_SendData(0x01);//0x01


    EPD_7IN5_HD_SendCommand(0x11);  // Data entry mode      
    EPD_7IN5_HD_SendData(0x01);

    EPD_7IN5_HD_SendCommand(0x44); 
    EPD_7IN5_HD_SendData(0x00); // RAM x address start at 0
    EPD_7IN5_HD_SendData(0x00); 
    EPD_7IN5_HD_SendData(0x6F); 
    EPD_7IN5_HD_SendData(0x03); 
    EPD_7IN5_HD_SendCommand(0x45); 
    EPD_7IN5_HD_SendData(0xAF); 
    EPD_7IN5_HD_SendData(0x02);
    EPD_7IN5_HD_SendData(0x00); 
    EPD_7IN5_HD_SendData(0x00);


    EPD_7IN5_HD_SendCommand(0x3C); // VBD
    EPD_7IN5_HD_SendData(0x05); // LUT1, for white

    EPD_7IN5_HD_SendCommand(0x18);
    EPD_7IN5_HD_SendData(0X80);


    EPD_7IN5_HD_SendCommand(0x22);
    EPD_7IN5_HD_SendData(0XB1); //Load Temperature and waveform setting.
    EPD_7IN5_HD_SendCommand(0x20);
    EPD_7IN5_HD_WaitUntilIdle();
    
    EPD_7IN5_HD_SendCommand(0x4E); // set RAM x address count to 0;
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendCommand(0x4F); 
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
    // return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5_HD_Clear(void)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5_HD_WIDTH % 8 == 0)?(EPD_7IN5_HD_WIDTH / 8 ):(EPD_7IN5_HD_WIDTH / 8 + 1);
    Height = EPD_7IN5_HD_HEIGHT;
    
    EPD_7IN5_HD_SendCommand(0x4F); 
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendCommand(0x24);
    UDOUBLE i;
    for(i=0; i<58080; i++) {
        EPD_7IN5_HD_SendData(0xff);
    }
 
    EPD_7IN5_HD_SendCommand(0x26);
    for(i=0; i<Height*Width; i++){
        EPD_7IN5_HD_SendData(0xff);
    }
    EPD_7IN5_HD_SendCommand(0x22);
    EPD_7IN5_HD_SendData(0xF7);//Load LUT from MCU(0x32)
    EPD_7IN5_HD_SendCommand(0x20);
    DEV_Delay_ms(10);
    EPD_7IN5_HD_WaitUntilIdle();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5_HD_Display(const UBYTE *blackimage)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5_HD_WIDTH % 8 == 0)?(EPD_7IN5_HD_WIDTH / 8 ):(EPD_7IN5_HD_WIDTH / 8 + 1);
    Height = EPD_7IN5_HD_HEIGHT;
    
    EPD_7IN5_HD_SendCommand(0x4F); 
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendCommand(0x24);

    UDOUBLE i;
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_7IN5_HD_SendData(blackimage[i + j * Width]);
        }
    }
    
    EPD_7IN5_HD_SendCommand(0x26);
    for(i=0; i<Height*Width; i++)	{
        EPD_7IN5_HD_SendData(0xff);
    }
    EPD_7IN5_HD_SendCommand(0x22);
    EPD_7IN5_HD_SendData(0xF7);//Load LUT from MCU(0x32)
    EPD_7IN5_HD_SendCommand(0x20);
    DEV_Delay_ms(10);
    EPD_7IN5_HD_WaitUntilIdle();
}
void EPD_7IN5_HD_WritePicture(const UBYTE *blackimage, UBYTE Block)
{
	UDOUBLE Width, Height;
  Width =(EPD_7IN5_HD_WIDTH % 8 == 0)?(EPD_7IN5_HD_WIDTH / 8 ):(EPD_7IN5_HD_WIDTH / 8 + 1);
  Height = EPD_7IN5_HD_HEIGHT;
	if(Block == 0){
	  EPD_7IN5_HD_SendCommand(0x4F); 
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height/2; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_7IN5_HD_SendData(blackimage[i + j * Width]);
        }
    }
	}else if(Block == 1){
		for (UDOUBLE j = 0; j < Height/2; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_7IN5_HD_SendData(blackimage[i + j * Width]);
        }
    }
		EPD_7IN5_HD_SendCommand(0x22);
    EPD_7IN5_HD_SendData(0xF7);//Load LUT from MCU(0x32)
    EPD_7IN5_HD_SendCommand(0x20);
    DEV_Delay_ms(10);
    EPD_7IN5_HD_WaitUntilIdle();
	}
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:	image_width and start_X :Must be a multiple of 8
******************************************************************************/
void EPD_7IN5_HD_DisplayImage(const UBYTE *blackimage,UDOUBLE start_X, UDOUBLE start_Y, UDOUBLE image_width, UDOUBLE image_high)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5_HD_WIDTH % 8 == 0)?(EPD_7IN5_HD_WIDTH / 8 ):(EPD_7IN5_HD_WIDTH / 8 + 1);
    Height = EPD_7IN5_HD_HEIGHT;
		
	  EPD_7IN5_HD_SendCommand(0x4F); 
    EPD_7IN5_HD_SendData(0x00);
    EPD_7IN5_HD_SendData(0x00);
		//send black data
	
		
    EPD_7IN5_HD_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
						if((i>=start_X/8) && (i<(start_X+image_width)/8) && (j>=start_Y) && (j<(start_Y+image_high))){
							EPD_7IN5_HD_SendData(~blackimage[i-start_X/8 + (j - start_Y) * image_width/8]);
						}else{
							EPD_7IN5_HD_SendData(0xff);
						}
						
        }
    }
		EPD_7IN5_HD_SendCommand(0x22);
    EPD_7IN5_HD_SendData(0xF7);//Load LUT from MCU(0x32)
    EPD_7IN5_HD_SendCommand(0x20);
    DEV_Delay_ms(10);
    EPD_7IN5_HD_WaitUntilIdle();
}
/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5_HD_Sleep(void)
{
    EPD_7IN5_HD_SendCommand(0x10);
    EPD_7IN5_HD_SendData(0x01);
}
