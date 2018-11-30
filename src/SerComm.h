/*
 * SCWrapper.h
 *
 *  Created on: Nov 26, 2018
 *      Author: David Smail
 */

#ifndef SERCOMM_H_
#define SERCOMM_H_

INT_16 SC_GetChar(void);
void SC_Init (void);
void SC_PutChar (char c);
void SC_Puts (char *str);

#endif /* SERCOMM_H_ */
