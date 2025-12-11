/*****************************************************************************
* | File      	:	EPD_5in79.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-05
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
#include "EPD_5in79.h"
#include "Debug.h"

/************4 Gray******************************************************/
unsigned char LUT_DATA_4Gray[] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 
0x01, 0x4A, 0x00, 0x00, 0x00, 0x01, 0x00,
0x01, 0x82, 0x42, 0x00, 0x00, 0x10, 0x00,
0x01, 0x8A, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 
0x01, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00,
0x01, 0x82, 0x42, 0x00, 0x00, 0x10, 0x00,
0x01, 0x81, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 
0x01, 0x81, 0x00, 0x00, 0x00, 0x01, 0x00,
0x01, 0x82, 0x42, 0x00, 0x00, 0x10, 0x00,
0x01, 0x41, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x01, 0x8A, 0x00, 0x00, 0x00, 0x01, 0x00,
0x01, 0x82, 0x42, 0x00, 0x00, 0x10, 0x00,
0x01, 0x4A, 0x00, 0x00, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

0x02, 0x00, 0x00,    //FR, XON
0x22, 0x17, 0x41, 0xA8, 0x32, 0x40, 
//EOPT  VGH   VSH1  VSH2  VSL   VCOM

};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in79_Reset(void)
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
static void EPD_5in79_SendCommand(UBYTE Reg)
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
static void EPD_5in79_SendData(UBYTE Data)
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
void EPD_5in79_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	UBYTE busy;
	do
	{
		busy = DEV_Digital_Read(EPD_BUSY_PIN);
        DEV_Delay_ms(10);   
	}
	while(busy);   
	DEV_Delay_ms(200);     
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5in79_TurnOnDisplay(void)
{
    EPD_5in79_SendCommand(0x22);
    EPD_5in79_SendData(0xF7);
	EPD_5in79_SendCommand(0x20);			//DISPLAY REFRESH 	
	DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

static void EPD_5in79_TurnOnDisplay_Fast(void)
{
    EPD_5in79_SendCommand(0x22);
    EPD_5in79_SendData(0xC7);
	EPD_5in79_SendCommand(0x20);			//DISPLAY REFRESH 	
	DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

static void EPD_5in79_TurnOnDisplay_Partial(void)
{
    EPD_5in79_SendCommand(0x22);
    EPD_5in79_SendData(0xFF);
	EPD_5in79_SendCommand(0x20);			//DISPLAY REFRESH 	
	DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

static void EPD_5in79_TurnOnDisplay_4GRAY(void)
{
    EPD_5in79_SendCommand(0x22);
    EPD_5in79_SendData(0xCF);
	EPD_5in79_SendCommand(0x20);			//DISPLAY REFRESH 	
	DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_5in79_Lut(void)
{
    unsigned int count;
    EPD_5in79_SendCommand(0x32);
    for(count = 0; count < 227 ; count++) {
        EPD_5in79_SendData(LUT_DATA_4Gray[count]);
    }
    EPD_5in79_SendCommand(0x3f); 
	EPD_5in79_SendData(LUT_DATA_4Gray[227]);

    EPD_5in79_SendCommand(0x03);     
	EPD_5in79_SendData(LUT_DATA_4Gray[228]);

	EPD_5in79_SendCommand(0x04);   
	EPD_5in79_SendData(LUT_DATA_4Gray[229]); 
	EPD_5in79_SendData(LUT_DATA_4Gray[230]); 
	EPD_5in79_SendData(LUT_DATA_4Gray[231]);   

	EPD_5in79_SendCommand(0x2C); 
	EPD_5in79_SendData(LUT_DATA_4Gray[232]); 
}


/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_5in79_Init(void)
{
    EPD_5in79_Reset();

    EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
	EPD_5in79_SendCommand(0x12); //POWER ON
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal

    EPD_5in79_SendCommand(0x11);
    EPD_5in79_SendData(0x01);

    EPD_5in79_SendCommand(0x44);	 						 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x00);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x31); //400/8-1
	EPD_5in79_SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0x4e);	 						 
	EPD_5in79_SendData(0x00);	
	EPD_5in79_SendCommand(0x4f);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01); 	

    EPD_5in79_ReadBusy();   
    
    EPD_5in79_SendCommand(0x91);
    EPD_5in79_SendData(0x00);

    EPD_5in79_SendCommand(0xC4);							 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x31);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x00); //400/8-1
	EPD_5in79_SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0xCE);	 						 
	EPD_5in79_SendData(0x31);	
	EPD_5in79_SendCommand(0xCF);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);

    EPD_5in79_ReadBusy();   

    return 0;
}

