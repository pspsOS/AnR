/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

#ifdef NDEBUG
	#include <baroInterface.h>
#else
	#include "../IncDebug/debugSettings.h"
	#include <unistd.h>
#endif

// Acquisition Finite States
#define ACQUIRE_IMU_BMP_GPS (0)
#define ACQUIRE_IMU_BMP (1)
#define ACQUIRE_IMU (2)

/* Global variable declarations */


/* Local variable declarations */
char gpsNmea[MAX_NMEA]; // Buffer that holds GPS String
bool daqScalingEnabled; // Enables/Disables DAQ Scaling
ui8 daqScaler;          // DAQ Scaling number � Sets the ratio
bool gpsNominal;        // Indicates whether the GPS is nominal
bool bmpNominal;        // Indicates whether the BMP is nominal
bool imuNominal;        // Indicates whether the IMU is nominal
bool alaNominal;        // Indicates whether the ALA is nominal
bool sendDaqStatus;     // Indicates whether to update the daqStatusData struct
ui8 bmpCounter;         // Counter used in DAQ Scaling for Finite State Machine
ui8 imuCounter;         // Counter used in DAQ Scaling for Finite State Machine
ui8 fsmState;           // Defines state of Finite State Machine

ui8 delayVal = 0;

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
	daqScalingEnabled = false;
	daqScaler = DEFAULT_DAQ_SCALER;
	gpsNominal = false;
	bmpNominal = false;
	imuNominal = false;
	alaNominal = false;
	sendDaqStatus = false;
	bmpCounter = 0;
	imuCounter = 0;


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


/** loop_A()
 * @brief Acquisition Loop
 * This function is the function that is called repeatedly by the Acquisition task.
 *
 * @retval ui8 delayVal: value which specifies which delay time to use
 * TODO: Make delayVal return the constants defined in main
 *
 * @author Jeff Kaji
 * @date 01/19/2021
 */
