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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#define NDEBUG

/* User-defined Macros */

#define getBit(A, X) ((((A >> X) & 0x01) == 0x01) ? (0x01) : (0x00))
#define setBit(A, X, V) (A & ~(0x01 << X) | (V << X))


/* User-defined Constants*/

#define MAX_NMEA (80)

// Nominal mode flow
#define PRELAUNCH (1)
#define LAUNCH (2)
#define ASCENT (3)
#define DESCENT_DROGUE (4)
#define DESCENT_MAIN (5)
#define TOUCHDOWN (6)
#define PROGRAM_END (7)


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
	bool hasUpdate;
	bool lock;
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
	bool hasUpdate;
	bool lock;

} imuData_t;


typedef struct monitorData {
	ui32 timeStamp;
	ui8 voltage;
	ui8 monitorStatus;

} monitorData_t;


typedef struct processedData {
	ui32 timeStamp;

} processedData_t;

typedef struct monitoringData {
	float batteryVoltage;
	bool continuity[3];
	bool buttonState;
	bool hasUpdate;
	bool lock;

} monitoringData_t;


/* Extern variable definitions */

extern volatile daqStatusData_t g_daqStatusData;
extern volatile gpsData_t g_gpsData;
extern volatile bmpData_t g_bmpData;
extern volatile imuData_t g_imuData;
extern volatile monitoringData_t g_monitoringData;

extern volatile ui8 currentNominalMode;
extern volatile ui8 currentContingency;

#endif /* COMMON_H_ */
