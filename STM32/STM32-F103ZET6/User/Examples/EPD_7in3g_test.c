/*****************************************************************************
* | File      	:   EPD_7in3g_test.c
* | Author      :   Waveshare team
* | Function    :   7.3inch e-paper (G) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-07-22
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
#include "EPD_Test.h"
#include "EPD_7in3g.h"
#include "time.h"

int EPD_test(void)
{
    printf("EPD_7IN3G_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_7IN3G_Init();
    EPD_7IN3G_Clear(EPD_7IN3G_WHITE);
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UDOUBLE Imagesize = ((EPD_7IN3G_WIDTH % 4 == 0)? (EPD_7IN3G_WIDTH / 4 ): (EPD_7IN3G_WIDTH / 4 + 1)) * EPD_7IN3G_HEIGHT;
		Imagesize = Imagesize/4;
		printf("Not enough memory, only part of the window is displayed\r\n");
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_7IN3G_WIDTH/2, EPD_7IN3G_HEIGHT/2 , 0, EPD_7IN3G_WHITE);
    Paint_SetScale(4);

#if 1   // show image for array
    printf("show image for array\r\n");
    EPD_7IN3G_Display(gImage_7in3g);
    DEV_Delay_ms(2000);
#endif   

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_7IN3G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_7IN3G_WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_7IN3G_YELLOW, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_7IN3G_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_7IN3G_WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_7IN3G_WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_7IN3G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_7IN3G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_7IN3G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_7IN3G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_7IN3G_WHITE, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_7IN3G_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "Red, yellow, white and black", &Font16, EPD_7IN3G_WHITE, EPD_7IN3G_YELLOW);
    Paint_DrawString_EN(10, 35, "Four color e-Paper", &Font12, EPD_7IN3G_YELLOW, EPD_7IN3G_BLACK);
    Paint_DrawString_CN(10, 125, "微雪电子", &Font24CN, EPD_7IN3G_RED, EPD_7IN3G_WHITE);
    Paint_DrawNum(10, 50, 123456, &Font12, EPD_7IN3G_RED, EPD_7IN3G_WHITE);

    printf("EPD_Display\r\n");
    EPD_7IN3G_Display_part(BlackImage, 200, 120, 400, 240);
    DEV_Delay_ms(3000);
#endif

    printf("Clear...\r\n");
    EPD_7IN3G_Clear(EPD_7IN3G_WHITE);

    printf("Goto Sleep...\r\n");
    EPD_7IN3G_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