ui8 loop_A() {



	if(daqScalingEnabled) {
		if(imuNominal || alaNominal) imuRead_A();
		else prints("--------IMU Skip %d\n", imuCounter);

		if(imuCounter == 0) {
			if(bmpNominal) bmpRead_A();
			else prints("----BMP Skip %d\n", bmpCounter);

			if(bmpCounter == 0) {
				if(gpsNominal) gpsRead_A();
			}
		}

		if(imuNominal || alaNominal || imuCounter > 0) {
			imuCounter += 1;
			delayVal = 2;
		} else if(bmpNominal || bmpCounter > 0) {
			bmpCounter += 1;
			delayVal = 1;
		} else {
			delayVal = 0;
		}

		if(imuCounter == IMU_MULTIPLIER) {
			bmpCounter += 1;
			imuCounter = 0;
		}

		if(bmpCounter == BMP_MULTIPLIER) {
			bmpCounter = 0;
		}


	} else {
		if(imuNominal || alaNominal) imuRead_A();
		if(bmpNominal) bmpRead_A();
		if(gpsNominal) gpsRead_A();
		delayVal = 0;
	}

	checkStatus_A();
	return delayVal;
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

	// local variables
	int time; //holds value to compare
	bool firstFlag; // flag signaling function is being called the first time

	firstFlag = false;


	// loads in data
	_loadGpsData();
	_splitNmea();


	//lock structure
	while(g_gpsData.lock)
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	g_gpsData.lock = true;



	// first time loading in
	if((!strcmp((char*)g_gpsData.nmeaGGA, "") && !strcmp((char*)g_gpsData.nmeaRMC, "")))
	{
		_addNmeaData();

		//load next packet
		_loadGpsData();
		_splitNmea();

		firstFlag = true;

	}

	//if no unsent data
	if((strcmp((char*)g_gpsData.nmeaGGA, "") && strcmp((char*)g_gpsData.nmeaRMC, "")) && !firstFlag)
	{
		_addNmeaData();
		//load next packet
		_loadGpsData();
		_splitNmea();

		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);

		if(time == g_gpsData.timeStamp )
		{

			// if time stamps are equal
			_addNmeaData();

			g_gpsData.hasUpdate = true;

			//unlocking
			g_gpsData.lock = false;


		}
		else
		{

			// time stamps are different

			// setting unreciveced data to zero
			if(!_getNmeaType())
			{
				//never recieved rmc
				//strncpy((char*)g_gpsData.nmeaRMC, "", 0);
				_clearNmea((char*)&g_gpsData.nmeaRMC);
				g_gpsData.speed = 0.0;

			}
			else
			{
				//never recieved gga
				//strncpy((char*)g_gpsData.nmeaGGA, "", 0);
				_clearNmea((char*)&g_gpsData.nmeaGGA);
				g_gpsData.alt = 0.0;
			}


			//unlocking
			g_gpsData.hasUpdate = true;
			g_gpsData.lock = false;

			do
			{
					#ifndef NDEBUG
						g_gpsData.hasUpdate = false; // Breaks infinite loop if run in testbed
					#endif
				retryTakeDelay(ACQUISITION_TASK_DELAY2 / 4);
			} while(g_gpsData.hasUpdate || g_gpsData.lock);

			//relock
			g_gpsData.lock = true;

			_addNmeaData();


			// setting unreciveced data to zero
			if(!_getNmeaType())
			{

				//never recieved gga
				//strncpy((char*)g_gpsData.nmeaGGA, "", 0);
				_clearNmea((char*)&g_gpsData.nmeaGGA);
				g_gpsData.alt = 0.0;

			}
			else
			{
				//never recieved rmc
				//strncpy((char*)g_gpsData.nmeaRMC, "", 0);
				_clearNmea((char*)&g_gpsData.nmeaRMC);
				g_gpsData.speed = 0.0;

			}

			//unlock
			g_gpsData.hasUpdate = false;
			g_gpsData.lock = false;


		}
	} else {

		// unsent data in struct
		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);

		if(time == g_gpsData.timeStamp ) {
			// if time stamps are equal
			_addNmeaData();

			g_gpsData.hasUpdate = true;

			//unlocking
			g_gpsData.lock = false;


	    } else {

			//timestamps are different

			//unlocking
				g_gpsData.hasUpdate = true; // This sets hasUpdate = true
				g_gpsData.lock = false;

				do {
					retryTakeDelay(ACQUISITION_TASK_DELAY2);
					#ifndef NDEBUG
						g_gpsData.hasUpdate = false; // Breaks infinite loop if run in testbed
					#endif
				} while(g_gpsData.hasUpdate || g_gpsData.lock); // Only breaks if hasUpdate = false or locked


				//relock
				while(g_gpsData.lock)
						retryTakeDelay(DEFAULT_TAKE_DELAY);
				g_gpsData.lock = true;

				_addNmeaData();


				// setting unreciveced data to zero
				if(!_getNmeaType())
				{

					//never recieved gga
					//strncpy((char*)g_gpsData.nmeaGGA, "", 0);
					_clearNmea((char*)&g_gpsData.nmeaGGA);
					g_gpsData.alt = 0.0;

				}
				else
				{
					//never recieved rmc
					//strncpy((char*)g_gpsData.nmeaRMC, "", 0);
					_clearNmea((char*)&g_gpsData.nmeaRMC);
					g_gpsData.speed = 0.0;

				}

				//unlock
				g_gpsData.hasUpdate = false;
				g_gpsData.lock = false;


		}


	}




	/*

	print("Reading:",);

// Parse each GPS packet type
	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		print("GGA\n");
		strncpy((char*)g_gpsData.nmeaGGA, gpsNmea, strlen(gpsNmea));
	}
	//	Type = GPRMC
	else if (!(strncmp(&gpsNmea[0], "$GPRMC", 6))) {
		//puts("RMC");
		print("RMC\n");
		strncpy((char*)g_gpsData.nmeaGGA, gpsNmea, strlen(gpsNmea));
	}

	//	Catch Bad Read
	else {
		gpsNominal = false;


	}

	g_gpsData.timeStamp = getTimeStamp();


*/
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
	#ifndef NDEBUG
		prints("    BMP Read %d\n", bmpCounter);
		// TODO: Implement BMP Simulation
	#else
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
	#ifndef NDEBUG
		prints("        IMU Read %d\n", imuCounter);
		// TODO: Implement IMU Simulation
	#else
		// TODO: Implement imuRead in hardware
	#endif
}


