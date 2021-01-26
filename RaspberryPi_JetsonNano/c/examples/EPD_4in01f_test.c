/*****************************************************************************
* | File      	:   EPD_4in01f.c
* | Author      :   Waveshare team
* | Function    :   4.01inch F e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-06
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
#include "EPD_4in01f.h"
#include "EPD_Test.h"
#include <time.h> 

int EPD_4in01f_test(void)
{
    printf("EPD_4in01F_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_4IN01F_Init();
	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_4IN01F_Clear(EPD_4IN01F_WHITE);
	clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);
    DEV_Delay_ms(100);

    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UDOUBLE Imagesize = ((EPD_4IN01F_WIDTH % 2 == 0)? (EPD_4IN01F_WIDTH / 2 ): (EPD_4IN01F_WIDTH / 2 + 1)) * EPD_4IN01F_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    Paint_NewImage(BlackImage, EPD_4IN01F_WIDTH, EPD_4IN01F_HEIGHT, 0, EPD_4IN01F_WHITE);
    Paint_SetScale(7);

#if 1
    printf("show image for array\r\n");
    Paint_Clear(EPD_4IN01F_WHITE);
    GUI_ReadBmp_RGB_7Color("./pic/4in01-1.bmp", 0, 0);
	EPD_4IN01F_Display(BlackImage);
    // EPD_4IN01F_Display_part(BlackImage, 0, 0, 640, 200);
    DEV_Delay_ms(4000); 
#endif
	
#if 1
    Paint_Clear(EPD_4IN01F_WHITE);
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_4IN01F_BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_4IN01F_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_4IN01F_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_4IN01F_BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, EPD_4IN01F_BLACK, EPD_4IN01F_WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, EPD_4IN01F_WHITE, EPD_4IN01F_BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, EPD_4IN01F_BLACK, EPD_4IN01F_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_4IN01F_WHITE, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(300, 0, "你好abc", &Font12CN, EPD_4IN01F_BLACK, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(300, 20, "你好abc", &Font12CN, EPD_4IN01F_GREEN, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(300, 40, "你好abc", &Font12CN, EPD_4IN01F_BLUE, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(300, 60, "你好abc", &Font12CN, EPD_4IN01F_RED, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(300, 80, "你好abc", &Font12CN, EPD_4IN01F_YELLOW, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(300, 100, "你好abc", &Font12CN, EPD_4IN01F_ORANGE, EPD_4IN01F_WHITE);
    Paint_DrawString_CN(150, 0, "微雪电子", &Font24CN, EPD_4IN01F_WHITE, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 40, "微雪电子", &Font24CN, EPD_4IN01F_GREEN, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 80, "微雪电子", &Font24CN, EPD_4IN01F_BLUE, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 120, "微雪电子", &Font24CN, EPD_4IN01F_RED, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 160, "微雪电子", &Font24CN, EPD_4IN01F_YELLOW, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 200, "微雪电子", &Font24CN, EPD_4IN01F_ORANGE, EPD_4IN01F_BLACK);
    Paint_DrawString_CN(150, 240, "微雪电子", &Font24CN, EPD_4IN01F_BLACK, EPD_4IN01F_YELLOW);

    EPD_4IN01F_Display(BlackImage);
    DEV_Delay_ms(4000); 
#endif
    printf("e-Paper Clear...\r\n");
    EPD_4IN01F_Clear(EPD_4IN01F_WHITE);
    DEV_Delay_ms(1000); 
	
	printf("e-Paper Sleep...\r\n");
    EPD_4IN01F_Sleep();

    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}
