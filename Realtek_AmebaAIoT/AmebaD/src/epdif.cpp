/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Realtek SD3
 *
 *  Copyright (C) Realtek     September 9 2017
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
#include "epdif.h"

#include <SPI.h>
#include <avr/pgmspace.h>

#include "image.h"

// Partial Update Delay, may have an influence on degradation
#define GxGDEH029A1_PU_DELAY 300

const unsigned char LUT_DATA[] PROGMEM = {
    0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char LUT_DATA_part[] PROGMEM = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

EpdIf::EpdIf(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin, int SCK_Pin, int SDI_Pin) {
    _BUSY_Pin = BUSY_Pin;
    _RES_Pin = RES_Pin;
    _DC_Pin = DC_Pin;
    _CS_Pin = CS_Pin;
    _SCK_Pin = SCK_Pin;
    _SDI_Pin = SDI_Pin;
};

EpdIf::~EpdIf(){};


void EpdIf::driver_delay_us(unsigned int xus) {
    for (; xus > 1; xus--)
        ;
}

void EpdIf::driver_delay_xms(unsigned long xms) {  //1ms
    unsigned long i = 0, j = 0;
    for (j = 0; j < xms; j++) {
        for (i = 0; i < 256; i++)
            ;
    }
}

void EpdIf::DELAY_S(unsigned int delaytime) {
    unsigned int i, j, k;
    for (i = 0; i < delaytime; i++) {
        for (j = 0; j < 4000; j++) {
            for (k = 0; k < 222; k++)
                ;
        }
    }
}

/**
    @brief: basic function for SPI delay
*/
void EpdIf::SpiDelay(unsigned char xrate) {
    unsigned char i;
    while (xrate) {
        for (i = 0; i < 2; i++)
            ;
        xrate--;
    }
}

/**
    @brief: basic function for transfering data
*/
void EpdIf::SpiTransfer(unsigned char value) {
    unsigned char i;
    SpiDelay(1);
    for (i = 0; i < 8; i++) {
        EPD_W21_CLK_0();
        SpiDelay(1);
        if (value & 0x80)
            EPD_W21_MOSI_1();
        else
            EPD_W21_MOSI_0();
        value = (value << 1);
        SpiDelay(1);
        driver_delay_us(1);
        EPD_W21_CLK_1();
        SpiDelay(1);
    }

    //  digitalWrite(CS_PIN, LOW);
    // data=transfer(&data, 1);
    // digitalWrite(CS_PIN, HIGH);
}

/**
    @brief: basic function for sending commands
*/
void EpdIf::SendCommand(unsigned char command) {
    SpiDelay(1);
    EPD_W21_DC_0();  // set DC pin to LOW
    EPD_W21_CS_0();
    SpiTransfer(command);
    EPD_W21_CS_1();
}

/**
    @brief: basic function for sending data
*/
void EpdIf::SendData(unsigned char command) {
    SpiDelay(1);
    EPD_W21_DC_1();  // command write
    EPD_W21_CS_0();
    SpiTransfer(command);
    EPD_W21_CS_1();
}

/**
    @brief: Wait until the busy_pin goes idle --> LOW
*/
void EpdIf::EPD_Busy(void) {
    while (1) {  //=1 BUSY
        if (isEPD_W21_BUSY() == 0) break;
    }
}

/**
    @brief: module reset.
            often used to awaken the module in deep sleep,
            see Epd::Sleep();
*/
void EpdIf::EPD_Reset(void) {
    EPD_W21_RST_0();  // module reset
    delay(1);
    EPD_W21_RST_1();
    delay(1);
}

/**
    @brief: After this command is transmitted, the chip would enter the
            deep-sleep mode to save power.
            The deep sleep mode would return to standby by hardware reset.
            You can use Epd::Init() to awaken
*/
void EpdIf::EPD_Sleep(void) {
    SendCommand(DEEP_SLEEP_MODE);
    SendData(0x01);
    delay(100);
    EPD_Busy();
}

/**
    @brief: update the display
            there are 2 memory areas embedded in the e-paper display
            but once this function is called,
            the the next action of SetFrameMemory or ClearFrame will
            set the other memory area.
*/
void EpdIf::EPD_UpdateDisplay(void) {
    SendCommand(DISPLAY_UPDATE_CONTROL_2);  //Display Update Control
    SendData(0xC4);
    SendCommand(MASTER_ACTIVATION);           //Activate Display Update Sequence
    SendCommand(TERMINATE_FRAME_READ_WRITE);  //Activate Display Update Sequence
    this->EPD_Busy();
}

/**
    @brief: basic function for hardware initialization
*/
void EpdIf::EPD_Init(void) {
    EPD_W21_RST_0();  // Module reset
    delay(1);       //At least 10ms delay
    EPD_W21_RST_1();
    delay(1);  //At least 10ms delay

    SendCommand(DRIVER_OUTPUT_CONTROL);
    SendData((EPD_HEIGHT - 1) & 0xFF);         // 0x27
    SendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);  // 0x01
    SendData(0x00);                            // GD = 0; SM = 0; TB = 0;8i8i
    SendCommand(BOOSTER_SOFT_START_CONTROL);
    SendData(0xD7);
    SendData(0xD6);
    SendData(0x9D);
    SendCommand(WRITE_VCOM_REGISTER);
    SendData(0xA8);  // 0xA8 or 0x9A
    SendCommand(SET_DUMMY_LINE_PERIOD);
    SendData(0x1A);  // 4 dummy lines per gate
    SendCommand(SET_GATE_TIME);
    SendData(0X08);  // 2us per line
    SendCommand(DATA_ENTRY_MODE_SETTING);
    SendData(0x01);  // X increment; Y increment 0x01 display image 0x03 display text
    // SendCommand(BORDER_WAVEFORM_CONTROL);
    // SendData(0x33);
    // comment out lines below still workable

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  //set Ram-X address start/end position
    SendData(0x00);
    SendData(0x0F);                                     //0x0C-->(15+1)*8=128
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);  //set Ram-Y address start/end position
    SendData(0x27);                                     //Y%256--->(295+1)=296
    SendData(0x01);                                     //Y%256
    SendData(0x00);
    SendData(0x00);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);  // set RAM x address count to 0;
    SendData(0x00);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);  // set RAM y address count to 0X127;
    SendData(0x27);                          //Y%256
    SendData(0x01);                          //Y/256
    this->EPD_Busy();
    this->SetLUT(LUT_DATA);
    /* EPD hardware init end */
}