UBYTE EPD_5in79_Init_Fast(void)
{
    EPD_5in79_Reset();

    EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
	EPD_5in79_SendCommand(0x12); //POWER ON
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
    
    EPD_5in79_SendCommand(0x18); //Read built-in temperature sensor
    EPD_5in79_SendData(0x80);	

    EPD_5in79_SendCommand(0x22); // Load temperature value
    EPD_5in79_SendData(0xB1);		
    EPD_5in79_SendCommand(0x20);	
    EPD_5in79_ReadBusy();   

    EPD_5in79_SendCommand(0x1A); // Write to temperature register
    EPD_5in79_SendData(0x64);		
    EPD_5in79_SendData(0x00);	
                
    EPD_5in79_SendCommand(0x22); // Load temperature value
    EPD_5in79_SendData(0x91);		
    EPD_5in79_SendCommand(0x20);	
    EPD_5in79_ReadBusy();   

    EPD_5in79_SendCommand(0x11);
    EPD_5in79_SendData(0x01);
    
    EPD_5in79_SendCommand(0x44);	 						 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x00);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x31); //400/8-1
	EPD_5in79_SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0x4e);	 						 
	EPD_5in79_SendData(0x00);	
	EPD_5in79_SendCommand(0x4f);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01); 	

    EPD_5in79_ReadBusy();   
    
    EPD_5in79_SendCommand(0x91);
    EPD_5in79_SendData(0x00);

    EPD_5in79_SendCommand(0xC4);							 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x31);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x00); //400/8-1
	EPD_5in79_SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0xCe);	 						 
	EPD_5in79_SendData(0x31);	
	EPD_5in79_SendCommand(0xCf);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);

    EPD_5in79_ReadBusy();   
    

    return 0;
}

UBYTE EPD_5in79_Init_Partial(void)
{
    EPD_5in79_Reset();

    EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
	EPD_5in79_SendCommand(0x12); //POWER ON
	EPD_5in79_ReadBusy();        //waiting for the electronic paper IC to release the idle signal

    EPD_5in79_SendCommand(0x3C);  //BorderWavefrom
    EPD_5in79_SendData(0x80);

    return 0;
}

UBYTE EPD_5in79_Init_4Gray(void)
{
    EPD_5in79_Reset();

    EPD_5in79_ReadBusy();   
	EPD_5in79_SendCommand(0x12); 
	EPD_5in79_ReadBusy(); 

    EPD_5in79_SendCommand(0x0C);     
    EPD_5in79_SendData(0x8B);    
    EPD_5in79_SendData(0x9C);    
    EPD_5in79_SendData(0xA6);    
    EPD_5in79_SendData(0x0F);

    EPD_5in79_SendCommand(0x3C); //set border 
    EPD_5in79_SendData(0x81);

    EPD_5in79_ReadBusy();	

    EPD_5in79_SendCommand(0x11);
    EPD_5in79_SendData(0x01);
    
    EPD_5in79_SendCommand(0x44);	 						 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x00);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x31); //400/8-1
	EPD_5in79_SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0x4e);	 						 
	EPD_5in79_SendData(0x00);	
	EPD_5in79_SendCommand(0x4f);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01); 	

    EPD_5in79_ReadBusy();   
    
    EPD_5in79_SendCommand(0x91);
    EPD_5in79_SendData(0x00);

    EPD_5in79_SendCommand(0xC4);							 // Set Ram X- address Start / End position
	EPD_5in79_SendData(0x31);     						 // XStart, POR = 00h
	EPD_5in79_SendData(0x00); //400/8-1
	EPD_5in79_SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);  //300-1	
	EPD_5in79_SendData(0x00);     									// YEnd L
	EPD_5in79_SendData(0x00);											// YEnd H 

    EPD_5in79_SendCommand(0xCe);	 						 
	EPD_5in79_SendData(0x31);	
	EPD_5in79_SendCommand(0xCf);	 
	EPD_5in79_SendData(0x0f);  
	EPD_5in79_SendData(0x01);

    EPD_5in79_Lut();

    return 0;
}	

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in79_Clear(void)
{
    UDOUBLE i;
    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for(i=0; i<13600; i++) 
    {
        EPD_5in79_SendData(0xff);
    }

    EPD_5in79_SendCommand(0X26);
    for(i=0; i<13600; i++)	
    {
        EPD_5in79_SendData(0x00);
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for(i=0; i<13600; i++) 
    {
        EPD_5in79_SendData(0xff);
    }

    EPD_5in79_SendCommand(0xA6);
    for(i=0; i<13600; i++)	
    {
        EPD_5in79_SendData(0x00);
    }

    EPD_5in79_TurnOnDisplay();
}

void EPD_5in79_Clear_Black(void)
{
    UDOUBLE i;
    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for(i=0; i<13600; i++) 
    {
        EPD_5in79_SendData(0x00);
    }

    EPD_5in79_SendCommand(0X26);
    for(i=0; i<13600; i++)	
    {
        EPD_5in79_SendData(0x00);
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for(i=0; i<13600; i++) 
    {
        EPD_5in79_SendData(0x00);
    }

    EPD_5in79_SendCommand(0xA6);
    for(i=0; i<13600; i++)	
    {
        EPD_5in79_SendData(0x00);
    }

    EPD_5in79_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in79_Display(const UBYTE *Image)
{
    UDOUBLE Width, Width1, Height;
    Width =(EPD_5in79_WIDTH % 16 == 0)?(EPD_5in79_WIDTH / 16 ):(EPD_5in79_WIDTH / 16 + 1);
    Width1 =(EPD_5in79_WIDTH % 8 == 0)?(EPD_5in79_WIDTH / 8 ):(EPD_5in79_WIDTH / 8 + 1);
    Height = EPD_5in79_HEIGHT;

    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i]);
        }
    }

    EPD_5in79_SendCommand(0X26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i + Width - 1]);
        }
    }

    EPD_5in79_SendCommand(0XA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    EPD_5in79_TurnOnDisplay();
}

