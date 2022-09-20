#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
	
#ifdef _1in64g
    EPD_1in64g_test();
	
#elif _2in36g
    EPD_2in36g_test();
	
#elif _3in0g
    EPD_3in0g_test();
	
#elif _4in37g
    EPD_4in37g_test();
	
#elif _7in3g
    EPD_7in3g_test();
	
#elif _1in54des
	EPD_1in54_DES_test();
	
#elif _2in13des
	EPD_2in13_DES_test();
	
#elif _2in9des
	EPD_2in9_DES_test();
	
#elif _1in02d
    EPD_1in02d_test();
	
#elif _1in54
    EPD_1in54_test();
	
#elif _1in54V2
    EPD_1in54_V2_test();
	
#elif _1in54b
    EPD_1in54b_test();
	
#elif _1in54bV2
    EPD_1in54b_V2_test();
	
#elif _1in54c
    EPD_1in54c_test();
	
#elif _2in66
	EPD_2in66_test();
	
#elif _2in66b
	EPD_2in66b_test();
	
#elif _2in7
    EPD_2in7_test();
	
#elif _2in7V2
	EPD_2in7_V2_test();
	
#elif _2in7b
    EPD_2in7b_test();
	
#elif _2in7bV2
	EPD_2in7b_V2_test();
	
#elif _2in9
    EPD_2in9_test();
	
#elif _2in9V2
	EPD_2in9_V2_test();
	
#elif _2in9bc
    EPD_2in9bc_test();
	
#elif _2in9bV3
    EPD_2in9b_V3_test();
	
#elif _2in9d
    EPD_2in9d_test();
	
#elif _2in13
    EPD_2in13_test();
	
#elif _2in13V2
	EPD_2in13_V2_test();
	
#elif _2in13V3
	EPD_2in13_V3_test();
	
#elif _2in13bc
    EPD_2in13bc_test();
	
#elif _2in13bV3
    EPD_2in13b_V3_test();
	
#elif _2in13bV4
	EPD_2in13b_V4_test();
	
#elif _2in13d
    EPD_2in13d_test();
	
#elif _3in52
    EPD_3in52_test();
	
#elif _3in7
    EPD_3in7_test();
	
#elif _4in01f
	EPD_4in01f_test();
	
#elif _4in2
    EPD_4in2_test();
	
#elif _4in2V2
	EPD_4in2_V2_test();
	
#elif _4in2bc
    EPD_4in2bc_test();
	
#elif _4in2bV2
    EPD_4in2b_V2_test();
	
#elif _4in37b
	EPD_4in37b_test();
	
#elif _5in65f
	EPD_5in65f_test();
	
#elif _5in83
    EPD_5in83_test();
	
#elif _5in83V2
    EPD_5in83_V2_test();
	
#elif _5in83bc
    EPD_5in83bc_test();
	
#elif _5in83bV2
    EPD_5in83b_V2_test();
	
#elif _5in84
	EPD_5in84_test();
	
#elif _7in5
    EPD_7in5_test();
	
#elif _7in5V2
    EPD_7in5_V2_test();
	
#elif _7in5bc
    EPD_7in5bc_test();
	
#elif _7in5bV2
    EPD_7in5b_V2_test();
	
#elif _7in5HD
    EPD_7in5_HD_test();
	
#elif _7in5bHD
    EPD_7in5b_HD_test();
	
#elif _10in2b
	EPD_10in2b_test();
	
#elif _13in3
	EPD_13in3_test();
#else
	printf("Please specify the EPD model when making. \r\n");
	printf("Example: When you run the EPD_7in5_V2_test() program, input: sudo make clean && make -j6 EPD=_7in5V2 \r\n");
	printf("Don't know which program you need to run? Refer to the user manual (Wiki) and main.c \r\n");
#endif
	
    return 0;
}