void EpdIf::EPD_InitText(void) {
    EPD_W21_RST_0();  // Module reset
    delay(1);       //At least 10ms delay
    EPD_W21_RST_1();
    delay(1);  //At least 10ms delay

    SendCommand(DRIVER_OUTPUT_CONTROL);
    SendData((EPD_HEIGHT - 1) & 0xFF);         // 0x27
    SendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);  // 0x01
    SendData(0x00);                            // GD = 0; SM = 0; TB = 0;8i8i
    SendCommand(BOOSTER_SOFT_START_CONTROL);
    SendData(0xD7);
    SendData(0xD6);
    SendData(0x9D);
    SendCommand(WRITE_VCOM_REGISTER);
    SendData(0x9A);
    SendCommand(SET_DUMMY_LINE_PERIOD);
    SendData(0x1A);  // 4 dummy lines per gate
    SendCommand(SET_GATE_TIME);
    SendData(0X08);  // 2us per line
    SendCommand(DATA_ENTRY_MODE_SETTING);
    SendData(0x03);  // X increment; Y increment
    // SendCommand(BORDER_WAVEFORM_CONTROL);
    // SendData(0x33);

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  //set Ram-X address start/end position
    SendData(0x00);
    SendData(0x0F);                                     //0x0C-->(15+1)*8=128
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);  //set Ram-Y address start/end position
    SendData(0x27);                                     //Y%256--->(295+1)=296
    SendData(0x01);                                     //Y%256
    SendData(0x00);
    SendData(0x00);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);  // set RAM x address count to 0;
    SendData(0x00);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);  // set RAM y address count to 0X127;
    SendData(0x27);                          //Y%256
    SendData(0x01);                          //Y/256
    this->EPD_Busy();
    this->SetLUT(LUT_DATA);
    /* EPD hardware init end */
}

