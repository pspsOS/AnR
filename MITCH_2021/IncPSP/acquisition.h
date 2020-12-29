/*
 * acquisition.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_ACQUISITION_H_
#define INC_ACQUISITION_H_

#include "common.h"

// Interface Structs


// Setup
void setup();

bool gpsSetup();
bool bmpSetup();
bool imuSetup();
bool alaSetup();

// Loop
void gpsRead();
void bmpRead();
void imuRead();

void checkStatus();
void sendUpdate();


// Private
void _splitNmea();
void _findNmeaAddr(int addr);

// Test functions
void __setNmea(char *nmea);
void __printNmea();

float __getFloat(int addr);

void __debug();

#endif /* INC_ACQUISITION_H_ */
