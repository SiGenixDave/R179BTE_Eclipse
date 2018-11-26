/*
 * NVRAM.h
 *
 *  Created on: Nov 26, 2018
 *      Author: David Smail
 */

#ifndef NVRAM_H_
#define NVRAM_H_

#include "Types.h"

void NVRamService (void);
BOOLEAN NVRamTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
BOOLEAN NVRamTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable);

#endif /* NVRAM_H_ */
