/**
 *  @filename   :   epdif_7in5.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Realtek SD3
 */

#include "epdif_7in5.h"
#include <SPI.h>

int EpdIf7in5::_BUSY_Pin;
int EpdIf7in5::_RES_Pin;
int EpdIf7in5::_DC_Pin;
int EpdIf7in5::_CS_Pin;

unsigned char Voltage_Frame_7IN5_V2[]={
  0x6, 0x3F, 0x3F, 0x11, 0x24, 0x7, 0x17,
};

unsigned char LUT_VCOM_7IN5_V2[]={  
  0x0,  0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0xF,  0x1,  0xF,  0x1,  0x2,  
  0x0,  0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
};            

unsigned char LUT_WW_7IN5_V2[]={  
  0x10, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x84, 0xF,  0x1,  0xF,  0x1,  0x2,  
  0x20, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
};

unsigned char LUT_BW_7IN5_V2[]={  
  0x10, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x84, 0xF,  0x1,  0xF,  0x1,  0x2,  
  0x20, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
};

unsigned char LUT_WB_7IN5_V2[]={  
  0x80, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x84, 0xF,  0x1,  0xF,  0x1,  0x2,  
  0x40, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
};

unsigned char LUT_BB_7IN5_V2[]={  
  0x80, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x84, 0xF,  0x1,  0xF,  0x1,  0x2,  
  0x40, 0xF,  0xF,  0x0,  0x0,  0x1,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  
};

