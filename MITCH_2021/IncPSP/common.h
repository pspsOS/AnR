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

#ifndef NDEBUG
#define ENABLE_PRINT 1
#else
#define ENABLE_PRINT 0
#endif

#define print(fmt, ...) \
            do { if (ENABLE_PRINT) fprintf(stdout, fmt); } while (0)

#define prints(fmt, ...) \
            do { if (ENABLE_PRINT) fprintf(stdout, fmt, __VA_ARGS__); } while (0)


/* User-defined Constants*/

#define MAX_NMEA (80)
#define MAX_TRANSMISSION_SIZE (100)
#define TAN_THETA_SQUARED (32) // Theta is about 80 degrees

#define ALTITUDE_LIST_SIZE (20)
#define ALA_LIST_SIZE (40)

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

typedef struct altitudeNode altitudeNode_t;
typedef struct alaNode alaNode_t;
typedef struct staticOrientationNode staticOrientationNode_t;


typedef struct daqStatusData {
	ui32 timeStamp;
	bool daqScaling;
	bool gpsNominal;
	bool bmpNominal;
	bool imuNominal;
	bool alaNominal;
	bool hasUpdate;
	bool lock;
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
	ui32 timeStamp;
	float batteryVoltage;
	bool continuity[4];
	bool buttonState;
	bool hasUpdate;
	bool lock;

} monitoringData_t;

typedef struct transmissionData {
	float altitude;
	float accelerationDataX;
	float accelerationDataY;
	float accelerationDataZ;
	float trajectoryDataX;
	float trajectoryDataY;
	float trajectoryDataZ;
	float resolvedOrientationX;
	float resolvedOrientationY;
	float resolvedOrientationZ;
	float rawData;
	bool lock;
	bool hasUpdate;
} transmissionData_t;



typedef struct altitudeNode {
	float altitude;
	bool lock;
	altitudeNode_t *nextNode;
} altitudeNode_t;

typedef struct alaNode {
	float gForce;
	bool lock;
	alaNode_t *nextNode;
} alaNode_t;


typedef struct staticOrientationNode {
	bool lock;
	staticOrientationNode_t *nextNode;
} staticOrientationNode_t;

/* Extern variable definitions */

extern volatile daqStatusData_t g_daqStatusData;
extern volatile gpsData_t g_gpsData;
extern volatile bmpData_t g_bmpData;
extern volatile imuData_t g_imuData;
extern volatile monitoringData_t g_monitoringData;
extern volatile transmissionData_t g_transmissionData;

extern volatile ui8 g_currentNominalMode;
extern volatile ui8 g_currentContingency;

extern altitudeNode_t *g_headAltitudeNode;
extern alaNode_t *g_headALANode;
extern staticOrientationNode_t *g_headStaticOrientationNode;

#endif /* COMMON_H_ */
