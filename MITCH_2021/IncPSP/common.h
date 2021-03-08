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
#include <math.h>

#include "main.h" // NDEBUG is now defined in main.h

#ifdef NDEBUG
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "retarget.h"
#endif


/* User-defined Macros */
#define MIN(A, B) ( ((A)<(B)) ? (A) : (B))
#define MAX(A, B) ( ((A)>(B)) ? (A) : (B))
#define getBit(A, X) ((((A >> X) & 0x01) == 0x01) ? (0x01) : (0x00))
#define setBit(A, X, V) (A & ~((0x01 << X) | (V << X)))


/* User-defined Constants*/

#define MAX_NMEA (80)
#define DEFAULT_DAQ_SCALER (10)
#define MAX_TRANSMISSION_SIZE (100)
#define TAN_THETA_SQUARED (32) // Theta is about 80 degrees

#define DEFAULT_TAKE_DELAY (50)

#define ALTITUDE_ARRAY_SIZE (20)
#define ALA_ARRAY_SIZE (40)
#define STATIC_ORIENTATION_ARRAY_SIZE (20)
#define IMU_ARRAY_SIZE (40)

// Nominal mode flow
#define PRELAUNCH (1)
#define LAUNCH (2)
#define ASCENT (3)
#define DESCENT_DROGUE (4)
#define DESCENT_MAIN (5)
#define TOUCHDOWN (6)
#define PROGRAM_END (7)


// Sensor Defines



typedef enum {
	TASK_UPDATE = 0,
	SETUP_WARNING = 1
} Message_ID;

// Function returns
#define SUCCESSFUL_RETURN (0)



/* User typedef */

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef int16_t i16;
typedef int32_t i32;

/* Typedef structs and types */

typedef struct altitudeNode altitudeNode_t;
typedef struct alaNode alaNode_t;
typedef struct staticOrientationNode staticOrientationNode_t;
typedef struct imuNode imuNode_t;


typedef struct daqStatusData {
	ui32 timeStamp;
	bool daqScalingEnabled;
	bool gpsNominal;
	bool bmpNominal;
	bool imuNominal;
	bool alaNominal;
	bool hasUpdate;
	bool lock;
} daqStatusData_t;

typedef struct daqScalingData {
	bool enableDaqScaling;
	bool hasUpdate;
	bool lock;
} daqScalingData_t;

typedef struct gpsData {
	ui32 timeStamp;
	char nmeaGGA[MAX_NMEA];
	char nmeaRMC[MAX_NMEA];
	ui8 fix;
	float alt;
	float speed;
	int utc;
	bool hasUpdate;
	bool lock;
} gpsData_t;


typedef struct bmpData {
	ui32 timeStamp;
	i32 pressure;     // MIN: 1000, MAX: 120000, VALUE: 110002 = 1100.02 mbar
	i32 temperature;  // MIN:-4000, MAX: 8500,   VALUE: 2000 = 20.00 degC
	bool hasUpdate;
	bool lock;
} bmpData_t;


typedef struct imuData {
	ui32 timeStamp;
	i16 accel_xout;
	i16 accel_yout;
	i16 accel_zout;
	i16 gyro_xout;
	i16 gyro_yout;
	i16 gyro_zout;
	i16 mag_xout;
	i16 mag_yout;
	i16 mag_zout;
	i16 alaZ;
	bool hasUpdate;
	bool lock;

} imuData_t;

typedef struct processedData {
	ui32 timeStamp;

} processedData_t;

typedef struct monitoringData {
	ui32 timeStamp;
	float batteryVoltage;
	bool continuity[4];
	bool buttonState;
	bool hardwareDeploymentDisable;
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

/* Node structures */

typedef struct altitudeNode {
	float currentAltitude;
	float runningAltitude;
	bool lock;
} altitudeNode_t;

typedef struct alaNode {
	int16_t currentForce;
	float runningForce;
	bool lock;
} alaNode_t;

typedef struct staticOrientationNode {
	bool staticOrientation;
	int numNotOptimal;
	bool lock;
} staticOrientationNode_t;

typedef struct imuNode {
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyrX;
	int16_t gyrY;
	int16_t gyrZ;
	int16_t magX;
	int16_t magY;
	int16_t magZ;
	int16_t alaZ;
	bool lock;
} imuNode_t;


/* VLQ Struct */

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

/* Extern variable definitions */

extern volatile daqStatusData_t g_daqStatusData;
extern volatile daqScalingData_t g_daqScalingData;
extern volatile gpsData_t g_gpsData;
extern volatile bmpData_t g_bmpData;
extern volatile imuData_t g_imuData;
extern volatile monitoringData_t g_monitoringData;
extern volatile transmissionData_t g_transmissionData;

extern volatile ui8 g_currentNominalMode;
extern volatile ui8 g_currentContingency;
extern volatile ui32 g_launchTime;
extern volatile bool g_chuteDisarm;
extern volatile bool g_chuteDeployment;

extern altitudeNode_t g_altitudeArray[ALTITUDE_ARRAY_SIZE];
extern alaNode_t g_alaArray[ALA_ARRAY_SIZE];
extern staticOrientationNode_t g_staticOrientationArray[STATIC_ORIENTATION_ARRAY_SIZE];
extern imuNode_t g_imuArray[IMU_ARRAY_SIZE];

extern uint16_t newestAltitudeIndex;
extern uint16_t newestAlaIndex;
extern uint16_t newestStaticOrientationIndex;
extern uint16_t newestImuIndex;

/* Common Function Prototypes */

ui32 getTimeStamp(void );
void retryTakeDelay(int );

#ifdef NDEBUG

#endif

int insertNewAltitude(float );
int insertNewALA(float );
int insertNewStaticOrientation();
int insertNewIMUNode();

VLQ_t convertToVLQ(uint32_t );

//void notify(Message_ID message, Device_ID device);




#endif /* COMMON_H_ */