EpdIf7in5::EpdIf7in5(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin) {
    _BUSY_Pin = BUSY_Pin;
    _RES_Pin = RES_Pin;
    _DC_Pin = DC_Pin;
    _CS_Pin = CS_Pin;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

EpdIf7in5::~EpdIf7in5() {
};

void EpdIf7in5::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

int EpdIf7in5::DigitalRead(int pin) {
    return digitalRead(pin);
}

void EpdIf7in5::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EpdIf7in5::SpiTransfer(unsigned char data) {
    digitalWrite(_CS_Pin, LOW);
    SPI.transfer(data);
    digitalWrite(_CS_Pin, HIGH);
}

int EpdIf7in5::IfInit(void) {
    // pinMode(_CS_PIN, OUTPUT);
    pinMode(_RES_Pin, OUTPUT);
    pinMode(_DC_Pin, OUTPUT);
    pinMode(_BUSY_Pin, INPUT); 
    SPI.begin();
    SPI.beginTransaction(SPISettings(7000000, MSBFIRST, SPI_DATA_MODE0)); // 7M is the sample rate
    return 0;
}

int EpdIf7in5::Init(void) {
    if (IfInit() != 0) {
        return -1; 
    }
    printf("Reset\r\n");
    Reset();

    SendCommand(0x01);  // power setting
    SendData(0x17);  // 1-0=11: internal power
    SendData(*(Voltage_Frame_7IN5_V2+6));  // VGH&VGL
    SendData(*(Voltage_Frame_7IN5_V2+1));  // VSH
    SendData(*(Voltage_Frame_7IN5_V2+2));  //  VSL
    SendData(*(Voltage_Frame_7IN5_V2+3));  //  VSHR
    
    SendCommand(0x82);  // VCOM DC Setting
    SendData(*(Voltage_Frame_7IN5_V2+4));  // VCOM

    SendCommand(0x06);  // Booster Setting
    SendData(0x27);
    SendData(0x27);
    SendData(0x2F);
    SendData(0x17);
    
    SendCommand(0x30);   // OSC Setting
    SendData(*(Voltage_Frame_7IN5_V2+0));  // 2-0=100: N=4  ; 5-3=111: M=7  ;  3C=50Hz     3A=100HZ

    SendCommand(0x04); //POWER ON
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0X00); //PANNEL SETTING
    SendData(0x3F);   //KW-3f   KWR-2F  BWROTP 0f BWOTP 1f

    SendCommand(0x61);  //tres
    SendData(0x03);   //source 800
    SendData(0x20);
    SendData(0x01);   //gate 480
    SendData(0xE0);

    SendCommand(0X15);
    SendData(0x00);

    SendCommand(0X50);  //VCOM AND DATA INTERVAL SETTING
    SendData(0x10);
    SendData(0x00);

    SendCommand(0X60);  //TCON SETTING
    SendData(0x22);

    SendCommand(0x65);  // Resolution setting
    SendData(0x00);
    SendData(0x00); //800*480
    SendData(0x00);
    SendData(0x00);

    SetLut_by_host(LUT_VCOM_7IN5_V2, LUT_WW_7IN5_V2, LUT_BW_7IN5_V2, LUT_WB_7IN5_V2, LUT_BB_7IN5_V2);

    // clear 
    SendCommand(0x13);
    for(unsigned long i=0; i<height*width; i++)  {
        SendData(0xff); // black
    }

    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
    
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void EpdIf7in5::SendCommand(unsigned char command) {
    DigitalWrite(_DC_Pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void EpdIf7in5::SendData(unsigned char data) {
    DigitalWrite(_DC_Pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void EpdIf7in5::WaitUntilIdle(void) {
    unsigned char busy;
    Serial.print("e-Paper Busy\r\n ");
    do{
        SendCommand(0x71);
        busy = DigitalRead(_BUSY_Pin);
    }while(busy == 0);
    Serial.print("e-Paper Busy Release\r\n ");
    DelayMs(20);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void EpdIf7in5::Reset(void) {
    DigitalWrite(_RES_Pin, HIGH);
    DelayMs(20); 
    DigitalWrite(_RES_Pin, LOW);                //module reset    
    DelayMs(4);
    DigitalWrite(_RES_Pin, HIGH);
    DelayMs(20);    
}

void EpdIf7in5::DisplayFrame(const unsigned char* frame_buffer) {
    
    SendCommand(0x13);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(~frame_buffer[i + j * width/8]);
        }
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}

/**
 * @brief: This displays the frame data from SRAM
 */
void EpdIf7in5::DisplayFrame(void) {
    SendCommand(DISPLAY_REFRESH); 
    DelayMs(100);
    WaitUntilIdle();
}

void EpdIf7in5::DisplayFrame(const unsigned char* pbuffer, unsigned long xStart, unsigned long yStart,unsigned long Picture_Width,unsigned long Picture_Height) {
    SendCommand(0x13);
    // xStart = xStart/8;
    // xStart = xStart*8;
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
                SendData(~(pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]))) );
                // SendData(0xff);
            }
            
            else 
            {
                SendData(0x00);
            }
        }
    }
    SendCommand(0x12);
    DelayMs(10);
    WaitUntilIdle();
}

void EpdIf7in5::SetLut_by_host(unsigned char* lut_vcom,  unsigned char* lut_ww, unsigned char* lut_bw, unsigned char* lut_wb, unsigned char* lut_bb)
{
  unsigned char count;

  SendCommand(0x20); //VCOM 
  for(count=0; count<42; count++)
    SendData(lut_vcom[count]);

  SendCommand(0x21); //LUTBW
  for(count=0; count<42; count++)
    SendData(lut_ww[count]);

  SendCommand(0x22); //LUTBW
  for(count=0; count<42; count++)
    SendData(lut_bw[count]);

  SendCommand(0x23); //LUTWB
  for(count=0; count<42; count++)
    SendData(lut_wb[count]);

  SendCommand(0x24); //LUTBB
  for(count=0; count<42; count++)
    SendData(lut_bb[count]);
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use EPD_Reset() to awaken
 */
void EpdIf7in5::Sleep(void) {
    SendCommand(0X02);
    WaitUntilIdle();
    SendCommand(0X07);
    SendData(0xA5);
}

void EpdIf7in5::Clear(void) {
    SendCommand(0x10);
    for(unsigned long i=0; i<height*width; i++) {
        SendData(0x00);
    }

    SendCommand(0x13);
    for(unsigned long i=0; i<height*width; i++) {
        SendData(0x00);
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}

/* END OF FILE */