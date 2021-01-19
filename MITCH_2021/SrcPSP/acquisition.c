/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

#ifdef NDEBUG
	#include "../IncInterface/baro.h"
#else
	#include "../IncDebug/debugSettings.h"
	#include <unistd.h>
#endif

// Acquisition Finite States
#define ACQUIRE_IMU_BMP_GPS (0)
#define ACQUIRE_IMU_BMP (1)
#define ACQUIRE_IMU (2)

#define GPS_FREQ (1)   //   1 Hz
#define BMP_FREQ (10)  //  10 Hz
#define IMU_FREQ (100) // 100 Hz

/* Global variable declarations */


/* Local variable declarations */
char gpsNmea[MAX_NMEA]; // Buffer that holds GPS String
bool daqScaling;        // Enables/Disables DAQ Scaling
ui8 daqScaler;          // DAQ Scaling number � Sets the ratio
bool gpsNominal;        // Indicates whether the GPS is nominal
bool bmpNominal;        // Indicates whether the BMP is nominal
bool imuNominal;        // Indicates whether the IMU is nominal
bool alaNominal;        // Indicates whether the ALA is nominal
bool sendDaqStatus;     // Indicates whether to update the daqStatusData struct
ui8 gpsCounter;         // Counter used in DAQ Scaling for Finite State Machine
ui8 bmpCounter;         // Counter used in DAQ Scaling for Finite State Machine
ui8 fsmState;           // Defines state of Finite State Machine

// Used for parsing NMEA data
ui8 _nmeaAddrStart;
ui8 _nmeaAddrEnd;

// File pointers for Debugging
#ifndef NDEBUG
	FILE *_gpsFile;
	FILE *_bmpFile;
	FILE *_imuFile;
	FILE *_alaFile;
#endif




/** setup_A()
 * @brief Setup Acquisition Task
 * Initialized local variables and runs setup functions
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/28/2020
 */
void setup_A() {

	// Initialize local variables
	daqScaling = false;
	daqScaler = DEFAULT_DAQ_SCALER;
	gpsNominal = false;
	bmpNominal = false;
	imuNominal = false;
	alaNominal = false;
	sendDaqStatus = false;
	gpsCounter = 0;
	bmpCounter = 0;
	fsmState = ACQUIRE_IMU_BMP_GPS;

	// Setup sensors
	gpsSetup_A();
	bmpSetup_A();
	imuSetup_A();
	alaSetup_A();

	// Unlock initialized interface structs
	g_daqStatusData.lock = false;
	g_daqScalingData.lock = false;

	// Send update
	sendUpdate_A();
}

void loop_A() {
	switch(fsmState) {
	case ACQUIRE_IMU_BMP_GPS:
		gpsRead_A();
	case ACQUIRE_IMU_BMP:
		bmpRead_A();
	case ACQUIRE_IMU:
		imuRead_A();
		break;
	}

/*	if(daqScaling) {
		switch(fsmState) {
		case
		}

	} else {
		gpsRead_A();
		bmpRead_A();
		imuRead_A();
	}*/
}


// Setup
/** gpsSetup_A()
 * @brief Setup GPS Sensor
 * Establishes connection with GPS
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void gpsSetup_A() {
	#ifndef NDEBUG
		_gpsFile = setupSensorFile_DS(GPS, &gpsNominal);
	#else
		// TODO: Implement gpsSetup
	#endif
}



/** bmpSetup_A()
 * @brief Setup BMP Sensor
 * Establishes connection with BMP
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void bmpSetup_A() {
	#ifndef NDEBUG
		_bmpFile = setupSensorFile_DS(BMP, &bmpNominal);
	#else
		barometerInit(&bmpNominal);
	#endif
}



/** imuSetup_A()
 * @brief Setup IMU Sensor
 * Establishes connection with IMU
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void imuSetup_A() {
	#ifndef NDEBUG
		_imuFile = setupSensorFile_DS(IMU, &imuNominal);
	#else
		// TODO: Implement imuSetup
	#endif
}



/** alaSetup_A()
 * @brief Setup ALA Sensor
 * Establishes connection with ALA
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void alaSetup_A() {
	#ifndef NDEBUG
		_alaFile = setupSensorFile_DS(ALA, &alaNominal);
	#else
		// TODO: Implement alaSetup
	#endif
}

// Loop





/** gpsRead_A()
 * @brief GPS Read
 * Read data from GPS
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/23/2020
 */
void gpsRead_A() {

	#ifndef NDEBUG
		if(!simulateGps) {
			if(notifyWhenReadAborted)
				print("GPS read aborted.\n");
			return;
		}
		fscanf(_gpsFile, "%s", gpsNmea);
	#else
		//TODO: Implement gpsRead w/ hardware
	#endif

	print("Reading:",);

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		print("GGA\n");

	}
	//	Type = GPRMC
	else if (!(strncmp(&gpsNmea[0], "$GPRMC", 6))) {
		//puts("RMC");
		print("RMC\n");
	}

	//	Catch Bad Read
	else {
		gpsNominal = false;


	}

	g_gpsData.timeStamp = getTimeStamp();

	strncpy((char*)g_gpsData.NMEA, gpsNmea, strlen(gpsNmea));

//	return;
}



/** bmpRead_A();
 * @brief BMP Read
 * Read data from BMP
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/23/2020
 */
void bmpRead_A() {
	i32 temperature = 0;
	i32 pressure = 0;
	#ifdef NDEBUG
	barometerRead(&temperature, &pressure);
	#endif

	if(bmpNominal) {
		while(g_bmpData.lock)
			retryTakeDelay(DEFAULT_TAKE_DELAY);

		g_bmpData.lock = true;
		g_bmpData.timeStamp = getTimeStamp();
		g_bmpData.pressure = pressure;
		g_bmpData.temperature = temperature;
		g_bmpData.hasUpdate = true;
		g_bmpData.lock = false;

	} else {
		sendDaqStatus = true;
	}
}



