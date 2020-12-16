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
	uint8_t daqStatus; // = d000GBIA
/* 		(d) Enable DAQ Scaling, default = 0
 * 		(G) GPS setup and nominal, default = 0
 * 		(B) Barometric Altimeter (BMP) setup and nominal, default = 0
 * 		(I) Inertial Measurement Unit (IMU) nominal, default = 0
 * 		(A) Analog Linear Accelerometer (ALA) nominal, default = 0
 */
} daqStatusData_t;

typedef struct gpsData {
	uint32_t timeStamp;
	float lla[3]; // Lat, Lon, Alt
	float speed;
	float angle;
	uint8_t gpsStatus; // = FQQQnnnn
/*		(F) Fix
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
	float data[10];
/* 0,1,2 = Acc X,Y,Z
 * 3,4,5 = Gyr X,Y,Z
 * 6,7,8 = Mag X,Y,Z
 *    10 = ALA Z
 */

} imuData_t;

typedef struct monitoring {
	uint32_t timeStamp;
	uint8_t voltage;
	uint8_t monitorStatus;

} monitoring_t;

typedef struct processedData {
	uint32_t timeStamp;

} processedData_t;



#endif /* COMMON_H_ */
