/*****************************************************************************
* | File      	:   EPD_3IN7_test.c
* | Author      :   Waveshare team
* | Function    :   3.7inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-16
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
#include "EPD_3in7.h"
#include <time.h> 

int EPD_3in7_test(void)
{
    printf("EPD_3IN7_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_3IN7_4Gray_Init();

    struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_3IN7_4Gray_Clear();
    clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);

    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;

    // Allocate enough for for 4 bits for each pixel (16 gray)
    // If you don't use the dithering, this will only need to be 2 bits per pixel
    // you may have to edit the startup_stm32fxxx.s file and set a big enough heap size
    int Imagesize = (EPD_3IN7_WIDTH + 1) / 2 * EPD_3IN7_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        return -1;
    }

    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 270, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);
    
#if 1   // show bmp    
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);

    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 50, 50);
    EPD_3IN7_4Gray_Display(BlackImage);
    DEV_Delay_ms(4000);
    
    printf("show 4 gray bmp------------------------\r\n");
    Paint_Clear(WHITE);
    GUI_ReadBmp_4Gray("./pic/3in7_Scale.bmp", 0, 0);
    EPD_3IN7_4Gray_Display(BlackImage);
    DEV_Delay_ms(4000);
#endif

#if 1 // Drawing on the image, partial display
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_SetScale(4);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
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
    Paint_DrawString_EN(10, 150, "GRAY1 with black background", &Font24, BLACK, GRAY1);
    Paint_DrawString_EN(10, 175, "GRAY2 with white background", &Font24, WHITE, GRAY2);
    Paint_DrawString_EN(10, 200, "GRAY3 with white background", &Font24, WHITE, GRAY3);
    Paint_DrawString_EN(10, 225, "GRAY4 with white background", &Font24, WHITE, GRAY4);
    printf("EPD_Display\r\n");
    EPD_3IN7_4Gray_Display(BlackImage);
    DEV_Delay_ms(4000);
#endif

#if 1 // partial update, just 1 Gray mode
    Paint_NewImage(BlackImage, 50, 120, 270, WHITE);
    EPD_3IN7_1Gray_Init();       //init 1 Gray mode
    EPD_3IN7_1Gray_Clear();
    Paint_SelectImage(BlackImage);
    Paint_SetScale(2);
    Paint_Clear(WHITE);
    printf("show time, partial update, just 1 Gary mode\r\n");
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 15;
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
	    Paint_Clear(WHITE);
		Paint_DrawRectangle(1, 1, 120, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawTime(10, 15, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }

        printf("Part refresh...\r\n");
        EPD_3IN7_1Gray_Display_Part(BlackImage, 40, 30, 90, 150); // Xstart must be a multiple of 8
        DEV_Delay_ms(500);
    }

#if 1   // show dithered bmp
    EPD_3IN7_4Gray_Init(); // Clear
    EPD_3IN7_4Gray_Clear();

    EPD_3IN7_1Gray_Init();       //init 1 Gray mode
    EPD_3IN7_1Gray_Clear();

    Paint_NewImage(BlackImage, 280, 480, 0, WHITE);
    Paint_SetScale(16);
    Paint_Clear(WHITE);

    printf("show dithered BMP with Sierra-Lite\r\n");
    GUI_ReadBmp_16Gray("./pic/3in7_4bit1.bmp", 0, 0);
    EPD_3IN7_1Gray_Display_Dithered(BlackImage, 0);
    DEV_Delay_ms(4000);

    printf("show dithered BMP with Floyd-Steinberg\r\n");
    EPD_3IN7_1Gray_Display_Dithered(BlackImage, 1);
    DEV_Delay_ms(4000);
#endif

#endif
    EPD_3IN7_4Gray_Init();
    printf("Clear...\r\n");
    EPD_3IN7_4Gray_Clear();
    
    // Sleep & close 5V
    printf("Goto Sleep...\r\n");
    EPD_3IN7_Sleep();

    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;
}

