#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "test.h"

int Version;

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    DEV_ModuleExit();
    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
	EPD_13in3e_test();
    
    DEV_ModuleExit();
    return 0;
}
