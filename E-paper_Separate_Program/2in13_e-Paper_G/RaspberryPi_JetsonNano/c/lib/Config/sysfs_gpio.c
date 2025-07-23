/*****************************************************************************
* | File        :   SYSFS_GPIO.c
* | Author      :   Waveshare team
* | Function    :   Drive SYSFS_ GPIO
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
#include "sysfs_gpio.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int SYSFS_GPIO_Export(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Export Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    write(fd, buffer, len);
    
    SYSFS_GPIO_Debug( "Export: Pin%d\r\n", Pin);

    close(fd);
    return 0;
}

int SYSFS_GPIO_Unexport(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "unexport Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    write(fd, buffer, len);
    
    SYSFS_GPIO_Debug( "Unexport: Pin%d\r\n", Pin);
    
    close(fd);
    return 0;
}

int SYSFS_GPIO_Direction(int Pin, int Dir)
{
    const char dir_str[]  = "in\0out";
    char path[DIR_MAXSIZ];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/direction", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Set Direction failed: Pin%d\n", Pin);
        return -1;
    }

    if (write(fd, &dir_str[Dir == IN ? 0 : 3], Dir == IN ? 2 : 3) < 0) {
        SYSFS_GPIO_Debug("failed to set direction!\r\n");
        return -1;
    }

    if(Dir == IN){
        SYSFS_GPIO_Debug("Pin%d:intput\r\n", Pin);
    }else{
        SYSFS_GPIO_Debug("Pin%d:Output\r\n", Pin);
    }
    
    close(fd);
    return 0;
}

int SYSFS_GPIO_Read(int Pin)
{
    char path[DIR_MAXSIZ];
    char value_str[3];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Read failed Pin%d\n", Pin);
        return -1;
    }

    if (read(fd, value_str, 3) < 0) {
        SYSFS_GPIO_Debug( "failed to read value!\n");
        return -1;
    }

    close(fd);
    return(atoi(value_str));
}

int SYSFS_GPIO_Write(int Pin, int value)
{
    const char s_values_str[] = "01";
    char path[DIR_MAXSIZ];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Write failed : Pin%d,value = %d\n", Pin, value);
        return -1;
    }

    if (write(fd, &s_values_str[value == LOW ? 0 : 1], 1) < 0) {
        SYSFS_GPIO_Debug( "failed to write value!\n");
        return -1;
    }
    
    close(fd);
    return 0;
}
