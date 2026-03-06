#include "EPD_5in83g.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ImageData.h"

void setup() {
    printf("EPD_5IN83G_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_5IN83G_Init();
    EPD_5IN83G_Clear(EPD_5IN83G_WHITE); // White
    DEV_Delay_ms(2000);

    //Create a new image cache
    UBYTE *Image;
    UDOUBLE Imagesize = ((EPD_5IN83G_WIDTH % 4 == 0)? (EPD_5IN83G_WIDTH / 4 ): (EPD_5IN83G_WIDTH / 4 + 1)) * EPD_5IN83G_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize/4)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(Image, EPD_5IN83G_WIDTH/2, EPD_5IN83G_HEIGHT/2, 0, EPD_5IN83G_WHITE);
    Paint_SetScale(4);

#if 1   // show bmp
    EPD_5IN83G_Init_Fast();
    EPD_5IN83G_Display(Image4color);
    DEV_Delay_ms(5000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    EPD_5IN83G_Init();
    printf("SelectImage:Image\r\n");
    Paint_NewImage(Image, EPD_5IN83G_WIDTH/2, EPD_5IN83G_HEIGHT/2, 0, EPD_5IN83G_WHITE);
    Paint_SetScale(4);
    Paint_SelectImage(Image);
    Paint_Clear(EPD_5IN83G_WHITE);

    // 2.Drawing on the image
    printf("Drawing:Image\r\n");
    Paint_DrawPoint(10, 80, EPD_5IN83G_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_5IN83G_YELLOW, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_5IN83G_BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_5IN83G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_5IN83G_RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_5IN83G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_5IN83G_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_5IN83G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_5IN83G_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_5IN83G_RED, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_5IN83G_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "Red,yellow,white and black", &Font16, EPD_5IN83G_RED, EPD_5IN83G_YELLOW);
    Paint_DrawString_EN(10, 20, "Four color e-Paper", &Font12, EPD_5IN83G_YELLOW, EPD_5IN83G_BLACK);
    Paint_DrawString_CN(150, 20, "微雪电子", &Font24CN, EPD_5IN83G_RED, EPD_5IN83G_WHITE);
    Paint_DrawNum(10, 35, 123456, &Font12, EPD_5IN83G_RED, EPD_5IN83G_WHITE);

    printf("EPD_Display\r\n");
    EPD_5IN83G_DisplayPart(Image,0,0,EPD_5IN83G_WIDTH/2, EPD_5IN83G_HEIGHT/2);
    DEV_Delay_ms(3000);
#endif

    printf("Clear...\r\n");
    EPD_5IN83G_Init();
    EPD_5IN83G_Clear(EPD_5IN83G_WHITE);
    DEV_Delay_ms(2000);

    printf("Goto Sleep...\r\n");
    EPD_5IN83G_Sleep();
    free(Image);
    Image = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
}


void loop() {

}