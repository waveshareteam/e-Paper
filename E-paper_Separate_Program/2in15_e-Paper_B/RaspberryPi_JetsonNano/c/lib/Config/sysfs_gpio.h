/*****************************************************************************
* | File        :   sysfs_gpio.h
* | Author      :   Waveshare team
* | Function    :   Drive SC16IS752 GPIO
* | Info        :   Read and write /sys/class/gpio
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-04
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
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __SYSFS_GPIO_
#define __SYSFS_GPIO_

#include <stdio.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define NUM_MAXBUF  4
#define DIR_MAXSIZ  60

#define SYSFS_GPIO_DEBUG 1
#if SYSFS_GPIO_DEBUG 
	#define SYSFS_GPIO_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
	#define SYSFS_GPIO_Debug(__info,...)  
#endif 

// BCM GPIO for Jetson nano
#define GPIO4 216 // 7, 4
#define GPIO17 50 // 11, 17
#define GPIO18 79 // 12, 18
#define GPIO27 14 // 13, 27
#define GPIO22 194 // 15, 22
#define GPIO23 232 // 16, 23
#define GPIO24 15 // 18, 24
#define SPI0_MOSI 16 // 19, 10
#define SPI0_MISO 17 // 21, 9
#define GPIO25 13 // 22, 25
#define SPI0_SCK 18 // 23, 11
#define SPI0_CS0 19 // 24, 8
#define SPI0_CS1 20 // 26, 7
#define GPIO5 149 // 29, 5
#define GPIO6 200 // 31, 6
#define GPIO12 168 // 32, 12
#define GPIO13 38 // 33, 13
#define GPIO19 76 // 35, 19
#define GPIO16 51 // 36, 16
#define GPIO26 12 // 37, 26
#define GPIO20 77 // 38, 20
#define GPIO21 78 // 40, 21
// 22PIN + 2PIN UART0 + 2PIN I2C0 + 2PIN I2C
// + 2PIN 3V3 + 2PIN 5V + 8PIN GND  = 40PIN

int SYSFS_GPIO_Export(int Pin);
int SYSFS_GPIO_Unexport(int Pin);
int SYSFS_GPIO_Direction(int Pin, int Dir);
int SYSFS_GPIO_Read(int Pin);
int SYSFS_GPIO_Write(int Pin, int value);

#endif