/*
 * RTC.h
 *
 *  Created on: Nov 26, 2018
 *      Author: David Smail
 */

#ifndef RTC_H_
#define RTC_H_

#include "Types.h"

void RTCService (void);
BOOLEAN RTCTableUpdate(UINT_16 tableIndex, BOOLEAN readEnable, BOOLEAN writeEnable);
BOOLEAN RTCTableUpdateAll(BOOLEAN readEnable, BOOLEAN writeEnable);


#endif /* RTC_H_ */
