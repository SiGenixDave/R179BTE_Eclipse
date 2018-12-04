/*****************************************************************************
 *
 * Copyright (C) 2018-19 Bombardier
 *
 * File Name: Flash.h
 *
 * Abstract: Header file for the Flash
 *
 * Revision History:
 *   12/03/2018 - das - Created
 *
 ******************************************************************************/
#ifndef FLASH_H_
#define FLASH_H_

#include "Types.h"

/*--------------------------------------------------------------------------
 GLOBAL CONSTANTS
 --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 GLOBAL TYPES
 --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 GLOBAL VARIABLES
 --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 GLOBAL PROTOTYPES
 --------------------------------------------------------------------------*/
void FlashService (const char *str);
BOOLEAN FlashTableUpdate (char cmdPtr[][MAX_PARAM_LENGTH]);

#endif /* FLASH_H_ */
