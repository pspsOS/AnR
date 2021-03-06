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

// Parent Functions
void setup_A();
ui8 loop_A();


// Setup
void gpsSetup_A();
void bmpSetup_A();
void imuSetup_A();
void alaSetup_A();

// Loop
void gpsRead_A();
void bmpRead_A();
void imuRead_A();
void checkStatus_A();
void sendUpdate_A();
void updateLeds_A();

// Private
void _splitNmea();
void _findNmeaAddr(int addr);
void _loadGpsData();
int _getNmeaType();
void _addNmeaData();
void _clearNmea(char*);
// Test functions
void __printGpsData();

void __setNmea(char *nmea);
void __printNmea();

float __getFloat(int addr);

void __debug();

#endif /* INC_ACQUISITION_H_ */