void EPD_5in79_Display_Base(const UBYTE *Image)
{
    UDOUBLE Width, Width1, Height;
    Width =(EPD_5in79_WIDTH % 16 == 0)?(EPD_5in79_WIDTH / 16 ):(EPD_5in79_WIDTH / 16 + 1);
    Width1 =(EPD_5in79_WIDTH % 8 == 0)?(EPD_5in79_WIDTH / 8 ):(EPD_5in79_WIDTH / 8 + 1);
    Height = EPD_5in79_HEIGHT;

    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i]);
        }
    }

    EPD_5in79_SendCommand(0X26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i + Width - 1]);
        }
    }

    EPD_5in79_SendCommand(0XA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    EPD_5in79_TurnOnDisplay();

    EPD_5in79_SendCommand(0x26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i]);
        }
    }

    EPD_5in79_SendCommand(0xA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i + Width - 1]);
        }
    }
}

void EPD_5in79_Display_Base_color(UBYTE color)
{
    UDOUBLE Width, Height;
    Width =(EPD_5in79_WIDTH % 16 == 0)?(EPD_5in79_WIDTH / 16 ):(EPD_5in79_WIDTH / 16 + 1);
    Height = EPD_5in79_HEIGHT;

    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(color);
        }
    }

    EPD_5in79_SendCommand(0X26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(color);
        }
    }

    EPD_5in79_SendCommand(0XA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    EPD_5in79_TurnOnDisplay();

    // M part 396*272
    EPD_5in79_SendCommand(0X26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(color);
        }
    }

    EPD_5in79_SendCommand(0XA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(color);
        }
    }

    
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in79_Display_Fast(const UBYTE *Image)
{
    UDOUBLE Width, Width1, Height;
    Width =(EPD_5in79_WIDTH % 16 == 0)?(EPD_5in79_WIDTH / 16 ):(EPD_5in79_WIDTH / 16 + 1);
    Width1 =(EPD_5in79_WIDTH % 8 == 0)?(EPD_5in79_WIDTH / 8 ):(EPD_5in79_WIDTH / 8 + 1);
    Height = EPD_5in79_HEIGHT;

    // M part 396*272
    EPD_5in79_SendCommand(0x24);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i]);
        }
    }

    EPD_5in79_SendCommand(0X26);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    // S part 396*272
    EPD_5in79_SendCommand(0xA4);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(Image[j * Width1 + i + Width - 1]);
        }
    }

    EPD_5in79_SendCommand(0XA6);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            EPD_5in79_SendData(0x00);
        }
    }

    EPD_5in79_TurnOnDisplay_Fast();

}

