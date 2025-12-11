/*****************************************************************************
* | File      	:   EPD_10in85_test.c
* | Author      :   Waveshare team
* | Function    :   10.85inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.1
* | Date        :   2021-10-30
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
#include "EPD_10in85.h"
#include <time.h> 
#include <stdio.h>

int EPD_10in85_test(void)
{

    Debug("EPD_10in85_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    Debug("e-Paper Init and Clear...\r\n");
	EPD_10in85_Init();
    EPD_10in85_Clear();

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_10in85_WIDTH % 8 == 0)? (EPD_10in85_WIDTH / 8 ): (EPD_10in85_WIDTH / 8 + 1)) * EPD_10in85_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize*2)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        return -1;
    }
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_10in85_WIDTH*2, EPD_10in85_HEIGHT, 0, BLACK);
	Paint_Clear(WHITE);
	
#if 1  // show bmp
    Debug("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/10in85.bmp", 0, 0);
    EPD_10in85_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0   //show image for array    
    Debug("show image for array\r\n");
    EPD_10in85_Display(gImage_10in85);
    DEV_Delay_ms(1000);
#endif

#if 1 // Drawing on the image
	Paint_NewImage(BlackImage, EPD_10in85_WIDTH*2, EPD_10in85_HEIGHT, 0, WHITE);  	
    Debug("Drawing\r\n");
    //1.Select Image
    EPD_10in85_Init();
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID); 
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);      
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE); 
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK); 
    Paint_DrawString_CN(130, 0,"ÄãºÃabc", &Font12CN, BLACK, WHITE);  
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
    EPD_10in85_Display(BlackImage);
    DEV_Delay_ms(3000);
#endif

#if 0   //Partial refresh, example shows time
    // EPD_10in85_Init();
    // EPD_10in85_Clear();
	EPD_10in85_Init_Part();
	// Paint_NewImage(BlackImage, 120, 50, 0, WHITE);
    
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
	// Paint_SetScale(2);
    // Paint_Clear(WHITE);
    
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
        
        Paint_Clear(WHITE);
		Paint_DrawRectangle(50, 130, 170, 180, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
        Paint_DrawTime(60, 145, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		printf("Part refresh...\r\n");
        EPD_10in85b_Display_Partial(BlackImage, 0, 0, EPD_10in85_WIDTH*2, EPD_10in85_HEIGHT); // Xstart must be a multiple of 8
        DEV_Delay_ms(500);
    }
#endif

    Debug("Clear...\r\n");
    EPD_10in85_Init();
    EPD_10in85_Clear();
    DEV_Delay_ms(500);
	
    Debug("Goto Sleep...\r\n");
    EPD_10in85_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
	
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    return 0;
}

