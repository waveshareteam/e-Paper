/*****************************************************************************
* | File      	:	EPD_7in5.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-12-18
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
#include "EPD_7in5_V2_old.h"
#include "Debug.h"
#include <time.h>

UBYTE Voltage_Frame_7IN5_V2[]={
	0x6, 0x3F, 0x3F, 0x11, 0x24, 0x7, 0x17,
};

UBYTE LUT_VCOM_7IN5_V2[]={	
	0x0,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x0,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};						

UBYTE LUT_WW_7IN5_V2[]={	
	0x10,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x20,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

UBYTE LUT_BW_7IN5_V2[]={	
	0x10,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x20,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

UBYTE LUT_WB_7IN5_V2[]={	
	0x80,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x40,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

UBYTE LUT_BB_7IN5_V2[]={	
	0x80,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x84,	0xF,	0x1,	0xF,	0x1,	0x2,	
	0x40,	0xF,	0xF,	0x0,	0x0,	0x1,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	
};

const unsigned char						
Lut_all_fresh[]={0x67,	0xBF,	0x3F,	0x0D,	0x00,	0x1C,
//VCOM					
0x00,	0x32,	0x32,	0x00,	0x00,	0x01,
0x00,	0x0A,	0x0A,	0x00,	0x00,	0x00,
0x00,	0x28,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//WW					
0x60,	0x32,	0x32,	0x00,	0x00,	0x01,
0x60,	0x0A,	0x0A,	0x00,	0x00,	0x00,
0x80,	0x28,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//BW					
0x60,	0x32,	0x32,	0x00,	0x00,	0x01,
0x60,	0x0A,	0x0A,	0x00,	0x00,	0x00,
0x80,	0x28,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//WB					
0x90,	0x32,	0x32,	0x00,	0x00,	0x01,
0x60,	0x0A,	0x0A,	0x00,	0x00,	0x00,
0x40,	0x28,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//BB					
0x90,	0x32,	0x32,	0x00,	0x00,	0x01,
0x60,	0x0A,	0x0A,	0x00,	0x00,	0x00,
0x40,	0x28,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//Reserved					
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,					
};					

			
					
const unsigned char						
Lut_partial[]={0x67,	0xBF,	0x3F,	0x0D,	0x00,	0x1C,
//VCOM					
0x00,	0x14,	0x02,	0x00,	0x00,	0x01,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//WW					
0x20,	0x14,	0x02,	0x00,	0x00,	0x01,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//BW					
0x80,	0x14,	0x02,	0x00,	0x00,	0x01,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//WB					
0x40,	0x14,	0x02,	0x00,	0x00,	0x01,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//BB					
0x00,	0x14,	0x02,	0x00,	0x00,	0x01,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
//Reserved					
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,	0xFF,	0xFF,	0xFF,	0xFF,	0xFF,
0xFF,					
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_Reset(void)
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
static void EPD_SendCommand(UBYTE Reg)
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
static void EPD_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_SendData2(UBYTE *pData, UDOUBLE len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_Write_nByte(pData, len);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
	do{
		DEV_Delay_ms(5);  
	}while(!(DEV_Digital_Read(EPD_BUSY_PIN)));   
	DEV_Delay_ms(5);      
    Debug("e-Paper busy release\r\n");
}

static void EPD_7IN5_V2_LUT(UBYTE* lut_vcom,  UBYTE* lut_ww, UBYTE* lut_bw, UBYTE* lut_wb, UBYTE* lut_bb)
{
	UBYTE count;

	EPD_SendCommand(0x20); //VCOM	
	for(count=0; count<42; count++)
		EPD_SendData(lut_vcom[count]);

	EPD_SendCommand(0x21); //LUTBW
	for(count=0; count<42; count++)
		EPD_SendData(lut_ww[count]);

	EPD_SendCommand(0x22); //LUTBW
	for(count=0; count<42; count++)
		EPD_SendData(lut_bw[count]);

	EPD_SendCommand(0x23); //LUTWB
	for(count=0; count<42; count++)
		EPD_SendData(lut_wb[count]);

	EPD_SendCommand(0x24); //LUTBB
	for(count=0; count<42; count++)
		EPD_SendData(lut_bb[count]);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5_V2_TurnOnDisplay(void)
{	
    EPD_SendCommand(0x12);			//DISPLAY REFRESH
    DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!
    EPD_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_7IN5_V2_Init(void)
{
    EPD_Reset();

    // EPD_SendCommand(0x01);			//POWER SETTING
    // EPD_SendData(0x07);
    // EPD_SendData(0x07);		//VGH=20V,VGL=-20V
    // EPD_SendData(0x3f);		//VDH=15V
    // EPD_SendData(0x3f);		//VDL=-15V

	EPD_SendCommand(0x01);  // power setting
	EPD_SendData(0x17);  // 1-0=11: internal power
	EPD_SendData(*(Voltage_Frame_7IN5_V2+6));  // VGH&VGL
	EPD_SendData(*(Voltage_Frame_7IN5_V2+1));  // VSH
	EPD_SendData(*(Voltage_Frame_7IN5_V2+2));  //  VSL
	EPD_SendData(*(Voltage_Frame_7IN5_V2+3));  //  VSHR
	
	EPD_SendCommand(0x82);  // VCOM DC Setting
	EPD_SendData(*(Voltage_Frame_7IN5_V2+4));  // VCOM

	EPD_SendCommand(0x06);  // Booster Setting
	EPD_SendData(0x27);
	EPD_SendData(0x27);
	EPD_SendData(0x2F);
	EPD_SendData(0x17);
	
	EPD_SendCommand(0x30);   // OSC Setting
	EPD_SendData(*(Voltage_Frame_7IN5_V2+0));  // 2-0=100: N=4  ; 5-3=111: M=7  ;  3C=50Hz     3A=100HZ

    EPD_SendCommand(0x04); //POWER ON
    DEV_Delay_ms(100);
    EPD_WaitUntilIdle();

    EPD_SendCommand(0X00);			//PANNEL SETTING
    EPD_SendData(0x3F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_SendCommand(0x61);        	//tres
    EPD_SendData(0x03);		//source 800
    EPD_SendData(0x20);
    EPD_SendData(0x01);		//gate 480
    EPD_SendData(0xE0);

    EPD_SendCommand(0X15);
    EPD_SendData(0x00);

    EPD_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    EPD_SendData(0x10);
    EPD_SendData(0x00);

    EPD_SendCommand(0X60);			//TCON SETTING
    EPD_SendData(0x22);

    EPD_SendCommand(0x65);  // Resolution setting
    EPD_SendData(0x00);
    EPD_SendData(0x00);//800*480
    EPD_SendData(0x00);
    EPD_SendData(0x00);
	
	EPD_7IN5_V2_LUT(LUT_VCOM_7IN5_V2, LUT_WW_7IN5_V2, LUT_BW_7IN5_V2, LUT_WB_7IN5_V2, LUT_BB_7IN5_V2);
	
    return 0;
}

void Epaper_LUT_By_MCU( UBYTE* wavedata)
{
    UBYTE count;
    UBYTE VCEND,BDEND,EVS,XON,PLL;

    VCEND=wavedata[0]&0x08;
    BDEND=(wavedata[1]&0xC0)>>6;
    EVS=VCEND|BDEND;
    PLL=(wavedata[0]&0xF0)>>4;
    XON=wavedata[2]&0xC0;

    EPD_SendCommand(0x52);	      //EVS
    EPD_SendData(EVS);

    EPD_SendCommand(0x30);			  //PLL setting 
    EPD_SendData(PLL);

    EPD_SendCommand(0x01);       //Set VGH VGL VSH VSL VSHR
    EPD_SendData (0x17);
    EPD_SendData ((*wavedata++)&0x07);  	//VGH/VGL Voltage Level selection
    EPD_SendData ((*wavedata++)&0x3F);		//VSH for black
    EPD_SendData ((*wavedata++)&0x3F);  	//VSL for white
    EPD_SendData ((*wavedata++)&0x3F);  	//VSHR for red

    EPD_SendCommand(0x2A);	      //LUTOPT
    EPD_SendData(XON);
    EPD_SendData(*wavedata++);

    EPD_SendCommand(0x82);       //VCOM_DC setting
    EPD_SendData (*wavedata++);  //Vcom value


    EPD_SendCommand(0x20);
    for(count=0;count<42;count++)
        EPD_SendData(*wavedata++);

    EPD_SendCommand(0x21);	
    for(count=0;count<42;count++)
        EPD_SendData(*wavedata++);

    EPD_SendCommand(0x22);	
    for(count=0;count<42;count++)
        EPD_SendData(*wavedata++);

    EPD_SendCommand(0x23);	
    for(count=0;count<42;count++)
        EPD_SendData(*wavedata++);

    EPD_SendCommand(0x24);	
    for(count=0;count<42;count++)
        EPD_SendData(*wavedata++);
 
	 
}

UBYTE EPD_7IN5_V2_Init2(void)
{
    EPD_Reset();

    EPD_SendCommand(0x00);  // Panel setting   
    EPD_SendData(0x3F);

    EPD_SendCommand(0x06);  // Booster Setting
    EPD_SendData(0x17);  
    EPD_SendData(0x17);
    EPD_SendData(0x28);
    EPD_SendData(0x18);

    EPD_SendCommand(0x50);  // VCOM and DATA interval setting    
    EPD_SendData(0x22);
    EPD_SendData(0x07);

    EPD_SendCommand(0x60);  // TCON setting
    EPD_SendData(0x22);  // S-G G-S

    EPD_SendCommand(0x61);  // Resolution setting
    EPD_SendData(0x03);//800*480
    EPD_SendData(0x20);
    EPD_SendData(0x01);
    EPD_SendData(0xE0); 

    EPD_SendCommand(0x65);  // Resolution setting
    EPD_SendData(0x00);
    EPD_SendData(0x00);
    EPD_SendData(0x00);
    EPD_SendData(0x00);

    EPD_SendCommand(0x04); //POWER ON
    DEV_Delay_ms(100);
    EPD_WaitUntilIdle();    

    return 0;
}

UBYTE EPD_7IN5_V2_Init_Fast(void)
{
    EPD_7IN5_V2_Init2();
    Epaper_LUT_By_MCU((UBYTE*)Lut_all_fresh);
    return 0;
}

UBYTE EPD_7IN5_V2_Init_Partial(void)
{
    EPD_7IN5_V2_Init2();
    Epaper_LUT_By_MCU((UBYTE*)Lut_partial);
    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5_V2_Clear(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    UBYTE image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    UWORD i;
    EPD_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }

    EPD_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_ClearBlack(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    UBYTE image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    UWORD i;
    EPD_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }

    EPD_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}
/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5_V2_Display(UBYTE *blackimage)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
	
    EPD_SendCommand(0x10);
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }

    EPD_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            blackimage[i + j * Width] = ~blackimage[i + j * Width];
        }
    }
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_Display_Partial(UBYTE *blackimage,UDOUBLE x_start, UDOUBLE y_start, UDOUBLE x_end, UDOUBLE y_end)
{
    UDOUBLE Width, Height;
    
    Width =((x_end - x_start) % 8 == 0)?((x_end - x_start) / 8 ):((x_end - x_start) / 8 + 1);
    Height = y_end - y_start;

	EPD_SendCommand(0x91);		//This command makes the display enter partial mode
	EPD_SendCommand(0x90);		//resolution setting
	EPD_SendData(x_start/256);
	EPD_SendData(x_start%256);   //x-start    

	EPD_SendData((x_end-1)/256);		
	EPD_SendData((x_end-1)%256);  //x-end	

	EPD_SendData(y_start/256);  //
	EPD_SendData(y_start%256);   //y-start    

	EPD_SendData((y_end-1)/256);		
	EPD_SendData((y_end-1)%256);  //y-end
	EPD_SendData(0x01);
	
    // for (UDOUBLE j = 0; j < Height; j++) {
    //     for (UDOUBLE i = 0; i < Width; i++) {
    //         blackimage[i+j*Width] = ~blackimage[i+j*Width]; 
    //     }
    // }
    
    EPD_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }

    EPD_7IN5_V2_TurnOnDisplay();
    EPD_SendCommand(0x92);
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5_V2_Sleep(void)
{
    EPD_SendCommand(0X02);  	//power off
    EPD_WaitUntilIdle();
    EPD_SendCommand(0X07);  	//deep sleep
    EPD_SendData(0xA5);
}
