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

#include "Types.h"
#include "CmdProc.h"
#include "Flash.h"
#include "SerComm.h"


int main (void)
{

#ifdef WIN32
    /*********************************/
    // Used exclusively for winsock
    setvbuf (stdout, NULL, _IONBF, 0);
    setvbuf (stderr, NULL, _IONBF, 0);
#ifdef _DEBUG
    setbuf(stdout,NULL); // this disables buffering for stdout.
#endif

    /**********************************/
#endif

    SC_Init();
    ResetStateMachine ();

    while (TRUE)
    {
        ApplicationService ();
    }

    return (EXIT_SUCCESS);
}
