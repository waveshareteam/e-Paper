/*****************************************************************************
* | File      	:   EPD_2in9b_V4_test.c
* | Author      :   Waveshare team
* | Function    :   2.9inch B V4 e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-12-18
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
#include "EPD_2in9b_V4.h"
#include <time.h> 
 
int EPD_2in9b_V4_test(void)
{
    printf("EPD_2IN9B_V4_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN9B_V4_Init();
    
    struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_2IN9B_V4_Clear();
    clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);

    DEV_Delay_ms(500);
    

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage; // Red or Yellow
    UWORD Imagesize = ((EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1)) * EPD_2IN9B_V4_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN9B_V4_WIDTH, EPD_2IN9B_V4_HEIGHT, 270, WHITE);
    Paint_NewImage(RYImage, EPD_2IN9B_V4_WIDTH, EPD_2IN9B_V4_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    
#if 1   // show bmp
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/100x100.bmp", 50, 10);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);    
    EPD_2IN9B_V4_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);

    printf("show red bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/2in9bc-b.bmp", 0, 0);
    Paint_SelectImage(RYImage);
    GUI_ReadBmp("./pic/2in9bc-ry.bmp", 0, 0);
    EPD_2IN9B_V4_Display(BlackImage, RYImage);
    // DEV_Delay_ms(2000);

#endif

#if 1  // show image for array    
    EPD_2IN9B_V4_Init_Fast();
    printf("show image for array\r\n");
    EPD_2IN9B_V4_Display_Fast(gImage_2in9bc_b, gImage_2in9bc_ry);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
    EPD_2IN9B_V4_Init();
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    //2.Draw red image
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    // Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(130, 0,"���abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

    printf("EPD_Display\r\n");
    EPD_2IN9B_V4_Display_Base(BlackImage, RYImage);
    // EPD_2IN9B_V4_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    	
    // If you didn't use the EPD_2IN9B_V4_Display_Base() function to refresh the image before,
    // use the EPD_2IN9B_V4_Display_Base_color() function to refresh the background color, 
    // otherwise the background color will be garbled 
    // EPD_2IN9B_V4_Init();
    // EPD_2IN9B_V4_Display_Base(BlackImage, RYImage);
    // EPD_2IN9B_V4_Display_Base_color(WHITE);
	Paint_NewImage(BlackImage, 50, 120, 270, WHITE);
    
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	Paint_SetScale(2);
    Paint_Clear(WHITE);
    
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
        EPD_2IN9B_V4_Display_Partial(BlackImage, 70, 35, 120, 155); // Xstart must be a multiple of 8
        DEV_Delay_ms(500);
    }
#endif

    printf("Clear...\r\n");
    EPD_2IN9B_V4_Init();
    EPD_2IN9B_V4_Clear();

    printf("Goto Sleep...\r\n");
    EPD_2IN9B_V4_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

