/*****************************************************************************
* | File      	:   EPD_13IN3K_test.c
* | Author      :   Waveshare team
* | Function    :   13.3inch e-paper (K) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-07-18
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
#include "EPD_13in3k.h"
#include <time.h> 

int EPD_13in3k_test(void)
{
    printf("EPD_13IN3K_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_13IN3K_Init();

	struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_13IN3K_Clear();
	clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);	

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_13IN3K_WIDTH % 8 == 0)? (EPD_13IN3K_WIDTH / 8 ): (EPD_13IN3K_WIDTH / 8 + 1)) * EPD_13IN3K_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);
	Paint_Clear(WHITE);
	
#if 1   // show bmp
	Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);
    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/13in3.bmp", 0, 0);
    EPD_13IN3K_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0   // show bmp
	Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);
    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/13in3_1.bmp", 0, 0);
    EPD_13IN3K_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0   // show bmp
	Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);
    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/13in3_2.bmp", 0, 0);
    EPD_13IN3K_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0  //show image for array  
    Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_13in3k);

    EPD_13IN3K_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1  // Drawing on the image
	Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
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

    Paint_DrawString_CN(130, 0,"你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    Paint_DrawCircle(200, 310, 90, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(200, 310, 40, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	
    Paint_DrawCircle(480, 480, 150, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(480, 480, 110, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(750, 270, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(750, 270, 70, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	
    Paint_DrawLine(200, 310, 480, 480, BLACK, DOT_PIXEL_3X3, LINE_STYLE_DOTTED);
    Paint_DrawLine(480, 480, 750, 270, BLACK, DOT_PIXEL_3X3, LINE_STYLE_DOTTED);
    Paint_DrawLine(750, 270, 200, 310, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);	
	
    EPD_13IN3K_Display_Base(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 1
    printf("Partial refresh\r\n");
    // If you didn't use the EPD_13IN3K_Display_Base() function to refresh the image before,
    // use the EPD_13IN3K_color_Base() function to refresh the background color, 
    // otherwise the background color will be garbled 
    EPD_13IN3K_Init_Part();
    // EPD_13IN3K_color_Base(WHITE);   
	printf("Partial refresh\r\n");
    Paint_NewImage(BlackImage, 200, 50, 0, WHITE);
	PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 10;
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
		Paint_DrawTime(20, 10, &sPaint_time, &Font20, WHITE, BLACK);
		EPD_13IN3K_Display_Part(BlackImage, 10, 150, 200, 50);
		DEV_Delay_ms(500);//Analog clock 1s
		num = num - 1;
		if(num == 0) {
			break;
		}
    }
#endif

#if 1 // show image for array
    free(BlackImage);
    printf("show Gray------------------------\r\n");
    Imagesize = ((EPD_13IN3K_WIDTH % 4 == 0)? (EPD_13IN3K_WIDTH / 4 ): (EPD_13IN3K_WIDTH / 4 + 1)) * EPD_13IN3K_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    EPD_13IN3K_Init_4GRAY();
    printf("4 grayscale display\r\n");
    Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 90, WHITE);
    Paint_SetScale(4);
    Paint_Clear(0xff);
    
    Paint_DrawPoint(10, 80, GRAY4, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, GRAY4, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, GRAY4, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, GRAY4, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, GRAY2, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, GRAY4, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, GRAY4, GRAY1);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, GRAY3, GRAY1);
    Paint_DrawNum(10, 33, 123456789, &Font12, GRAY4, GRAY2);
    Paint_DrawNum(10, 50, 987654321, &Font16, GRAY1, GRAY4);
    Paint_DrawString_CN(150, 0,"你好abc", &Font12CN, GRAY4, GRAY1);
    Paint_DrawString_CN(150, 20,"你好abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(150, 40,"你好abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(150, 60,"你好abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(10, 130, "微雪电子", &Font24CN, GRAY1, GRAY4);
    EPD_13IN3K_4GrayDisplay(BlackImage);
    DEV_Delay_ms(3000);

    Paint_NewImage(BlackImage, EPD_13IN3K_WIDTH, EPD_13IN3K_HEIGHT, 0, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);
    GUI_ReadBmp_4Gray("./pic/13in3k_Scale.bmp", 0, 0);
    EPD_13IN3K_4GrayDisplay(BlackImage);
    DEV_Delay_ms(3000);

#endif

	printf("Clear...\r\n");
    EPD_13IN3K_Init();
    EPD_13IN3K_Clear();
	
    printf("Goto Sleep...\r\n");
    EPD_13IN3K_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}