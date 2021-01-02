/*****************************************************************************
* | File      	:   EPD_1IN02_test.c
* | Author      :   Waveshare team
* | Function    :   1.02inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-29
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
#include "EPD_1in02d.h"

int EPD_1in02d_test(void)
{
    printf("EPD_1IN02_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_1IN02_Init();
    EPD_1IN02_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage,*old_Image;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_1IN02_WIDTH % 8 == 0)? (EPD_1IN02_WIDTH / 8 ): (EPD_1IN02_WIDTH / 8 + 1)) * EPD_1IN02_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((old_Image = (UBYTE *)malloc(Imagesize)) == NULL) {
			printf("Failed to apply for black memory2...\r\n");
			return -1;
	}
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_1IN02_WIDTH, EPD_1IN02_HEIGHT, 270, WHITE);
    Paint_NewImage(old_Image, EPD_1IN02_WIDTH, EPD_1IN02_HEIGHT, 270, WHITE);
    Paint_SelectImage(old_Image);
	Paint_Clear(WHITE);
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);

#if 0  // show bmp    
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x80.bmp", 0, 0);
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);

    printf("show bmp------------------------\r\n");
    Paint_Clear(WHITE);    
    GUI_ReadBmp("./pic/1in02.bmp", 0, 0);
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif
    
#if 0   //show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_1in02d);

    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif
#if 1
  // Drawing on the image
  //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 15, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 25, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    
    Paint_DrawLine(30, 10, 80, 70, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(80, 10, 30, 70, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);    
    Paint_DrawRectangle(30, 10, 90, 70, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawCircle(60, 40, 25, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    printf("EPD_1IN02_Display\r\n");
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);

    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    printf("EPD_1IN02_Display\r\n");
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);

    Paint_Clear(WHITE);
    Paint_DrawString_CN(0, 10,"ÄãºÃabcÊ÷Ý®ÅÉ", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(0, 30,"Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
    printf("EPD_1IN02_Display\r\n");
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);
    Paint_Clear(WHITE);
    EPD_1IN02_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    		
    printf("Partial refresh\r\n");
    Paint_Clear(WHITE);
    EPD_1IN02_Display(BlackImage);
    
    EPD_1IN02_Part_Init();
    Paint_SelectImage(BlackImage);
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
        Paint_ClearWindows(20, 20, 20 + Font20.Width * 7, 20 + Font20.Height, WHITE);
        Paint_DrawTime(20, 20, &sPaint_time, &Font20, WHITE, BLACK);
        num = num - 1;
        if(num == 0) {
            break;
        }
        EPD_1IN02_DisplayPartial(old_Image, BlackImage);
        memcpy(old_Image, BlackImage, Imagesize);
        // DEV_Delay_ms(100);//Analog clock 1s
    }

#endif

    printf("Clear...\r\n");
    EPD_1IN02_Init();
    EPD_1IN02_Clear();

    printf("Goto Sleep...\r\n");
    EPD_1IN02_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

