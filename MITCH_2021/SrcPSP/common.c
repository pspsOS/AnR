/*
 * common.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Jeff
 */

#include "../IncPSP/common.h"


ui32 getTimeStamp(void) {
#ifndef __MAIN_H
	return 0;
#else
	return HAL_GetTick();
#endif
}