/** checkStatus_A()
 * @brief Check Status of Acquisition Task
 * Check status of DAQ Scaling and Sensors. Update if necessary.
 *
 * @author Jeff Kaji
 * @date 12/23/2020
 */
void checkStatus_A() {
	// Check for daqScalingEnabled update
	while(g_daqScalingData.lock) {
		retryTakeDelay(DEFAULT_TAKE_DELAY);
	}
	g_daqScalingData.lock = true;
	if(g_daqScalingData.hasUpdate) {
		daqScalingEnabled = g_daqScalingData.enableDaqScaling;
		g_daqScalingData.hasUpdate = false;
		sendDaqStatus = true;
		#ifndef NDEBUG
		prints("-- DAQ Scaling: %s --\n", (daqScalingEnabled) ? _TRUE : _FALSE);
		#endif
	}
	g_daqScalingData.lock = false;

	// GPS handling
	if(!gpsNominal) {
#ifndef NDEBUG
		if(simulateGps)
#endif
		gpsSetup_A();
		if(gpsNominal) {
			sendDaqStatus = true;
		}
	}

	// BMP Handling
	if(!bmpNominal) {
#ifndef NDEBUG
		if(simulateGps)
#endif
		bmpSetup_A();
		if(bmpNominal) {
			sendDaqStatus = true;
		}
	}

	// IMU Handling
	if(!imuNominal) {
#ifndef NDEBUG
		if(simulateImu)
#endif
		imuSetup_A();
		if(imuNominal) {
			sendDaqStatus = true;
		}
	}

	// ALA Handling
	if(!alaNominal) {
#ifndef NDEBUG
		if(simulateAla)
#endif
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
 * Is called by the function checkStatus_A().
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
	g_daqStatusData.daqScalingEnabled = daqScalingEnabled;
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
 * @brief adds Nmea data to struct
 * gets nmea varaibles from the string and adds variables to struct depending on Nmea type
 *
 * @param None
 * @retval None
 *
 * @author Jack Wiley
 * @date 1/25/2020
 */

void _addNmeaData()
{
	// local variables
	int time; //holds value until copied into struct
	float altitude;  //holds value until copied into struct
	float speed; //holds value until copied into struct

	if(!_getNmeaType())
			{
		//Type GCA

		//adds GCA to struct
		strncpy((char*)g_gpsData.nmeaGGA, gpsNmea, strlen(gpsNmea));

		//adds time to stuct
		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);

		g_gpsData.timeStamp = time;



		//adds altitude to struct
		altitude = 0;
		_findNmeaAddr(7);
		altitude = atof(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.alt = altitude;

	}
	else
	{
		//Type RMC
		strncpy((char*)g_gpsData.nmeaRMC, gpsNmea, strlen(gpsNmea));

		//adds time to stuct
		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.timeStamp = time;

		//adds speed to struct
		speed = 0;
		_findNmeaAddr(7);
		speed = atof(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.speed = speed;



	}


}


/**
 * @brief gets the Nmea type
 * returns 0 if the Nmea is GCC, and 1 if the Nmea is RMC
 *
 * @param None
 * @retval type, 1 or 0 depending on description above
 *
 * @author Jack Wiley
 * @date 1/25/2020
 */
int _getNmeaType()
{

	//	Type = GPGGA
	if (!(strncmp(&gpsNmea[0], "$GPGGA", 6))) {
		return 0;
	}
		//	Type = GPRMC
	else
	{
		return 1;
	}

}

/**
 * @brief Loads Gps Data
 * Takes data from either hardware or text file depending on if debuging
 *
 * @param None
 * @retval None
 *
 * @author Jack Wiley
 * @date 1/25/2020
 */
void _loadGpsData()
{
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

void _clearNmea(char *nmea) {
	for(int i = 0; i < MAX_NMEA; i++)
		nmea[i] = 0;
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
