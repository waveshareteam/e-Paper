/*****************************************************************************
* | File        :   EPD_7in3f_test.c
* | Author      :   Waveshare team
* | Function    :   7.3inch e-Paper (F) Demo
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2022-10-20
* | Info        :
* -----------------------------------------------------------------------------
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
#include "EPD_7in3f.h"

int EPD_test(void)
{
	printf("EPD_7IN3F_test Demo\r\n");
	if(DEV_Module_Init()!=0){
		return -1;
	}

	printf("e-Paper Init and Clear...\r\n");
	EPD_7IN3F_Init();

	EPD_7IN3F_Clear(EPD_7IN3F_WHITE); // WHITE 
	DEV_Delay_ms(1000);

	//Create a new image cache
	UBYTE *BlackImage;
	UDOUBLE Imagesize = ((EPD_7IN3F_WIDTH % 2 == 0)? (EPD_7IN3F_WIDTH / 2 ): (EPD_7IN3F_WIDTH / 2 + 1)) * EPD_7IN3F_HEIGHT;
	if((BlackImage = (UBYTE *)malloc(Imagesize/4)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		return -1;
	}
	printf("Paint_NewImage\r\n");
	Paint_NewImage(BlackImage, EPD_7IN3F_WIDTH/2, EPD_7IN3F_HEIGHT/2, 0, EPD_7IN3F_WHITE);
	Paint_SetScale(7);

#if 1
	EPD_7IN3F_Display(gImage_7in3f);
	DEV_Delay_ms(5000); 
#endif
	
#if 1   // Drawing on the image
	// 1.Select Image
	printf("SelectImage:BlackImage\r\n");
	Paint_SelectImage(BlackImage);
	Paint_Clear(EPD_7IN3F_WHITE);

	// 2.Drawing on the image
	printf("Drawing:BlackImage\r\n");
	Paint_DrawPoint(10, 80, EPD_7IN3F_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(10, 90, EPD_7IN3F_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(10, 100, EPD_7IN3F_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
	Paint_DrawLine(20, 70, 70, 120, EPD_7IN3F_ORANGE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawLine(70, 70, 20, 120, EPD_7IN3F_ORANGE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
	Paint_DrawRectangle(20, 70, 70, 120, EPD_7IN3F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(80, 70, 130, 120, EPD_7IN3F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawCircle(45, 95, 20, EPD_7IN3F_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawCircle(105, 95, 20, EPD_7IN3F_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawLine(85, 95, 125, 95, EPD_7IN3F_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawLine(105, 75, 105, 115, EPD_7IN3F_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
	Paint_DrawString_CN(10, 160, "ÄãºÃAbc", &Font12CN, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
	Paint_DrawString_CN(10, 180, "Î¢Ñ©µç×Ó", &Font24CN, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
	Paint_DrawNum(10, 33, 123456789, &Font12, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
	Paint_DrawNum(10, 50, 987654321, &Font16, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(300, 0, "Waveshare", &Font16, EPD_7IN3F_BLACK, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(300, 20, "Waveshare", &Font16, EPD_7IN3F_GREEN, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(300, 40, "Waveshare", &Font16, EPD_7IN3F_BLUE, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(300, 60, "Waveshare", &Font16, EPD_7IN3F_RED, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(300, 80, "Waveshare", &Font16, EPD_7IN3F_YELLOW, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(300, 100, "Waveshare", &Font16, EPD_7IN3F_ORANGE, EPD_7IN3F_WHITE);
	Paint_DrawString_EN(160, 0, "hello world", &Font12, EPD_7IN3F_WHITE, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 30, "hello world", &Font12, EPD_7IN3F_GREEN, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 60, "hello world", &Font12, EPD_7IN3F_BLUE, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 90, "hello world", &Font12, EPD_7IN3F_RED, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 120, "hello world", &Font12, EPD_7IN3F_YELLOW, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 150, "hello world", &Font12, EPD_7IN3F_ORANGE, EPD_7IN3F_BLACK);
	Paint_DrawString_EN(160, 180, "hello world", &Font12, EPD_7IN3F_BLACK, EPD_7IN3F_YELLOW);

	printf("EPD_Display\r\n");
	EPD_7IN3F_DisplayPart(BlackImage, 200, 120, 400, 240);
	DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
	//1.Select Image
	printf("SelectImage:BlackImage\r\n");
	Paint_SelectImage(BlackImage);
	Paint_Clear(EPD_7IN3F_WHITE);

	int hNumber, hWidth, vNumber, vWidth;
	hNumber = 10;
	hWidth = EPD_7IN3F_HEIGHT/2/hNumber; // 480/2/10=24
	vNumber = 20;
	vWidth = EPD_7IN3F_WIDTH/2/vNumber; // 800/2/20=20
	
	// 2.Drawing on the image
	printf("Drawing:BlackImage\r\n");
	for(int i=0; i<hNumber; i++) {  // horizontal
		Paint_DrawRectangle(1, 1+i*hWidth, EPD_7IN3F_WIDTH/2, hWidth*(1+i), EPD_7IN3F_BLACK + (i % 2), DOT_PIXEL_1X1, DRAW_FILL_FULL);
	}
	for(int i=0; i<vNumber; i++) {  // vertical
		if(i%2) {
			Paint_DrawRectangle(1+i*vWidth, 1, vWidth*(i+1), EPD_7IN3F_HEIGHT/2, EPD_7IN3F_GREEN + (i%5), DOT_PIXEL_1X1, DRAW_FILL_FULL);
		}
	}

	printf("EPD_Display\r\n");
	EPD_7IN3F_DisplayPart(BlackImage, 200, 120, 400, 240);
	DEV_Delay_ms(3000);
#endif

	printf("Clear...\r\n");
	EPD_7IN3F_Clear(EPD_7IN3F_WHITE);

	printf("Goto Sleep...\r\n");
	EPD_7IN3F_Sleep();
	free(BlackImage);
	BlackImage = NULL;
	DEV_Delay_ms(2000); // important, at least 2s
	// close 5V
	printf("close 5V, Module enters 0 power consumption ...\r\n");
	DEV_Module_Exit();
	
	return 0;
}

