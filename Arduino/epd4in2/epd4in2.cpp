/**
 *  @filename   :   epd4in2.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd4in2.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
    SendCommand(0x01);
    SendData(0x03);                  // VDS_EN, VDG_EN
    SendData(0x00);                  // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    SendData(0x2b);                  // VDH
    SendData(0x2b);                  // VDL

    SendCommand(0x06);
    SendData(0x17);
    SendData(0x17);
    SendData(0x17);                  //07 0f 17 1f 27 2F 37 2f
    SendCommand(0x04);
    WaitUntilIdle();
    SendCommand(0x00);
    SendData(0xbf);    // KW-BF   KWR-AF  BWROTP 0f

    SendCommand(0x30);
    SendData(0x3c);        // 3A 100HZ   29 150Hz 39 200HZ  31 171HZ

    SendCommand(0x61); // resolution setting
    SendData(0x01);
    SendData(0x90); //128
    SendData(0x01); //
    SendData(0x2c);

    SendCommand(0x82); // vcom_DC setting
    SendData(0x12);

    SendCommand(0X50); // VCOM AND DATA INTERVAL SETTING
    SendData(0x97); // 97white border 77black border    VBDF 17|D7 VBDW 97 VBDB 57    VBDF F7 VBDW 77 VBDB 37  VBDR B7

    SetLut();
    
    /* EPD hardware init end */
    return 0;
}

int Epd::Init_4Gray(void) {
	/* this calls the peripheral hardware interface, see epdif */
	if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
	SendCommand(0x01);			//POWER SETTING
	SendData (0x03);
	SendData (0x00);       //VGH=20V,VGL=-20V
	SendData (0x2b);		//VDH=15V															 
	SendData (0x2b);		//VDL=-15V
	SendData (0x13);

	SendCommand(0x06);         //booster soft start
	SendData (0x17);		//A
	SendData (0x17);		//B
	SendData (0x17);		//C 

	SendCommand(0x04);
	WaitUntilIdle();

	SendCommand(0x00);			//panel setting
	SendData(0x3f);		//KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	SendCommand(0x30);			//PLL setting
	SendData (0x3c);      	//100hz 

	SendCommand(0x61);			//resolution setting
	SendData (0x01);		//400
	SendData (0x90);     	 
	SendData (0x01);		//300
	SendData (0x2c);

	SendCommand(0x82);			//vcom_DC setting
	SendData (0x12);

	SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING			
	SendData(0x97);
}




/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void) {
    while(DigitalRead(busy_pin) == 0) {      //0: busy, 1: idle
        // Wait
    }      
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);   
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);   
}

/**
 *  @brief: transmit partial data to the SRAM
 */
void Epd::SetPartialWindow(const unsigned char* buffer_black, int x, int y, int w, int l) {
    SendCommand(PARTIAL_IN);
    SendCommand(PARTIAL_WINDOW);
    SendData(x >> 8);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(((x & 0xf8) + w  - 1) >> 8);
    SendData(((x & 0xf8) + w  - 1) | 0x07);
    SendData(y >> 8);        
    SendData(y & 0xff);
    SendData((y + l - 1) >> 8);        
    SendData((y + l - 1) & 0xff);
    SendData(0x01);         // Gates scan both inside and outside of the partial window. (default) 
    DelayMs(2);
    SendCommand(DATA_START_TRANSMISSION_2);
    if (buffer_black != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_black[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);  
        }  
    }
    DelayMs(2);
    SendCommand(PARTIAL_OUT);  
}

