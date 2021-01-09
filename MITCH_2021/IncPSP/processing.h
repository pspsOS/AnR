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

bool getBMPData_P();
bool getIMUData_P();
bool getGPSData_P();
void processData_P();
void transmitData_P();

bool pointyEndUp_P();
void CalcAltBMP_P();
void CalcFlightDataBMP_P();

#endif /* INC_PROCESSING_H_ */
