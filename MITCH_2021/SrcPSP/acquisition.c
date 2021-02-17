/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/acquisition.h"

#ifdef NDEBUG
	#include "baroInterface.h"
	#include "genericInterface.h"
	#include "gpsInterface.h"
#else
	#include "../IncDebug/debugSettings.h"
	#include <unistd.h>
#endif


/* Global variable declarations */


/* Local variable declarations */
char gpsNmea[MAX_NMEA]; // Buffer that holds GPS String
char unsplitGpsNmea[MAX_NMEA]; // Buffer that holds GPS String and is not split
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
ui16 ala;
//extern ADC_HandleTypeDef hadc1;
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

#if defined(NDEBUG) && !defined(BYPASS_ACQUISITION_LEDS)
	#ifdef ACQUISITION_LEDS_BANKED
		extern srBank_t ACQUISITION_LED_Bank;
	#endif
	extern led_t U1S_CHECK_Led;
	extern led_t U2S_CHECK_Led;
	extern led_t U3S_CHECK_Led;
	extern led_t U4S_CHECK_Led;

	extern led_t SENSOR_NOMINAL_Led;
	extern led_t SENSOR_ERROR_Led;

#elif defined(NDEBUG)
	srBank_t ACQUISITION_LED_Bank;
	led_t U1S_CHECK_Led;
	led_t U2S_CHECK_Led;
	led_t U3S_CHECK_Led;
	led_t U4S_CHECK_Led;

	led_t SENSOR_NOMINAL_Led;
	led_t SENSOR_ERROR_Led;
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
	gpsNominal = true;
	bmpNominal = true;
	imuNominal = true;
	alaNominal = true;
	sendDaqStatus = false;
	bmpCounter = 0;
	imuCounter = 0;

#if defined(NDEBUG) && defined(BYPASS_ACQUISITION_LEDS)
	ACQUISITION_LED_Bank = newSrBank(0, FAKE_GPIO);

	U1S_CHECK_Led = newPinLed(0, FAKE_GPIO);
	U2S_CHECK_Led = newPinLed(0, FAKE_GPIO);
	U3S_CHECK_Led = newPinLed(0, FAKE_GPIO);
	U4S_CHECK_Led = newPinLed(0, FAKE_GPIO);

	SENSOR_NOMINAL_Led = newPinLed(0, FAKE_GPIO);
	SENSOR_ERROR_Led = newPinLed(0, FAKE_GPIO);
#endif

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
	updateLeds_A();


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
		else printf("########IMU Skip %d\r\n", imuCounter);

		if(imuCounter == 0) {
			if(bmpNominal) bmpRead_A();
			else printf("####BMP Skip %d\r\n", bmpCounter);

			if(bmpCounter == 0) {
				if(gpsNominal) gpsRead_A();
				else printf("GPS Skip\r\n");
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
		gpsInit(&gpsNominal);
		//notify(TASK_UPDATE, GPS);
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
		//notify(TASK_UPDATE, BMP);
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
		//notify(TASK_UPDATE, IMU);
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
		//notify(TASK_UPDATE, ALA);
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
 * @author Jack Wiley
 * @date 02/01/2021
 */
void gpsRead_A() {
#ifdef BYPASS_GPS
	printf("GPS Read \r\n");
	return;
#endif
	// local variables
	int time; //holds value to compare
	bool firstFlag; // flag signaling function is being called the first time

	firstFlag = false;


	// loads in data
	_loadGpsData();
	strncpy((char*)unsplitGpsNmea, gpsNmea, strlen(gpsNmea));

	_splitNmea();

	//recieving no new data
	if((!strcmp((char*)g_gpsData.nmeaGGA, unsplitGpsNmea) || !strcmp((char*)g_gpsData.nmeaRMC, unsplitGpsNmea)))
	{
		g_gpsData.hasUpdate = false;
		// leave function
		return;
	}



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
		strncpy((char*)unsplitGpsNmea, gpsNmea, strlen(gpsNmea));
		_splitNmea();

		firstFlag = true;

	}

	//if no unsent data
	if((strcmp((char*)g_gpsData.nmeaGGA, "") && strcmp((char*)g_gpsData.nmeaRMC, "")) && !firstFlag)
	{


		_addNmeaData();
		//load next packet
		_loadGpsData();
		strncpy((char*)unsplitGpsNmea, gpsNmea, strlen(gpsNmea));
		_splitNmea();

		//recieving no new data
		if((!strcmp((char*)g_gpsData.nmeaGGA, unsplitGpsNmea) || !strcmp((char*)g_gpsData.nmeaRMC, unsplitGpsNmea)))
		{
			g_gpsData.hasUpdate = false;
			// leave function
			return;
		}

		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);

		//printf("%d\n",g_gpsData.utc);
		if(time == g_gpsData.utc )
		{
			//printf("here\n");
			// if time stamps are equal
			_addNmeaData();

			g_gpsData.hasUpdate = true;

			//unlocking
			g_gpsData.lock = false;

			__printGpsData();

		}
		else
		{

			// time stamps are different

			// setting unreciveced data to zero
			if((strcmp((char*)g_gpsData.nmeaGGA, "")))
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
				g_gpsData.fix = 0;

			}


			//unlocking
			g_gpsData.hasUpdate = true;
			g_gpsData.lock = false;
			__printGpsData();
			do
			{
					#ifndef NDEBUG
						g_gpsData.hasUpdate = false; // Breaks infinite loop if run in testbed
					#endif
				retryTakeDelay(ACQUISITION_TASK_DELAY2 / 4);
			} while(g_gpsData.hasUpdate || g_gpsData.lock);

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
				g_gpsData.fix = 0;

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

		if(time == g_gpsData.utc ) {

			// if time stamps are equal
			_addNmeaData();

			g_gpsData.hasUpdate = true;

			//unlocking
			g_gpsData.lock = false;
			__printGpsData();


	    } else {

			//timestamps are different

			//unlocking
				g_gpsData.hasUpdate = true; // This sets hasUpdate = true
				g_gpsData.lock = false;
				__printGpsData();
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
					g_gpsData.fix = 0;

				}

				//unlock
				g_gpsData.hasUpdate = false;
				g_gpsData.lock = false;

		}
	}
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
	printf("    BMP Read %d\r\n", bmpCounter);
	return;
	i32 temperature = 0;
	i32 pressure = 0;
	#ifndef NDEBUG

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
	printf("        IMU Read %d\r\n", imuCounter);
	#ifndef NDEBUG

		// TODO: Implement IMU Simulation
	#else
		#ifndef BYPASS_ALA
			ADC_Select_CH1();
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 1000);
			ala = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
		#endif
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
		printf("-- DAQ Scaling: %s --\n", (daqScalingEnabled) ? _TRUE : _FALSE);
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
		updateLeds_A();
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

	#if !defined(SUPRESS_TASK_UPDATES) && !defined(SUPRESS_ALL)
		printf("    Task Update: Acquisition DAQ Status Data sent\r\n");
	#endif
}


