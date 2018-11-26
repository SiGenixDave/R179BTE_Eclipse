/*
 * SCWrapper.c
 *
 *  Created on: Nov 26, 2018
 *      Author: David Smail
 */
#include <stdio.h>

void SC_PutChar(char c)
{
    putchar(c);
}

void SC_Puts(char *str)
{
    puts(str);
    puts("\n\r");
}

