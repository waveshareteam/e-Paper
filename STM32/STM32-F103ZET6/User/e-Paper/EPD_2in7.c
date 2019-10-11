/*****************************************************************************
* | File      	:   EPD_2in7.c
* | Author      :   Waveshare team
* | Function    :   2.7inch e-paper
* | Info        :
*----------------
* |	This version:   V3.1
* | Date        :   2019-10-10
* | Info        :
* -----------------------------------------------------------------------------
* V3.1(2019-10-10):
* 1. Add 4 grayscale display
*     Add   EPD_2in7_gray_lut_vcom[]
*     Add   EPD_2in7_gray_lut_ww[]
*     Add   EPD_2in7_gray_lut_bw[]
*     Add   EPD_2in7_gray_lut_wb[]
*     Add   EPD_2in7_gray_lut_bb[]
*     Add   EPD_2in7_gray_SetLut()
*     Add   EPD_2IN7_Init_4Gray()
*     Add   EPD_2IN7_4GrayDisplay()
* V3.0(2019-06-12):
* 1.Change:
*    lut_vcom_dc[] => EPD_2in7_lut_vcom_dc[]
*    lut_ww[] => EPD_2in7_lut_ww[] 
*    lut_bw[] => EPD_2in7_lut_bw[]
*    lut_bb[] => EPD_2in7_lut_bb[]
*    lut_wb[] => EPD_2in7_lut_wb[]
*    EPD_Reset() => EPD_2in7_Reset()
*    EPD_SendCommand() => EPD_2in7_SendCommand()
*    EPD_SendData() => EPD_2in7_SendData()
*    EPD_WaitUntilIdle() => EPD_2in7_ReadBusy()
*    EPD_SetLut() => EPD_2in7_SetLut()
*    EPD_Init() => EPD_2in7_Init()
*    EPD_Clear() => EPD_2in7_Clear()
*    EPD_Display() => EPD_2in7_Display()
*    EPD_Sleep() => EPD_2in7_Sleep()
* 2.remove commands define:
*   #define PANEL_SETTING                               0x00
*   #define POWER_SETTING                               0x01
*   #define POWER_OFF                                   0x02
*   #define POWER_OFF_SEQUENCE_SETTING                  0x03
*   #define POWER_ON                                    0x04
*   #define POWER_ON_MEASURE                            0x05
*   #define BOOSTER_SOFT_START                          0x06
*   #define DEEP_SLEEP                                  0x07
*   #define DATA_START_TRANSMISSION_1                   0x10
*   #define DATA_STOP                                   0x11
*   #define DISPLAY_REFRESH                             0x12
*   #define DATA_START_TRANSMISSION_2                   0x13
*   #define PLL_CONTROL                                 0x30
*   #define TEMPERATURE_SENSOR_COMMAND                  0x40
*   #define TEMPERATURE_SENSOR_CALIBRATION              0x41
*   #define TEMPERATURE_SENSOR_WRITE                    0x42
*   #define TEMPERATURE_SENSOR_READ                     0x43
*   #define VCOM_AND_DATA_INTERVAL_SETTING              0x50
*   #define LOW_POWER_DETECTION                         0x51
*   #define TCON_SETTING                                0x60
*   #define TCON_RESOLUTION                             0x61
*   #define SOURCE_AND_GATE_START_SETTING               0x62
*   #define GET_STATUS                                  0x71
*   #define AUTO_MEASURE_VCOM                           0x80
*   #define VCOM_VALUE                                  0x81
*   #define VCM_DC_SETTING_REGISTER                     0x82
*   #define PROGRAM_MODE                                0xA0
*   #define ACTIVE_PROGRAM                              0xA1
*   #define READ_OTP_DATA                               0xA2
* -----------------------------------------------------------------------------
* V2.0(2018-11-06):
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
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
#include "EPD_2in7.h"
#include "Debug.h"

static const unsigned char EPD_2in7_lut_vcom_dc[] = {
    0x00	,0x00,
    0x00	,0x08	,0x00	,0x00	,0x00	,0x02,
    0x60	,0x28	,0x28	,0x00	,0x00	,0x01,
    0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
    0x00	,0x12	,0x12	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00
};
static const unsigned char EPD_2in7_lut_ww[] = {
    0x40	,0x08	,0x00	,0x00	,0x00	,0x02,
    0x90	,0x28	,0x28	,0x00	,0x00	,0x01,
    0x40	,0x14	,0x00	,0x00	,0x00	,0x01,
    0xA0	,0x12	,0x12	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
static const unsigned char EPD_2in7_lut_bw[] = {
    0x40	,0x08	,0x00	,0x00	,0x00	,0x02,
    0x90	,0x28	,0x28	,0x00	,0x00	,0x01,
    0x40	,0x14	,0x00	,0x00	,0x00	,0x01,
    0xA0	,0x12	,0x12	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
static const unsigned char EPD_2in7_lut_bb[] = {
    0x80	,0x08	,0x00	,0x00	,0x00	,0x02,
    0x90	,0x28	,0x28	,0x00	,0x00	,0x01,
    0x80	,0x14	,0x00	,0x00	,0x00	,0x01,
    0x50	,0x12	,0x12	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
static const unsigned char EPD_2in7_lut_wb[] = {
    0x80	,0x08	,0x00	,0x00	,0x00	,0x02,
    0x90	,0x28	,0x28	,0x00	,0x00	,0x01,
    0x80	,0x14	,0x00	,0x00	,0x00	,0x01,
    0x50	,0x12	,0x12	,0x00	,0x00	,0x01,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
    0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

//////////////////////////////////////full screen update LUT////////////////////////////////////////////
//0~3 gray
static const unsigned char EPD_2in7_gray_lut_vcom[] =
{
0x00	,0x00,
0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,				
};
//R21
static const unsigned char EPD_2in7_gray_lut_ww[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R22H	r
static const unsigned char EPD_2in7_gray_lut_bw[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R23H	w
static const unsigned char EPD_2in7_gray_lut_wb[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R24H	b
static const unsigned char EPD_2in7_gray_lut_bb[] ={
0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2in7_Reset(void)
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
static void EPD_2in7_SendCommand(UBYTE Reg)
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
static void EPD_2in7_SendData(UBYTE Data)
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
static void EPD_2in7_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    UBYTE busy;
    do {
        EPD_2in7_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_2in7_SetLut(void)
{
    unsigned int count;
    EPD_2in7_SendCommand(0x20); //vcom
    for(count = 0; count < 44; count++) {
        EPD_2in7_SendData(EPD_2in7_lut_vcom_dc[count]);
    }

    EPD_2in7_SendCommand(0x21); //ww --
    for(count = 0; count < 42; count++) {
        EPD_2in7_SendData(EPD_2in7_lut_ww[count]);
    }

    EPD_2in7_SendCommand(0x22); //bw r
    for(count = 0; count < 42; count++) {
        EPD_2in7_SendData(EPD_2in7_lut_bw[count]);
    }

    EPD_2in7_SendCommand(0x23); //wb w
    for(count = 0; count < 42; count++) {
        EPD_2in7_SendData(EPD_2in7_lut_bb[count]);
    }

    EPD_2in7_SendCommand(0x24); //bb b
    for(count = 0; count < 42; count++) {
        EPD_2in7_SendData(EPD_2in7_lut_wb[count]);
    }
}

void EPD_2in7_gray_SetLut(void)
{
	unsigned int count;	 
    EPD_2in7_SendCommand(0x20);							//vcom
		for(count=0;count<44;count++)
			{EPD_2in7_SendData(EPD_2in7_gray_lut_vcom[count]);}
		
	EPD_2in7_SendCommand(0x21);							//red not use
	for(count=0;count<42;count++)
		{EPD_2in7_SendData(EPD_2in7_gray_lut_ww[count]);}

		EPD_2in7_SendCommand(0x22);							//bw r
		for(count=0;count<42;count++)
			{EPD_2in7_SendData(EPD_2in7_gray_lut_bw[count]);}

		EPD_2in7_SendCommand(0x23);							//wb w
		for(count=0;count<42;count++)
			{EPD_2in7_SendData(EPD_2in7_gray_lut_wb[count]);}

		EPD_2in7_SendCommand(0x24);							//bb b
		for(count=0;count<42;count++)
			{EPD_2in7_SendData(EPD_2in7_gray_lut_bb[count]);}

		EPD_2in7_SendCommand(0x25);							//vcom
		for(count=0;count<42;count++)
			{EPD_2in7_SendData(EPD_2in7_gray_lut_ww[count]);}
         
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7_Init(void)
{
    EPD_2in7_Reset();

    EPD_2in7_SendCommand(0x01); // POWER_SETTING
    EPD_2in7_SendData(0x03); // VDS_EN, VDG_EN
    EPD_2in7_SendData(0x00); // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    EPD_2in7_SendData(0x2b); // VDH
    EPD_2in7_SendData(0x2b); // VDL
    EPD_2in7_SendData(0x09); // VDHR
	
    EPD_2in7_SendCommand(0x06);  // BOOSTER_SOFT_START
    EPD_2in7_SendData(0x07);
    EPD_2in7_SendData(0x07);
    EPD_2in7_SendData(0x17);
	
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0x60);
    EPD_2in7_SendData(0xA5);
	
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0x89);
    EPD_2in7_SendData(0xA5);
		
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0x90);
    EPD_2in7_SendData(0x00);
		
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0x93);
    EPD_2in7_SendData(0x2A);
		
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0xA0);
    EPD_2in7_SendData(0xA5);
		
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0xA1);
    EPD_2in7_SendData(0x00);
		
    // Power optimization
    EPD_2in7_SendCommand(0xF8);
    EPD_2in7_SendData(0x73);
    EPD_2in7_SendData(0x41);
		
    EPD_2in7_SendCommand(0x16); // PARTIAL_DISPLAY_REFRESH
    EPD_2in7_SendData(0x00);
		
    EPD_2in7_SendCommand(0x04); // POWER_ON
    EPD_2in7_ReadBusy();

    EPD_2in7_SendCommand(0x00); // PANEL_SETTING
    EPD_2in7_SendData(0xAF); // KW-BF   KWR-AF    BWROTP 0f
    EPD_2in7_SendCommand(0x30); // PLL_CONTROL
    EPD_2in7_SendData(0x3A); // 3A 100HZ   29 150Hz 39 200HZ    31 171HZ
    EPD_2in7_SendCommand(0x82);  // VCM_DC_SETTING_REGISTER
    EPD_2in7_SendData(0x12);

    EPD_2in7_SetLut();
}

void EPD_2IN7_Init_4Gray(void)
{
    EPD_2in7_Reset();
    EPD_2in7_SendCommand(0x01);			//POWER SETTING
    EPD_2in7_SendData (0x03);
    EPD_2in7_SendData (0x00);    
    EPD_2in7_SendData (0x2b);															 
    EPD_2in7_SendData (0x2b);		


    EPD_2in7_SendCommand(0x06);         //booster soft start
    EPD_2in7_SendData (0x07);		//A
    EPD_2in7_SendData (0x07);		//B
    EPD_2in7_SendData (0x17);		//C 

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0x60);
    EPD_2in7_SendData (0xA5);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0x89);
    EPD_2in7_SendData (0xA5);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0x90);
    EPD_2in7_SendData (0x00);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0x93);
    EPD_2in7_SendData (0x2A);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0xa0);
    EPD_2in7_SendData (0xa5);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0xa1);
    EPD_2in7_SendData (0x00);

    EPD_2in7_SendCommand(0xF8);         //boost??
    EPD_2in7_SendData (0x73);
    EPD_2in7_SendData (0x41);

    EPD_2in7_SendCommand(0x16);
    EPD_2in7_SendData(0x00);	

    EPD_2in7_SendCommand(0x04);
    EPD_2in7_ReadBusy();

    EPD_2in7_SendCommand(0x00);			//panel setting
    EPD_2in7_SendData(0xbf);		//KW-BF   KWR-AF	BWROTP 0f

    EPD_2in7_SendCommand(0x30);			//PLL setting
    EPD_2in7_SendData (0x90);      	//100hz 

    EPD_2in7_SendCommand(0x61);			//resolution setting
    EPD_2in7_SendData (0x00);		//176
    EPD_2in7_SendData (0xb0);     	 
    EPD_2in7_SendData (0x01);		//264
    EPD_2in7_SendData (0x08);

    EPD_2in7_SendCommand(0x82);			//vcom_DC setting
    EPD_2in7_SendData (0x12);

    EPD_2in7_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING			
    EPD_2in7_SendData(0x97);
}



/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_WIDTH % 8 == 0)? (EPD_2IN7_WIDTH / 8 ): (EPD_2IN7_WIDTH / 8 + 1);
    Height = EPD_2IN7_HEIGHT;

    EPD_2in7_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(0XFF);
        }
    }

    EPD_2in7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(0XFF);
        }
    }
		
    EPD_2in7_SendCommand(0x12);
    EPD_2in7_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN7_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_WIDTH % 8 == 0)? (EPD_2IN7_WIDTH / 8 ): (EPD_2IN7_WIDTH / 8 + 1);
    Height = EPD_2IN7_HEIGHT;

    EPD_2in7_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(0XFF);
        }
    }

    EPD_2in7_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2in7_SendData(Image[i + j * Width]);
        }
    }
    EPD_2in7_SendCommand(0x12);
    EPD_2in7_ReadBusy();
}

void EPD_2IN7_4GrayDisplay(const UBYTE *Image)
{
    UDOUBLE i,j,k;
    UBYTE temp1,temp2,temp3;

    EPD_2in7_SendCommand(0x10);	       
    for(i=0;i<5808;i++)	               //5808*4  46464
    {
        temp3=0;
        for(j=0;j<2;j++)	
        {
            temp1 = Image[i*2+j];
            for(k=0;k<2;k++)	
            {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x01;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x00;  //black
                else if(temp2 == 0x80) 
                    temp3 |= 0x01;  //gray1
                else //0x40
                    temp3 |= 0x00; //gray2
                temp3 <<= 1;	
                
                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x01;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x00;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; //gray1
                else    //0x40
                        temp3 |= 0x00;	//gray2	
                if(j!=1 || k!=1)				
                    temp3 <<= 1;
                
                temp1 <<= 2;
            }
            
         }
        EPD_2in7_SendData(temp3);			
    }
    // new  data
    EPD_2in7_SendCommand(0x13);	       
    for(i=0;i<5808;i++)	               //5808*4  46464
    {
        temp3=0;
        for(j=0;j<2;j++)	
        {
            temp1 = Image[i*2+j];
            for(k=0;k<2;k++)	
            {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x01;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x00;  //black
                else if(temp2 == 0x80) 
                    temp3 |= 0x00;  //gray1
                else //0x40
                    temp3 |= 0x01; //gray2
                temp3 <<= 1;	
                
                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x01;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x00;
                else if(temp2 == 0x80)
                    temp3 |= 0x00; //gray1
                else    //0x40
                        temp3 |= 0x01;	//gray2
                if(j!=1 || k!=1)					
                    temp3 <<= 1;
                
                temp1 <<= 2;
            }
            
         }
        EPD_2in7_SendData(temp3);	
    }
    
    EPD_2in7_gray_SetLut();
    EPD_2in7_SendCommand(0x12);
    DEV_Delay_ms(200);
    EPD_2in7_ReadBusy();
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7_Sleep(void)
{
    EPD_2in7_SendCommand(0X50);
    EPD_2in7_SendData(0xf7);
    EPD_2in7_SendCommand(0X02);  	//power off
    EPD_2in7_SendCommand(0X07);  	//deep sleep
    EPD_2in7_SendData(0xA5);
}