void Epd::Set_4GrayDisplay(const char *Image, int x, int y, int w, int l)
{
    int i,j,k,m;
	int z=0;
    unsigned char temp1,temp2,temp3;
/****Color display description****
      white  gray1  gray2  black
0x10|  01     01     00     00
0x13|  01     00     01     00
*********************************/
	SendCommand(0x10);
	z=0;
	x= x/8*8;
	for(m = 0; m<EPD_HEIGHT;m++)
		for(i=0;i<EPD_WIDTH/8;i++)
		{
			if(i >= x/8 && i <(x+w)/8 && m >= y && m < y+l){
				
				temp3=0;
				for(j=0;j<2;j++)	
				{
					temp1 = pgm_read_byte(&Image[z*2+j]);
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
				z++;
				SendData(temp3);
				
			}else{
				SendData(0xff);
			}				
		}
    // new  data
    SendCommand(0x13);
	z=0;
	for(m = 0; m<EPD_HEIGHT;m++)
		for(i=0;i<EPD_WIDTH/8;i++)
		{
			if(i >= x/8 && i <(x+w)/8 && m >= y && m < y+l){
				
				temp3=0;
				for(j=0;j<2;j++)	
				{
					temp1 = pgm_read_byte(&Image[z*2+j]);
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
				z++;
				SendData(temp3);	
			}else {
				SendData(0xff);	
			}
		}
    
    set4Gray_lut();
    SendCommand(DISPLAY_REFRESH); 
    DelayMs(100);
    WaitUntilIdle();
}
/**
 *  @brief: set the look-up table
 */
void Epd::SetLut(void) {
    unsigned int count;     
    SendCommand(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 36; count++) {
        SendData(lut_vcom0[count]);
    }
    
    SendCommand(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 36; count++) {
        SendData(lut_ww[count]);
    }   
    
    SendCommand(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 36; count++) {
        SendData(lut_bw[count]);
    } 

    SendCommand(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 36; count++) {
        SendData(lut_bb[count]);
    } 

    SendCommand(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 36; count++) {
        SendData(lut_wb[count]);
    } 
}

void Epd::set4Gray_lut(void)
{
	unsigned int count;	 
	{
		SendCommand(0x20);							//vcom
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_vcom[count]);}
		
		SendCommand(0x21);							//red not use
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_ww[count]);}

		SendCommand(0x22);							//bw r
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_bw[count]);}

		SendCommand(0x23);							//wb w
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_wb[count]);}

		SendCommand(0x24);							//bb b
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_bb[count]);}

		SendCommand(0x25);							//vcom
		for(count=0;count<42;count++)
			{SendData(EPD_4IN2_4Gray_lut_ww[count]);}
	}	         
}
/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer) {
    SendCommand(RESOLUTION_SETTING);
    SendData(width >> 8);        
    SendData(width & 0xff);
    SendData(height >> 8);
    SendData(height & 0xff);

    SendCommand(VCM_DC_SETTING);
    SendData(0x12);                   

    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendCommand(0x97);    //VBDF 17|D7 VBDW 97  VBDB 57  VBDF F7  VBDW 77  VBDB 37  VBDR B7

    if (frame_buffer != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);
        for(int i = 0; i < width / 8 * height; i++) {
            SendData(0xFF);      // bit set: white, bit reset: black
        }
        DelayMs(2);
        SendCommand(DATA_START_TRANSMISSION_2); 
        for(int i = 0; i < width / 8 * height; i++) {
            SendData(pgm_read_byte(&frame_buffer[i]));
        }  
        DelayMs(2);                  
    }

    SendCommand(0x12); 
    DelayMs(100);
    WaitUntilIdle();
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(RESOLUTION_SETTING);
    SendData(width >> 8);
    SendData(width & 0xff);
    SendData(height >> 8);        
    SendData(height & 0xff);

    SendCommand(DATA_START_TRANSMISSION_1);           
    DelayMs(2);
    for(int i = 0; i < width / 8 * height; i++) {
        SendData(0xFF);  
    }  
    DelayMs(2);
    SendCommand(DATA_START_TRANSMISSION_2);           
    DelayMs(2);
    for(int i = 0; i < width / 8 * height; i++) {
        SendData(0xFF);  
    }  
    DelayMs(2);
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    SetLut();
    SendCommand(DISPLAY_REFRESH); 
    DelayMs(100);
    WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);                       //border floating    
    SendCommand(VCM_DC_SETTING);          //VCOM to 0V
    SendCommand(PANEL_SETTING);
    DelayMs(100);          

    SendCommand(POWER_SETTING);           //VG&VS to 0V fast
    SendData(0x00);        
    SendData(0x00);        
    SendData(0x00);              
    SendData(0x00);        
    SendData(0x00);
    DelayMs(100);          
                
    SendCommand(POWER_OFF);          //power off
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);         //deep sleep
    SendData(0xA5);
}

const unsigned char lut_vcom0[] =
{
    0x00, 0x08, 0x08, 0x00, 0x00, 0x02,  
  0x00, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,   

};
const unsigned char lut_ww[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
const unsigned char lut_bw[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      
};

const unsigned char lut_bb[] ={
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
             
};

const unsigned char lut_wb[] ={
  0x20, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x10, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            
};

/******************************gray*********************************/
//0~3 gray
const unsigned char EPD_4IN2_4Gray_lut_vcom[] =
{
0x00	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x60	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x00	,0x00	,0x00	,0x01,
0x00	,0x13	,0x0A	,0x01	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00
				
};
//R21
const unsigned char EPD_4IN2_4Gray_lut_ww[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x10	,0x14	,0x0A	,0x00	,0x00	,0x01,
0xA0	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R22H	r
const unsigned char EPD_4IN2_4Gray_lut_bw[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0C	,0x01	,0x03	,0x04	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R23H	w
const unsigned char EPD_4IN2_4Gray_lut_wb[] ={
0x40	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x00	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x99	,0x0B	,0x04	,0x04	,0x01	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};
//R24H	b
const unsigned char EPD_4IN2_4Gray_lut_bb[] ={
0x80	,0x0A	,0x00	,0x00	,0x00	,0x01,
0x90	,0x14	,0x14	,0x00	,0x00	,0x01,
0x20	,0x14	,0x0A	,0x00	,0x00	,0x01,
0x50	,0x13	,0x01	,0x00	,0x00	,0x01,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
0x00	,0x00	,0x00	,0x00	,0x00	,0x00,
};


/* END OF FILE */
