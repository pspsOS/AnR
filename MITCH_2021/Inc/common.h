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


/* User-defined Constants*/

#define TRUE 1
#define FALSE 0
#define MAX_NMEA 80

/* Common Function Prototypes */

uint32_t getTimeStamp(void);

/* Typedef structs and types */

typedef unsigned char byte;

// TODO: Refine bool definition
typedef unsigned char bool;

typedef struct daqStatusData {
	uint32_t timeStamp;
	uint8_t daqStatus; // = d000GBIA
/* 		(d) Enable DAQ Scaling, default = 0
 * 		(G) GPS setup and nominal, default = 0
 * 		(B) Barometric Altimeter (BMP) setup and nominal, default = 0
 * 		(I) Inertial Measurement Unit (IMU) setup and nominal, default = 0
 * 		(A) Analog Linear Accelerometer (ALA) nominal, default = 0
 */
} daqStatusData_t;

typedef struct gpsData {
	uint32_t timeStamp;
	char NMEA[MAX_NMEA];
	byte fix;
	byte quality;
	byte numSats;
	float lat;
	float lon;
	float alt;
	float speed;
	float angle;
/*	uint8_t gpsStatus; // = FQQQnnnn
		(F) Fix
 * 	  (QQQ) Quality
 * 	 (nnnn) Number of Satellites
 */
} gpsData_t;

typedef struct bmpData {
	uint32_t timeStamp;
	float pressure;
	float temperature;

} bmpData_t;

typedef struct imuData {
	uint32_t timeStamp;
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
	uint32_t timeStamp;
	uint8_t voltage;
	uint8_t monitorStatus;

} monitorData_t;

typedef struct processedData {
	uint32_t timeStamp;

} processedData_t;



#endif /* COMMON_H_ */
