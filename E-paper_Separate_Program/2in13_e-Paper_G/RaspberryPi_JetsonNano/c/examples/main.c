#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void Handler(int signo)
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

// This reserved interface will be used for future screen upgrade versions. Simply add EPD=reserved after the compiler command
// reserved needs to be changed to its screen name
#if epd2in13g
    EPD_2in13g_test(); 
#else
    EPD_2in13g_V2_test(); 
#endif
    
    return 0;
}
