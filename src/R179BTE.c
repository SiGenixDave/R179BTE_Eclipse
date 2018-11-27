/*
 ============================================================================
 Name        : R179BTE.c
 Author      : DAS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "CmdProc.h"
#include "Flash.h"

void TestCmdProc(void)
{
    char str[] = "<FL1aR-W+>";
    int index = 0;

    while (index < strlen(str))
    {
        ProcessSerialInputChar(str[index]);
        index++;
    }

}

int main(void) {

    INT_32 expVal;
    INT_32 actVal;

#ifdef WIN32
    /*********************************/
    // Used exclusively for winsock
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    #ifdef _DEBUG
    setbuf(stdout,NULL); // this disables buffering for stdout.
    #endif

    /**********************************/
#endif

	ResetStateMachine();
	TestCmdProc();

	if (!FlashService(&expVal, &actVal))
	{

	}


	return (EXIT_SUCCESS);
}
