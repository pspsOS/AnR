/*
 * storage.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#include "common.h"

/* Struct Typedef */

typedef struct VLQ {
	uint8_t quantityLength;
	union {
		uint8_t oneByteVLQ[1];
		uint8_t twoByteVLQ[2];
		uint8_t threeByteVLQ[3];
		uint8_t fourByteVLQ[4];
		uint8_t fiveByteVLQ[5];
	};
} VLQ_t;

/* Function Prototypes */

bool storageSetup();
void storageWrite();

void storeDaqStatus();
void storeDaqScaling();
void storeGpsData();
void storeBmpData();
void storeImuData();
void storeProcessedData();
void storeMonitoringData();

//Data Compression Functions

VLQ_t convertToUVLQ(uint32_t );
VLQ_t convertToSVLQ(int32_t );

#endif /* INC_STORAGE_H_ */