/**
 * @brief Updates Sensor LEDs
 *
 * @author Jeff Kaji
 * @date 01/11/2021
 */
void updateLeds_A() {
#ifdef NDEBUG
	setLed(&U1S_CHECK_Led, imuNominal);
	setLed(&U2S_CHECK_Led, alaNominal);
	setLed(&U3S_CHECK_Led, bmpNominal);
	setLed(&U4S_CHECK_Led, gpsNominal);

	if(gpsNominal && bmpNominal && imuNominal && alaNominal) {
		setLed(&SENSOR_NOMINAL_Led, GPIO_PIN_SET);
		setLed(&SENSOR_ERROR_Led, GPIO_PIN_RESET);
	} else {
		setLed(&SENSOR_NOMINAL_Led, GPIO_PIN_RESET);
		setLed(&SENSOR_ERROR_Led, GPIO_PIN_SET);
	}

#ifdef ACQUISITION_LEDS_BANKED
	setSrBank(&ACQUISITION_LED_Bank);
#endif

	#if !defined(SUPRESS_TASK_UPDATES) && !defined(SUPRESS_ALL)
		printf("    Task Update: Acquisition LEDs Updated\r\n");
	#endif
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
	int fix; //holds value until copied into struct

	if(!_getNmeaType())
			{
		//Type GGA
		//adds GCA to struct
		strncpy((char*)g_gpsData.nmeaGGA, unsplitGpsNmea, strlen(unsplitGpsNmea));

		//adds time to stuct
		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);

		g_gpsData.utc = time;
		g_gpsData.timeStamp = getTimeStamp();


		//adds altitude to struct
		altitude = 0;
		_findNmeaAddr(9);
		altitude = atof(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.alt = altitude;

		//adds fix to struct
		fix = 0;
		_findNmeaAddr(6);
		fix = atof(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.fix = fix;

	}
	else
	{
		//Type RMC
		strncpy((char*)g_gpsData.nmeaRMC, unsplitGpsNmea, strlen(unsplitGpsNmea));

		//adds time to stuct
		time = 0;
		_findNmeaAddr(1);
		time = atoi(&gpsNmea[_nmeaAddrStart]);
		g_gpsData.utc = time;
		g_gpsData.timeStamp = getTimeStamp();

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
				printf("GPS read aborted.\n");
			return;
		}
		fscanf(_gpsFile, "%s", gpsNmea);
	#else
		//TODO: Implement gpsRead w/ hardware
		gpsLoadString(gpsNmea);

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
 * @brief Prints Gps Data
 *
 * @param None
 * @retval None
 *
 * @author Jack Wiley
 * @date 1/26/2020
 */
void __printGpsData()
{

	printf("Time: %d\n",g_gpsData.utc);
	printf("GGA: %s\n",g_gpsData.nmeaGGA);
	printf("RMC: %s\n",g_gpsData.nmeaRMC);
	printf("Fix: %d\n",g_gpsData.fix);
	printf("Alt: %f\n",g_gpsData.alt)	;
	printf("Speed: %f\n\n",g_gpsData.speed);
}

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
 * @brief Prints NMEA string
 * printf NMEA string to console. Used for debugging.
 *
 * @param None
 * @retval None
 *
 * @author Jeff Kaji
 * @date 12/26/2020
 */
void __printNmea() {
	printf("\n%s", gpsNmea);
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
	printf("Start: %d\n", _nmeaAddrStart);
	printf("End:   %d\n", _nmeaAddrEnd);
	printf("%s\n", &gpsNmea[_nmeaAddrStart]);
	return atof(&gpsNmea[_nmeaAddrStart]);
}
void __debug() {
	printf("%s",&gpsNmea[_nmeaAddrEnd + 1]);
}
