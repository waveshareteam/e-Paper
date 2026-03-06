/*****************************************************************************
* | File        :   sysfs_software_spi.h
* | Author      :   Waveshare team
* | Function    :   Read and write /sys/class/gpio, software spi
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-05
* | Info        :   Basic version
*
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
# Read_data OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "sysfs_software_spi.h"

SOFTWARE_SPI software_spi;

/******************************************************************************
function:
parameter:
Info:
******************************************************************************/
void SYSFS_software_spi_begin(void)
{
    // gpio
    software_spi.SCLK_PIN = SPI0_SCK;
    software_spi.MOSI_PIN = SPI0_MOSI;
    software_spi.MISO_PIN = SPI0_MISO;

    //software spi configure
    software_spi.Mode = SOFTWARE_SPI_Mode0;
    software_spi.Type = SOFTWARE_SPI_Master;
    software_spi.Delay = SOFTWARE_SPI_CLOCK_DIV2;
    software_spi.Order = SOFTWARE_SPI_MSBFIRST; // MSBFIRST

    SYSFS_GPIO_Export(software_spi.SCLK_PIN);
    SYSFS_GPIO_Export(software_spi.MOSI_PIN);
    SYSFS_GPIO_Export(software_spi.MISO_PIN);

    SYSFS_GPIO_Direction(software_spi.SCLK_PIN, OUT);
    SYSFS_GPIO_Direction(software_spi.MOSI_PIN, OUT);
    SYSFS_GPIO_Direction(software_spi.MISO_PIN, IN);
}

void SYSFS_software_spi_end(void)
{
    SYSFS_GPIO_Write(software_spi.SCLK_PIN, LOW);
    SYSFS_GPIO_Write(software_spi.MOSI_PIN, LOW);

    SYSFS_GPIO_Unexport(software_spi.SCLK_PIN);
    SYSFS_GPIO_Unexport(software_spi.MOSI_PIN);
}

void SYSFS_software_spi_setBitOrder(uint8_t order)
{
    software_spi.Order = order & 1;
}

void SYSFS_software_spi_setDataMode(uint8_t mode)
{
    if(mode > 4) {
        SYSFS_SOFTWARE_SPI_Debug("MODE must be 0-3\r\n");
        return;
    }
    software_spi.Mode = mode;

    switch (software_spi.Mode) {
    case SOFTWARE_SPI_Mode0:
        software_spi.CPOL = 0;
        software_spi.CPHA = 0;
        break;
    case SOFTWARE_SPI_Mode1:
        software_spi.CPOL = 0;
        software_spi.CPHA = 1;
        break;
    case SOFTWARE_SPI_Mode2:
        software_spi.CPOL = 1;
        software_spi.CPHA = 0;
        break;
    case SOFTWARE_SPI_Mode3:
        software_spi.CPOL = 1;
        software_spi.CPHA = 1;
        break;
    }
}

void SYSFS_software_spi_setClockDivider(uint8_t div)
{
    if(div > 8) {
        SYSFS_SOFTWARE_SPI_Debug("div must be 0-7\r\n");
        return;
    }
    switch (div) {
    case SOFTWARE_SPI_CLOCK_DIV2:
        software_spi.Delay = 2;
        break;
    case SOFTWARE_SPI_CLOCK_DIV4:
        software_spi.Delay = 4;
        break;
    case SOFTWARE_SPI_CLOCK_DIV8:
        software_spi.Delay = 8;
        break;
    case SOFTWARE_SPI_CLOCK_DIV16:
        software_spi.Delay = 16;
        break;
    case SOFTWARE_SPI_CLOCK_DIV32:
        software_spi.Delay = 32;
        break;
    case SOFTWARE_SPI_CLOCK_DIV64:
        software_spi.Delay = 64;
        break;
    case SOFTWARE_SPI_CLOCK_DIV128:
        software_spi.Delay = 128;
        break;
    default:
        software_spi.Delay = 128;
        break;
    }
}

/******************************************************************************
function:	SPI Mode 0
parameter:
Info:
******************************************************************************/
uint8_t SYSFS_software_spi_transfer(uint8_t value)
{
    // printf("value = %d\r\n", value);
    uint8_t Read_data;
    if (software_spi.Order == SOFTWARE_SPI_LSBFIRST) {
        uint8_t temp =
            ((value & 0x01) << 7) |
            ((value & 0x02) << 5) |
            ((value & 0x04) << 3) |
            ((value & 0x08) << 1) |
            ((value & 0x10) >> 1) |
            ((value & 0x20) >> 3) |
            ((value & 0x40) >> 5) |
            ((value & 0x80) >> 7);
        value = temp;
    }

    uint8_t delay = software_spi.Delay >> 1;
    for(int j=delay; j > 0; j--);

    // printf("value = %d\r\n", value);
    uint8_t Read_miso = 0;
    
    SYSFS_GPIO_Write(software_spi.SCLK_PIN, 0);
    for (uint8_t bit = 0; bit < 8; bit++) {        
        SYSFS_GPIO_Write(software_spi.SCLK_PIN, 0);
        // for(int j=delay; j > 0; j--);// DELAY

        if (software_spi.CPHA) {
            Read_miso = SYSFS_GPIO_Read(software_spi.MISO_PIN);
            if (software_spi.Order == SOFTWARE_SPI_LSBFIRST) {
                Read_data <<= 1;
                Read_data |= Read_miso;
            } else {
                Read_data >>= 1;
                Read_data |= Read_miso << 7;
            }
        } else {
            SYSFS_GPIO_Write(software_spi.MOSI_PIN, ((value<<bit) & 0x80) ? HIGH : LOW);
        }

        // for(int j=delay; j > 0; j--);// DELAY
        SYSFS_GPIO_Write(software_spi.SCLK_PIN, 1);
        // for(int j=delay; j > 0; j--);// DELAY

        if (software_spi.CPHA) {
            SYSFS_GPIO_Write(software_spi.MOSI_PIN, ((value<<bit) & 0x80) ? HIGH : LOW);
        } else {
            Read_miso = SYSFS_GPIO_Read(software_spi.MISO_PIN);
            if (software_spi.Order == SOFTWARE_SPI_LSBFIRST) {
                Read_data <<= 1;
                Read_data |= Read_miso;
            } else {
                Read_data >>= 1;
                Read_data |= Read_miso << 7;
            }
        }

        // for(int j=delay; j > 0; j--);// DELAY
    }
    return Read_data;
}
