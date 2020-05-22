/*****************************************************************************
* | File      	:   EPD_3in7.c
* | Author      :   Waveshare team
* | Function    :   3.7inch e-paper
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2020-04-24
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
#include "EPD_3in7.h"
#include "Debug.h"

static unsigned char const lut_vcom_du[] ={
0x00,0x00,0x00,0x00,0x05,0x01,
0x00,0x00,0x00,0x08,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_ww_du[] = {
0x00,0x00,0x00,0x00,0x05,0x01,
0x00,0x00,0x00,0x08,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_wb_du[] ={
0x02,0x00,0x00,0x00,0x05,0x01,
0x05,0x00,0x00,0x08,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };

static unsigned char const lut_bb_du[] ={
0x00,0x00,0x00,0x00,0x05,0x01,
0x00,0x00,0x00,0x08,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };


// VDH
static unsigned char const lut_bw_du[] ={
0x01,0x00,0x00,0x00,0x05,0x01,
0x0A,0x00,0x00,0x08,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };

/********GC*********/
static unsigned char const lut_vcom_gc[] ={
0x00,0x00,0x00,0x0A,0x05,0x01,
0x00,0x00,0x08,0x0A,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};


static unsigned char const lut_ww_gc[] = {
0x05,0x00,0x00,0x0A,0x05,0x01,
0x0A,0x00,0x08,0x0A,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_wb_gc[] ={
0x0A,0x00,0x00,0x0A,0x05,0x01,
0x15,0x00,0x08,0x0A,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };

static unsigned char const lut_bb_gc[] ={ 
0x0A,0x00,0x00,0x0A,0x05,0x01,
0x05,0x00,0x08,0x0A,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };

// VDH
static unsigned char const lut_bw_gc[] ={
0x05,0x00,0x00,0x0A,0x05,0x01,
0x2A,0x00,0x08,0x0A,0x05,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00 };

/***********A2**********/
static unsigned char const lut_vcom_a2[] ={
0x00,0x00,0x00,0x00,0x08,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_ww_a2[] = {
0x00,0x00,0x00,0x00,0x08,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_wb_a2[] ={
0x05,0x00,0x00,0x00,0x08,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char const lut_bb_a2[] ={ 
0x00,0x00,0x00,0x00,0x08,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

// VDH
static unsigned char const lut_bw_a2[] ={
0x0A,0x00,0x00,0x00,0x08,0x01,
0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_3IN7_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_3IN7_SendCommand(UBYTE Reg)
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
static void EPD_3IN7_SendData(UBYTE Data)
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
static void EPD_3IN7_ReadBusyHigh(void)
{
    Debug("e-Paper busy High\r\n");
    UBYTE busy;
    do {
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
    }while(busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

static void EPD_3IN7_ReadBusyLow(void)
{
    Debug("e-Paper busy Low\r\n");
    UBYTE busy;
    do {
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
    }while(!busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
void EPD_Load_LUTC(unsigned int LUT, unsigned char const *LUT_Value)
{
  int i;  
  EPD_3IN7_SendCommand(LUT);
  for(i=0;i<42;i++)
  {
    EPD_3IN7_SendData(*LUT_Value);
    LUT_Value++;
  }
}
void EPD_Load_LUT(unsigned int LUT, unsigned char const *LUT_Value)
{
  int i;  
  EPD_3IN7_SendCommand(LUT);
  for(i=0;i<42;i++)
  {
    EPD_3IN7_SendData(*LUT_Value);
    LUT_Value++;
  }
}

static void Upload_Temperature_LUT(unsigned char WF_MODE)
{
  if(WF_MODE == 0){                     //GC mode
    EPD_3IN7_SendCommand(0x26);
    EPD_3IN7_SendData(0x00);
    EPD_Load_LUTC(0x20,lut_vcom_gc); 
    EPD_Load_LUT(0x21,lut_ww_gc);
    EPD_Load_LUTC(0x22,lut_bw_gc);
    EPD_Load_LUT(0x23,lut_wb_gc);
    EPD_Load_LUT(0x24,lut_bb_gc);
  }
  else if(WF_MODE == 1){                //DU mode
    EPD_3IN7_SendCommand(0x26);
    EPD_3IN7_SendData(0x00);
    EPD_Load_LUTC(0x20,lut_vcom_du); 
    EPD_Load_LUT(0x21,lut_ww_du);
    EPD_Load_LUTC(0x22,lut_bw_du);
    EPD_Load_LUT(0x23,lut_wb_du);
    EPD_Load_LUT(0x24,lut_bb_du);    
  }
    else{                               //A2 mode
    EPD_3IN7_SendCommand(0x26);
    EPD_3IN7_SendData(0x00);
    EPD_Load_LUTC(0x20,lut_vcom_a2); 
    EPD_Load_LUT(0x21,lut_ww_a2);
    EPD_Load_LUTC(0x22,lut_bw_a2);
    EPD_Load_LUT(0x23,lut_wb_a2);
    EPD_Load_LUT(0x24,lut_bb_a2);    
  }
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN7_Init(void)
{
    EPD_3IN7_Reset();

    // EPD_3IN7_SendCommand(0x01); // POWER_SETTING
    // EPD_3IN7_SendData(0x03); // VDS_EN, VDG_EN
    // EPD_3IN7_SendData(0x00); // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    // EPD_3IN7_SendData(0x2b); // VDH
   
    EPD_3IN7_SendCommand(0x01);
    EPD_3IN7_SendData(0x03);
    EPD_3IN7_SendData(0x00);  
    EPD_3IN7_SendData(0x3F);  
    EPD_3IN7_SendData(0x3F);  
    EPD_3IN7_SendData(0x0F);

    EPD_3IN7_SendCommand(0x06);
    EPD_3IN7_SendData(0x57);
    EPD_3IN7_SendData(0x36);  //0x63
    EPD_3IN7_SendData(0x3A);  //0x31

    EPD_3IN7_SendCommand(0x60);
    EPD_3IN7_SendData(0x04);

    EPD_3IN7_SendCommand(0x00);
    EPD_3IN7_SendData(0x7F);  

    EPD_3IN7_SendCommand(0x03);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(0x30);
    EPD_3IN7_SendData(0x3C);  //50Hz; 0x19 for 86Hz frame rate   

    EPD_3IN7_SendCommand(0x61);
    EPD_3IN7_SendData(0x01);   
    EPD_3IN7_SendData(0x18);   
    EPD_3IN7_SendData(0x01);
    EPD_3IN7_SendData(0xE0);

    //EPD_3IN7_SendCommand(VDCS);  //set VCOM
    //EPD_3IN7_SendData(0x1E);

    EPD_3IN7_SendCommand(0x50);
    EPD_3IN7_SendData(0x17);  //set to 0x17 instead of orignal 0x77 to make DU/A2 work fine

    EPD_3IN7_SendCommand(0x26);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(0x62);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00); 
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00); 

    // fast discharge settings, do not remove any of these  
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x60);
    EPD_3IN7_SendData(0xA5);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x89);
    EPD_3IN7_SendData(0xA5); 
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0xA1);
    EPD_3IN7_SendData(0x00); 
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x73);
    EPD_3IN7_SendData(0x05); 
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x7E);
    EPD_3IN7_SendData(0x31);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0xB8);
    EPD_3IN7_SendData(0x80);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x92);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x87);
    EPD_3IN7_SendData(0x11);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x88);
    EPD_3IN7_SendData(0x02);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0xA8);
    EPD_3IN7_SendData(0x30); 
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0x92);
    EPD_3IN7_SendData(0x08);
    EPD_3IN7_SendCommand(0xF8);
    EPD_3IN7_SendData(0xA8);
    EPD_3IN7_SendData(0x3A);

    // SourceChannel= 280; 
    // GateChannel= 480;
    // 16800

    // unsigned char temp;

    EPD_3IN7_SendCommand(0x04);
    EPD_3IN7_ReadBusyHigh();

    EPD_3IN7_SendCommand(0x80); //auto measure VCOM
    EPD_3IN7_SendData(0x11);
    EPD_3IN7_ReadBusyHigh();

    EPD_3IN7_SendCommand(0x81);
    // temp = spi_9b_get();
    // EPD_3IN7_SendCommand(VDCS);
    // EPD_3IN7_SendData(temp);

    // EPD_DTM1_Initial1();
    // UWORD Width, Height;
    // Width = (EPD_3IN7_WIDTH % 8 == 0)? (EPD_3IN7_WIDTH / 8 ): (EPD_3IN7_WIDTH / 8 + 1);
    // Height = EPD_3IN7_HEIGHT;
    // EPD_3IN7_SendCommand(0x10);
    // for (UWORD j = 0; j < Height; j++) {
        // for (UWORD i = 0; i < Width; i++) {
            // EPD_3IN7_SendData(0XFF);
        // }
    // }
    // EPD_3IN7_SendCommand(0x11);
    EPD_3IN7_SendCommand(0x02);
    EPD_3IN7_ReadBusyLow();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_3IN7_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_3IN7_WIDTH % 8 == 0)? (EPD_3IN7_WIDTH / 8 ): (EPD_3IN7_WIDTH / 8 + 1);
    Height = EPD_3IN7_HEIGHT;
    
    
    EPD_3IN7_SendCommand(0x04);
    // EPD_3IN7_ReadBusyHigh();
    
    EPD_3IN7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7_SendData(0xf0);
        }
    }
    EPD_3IN7_SendCommand(0x11);
    Upload_Temperature_LUT(0);    //0:GC; 1:DU; else: A2
    EPD_3IN7_SendCommand(0x12);
    EPD_3IN7_ReadBusyHigh();
    EPD_3IN7_SendCommand(0x02);  
    EPD_3IN7_ReadBusyLow();
    
    // printf("0x10\r\n");
    // EPD_3IN7_SendCommand(0x10);
    // for (UWORD j = 0; j < Height; j++) {
        // for (UWORD i = 0; i < Width; i++) {
            // EPD_3IN7_SendData(0XFF);
        // }
    // }
    // EPD_3IN7_SendCommand(0x11);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN7_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_3IN7_WIDTH % 8 == 0)? (EPD_3IN7_WIDTH / 8 ): (EPD_3IN7_WIDTH / 8 + 1);
    Height = EPD_3IN7_HEIGHT;
    // EPD_3IN7_Init();
    
    EPD_3IN7_SendCommand(0x04);
    // EPD_3IN7_ReadBusyHigh();
    
    EPD_3IN7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7_SendData(Image[i + j * Width]);
        }
    }
    EPD_3IN7_SendCommand(0x11);
    Upload_Temperature_LUT(0);    //0:GC; 1:DU; else: A2
    EPD_3IN7_SendCommand(0x12);
    EPD_3IN7_ReadBusyHigh();
    EPD_3IN7_SendCommand(0x02);  
    EPD_3IN7_ReadBusyLow();
    
    printf("0x10\r\n");
    EPD_3IN7_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7_SendData(0XFF);
        }
    }
    EPD_3IN7_SendCommand(0x11);
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN7_Sleep(void)
{
    // EPD_3IN7_SendCommand(0X50);
    // EPD_3IN7_SendData(0xf7);
    // EPD_3IN7_SendCommand(0X02);  	//power off
    // EPD_3IN7_SendCommand(0X07);  	//deep sleep
    // EPD_3IN7_SendData(0xA5);
}
