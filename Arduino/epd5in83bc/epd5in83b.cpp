/**
 *  @filename   :   epd5in83b.cpp
 *  @brief      :   Implements for e-paper library
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
#include "epd5in83b.h"

Epd::~Epd()
{
};

Epd::Epd()
{
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void)
{
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    SendCommand(POWER_SETTING);
    SendData(0x37);
    SendData(0x00);

    SendCommand(PANEL_SETTING);
    SendData(0xCF);
    SendData(0x08);

    SendCommand(BOOSTER_SOFT_START);
    SendData(0xc7);
    SendData(0xcc);
    SendData(0x28);

    SendCommand(PLL_CONTROL);
    SendData(0x3A);

    SendCommand(TEMPERATURE_CALIBRATION);
    SendData(0x00);

    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x77);

    SendCommand(TCON_SETTING);
    SendData(0x22);

    SendCommand(TCON_RESOLUTION);
    SendData(0x02);     //source 600
    SendData(0x58);
    SendData(0x01);     //gate 448
    SendData(0xC0);

    SendCommand(VCM_DC_SETTING);
    SendData(0x20);      //decide by LUT file

    SendCommand(0xe5);           //FLASH MODE
    SendData(0x03);

    SendCommand(POWER_ON);
    WaitUntilIdle();

    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data)
{
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void)
{
    while(DigitalRead(busy_pin) == 0) {      //0: busy, 1: idle
        DelayMs(100);
    }
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(200);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
}

void Epd::DisplayFrame(const unsigned char* image_black, const unsigned char* image_red)
{
    unsigned char Data_Black, Data_Red, Data;
    unsigned int i, j, k;
    SendCommand(DATA_START_TRANSMISSION_1);
    for(i = 0; i < 448; i++) {
        for(j = 0; j < 600 / 8; i++) {
        Data_Black = pgm_read_byte(image_black + i*(600 / 8) + k);
        Data_Red = pgm_read_byte(image_red + i*(600 / 8) + k);
        k = 0;
        while (k < 8) {
            if ((Data_Red & 0x80) == 0x00) {
                Data = 0x04;                //red
            } else if ((Data_Black & 0x80) == 0x00) {
                Data = 0x00;               //black
            } else {
                Data = 0x03;                //white
            }
            Data = (Data << 4) & 0xFF;
            Data_Black = (Data_Black << 1) & 0xFF;
            Data_Red = (Data_Red << 1) & 0xFF;
            k += 1;

            if((Data_Red & 0x80) == 0x00) {
                Data |= 0x04;              //red
            } else if ((Data_Black & 0x80) == 0x00) {
                Data |= 0x00;              //black
            } else {
                Data |= 0x03;              //white
            }
            Data_Black = (Data_Black << 1) & 0xFF;
            Data_Red = (Data_Red << 1) & 0xFF;
            SendData(Data);
            k += 1;
        }
        }
    }
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::Clean(void)
{
    SendCommand(DATA_START_TRANSMISSION_1);
    for (long i = 0; i < 448; i++) {
        for (long j = 0; j < 600 / 2; j++) {
            SendData(0x33);
        }
    }
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

void Epd::DisplayOneQuarterFrame(const unsigned char* image_black, const unsigned char* image_red)
{
    unsigned char Data_Black, Data_Red, Data;
    unsigned int j;
    SendCommand(DATA_START_TRANSMISSION_1);

    for (long i = 0; i < 224; i++) {
        for (long k = 0; k < 38; k++) {//38 => 300 / 8 = 37.5
            Data_Black = pgm_read_byte(image_black + i*38 + k);
            Data_Red = pgm_read_byte(image_red + i*38 + k);
            j = 0;
            if(k == 37) {
                while (j < 4) {
                    if ((Data_Red & 0x80) == 0x00) {
                        Data = 0x04;                //red
                    } else if ((Data_Black & 0x80) == 0x00) {
                        Data = 0x00;               //black
                    } else {
                        Data = 0x03;                //white
                    }
                    Data = (Data << 4) & 0xFF;
                    Data_Black = (Data_Black << 1) & 0xFF;
                    Data_Red = (Data_Red << 1) & 0xFF;
                    j += 1;

                    if((Data_Red & 0x80) == 0x00) {
                        Data |= 0x04;              //red
                    } else if ((Data_Black & 0x80) == 0x00) {
                        Data |= 0x00;              //black
                    } else {
                        Data |= 0x03;              //white
                    }
                    Data_Black = (Data_Black << 1) & 0xFF;
                    Data_Red = (Data_Red << 1) & 0xFF;
                    SendData(Data);
                    j += 1;
                }
            } else {
                while (j < 8) {
                    if ((Data_Red & 0x80) == 0x00) {
                        Data = 0x04;                //red
                    } else if ((Data_Black & 0x80) == 0x00) {
                        Data = 0x00;               //black
                    } else {
                        Data = 0x03;                //white
                    }
                    Data = (Data << 4) & 0xFF;
                    Data_Black = (Data_Black << 1) & 0xFF;
                    Data_Red = (Data_Red << 1) & 0xFF;
                    j += 1;

                    if((Data_Red & 0x80) == 0x00) {
                        Data |= 0x04;              //red
                    } else if ((Data_Black & 0x80) == 0x00) {
                        Data |= 0x00;              //black
                    } else {
                        Data |= 0x03;              //white
                    }
                    Data_Black = (Data_Black << 1) & 0xFF;
                    Data_Red = (Data_Red << 1) & 0xFF;
                    SendData(Data);
                    j += 1;
                }
            }
        }
        for (long k = 0; k < 150; k++) {                // 1/4 show white
            SendData(0x33);
        }
    }
    for (long i = 0; i < 224; i++) {
        for (long k = 0; k < 150; k++) {                 // 1/4 show red
            SendData(0x44);
        }
        for (long k = 0; k < 150; k++) {                 // 1/4 show black
            SendData(0x00);
        }
    }
    SendCommand(DISPLAY_REFRESH);
    DelayMs(100);
    WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5.
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void)
{
    SendCommand(POWER_OFF);
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);
    SendData(0xa5);
}

/* END OF FILE */