/** imuRead_A()
 * @brief IMU Read
 * Read data from IMU
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/23/2020
 */
void imuRead_A() {

}

/* TODO: Implement checkStatus
 * Checks status of acquisition tasks including:
 *   - Whether sensors are still nominal
 *   - Whether or not to enable/disable daqScaling
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
void checkStatus_A() {
	// Check for daqScaling update
	while(g_daqScalingData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_daqScalingData.lock = true;
	if(g_daqScalingData.hasUpdate) {
		daqScaling = g_daqScalingData.enableDaqScaling;
		g_daqScalingData.hasUpdate = false;
		sendDaqStatus = true;
	}
	g_daqScalingData.lock = false;

	// GPS handling
	if(!gpsNominal) {
		gpsSetup_A();
		if(gpsNominal) {
			sendDaqStatus = true;
		}
	}

	// BMP Handling
	if(!bmpNominal) {
		bmpSetup_A();
		if(bmpNominal) {
			sendDaqStatus = true;
		}
	}

	// IMU Handling
	if(!imuNominal) {
		imuSetup_A();
		if(imuNominal) {
			sendDaqStatus = true;
		}
	}

	// ALA Handling
	if(!alaNominal) {
		alaSetup_A();
		if(alaNominal) {
			sendDaqStatus = true;
		}
	}

	// Send update if needed
	if(sendDaqStatus) {
		sendUpdate_A();
	}
}
/**
 * @brief Send Update
 * Updates the daqStatusData interface struct.
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/31/2020
 */
void sendUpdate_A() {
	while(g_daqStatusData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_daqStatusData.lock = true;
	g_daqStatusData.timeStamp = getTimeStamp();
	g_daqStatusData.daqScaling = daqScaling;
	g_daqStatusData.gpsNominal = gpsNominal;
	g_daqStatusData.bmpNominal = bmpNominal;
	g_daqStatusData.imuNominal = imuNominal;
	g_daqStatusData.alaNominal = alaNominal;
	g_daqStatusData.hasUpdate = true;
	g_daqStatusData.lock = false;

	updateLeds_A();
}


/**
 * @brief Updates Sensor LEDs
 *
 * @author Jeff Kaji
 * @date 01/11/2021
 */
void updateLeds_A() {
#ifdef NDEBUG
	HAL_GPIO_WritePin(U1S_CHECK_GPIO_Port, U1S_CHECK_Pin, imuNominal);
	HAL_GPIO_WritePin(U2S_CHECK_GPIO_Port, U2S_CHECK_Pin, alaNominal);
	HAL_GPIO_WritePin(U3S_CHECK_GPIO_Port, U3S_CHECK_Pin, bmpNominal);
	HAL_GPIO_WritePin(U4S_CHECK_GPIO_Port, U4S_CHECK_Pin, gpsNominal);

	if(gpsNominal && bmpNominal && imuNominal && alaNominal) {
		HAL_GPIO_WritePin(SENSOR_NOMINAL_GPIO_Port, SENSOR_NOMINAL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SENSOR_ERROR_GPIO_Port, SENSOR_ERROR_Pin, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(SENSOR_NOMINAL_GPIO_Port, SENSOR_NOMINAL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SENSOR_ERROR_GPIO_Port, SENSOR_ERROR_Pin, GPIO_PIN_SET);
	}
#endif
}
/**
 * @brief Split NMEA String
 * Takes raw NMEA string and replaces ',' with 0, splitting each substring
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void _splitNmea() {
	for(int i = 0; i < 80; i++) {
		if(gpsNmea[i] == ',')
			gpsNmea[i] = 0;
		else if(gpsNmea[i] == 0)
			return;
	}
}

/**
 * @brief NMEA Address Identification
 * Finds the start and end addresses of a given field within split string gpsNmea.
 *   Stores start address in _nmeaAddrStart
 *   Stores end address in _nmeaAddrEnd
 *
 * @param addr: Address to locate
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/25/2020
 */
void _findNmeaAddr(int addr) {
    for(int i = 0; i < 80; i++) {

    	// Find instance of ','
        if(gpsNmea[i] == 0) {
            addr -= 1;

            // Set end address
            if(addr == -1) {
                _nmeaAddrEnd = i;
                break;
            }

            // Set start address
            _nmeaAddrStart = i+1;
        }
    }
}

// Test Functions

/**
 * @brief Set Local NMEA String
 * Sets the local string gpsNmea[80] to the input string.
 * As of now, is only intended for use in the testbed.
 *
 * @param nmea: New value for local string
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void __setNmea(char *nmea) {
	strncpy(gpsNmea, nmea, strlen(nmea));
}

/**
 * @brief Print NMEA string
 * Prints NMEA string to console. Used for debugging.
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void __printNmea() {
	prints("\n%s", gpsNmea);
}

/**
 * @brief Get float from NMEA
 * Returns a specific float from GPS string. Used for debugging.
 * @param addr: Address within the NMEA string
 * @return None
 *
 * @author Jeff Kaji
 * @date 12/27/2020
 */
float __getFloat(int addr) {
	_findNmeaAddr(addr);
	prints("Start: %d\n", _nmeaAddrStart);
	prints("End:   %d\n", _nmeaAddrEnd);
	prints("%s\n", &gpsNmea[_nmeaAddrStart]);
	return atof(&gpsNmea[_nmeaAddrStart]);
}
void __debug() {
	prints("%s",&gpsNmea[_nmeaAddrEnd + 1]);
}