void EpdIf::EPD_Part_Init(void) {
    EPD_Init();
    this->SetLUT(LUT_DATA_part);
    //POWER ON
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xC0);
    SendCommand(MASTER_ACTIVATION);  //Activate Display Update Sequence
    this->EPD_Busy();
    SendCommand(BORDER_WAVEFORM_CONTROL);  //BorderWavefrom
    SendData(0x01);
}

void EpdIf::EPD_Part_Update(void) {
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0x04);  //different
    SendCommand(MASTER_ACTIVATION);
    this->EPD_Busy();
}

void EpdIf::EPD_SetRAMValue_BaseMap(const unsigned char *datas) {
    unsigned int i;
    const unsigned char *datas_flag;
    unsigned int x_end, y_start1, y_start2, y_end1, y_end2;
    unsigned int x_start = 0, y_start = 296;
    unsigned int PART_COLUMN = 296, PART_LINE = 128;
    datas_flag = datas;

    //FULL update
    EPD_Init();
    SendCommand(WRITE_RAM);  //Write Black and White image to RAM
    for (i = 0; i < ALLSCREEN_GRAGHBYTES; i++) {
        SendData(pgm_read_byte(&datas[i]));
    }
    this->EPD_UpdateDisplay();
    delay(100);
    //PART update
    EPD_Part_Init();        //Local initialization (the top left corner of the screen is the origin of the coordinates)
    delay(10); 
    datas = datas_flag;     //Record array first address
    x_start = x_start / 8;  //Convert to byte
    x_end = (x_start + PART_LINE) / 8 - 1;
    y_start1 = 0;
    y_start2 = y_start - 1;

    if (y_start >= 256) {
        y_start1 = y_start2 / 256;
        y_start2 = y_start2 % 256;
    }
    y_end1 = 0;  //0
    y_end2 = 0;  //0
    if (y_end2 >= 256) {
        y_end1 = y_end2 / 256;
        y_end2 = y_end2 % 256;
    }

    // SetMemoryArea(x_start, y_start, x_end, y_end);
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  // set RAM x address start/end, in page 35
    SendData(x_start);                                  // RAM x address start at 00h;
    SendData(x_end);                                    // RAM x address end at 0fh(15+1)*8->128
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);  // set RAM y address start/end, in page 35
    SendData(y_start2);                                 // RAM y address start at 0127h;
    SendData(y_start1);                                 // RAM y address start at 0127h;
    SendData(y_end2);                                   // RAM y address end at 00h;
    SendData(y_end1);                                   // ????=0

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);  // set RAM x address count to 0;
    SendData(x_start);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);  // set RAM y address count to 0X127;
    SendData(y_start2);
    SendData(y_start1);
    SendCommand(WRITE_RAM);  //Write Black and White image to RAM
    for (i = 0; i < PART_COLUMN * PART_LINE / 8; i++) {
        SendData(pgm_read_byte(&datas[i]));
    }
    this->EPD_Part_Update();
    delay(100);

    datas = datas_flag;
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  // set RAM x address start/end, in page 35
    SendData(x_start);                                  // RAM x address start at 00h;
    SendData(x_end);                                    // RAM x address end at 0fh(15+1)*8->128
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);  // set RAM y address start/end, in page 35
    SendData(y_start2);                                 // RAM y address start at 0127h;
    SendData(y_start1);                                 // RAM y address start at 0127h;
    SendData(y_end2);                                   // RAM y address end at 00h;
    SendData(y_end1);                                   // ????=0
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);             // set RAM x address count to 0;
    SendData(x_start);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);  // set RAM y address count to 0X127;
    SendData(y_start2);
    SendData(y_start1);

    SendCommand(WRITE_RAM);  // Write Black and White image to RAM
    for (i = 0; i < PART_COLUMN * PART_LINE / 8; i++) {
        SendData(pgm_read_byte(&datas[i]));
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */

void EpdIf::EPD_Dis_Part(
    unsigned int x_start,
    unsigned int y_start,
    const unsigned char *datas,
    unsigned int PART_COLUMN,  //y height
    unsigned int PART_LINE) {  // x width

    unsigned int x_end, y_start1, y_start2, y_end1, y_end2;
    
    EPD_Part_Init();
    delay(10); 

    if (datas == NULL) {
        return;
    }

    x_start = x_start / 8;  // Convert to byte
    x_end = x_start + PART_LINE / 8 - 1;

    y_start1 = 0;
    y_start2 = y_start - 1;

    if (y_start >= 256) {
        y_start1 = y_start2 / 256;
        y_start2 = y_start2 % 256;
    }
    y_end1 = 0;
    y_end2 = y_start + PART_COLUMN - 1;

    if (y_end2 >= 256) {
        y_end1 = y_end2 / 256;
        y_end2 = y_end2 % 256;
    }

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  // set RAM x address start/end, in page 35
    SendData(x_start);                                  // RAM x address start at 00h;
    SendData(x_end);                                    // RAM x address end at 0fh(15+1)*8->128
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);  // set RAM y address start/end, in page 35
    SendData(y_start2);                                 // RAM y address start at 0127h;
    SendData(y_start1);                                 // RAM y address start at 0127h;
    SendData(y_end2);                                   // RAM y address end at 00h;
    SendData(y_end1);                                   // ????=0

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);  // set RAM x address count to 0;
    SendData(x_start);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);  // set RAM y address count to 0X127;
    SendData(y_start2);
    SendData(y_start1);
    this->EPD_Busy();

    SendCommand(WRITE_RAM);  //Write Black and White image to RAM
                             /* send the image data */
    for (unsigned int i = 0; i < PART_COLUMN * PART_LINE / 8; i++) {
        SendData(pgm_read_byte(&datas[i]));
    }
}

