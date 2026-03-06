/*****************************************************************************
* | File      	:	EPD_3in97_test.c
* | Author      :   Waveshare team
* | Function    :   3.97inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-12-31
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
#include "EPD_3in97.h"
#include <time.h> 

int EPD_3in97_test(void)
{
    printf("EPD_3in97_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_3in97_Init();
    EPD_3in97_Clear();
    DEV_Delay_ms(100);

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_3in97_WIDTH % 8 == 0)? (EPD_3in97_WIDTH / 8 ): (EPD_3in97_WIDTH / 8 + 1)) * EPD_3in97_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3in97_WIDTH, EPD_3in97_HEIGHT, 0, WHITE);

#if 1   // show bmp    
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    EPD_3in97_Init_Fast();
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 50, 50);
    EPD_3in97_Display_Fast(BlackImage);
    DEV_Delay_ms(500);

    printf("show bmp------------------------\r\n");
    Paint_Clear(WHITE);    
    GUI_ReadBmp("./pic/3in97.bmp", 0, 0);
    EPD_3in97_Display_Fast(BlackImage);
    DEV_Delay_ms(2000);
#endif
    

#if 1   // Drawing on the image
    printf("Drawing\r\n");
    //1.Select Image
    EPD_3in97_Init();
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

    Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(170, 15, 170, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(150, 35, 190, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 130, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(170, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(170, 85, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(5, 85, "waveshare", &Font20, BLACK, WHITE);
    Paint_DrawNum(5, 110, 123456789, &Font20, BLACK, WHITE);

    Paint_DrawString_CN(5, 135,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(5, 155, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);

    EPD_3in97_Display_Base(BlackImage);
	
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    
	Paint_NewImage(BlackImage, Font20.Width * 7, Font20.Height, 0, WHITE);
    Debug("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 5;
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
		EPD_3in97_Display_Partial(BlackImage, 150, 300, 150 + Font20.Width * 7, 300 + Font20.Height);
        DEV_Delay_ms(500);//Analog clock 1s
    }

#endif

#if 1 // show image for array
    free(BlackImage);
    printf("show Gray------------------------\r\n");
    Imagesize = ((EPD_3in97_WIDTH % 4 == 0)? (EPD_3in97_WIDTH / 4 ): (EPD_3in97_WIDTH / 4 + 1)) * EPD_3in97_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    EPD_3in97_Init_4GRAY();
    printf("4 grayscale display\r\n");
    Paint_NewImage(BlackImage, EPD_3in97_WIDTH, EPD_3in97_HEIGHT, 0, WHITE);
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
    Paint_DrawString_CN(130, 0,"ï¿½ï¿½ï¿½abc", &Font12CN, GRAY4, GRAY1);
    Paint_DrawString_CN(130, 20,"ï¿½ï¿½ï¿½abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(130, 40,"ï¿½ï¿½ï¿½abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(130, 60,"ï¿½ï¿½ï¿½abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(5, 130, "Î¢Ñ©ï¿½ï¿½ï¿½ï¿½", &Font24CN, GRAY1, GRAY4);
    EPD_3in97_Display_4Gray(BlackImage);
    DEV_Delay_ms(3000);

    Paint_NewImage(BlackImage, EPD_3in97_WIDTH, EPD_3in97_HEIGHT, 0, WHITE);
    Paint_SetScale(4);
    Paint_Clear(WHITE);
    GUI_ReadBmp_4Gray("./pic/3in97_4Gray.bmp", 0, 0);
    EPD_3in97_Display_4Gray(BlackImage);
    DEV_Delay_ms(3000);
#endif
    printf("Clear...\r\n");
    EPD_3in97_Init();
    EPD_3in97_Clear();
    DEV_Delay_ms(2000);

    printf("Goto Sleep...\r\n");
    EPD_3in97_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

