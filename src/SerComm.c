/*
 * SCWrapper.c
 *
 *  Created on: Nov 26, 2018
 *      Author: David Smail
 */
#include <stdio.h>
#include <string.h>

#include "Types.h"
#include "CmdProc.h"

const int COM_ID = 5;  /* comport 6 */

INT_16 SC_GetChar(void)
{
    INT_16 n;
    unsigned char buf;

    n = RS232_PollComport(COM_ID, &buf, 1);

    if (n == 0) { return (EOF); }
    else {return (buf);};
}

void SC_Init()
{
    char rs232mode[] = { '8', 'N', '1', 0 };

    if (RS232_OpenComport(COM_ID, 19200, rs232mode)) {
        printf("Can not open comport\n");
    }
}

void SC_PutChar(char c)
{
    RS232_SendByte (COM_ID, c);
}

void SC_Puts(char *str)
{
    RS232_SendBuf (COM_ID, str, strlen(str));
}

