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

#define SYSFS_GPIO_IN  0
#define SYSFS_GPIO_OUT 1

#define SYSFS_GPIO_LOW  0
#define SYSFS_GPIO_HIGH 1

#define NUM_MAXBUF  4
#define DIR_MAXSIZ  60

#define SYSFS_GPIO_DEBUG 0
#if SYSFS_GPIO_DEBUG 
	#define SYSFS_GPIO_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
	#define SYSFS_GPIO_Debug(__info,...)  
#endif 

// BCM GPIO for Jetson nano
#define GPIO4 4 // 7, 4
#define GPIO17 7 // 11, 17
#define GPIO18 18 // 12, 18
#define GPIO27 27 // 13, 27
#define GPIO22 22 // 15, 22
#define GPIO23 23 // 16, 23
#define GPIO24 24 // 18, 24
#define SPI0_MOSI 10 // 19, 10
#define SPI0_MISO 9 // 21, 9
#define GPIO25 28 // 22, 25
#define SPI0_SCK 11 // 23, 11
#define SPI0_CS0 8 // 24, 8
#define SPI0_CS1 7 // 26, 7
#define GPIO5 5 // 29, 5
#define GPIO6 6 // 31, 6
#define GPIO12 12 // 32, 12
#define GPIO13 13 // 33, 13
#define GPIO19 19 // 35, 19
#define GPIO16 16 // 36, 16
#define GPIO26 26 // 37, 26
#define GPIO20 20 // 38, 20
#define GPIO21 21 // 40, 21

int SYSFS_GPIO_Export(int Pin);
int SYSFS_GPIO_Unexport(int Pin);
int SYSFS_GPIO_Direction(int Pin, int Dir);
int SYSFS_GPIO_Read(int Pin);
int SYSFS_GPIO_Write(int Pin, int value);

#endif