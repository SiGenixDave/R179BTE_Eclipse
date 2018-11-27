/*
 * Flash.h
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "Types.h"

BOOLEAN FlashService (INT_32 *expectedValue, INT_32 *actualValue);
BOOLEAN FlashTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
BOOLEAN FlashTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable);

#endif /* FLASH_H_ */