void EpdIf::EPD_SetFrame(
    const unsigned char *image_buffer,
    int x,
    int y,
    int image_width,
    int image_height) {
    int x_end;
    int y_end;

    EPD_InitText();
    delay(10);

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0) {
        return;
    }
    
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= EPD_WIDTH) {
        x_end = EPD_WIDTH - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= EPD_HEIGHT) {
        y_end = EPD_HEIGHT - 1;
    } else {
        y_end = y + image_height - 1;
    }

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);  
    SendData((x >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    this->EPD_Busy();

    SendCommand(WRITE_RAM);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

void EpdIf::EPD_SetFrameMemory(const unsigned char *image_buffer) {
    SendCommand(WRITE_RAM);
    for (int i = 0; i < ALLSCREEN_GRAGHBYTES; i++) {
        SendData(pgm_read_byte(&image_buffer[i]));
    }
    // this->EPD_UpdateDisplay();
}

/**
 *  @brief: clear the frame memory with the White color.
 *          and update the display.
 */
void EpdIf::EPD_ClearScreen_White(void) {
    SendCommand(WRITE_RAM);
    /* send the color data */
    for (int k = 0; k < 296; k++) {
        for (int i = 0; i < 16; i++) {
            SendData(0xff);  // 0xFF All White
        }
    }
    // this->EPD_UpdateDisplay();
}

/**
 *  @brief: clear the frame memory with the Black color.
 *          and update the display.
 */
void EpdIf::EPD_ClearScreen_Black(void) {
    SendCommand(WRITE_RAM);
    /* send the color data */
    for (int k = 0; k < 296; k++) {
        for (int i = 0; i < 16; i++) {
            SendData(0x00);  // 0x00 All Black
        }
    }
    // this->EPD_UpdateDisplay();
}

/**
    @brief: set the Look-Up Table (LUT) register
*/
void EpdIf::SetLUT(const unsigned char *wave_data) {
    SendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < 30; i++)
        SendData(pgm_read_byte(&wave_data[i]));
}
//////////////////END ////////////////////
