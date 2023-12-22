/*****************************************************************************
* | File      	:	EPD_Test.h
* | Author      :   Waveshare team
* | Function    :   e-Paper test Demo
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2022-07-28
* | Info        :   
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
#ifndef _EPD_TEST_H_
#define _EPD_TEST_H_

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "Debug.h"
#include <stdlib.h> // malloc() free()

int EPD_1in64g_test(void);
int EPD_2in36g_test(void);
int EPD_3in0g_test(void);
int EPD_4in37g_test(void);
int EPD_7in3g_test(void);

int EPD_1in54_DES_test(void);
int EPD_2in13_DES_test(void);
int EPD_2in9_DES_test(void);

int EPD_1in02d_test(void);

int EPD_1in54_test(void);
int EPD_1in54_V2_test(void);
int EPD_1in54b_test(void);
int EPD_1in54b_V2_test(void);
int EPD_1in54c_test(void);

int EPD_2in66_test(void);
int	EPD_2in66b_test(void); 
int EPD_2in66g_test(void);

int EPD_2in7_test(void);
int EPD_2in7_V2_test(void);
int EPD_2in7b_test(void);
int EPD_2in7b_V2_test(void);

int EPD_2in9_test(void);
int EPD_2in9_V2_test(void);
int EPD_2in9bc_test(void);
int EPD_2in9b_V3_test(void);
int EPD_2in9b_V4_test(void);
int EPD_2in9d_test(void);

int EPD_2in13_test(void);
int EPD_2in13_V2_test(void);
int EPD_2in13_V3_test(void);
int EPD_2in13_V4_test(void);
int EPD_2in13bc_test(void);
int EPD_2in13b_V3_test(void);
int EPD_2in13b_V4_test(void);
int EPD_2in13d_test(void);
int EPD_2in13g_test(void);

int EPD_3in52_test(void);

int EPD_3in7_test(void);

int EPD_4in01f_test(void);

int EPD_4in2_test(void);
int EPD_4in2_V2_test(void);
int EPD_4in2bc_test(void);
int EPD_4in2b_V2_test(void);

int EPD_4in26_test(void);

int EPD_4in37b_test(void);

int EPD_5in65f_test(void);

int EPD_5in83_test(void);
int EPD_5in83_V2_test(void);
int EPD_5in83bc_test(void);
int EPD_5in83b_V2_test(void);

int EPD_5in84_test(void);

int EPD_7in3f_test(void);

int EPD_7in5_test(void);
int EPD_7in5_HD_test(void);
int EPD_7in5_V2_test(void);
int EPD_7in5_V2_test_old(void);
int EPD_7in5bc_test(void);
int EPD_7in5b_V2_test(void);
int EPD_7in5b_HD_test(void);

int EPD_10in2b_test(void);

int EPD_13in3k_test(void);
#endif
