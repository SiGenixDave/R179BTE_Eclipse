/*
 * Flash.h
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "Types.h"

void FlashService (void);
void FlashTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
void FlashTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable);

#endif /* FLASH_H_ */
