/**
 *  @filename   :   epdif_2in9_v2.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Realtek SD3
 */

#include "epdif_2in9_v2.h"
#include <SPI.h>

int EpdIf2in9V2::_BUSY_Pin;
int EpdIf2in9V2::_RES_Pin;
int EpdIf2in9V2::_DC_Pin;
int EpdIf2in9V2::_CS_Pin;

unsigned char _WF_PARTIAL_2IN9[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0A,0x0,0x0,0x0,0x0,0x0,0x2,  
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x22,0x17,0x41,0xB0,0x32,0x36,
};

unsigned char WS_20_30[159] =
{											
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x14,	0x8,	0x0,	0x0,	0x0,	0x0,	0x1,					
0xA,	0xA,	0x0,	0xA,	0xA,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x14,	0x8,	0x0,	0x1,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x44,	0x44,	0x44,	0x44,	0x44,	0x44,	0x0,	0x0,	0x0,			
0x22,	0x17,	0x41,	0x0,	0x32,	0x36
};	

EpdIf2in9V2::EpdIf2in9V2(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin) {
    _BUSY_Pin = BUSY_Pin;
    _RES_Pin = RES_Pin;
    _DC_Pin = DC_Pin;
    _CS_Pin = CS_Pin;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

EpdIf2in9V2::~EpdIf2in9V2() {
};

void EpdIf2in9V2::DigitalWrite(int pin, int value) {
    digitalWrite(pin, value);
}

int EpdIf2in9V2::DigitalRead(int pin) {
    return digitalRead(pin);
}

void EpdIf2in9V2::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EpdIf2in9V2::SpiTransfer(unsigned char data) {
    SPI.transfer(data);
}

int EpdIf2in9V2::IfInit(void) {
    pinMode(_RES_Pin, OUTPUT);
    pinMode(_DC_Pin, OUTPUT);
    pinMode(_BUSY_Pin, INPUT); 
    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_DATA_MODE0));
    return 0;
}

int EpdIf2in9V2::Init() {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
	
	Reset();
	
    /* EPD hardware init start */
	WaitUntilIdle();   
	SendCommand(0x12);  //SWRESET
	WaitUntilIdle();   
	
	SendCommand(0x01); //Driver output control      
	SendData(0x27);
	SendData(0x01);
	SendData(0x00);
	
	SendCommand(0x11); //data entry mode       
	SendData(0x03);

	SetMemoryArea(0, 0, width-1, height-1);

	SendCommand(0x21); //  Display update control
	SendData(0x00);
	SendData(0x80);	

	SetMemoryPointer(0, 0);
	WaitUntilIdle();

    SetLut_by_host(WS_20_30);
    /* EPD hardware init end */
    return 0;
}

