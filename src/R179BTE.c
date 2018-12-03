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

static char *GetDataString (UINT_32 data, eDataWidth dataWidth)
{
    static char value[10];

    /* Clear all of memory so that string will be NULL terminated */
    memset (value, 0, sizeof(value));

    UINT_16 i;
    UINT_16 shift;

    switch (dataWidth)
    {
        case BIT_WIDTH_8:
            shift = 8;
            break;

        case BIT_WIDTH_16:
            shift = 16;
            break;

        case BIT_WIDTH_32:
            shift = 32;
            break;

        default:
            shift = 0;
            break;
    }

    i = 0;
    while (shift != 0)
    {
        shift -= 4;
        /* Convert each nibble, starting with the MS Nibble to ASCII */
        value[i] = '0' + ((data >> shift) & 0xf);
        /* Convert values from A-F */
        if (value[i] > '9')
        {
            value[i] = value[i] + 7;
        }
        i++;
    }

    return (value);
}

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

    printf ("%s\n", GetDataString (0x12345678, BIT_WIDTH_8));
    printf ("%s\n", GetDataString (0x12345678, BIT_WIDTH_16));
    printf ("%s\n", GetDataString (0x12345678, BIT_WIDTH_32));
    printf ("%s\n", GetDataString (0xABCDEF11, BIT_WIDTH_8));
    printf ("%s\n", GetDataString (0xABCDEF11, BIT_WIDTH_16));
    printf ("%s\n", GetDataString (0xABCDEF11, BIT_WIDTH_32));

    SC_Init ();
    ResetStateMachine ();

    while (TRUE)
    {
        ApplicationService ();
    }

    return (EXIT_SUCCESS);
}
