/*****************************************************************************
* | File      	:   EPD_5in83_V2_test.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-09
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
#include "../e-Paper/EPD_5in83_V2.h"
#include "time.h"

int EPD_5in83_V2_test(void)
{
    Debug("EPD_5IN83_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
    }

    Debug("e-Paper Init and Clear...\r\n");
    EPD_5in83_V2_Init();
    EPD_5in83_V2_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UDOUBLE Imagesize = ((EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1)) * EPD_5in83_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize/2)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        while(1);
    }
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_5in83_V2_WIDTH, EPD_5in83_V2_HEIGHT/2, 0, WHITE);    

#if 1   // show image for array   
    Debug("show bmp------------------------\r\n");
    EPD_5in83_V2_Display(gImage_5in83_V2);
    DEV_Delay_ms(500);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    EPD_5in83_V2_Init();    
    Debug("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    Debug("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(130, 0, "你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    Debug("EPD_Display\r\n");
    EPD_5in83_V2_Display_Base_fImage(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    
    EPD_5in83_V2_Init_Part();
	Paint_NewImage(BlackImage, Font20.Width * 7, Font20.Height, 0, WHITE);
    Debug("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 20;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(0, 0, Font20.Width * 7, Font20.Height, WHITE);
        Paint_DrawTime(0, 0, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_5in83_V2_Display_Partial(BlackImage, 150, 300, 150 + Font20.Width * 7, 300 + Font20.Height);
        DEV_Delay_ms(1000);//Analog clock 1s
    }

#endif
#if 1 // show image for array
    free(BlackImage);
    Debug("show Gray------------------------\r\n");
    Imagesize = ((EPD_5in83_V2_WIDTH % 4 == 0)? (EPD_5in83_V2_WIDTH / 4 ): (EPD_5in83_V2_WIDTH / 4 + 1)) * EPD_5in83_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize/4)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
    }

    EPD_5in83_V2_Init_4GRAY();
    Debug("4 grayscale display\r\n");
    Paint_NewImage(BlackImage, EPD_5in83_V2_WIDTH, EPD_5in83_V2_HEIGHT/4, 0, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);
    
    Paint_DrawPoint(5, 80, GRAY4, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 90, GRAY4, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 100, GRAY4, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(15, 70, 65, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(65, 70, 15, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(15, 70, 65, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(75, 70, 125, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(40, 95, 20, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(100, 95, 20, GRAY2, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(80, 95, 120, 95, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(100, 75, 100, 115, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(5, 0, "waveshare", &Font16, GRAY4, GRAY1);
    Paint_DrawString_EN(5, 20, "hello world", &Font12, GRAY3, GRAY1);
    Paint_DrawNum(5, 33, 123456789, &Font12, GRAY4, GRAY2);
    Paint_DrawNum(5, 50, 987654321, &Font16, GRAY1, GRAY4);
    Paint_DrawString_CN(130, 0,"你好abc", &Font12CN, GRAY4, GRAY1);
    Paint_DrawString_CN(130, 20,"你好abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(130, 40,"你好abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(130, 60,"你好abc", &Font12CN, GRAY1, GRAY4);
    EPD_5in83_V2_4GrayDisplay_fImage(BlackImage);
    DEV_Delay_ms(3000);
#endif


    Debug("Clear...\r\n");
    EPD_5in83_V2_Init();
    EPD_5in83_V2_Clear();

    Debug("Goto Sleep...\r\n");
    EPD_5in83_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

