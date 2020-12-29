/*
 * processing.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_PROCESSING_H_
#define INC_PROCESSING_H_

#include "../IncPSP/common.h"

/* Function prototypes */

bool getBMPData();
bool getIMUData();
bool getGPSData();
void processData();
void transmitData();

#endif /* INC_PROCESSING_H_ */