void EPD_5in79_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    UDOUBLE Width, Width1, Width2, Height;
    Width =((Xend - Xstart) % 8 == 0)?((Xend - Xstart) / 8 ):((Xend - Xstart) / 8 + 1);
    Height = Yend - Ystart;

    EPD_5in79_SendCommand(0x22); 
    EPD_5in79_SendData(0xc0);   
    EPD_5in79_SendCommand(0x20); 
    EPD_5in79_ReadBusy(); 

    EPD_5in79_SendCommand(0x11);
    EPD_5in79_SendData(0x01);

    EPD_5in79_SendCommand(0x91);
    EPD_5in79_SendData(0x00);

    if(Xstart>395)
    {
        Width1 = 0;
        Width2 = Width;

        EPD_5in79_SendCommand(0xC4);							 // Set Ram X- address Start / End position
        EPD_5in79_SendData((792-Xstart)/8-1);     						 // XStart, POR = 00h
        EPD_5in79_SendData((792-Xend)/8); //400/8-1
        EPD_5in79_SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256);  //300-1	
        EPD_5in79_SendData(Ystart%256);     									// YEnd L
        EPD_5in79_SendData(Ystart/256);											// YEnd H 

        EPD_5in79_SendCommand(0xCE);	 						 
        EPD_5in79_SendData((792-Xstart)/8-1);	
        EPD_5in79_SendCommand(0xCF);	 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256); 

        EPD_5in79_SendCommand(0xA4);
        for (UDOUBLE j = 0; j < Height; j++) {
            for (UDOUBLE i = 1; i < Width2+1; i++) {
                EPD_5in79_SendData(Image[j * Width + i + Width1 - 1]);
            }
        }
    }
    else if(Xend<396)
    {
        Width1 = Width;
        Width2 = 0;

        EPD_5in79_SendCommand(0x44);	 						 // Set Ram X- address Start / End position
        EPD_5in79_SendData(Xstart/8);     						
        EPD_5in79_SendData((Xstart/8+Width1)-1); 
        EPD_5in79_SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256);  
        EPD_5in79_SendData(Ystart%256);     									
        EPD_5in79_SendData(Ystart/256);											

        EPD_5in79_SendCommand(0x4e);	 						 
        EPD_5in79_SendData(Xstart/8);	
        EPD_5in79_SendCommand(0x4f);	 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256); 	

        EPD_5in79_SendCommand(0x24);
        for (UDOUBLE j = 0; j < Height; j++) {
            for (UDOUBLE i = 0; i < Width1; i++) {
                EPD_5in79_SendData(Image[j * Width + i]);
            }
        }

    }
    else
    {
        Width1 = ((396 - Xstart) % 8 == 0)?((396 - Xstart) / 8 ):((396 - Xstart) / 8 + 1);
        Width2 = ((Xend - 395) % 8 == 0)?((Xend - 395) / 8 ):((Xend - 395) / 8 + 1);

        EPD_5in79_SendCommand(0x44);	 						 // Set Ram X- address Start / End position
        EPD_5in79_SendData(0x32-Width1);     						
        EPD_5in79_SendData(0x31); 
        EPD_5in79_SendCommand(0x45);	 									// Set Ram Y- address  Start / End position 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256);  
        EPD_5in79_SendData(Ystart%256);     								
        EPD_5in79_SendData(Ystart/256);										

        EPD_5in79_SendCommand(0x4e);	 						 
        EPD_5in79_SendData(0x32-Width1);	
        EPD_5in79_SendCommand(0x4f);	 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256);

        EPD_5in79_SendCommand(0x24);
        for (UDOUBLE j = 0; j < Height; j++) {
            for (UDOUBLE i = 0; i < Width1; i++) {
                EPD_5in79_SendData(Image[j * Width + i]);
            }
        }


        EPD_5in79_SendCommand(0xC4);							 // Set Ram X- address Start / End position
        EPD_5in79_SendData(0x31);     						
        EPD_5in79_SendData(0x32-Width2); 
        EPD_5in79_SendCommand(0xC5);	 									// Set Ram Y- address  Start / End position 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256);  
        EPD_5in79_SendData(Ystart%256);     									
        EPD_5in79_SendData(Ystart/256);											

        EPD_5in79_SendCommand(0xCE);	 						 
        EPD_5in79_SendData(0x31);	
        EPD_5in79_SendCommand(0xCF);	 
        EPD_5in79_SendData((Yend-1)%256);  
        EPD_5in79_SendData((Yend-1)/256); 

        EPD_5in79_SendCommand(0xA4);
        for (UDOUBLE j = 0; j < Height; j++) {
            for (UDOUBLE i = 1; i < Width2+1; i++) {
                EPD_5in79_SendData(Image[j * Width + i + Width1 - 1]);
            }
        }
    }

    EPD_5in79_TurnOnDisplay_Partial();

}

