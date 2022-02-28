/*****************************************************************************
* | File      	:   EPD_4in2_test.c
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-13
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
#include "EPD_4in2.h"
#include <string.h>
#include <time.h> 

int EPD_4in2_test(void)
{
    printf("EPD_4IN2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_4IN2_Init_Fast();

    struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_4IN2_Clear();
    DEV_Delay_ms(500);
    clock_gettime(CLOCK_REALTIME,&finish);
    Debug("%ld S\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_4IN2_WIDTH % 8 == 0)? (EPD_4IN2_WIDTH / 8 ): (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 0, WHITE);
    

#if 1  // show bmp

    EPD_4IN2_Init_Fast();
    
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 10);
    EPD_4IN2_Display(BlackImage);
    DEV_Delay_ms(2000);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/4in2.bmp", 0, 0);
    EPD_4IN2_Display(BlackImage);
    DEV_Delay_ms(2000);

#endif        

#if 1   // show image for array   
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_4in2);
    EPD_4IN2_Display(BlackImage);
    DEV_Delay_ms(500);
#endif

#if 1  // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
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
    Paint_DrawString_CN(130, 0, "���abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);

    printf("EPD_Display\r\n");
    EPD_4IN2_Display(BlackImage);
	DEV_Delay_ms(2000);

#endif
	
#if 0
    printf("Support for partial refresh, but the refresh effect is not good, but it is not recommended\r\n");
    EPD_4IN2_Init_Partial();
	printf("Partial refresh\r\n");
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
		Paint_ClearWindows(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, WHITE);
		Paint_DrawTime(150, 80, &sPaint_time, &Font20, WHITE, BLACK);
		EPD_4IN2_PartialDisplay(150, 80, 150 + Font20.Width * 7, 80 + Font20.Height, BlackImage);
		DEV_Delay_ms(500);//Analog clock 1s
		num = num - 1;
		if(num == 0) {
			break;
		}
    }
#endif
#if 1
    EPD_4IN2_Init_Fast();
	EPD_4IN2_Clear();
	EPD_4IN2_Init_4Gray();
	printf("show Gray------------------------\r\n");
	free(BlackImage);
	BlackImage = NULL;
	Imagesize = ((EPD_4IN2_WIDTH % 8 == 0)? (EPD_4IN2_WIDTH / 4 ): (EPD_4IN2_WIDTH / 4 + 1)) * EPD_4IN2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
	Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 0, WHITE);
	Paint_SetScale(4);
	Paint_Clear(WHITE);

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
    Paint_DrawString_CN(140, 0, "���abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(140, 40, "���abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(140, 80, "���abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(140, 120, "���abc", &Font12CN, GRAY4, GRAY1);
	
    Paint_DrawString_CN(220, 0, "΢ѩ����", &Font24CN, GRAY1, GRAY4);
    Paint_DrawString_CN(220, 40, "΢ѩ����", &Font24CN, GRAY2, GRAY3);
    Paint_DrawString_CN(220, 80, "΢ѩ����", &Font24CN, GRAY3, GRAY2);
    Paint_DrawString_CN(220, 120, "΢ѩ����", &Font24CN, GRAY4, GRAY1);
	
	EPD_4IN2_4GrayDisplay(BlackImage);
	DEV_Delay_ms(2000);

	Paint_Clear(WHITE);
    EPD_4IN2_4GrayDisplay(gImage_4in2_4Gray1);
	DEV_Delay_ms(2000);

	GUI_ReadBmp_4Gray("./pic/4in2_Scale.bmp",0 , 0);
	EPD_4IN2_4GrayDisplay(BlackImage);
	DEV_Delay_ms(2000);
	
	Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 20, 20);
    EPD_4IN2_4GrayDisplay(BlackImage);
	DEV_Delay_ms(2000);
#endif	
    EPD_4IN2_Init_Fast();
    EPD_4IN2_Clear();
    printf("Goto Sleep...\r\n");
    EPD_4IN2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