int EpdIf2in9V2::Epdif_Init() {
    /* this calls the peripheral hardware interface, see epdif */
	Reset();
	
    /* EPD hardware init start */
	WaitUntilIdle();   
	SendCommand(0x12);  //SWRESET
	WaitUntilIdle();   
	
	SendCommand(0x01); //Driver output control      
	SendData(0x27);
	SendData(0x01);
	SendData(0x00);
	
	SendCommand(0x11); //data entry mode       
	SendData(0x03);

	SetMemoryArea(0, 0, width-1, height-1);

	SendCommand(0x21); //  Display update control
	SendData(0x00);
	SendData(0x80);	

	SetMemoryPointer(0, 0);
	WaitUntilIdle();

    SetLut_by_host(WS_20_30);
    /* EPD hardware init end */
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void EpdIf2in9V2::SendCommand(unsigned char command) {
    DigitalWrite(_DC_Pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void EpdIf2in9V2::SendData(unsigned char data) {
    DigitalWrite(_DC_Pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void EpdIf2in9V2::WaitUntilIdle(void) {
	while(1) {	 //=1 BUSY
		if(DigitalRead(_BUSY_Pin)==LOW) 
			break;
		DelayMs(5);
	}
	DelayMs(5);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void EpdIf2in9V2::Reset(void) {
    DigitalWrite(_RES_Pin, HIGH);
    DelayMs(20);  
    DigitalWrite(_RES_Pin, LOW);                //module reset    
    DelayMs(5);
    DigitalWrite(_RES_Pin, HIGH); 
    DelayMs(20);  
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void EpdIf2in9V2::SetFrameMemory(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
) {
    int x_end;
    int y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }
    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    SendCommand(0x24);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}
void EpdIf2in9V2::SetFrameMemory_Partial(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
) {
    int x_end;
    int y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }

    DigitalWrite(_RES_Pin, LOW);
    DelayMs(2);
    DigitalWrite(_RES_Pin, HIGH);
    DelayMs(2);
	
	SetLut(_WF_PARTIAL_2IN9);
	SendCommand(0x37); 
	SendData(0x00);  
	SendData(0x00);  
	SendData(0x00);  
	SendData(0x00); 
	SendData(0x00);  	
	SendData(0x40);  
	SendData(0x00);  
	SendData(0x00);   
	SendData(0x00);  
	SendData(0x00);

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x80);	

	SendCommand(0x22); 
	SendData(0xC0);   
	SendCommand(0x20); 
	WaitUntilIdle();  
	
    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    SendCommand(0x24);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of 
 *          void SetFrameMemory(
 *              const unsigned char* image_buffer,
 *              int x,
 *              int y,
 *              int image_width,
 *              int image_height
 *          );
 *          Answer: SetFrameMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers 
 *          from the flash).
 */
void EpdIf2in9V2::SetFrameMemory(const unsigned char* image_buffer) {
    Epdif_Init();
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(0x24);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(pgm_read_byte(&image_buffer[i]));
    }

    SendCommand(0x26);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(pgm_read_byte(&image_buffer[i]));
    }
}

void EpdIf2in9V2::SetFrameMemory_Base(const unsigned char* image_buffer) {
    Epdif_Init();
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(0x24);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(pgm_read_byte(&image_buffer[i]));
    }
    SendCommand(0x26);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(pgm_read_byte(&image_buffer[i]));
    }
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void EpdIf2in9V2::ClearFrameMemory(unsigned char color) {
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(0x24);
    /* send the color data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        SendData(color);
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will 
 *          set the other memory area.
 */
void EpdIf2in9V2::DisplayFrame(void) {
    SendCommand(0x22);
    SendData(0xc7);
    SendCommand(0x20);
    WaitUntilIdle();
}

void EpdIf2in9V2::DisplayFrame_Partial(void) {
    SendCommand(0x22);
    SendData(0x0F);
    SendCommand(0x20);
    WaitUntilIdle();
}

void EpdIf2in9V2::SetLut(unsigned char *lut) {       
	unsigned char count;
	SendCommand(0x32);
	for(count=0; count<153; count++) 
		SendData(lut[count]); 
	WaitUntilIdle();
}

void EpdIf2in9V2::SetLut_by_host(unsigned char *lut) {
    SetLut((unsigned char *)lut);
	SendCommand(0x3f);
	SendData(*(lut+153));
	SendCommand(0x03);	// gate voltage
	SendData(*(lut+154));
	SendCommand(0x04);	// source voltage
	SendData(*(lut+155));	// VSH
	SendData(*(lut+156));	// VSH2
	SendData(*(lut+157));	// VSL
	SendCommand(0x2c);		// VCOM
	SendData(*(lut+158));
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void EpdIf2in9V2::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(0x45);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void EpdIf2in9V2::SetMemoryPointer(int x, int y) {
    SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(0x4F);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          You can use Epd::Init() to awaken
 */
void EpdIf2in9V2::Sleep() {
    SendCommand(0x10);
    SendData(0x01);
    WaitUntilIdle();
}

/* END OF FILE */
