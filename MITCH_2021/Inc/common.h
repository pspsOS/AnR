/*
 * common.h
 *
 *  Created on: Dec 15, 2020
 *      Author: vishv
 */

#ifndef COMMON_H_
#define COMMON_H_


/* User-defined Macros */

#define getBit(A, X) ((((A >> X) & 0x01) == 0x01) ? (0x01) : (0x00))
#define setBit(A, X, V) (A & ~(0x01 << X) | (V << X))



/* Typedef structs */

typedef struct daqStatusData {
	uint32_t timeStamp;
	uint8_t daqStatus;
} daqStatusData_t;

typedef struct gpsData {
	uint32_t timeStamp;

} gpsData_t;

typedef struct bmpData {
	uint32_t timeStamp;

} bmpData_t;

typedef struct imuData {
	uint32_t timeStamp;
	float data[10];

} imuData_t;

typedef struct monitoring {
	uint32_t timeStamp;
	uint8_t monitorStatus;
	uint8_t voltage;
} monitoring_t;

typedef struct processedData {
	uint32_t timeStamp;

} processedData_t;



#endif /* COMMON_H_ */
