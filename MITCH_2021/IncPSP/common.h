/*
 * common.h
 *
 *  Created on: Dec 15, 2020
 *      Author: vishv
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>

/* User-defined Macros */

#define getBit(A, X) ((((A >> X) & 0x01) == 0x01) ? (0x01) : (0x00))
#define setBit(A, X, V) (A & ~(0x01 << X) | (V << X))


/* User-defined Constants*/

#define MAX_NMEA (80)

/* User typedef */

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;


/* Common Function Prototypes */

ui32 getTimeStamp(void);
void retryTakeDelay(int length);

/* Typedef structs and types */

typedef struct daqStatusData {
	ui32 timeStamp;
	bool daqScaling;
	bool gpsNominal;
	bool bmpNominal;
	bool imuNominal;
	bool alaNominal;
	bool hasUpdate;
	bool lock;
/*	ui8 daqStatus; // = d000GBIA
/* 		(d) Enable DAQ Scaling, default = 0
 * 		(G) GPS setup and nominal, default = 0
 * 		(B) Barometric Altimeter (BMP) setup and nominal, default = 0
 * 		(I) Inertial Measurement Unit (IMU) setup and nominal, default = 0
 * 		(A) Analog Linear Accelerometer (ALA) nominal, default = 0
 */
} daqStatusData_t;


typedef struct gpsData {
	ui32 timeStamp;
	char NMEA[MAX_NMEA];
	ui8 fix;
	float alt;
	float speed;
	bool hasUpdate;
	bool lock;
} gpsData_t;


typedef struct bmpData {
	ui32 timeStamp;
	float pressure;
	float temperature;

} bmpData_t;


typedef struct imuData {
	ui32 timeStamp;
	float accX;
	float accY;
	float accZ;
	float gyrX;
	float gyrY;
	float gyrZ;
	float magX;
	float magY;
	float magZ;
	float alaZ;

} imuData_t;


typedef struct monitorData {
	ui32 timeStamp;
	ui8 voltage;
	ui8 monitorStatus;

} monitorData_t;


typedef struct processedData {
	ui32 timeStamp;

} processedData_t;

/* Extern variable definitions */

extern volatile daqStatusData_t g_daqStatusData;

#endif /* COMMON_H_ */
