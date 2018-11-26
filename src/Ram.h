/*
 * Flash.h
 *
 *  Created on: Nov 21, 2018
 *      Author: David Smail
 */

#ifndef RAM_H_
#define RAM_H_

#include "Types.h"

void RamService (void);
BOOLEAN RamTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
BOOLEAN RamTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable);

#endif /* RAM_H_ */
