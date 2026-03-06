#include "EPD_5in0.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ImageData.h"

void setup() {
    printf("EPD_5in0_test Demo\r\n");
    if(DEV_Module_Init()!=0){
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_5in0_Init();
    EPD_5in0_Clear();
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_5in0_WIDTH % 8 == 0)? (EPD_5in0_WIDTH / 8 ): (EPD_5in0_WIDTH / 8 + 1)) * EPD_5in0_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_5in0_WIDTH, EPD_5in0_HEIGHT, 0, WHITE);

#if 1   // show bmp    
    printf("show bmp------------------------\r\n");
    EPD_5in0_Init();
    EPD_5in0_Display(gImage_1);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    printf("Drawing\r\n");
    //1.Select Image
    EPD_5in0_Init();
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

    Paint_DrawString_CN(5, 135,"你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(5, 155, "微雪电子", &Font24CN, WHITE, BLACK);

    EPD_5in0_Display_Base(BlackImage);
	
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    
	Paint_NewImage(BlackImage, Font20.Width * 7, Font20.Height, 0, WHITE);
    printf("Partial refresh\r\n");
    Paint_SelectImage(BlackImage);
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
        Paint_ClearWindows(0, 0, Font20.Width * 7, Font20.Height, WHITE);
        Paint_DrawTime(0, 0, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
		EPD_5in0_Display_Partial(BlackImage, 150, 300, 150 + Font20.Width * 7, 300 + Font20.Height);
        DEV_Delay_ms(500);//Analog clock 1s
    }

#endif
    printf("Clear...\r\n");
    EPD_5in0_Init();
    EPD_5in0_Clear();
    DEV_Delay_ms(2000);

    printf("Goto Sleep...\r\n");
    EPD_5in0_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

}

void loop() {
  

}