void EPD_5in79_4GrayDisplay(UBYTE *Image)
{
    UDOUBLE i,j,k,o;
    UBYTE temp1,temp2,temp3;
    UDOUBLE Width, Width1, Height;
    Width =(EPD_5in79_WIDTH % 16 == 0)?(EPD_5in79_WIDTH / 16 ):(EPD_5in79_WIDTH / 16 + 1);
    Width1 =(EPD_5in79_WIDTH % 8 == 0)?(EPD_5in79_WIDTH / 8 ):(EPD_5in79_WIDTH / 8 + 1);
    Height = EPD_5in79_HEIGHT;

    EPD_5in79_SendCommand(0x24);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            temp3=0;
            for(o=0; o<2; o++) {
                temp1 = Image[(j * Width1 + i)*2+o];
                for(k=0; k<4; k++) {
                    temp2 = temp1&0xC0;
                    if(temp2 == 0xC0)       //white
                        temp3 |= 0x01;
                    else if(temp2 == 0x00)  //black
                        temp3 |= 0x00;
                    else if(temp2 == 0x80)  //gray1
                        temp3 |= 0x00; 
                    else //0x40             //gray2
                        temp3 |= 0x01; 

                    if(o!=1 || k!=3)
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_5in79_SendData(temp3);
            // printf("%x",temp3);
        }
    }

    EPD_5in79_SendCommand(0x26);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            temp3=0;
            for(o=0; o<2; o++) {
                temp1 = Image[(j * Width1 + i)*2+o];
                for(k=0; k<4; k++) {
                    temp2 = temp1&0xC0;
                    if(temp2 == 0xC0)       //white
                        temp3 |= 0x01;
                    else if(temp2 == 0x00)  //black
                        temp3 |= 0x00; 
                    else if(temp2 == 0x80)  //gray1
                        temp3 |= 0x01; 
                    else //0x40             //gray2
                        temp3 |= 0x00; 

                    if(o!=1 || k!=3)
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_5in79_SendData(temp3);
            // printf("%x",temp3);
        }
    }


    EPD_5in79_SendCommand(0xA4);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            temp3=0;
            for(o=0; o<2; o++) {
                temp1 = Image[(j * Width1 + i + Width - 1)*2+o];
                for(k=0; k<4; k++) {
                    temp2 = temp1&0xC0;
                    if(temp2 == 0xC0)       //white
                        temp3 |= 0x01;
                    else if(temp2 == 0x00)  //black
                        temp3 |= 0x00; 
                    else if(temp2 == 0x80)  //gray1
                        temp3 |= 0x00; 
                    else //0x40             //gray2
                        temp3 |= 0x01; 

                    if(o!=1 || k!=3)
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_5in79_SendData(temp3);
            // printf("%x",temp3);
        }
    }

    EPD_5in79_SendCommand(0xA6);
    for (j = 0; j < Height; j++) {
        for (i = 0; i < Width; i++) {
            temp3=0;
            for(o=0; o<2; o++) {
                temp1 = Image[(j * Width1 + i + Width - 1)*2+o];
                for(k=0; k<4; k++) {
                    temp2 = temp1&0xC0;
                    if(temp2 == 0xC0)       //white
                        temp3 |= 0x01;
                    else if(temp2 == 0x00)  //black
                        temp3 |= 0x00; 
                    else if(temp2 == 0x80)  //gray1
                        temp3 |= 0x01; 
                    else //0x40             //gray2
                        temp3 |= 0x00; 

                    if(o!=1 || k!=3)
                        temp3 <<= 1;

                    temp1 <<= 2;
                }
            }
            EPD_5in79_SendData(temp3);
            // printf("%x",temp3);
        }
    }

    EPD_5in79_TurnOnDisplay_4GRAY();

}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in79_Sleep(void)
{
    EPD_5in79_SendCommand(0X10);  	//deep sleep
    EPD_5in79_SendData(0x01);
}
