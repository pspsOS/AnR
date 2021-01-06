# Acquisition
This document was last edited by Jeffrey Kaji on *5 Jan 2021*. <br/>

## Description
This is a documentation for the code supporting the task _Acquisition_. <br>
Created for use by Purdue Space Program Solids. <br><br>
[View][1] the _Task Flow Chart_. <br>

__Required Files:__
* [acquisition.h](../MITCH_2021/IncPSP/acquisition.h)
* [acquisition.c](../MITCH_2021/SrcPSP/acquisition.c)


[1]: https://drive.google.com/file/d/17b3zl4mSmjUOjUfPsQ9OTFR39xYNh1Iw/view?usp=sharing "Task Flow Chart (Read Only)"


## acquisition.h
_acquisition.h_ is a header file. A header file is a file with extension _.h_ which contains C function declarations and macro definitions to be shared between several source files. <br>
```c
// Parent Functions
void setup_A();
void loop_A();


// Setup
void gpsSetup_A();
void bmpSetup_A();
void imuSetup_A();
void alaSetup_A();

// Loop
void gpsRead_A();
void bmpRead_A();
void imuRead_A();
void checkStatus_A();
void sendUpdate_A();
``` 
The above is the main contents of _acquisition.h_. This declares all the functions created in _acquisition.c_.

## acquisition.c
_acquisition.c_ is a source file. A source file is a file with extension _.c_ that contains program instructions. <br> <br>
_acquisition.c_ acquires the raw data from the sensors and sends the data to processing via the global interface structs.

```c
/* Local variable declarations */
char gpsNmea[MAX_NMEA]; // Buffer that holds GPS String
bool daqScaling;        // Enables/Disables DAQ Scaling
ui8 daqScaler;          // DAQ Scaling number – Sets the ratio
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
```
The above declares the local variables. Listed below are the current variables:
* __gpsNmea[MAX_NMEA]__ is a character array (string) that holds the latest read from the GPS sensor. It is then parsed and passed to Processing by the function __gpsRead_A()__ via the struct __g_gpsData__. The constant __MAX_NMEA__ is defined in the header file [__common.h__](Common.md).
* __daqScaling__ is true when DAQ Scaling is enabled. DAQ Scaling makes _Acquisition_ act as a finite state machine, making measurments across the different sensors at different rates. 
* __daqScaler__ determines the ratios of data acquisition between each of the sensors. Let __daqScaler = n__ and assume DAQ Scaling is enabled. For each GPS measurment, _Acquisition_ will take __n__ BMP measurments and __n<sup>2</sup>__ IMU measurements.
* __gpsNominal__ is true if connection with the GPS has been established and if it is acting nominally.
* __bmpNominal__ is true if connection with the BMP has been established and if it is acting nominally.
* __imuNominal__ is true if connection with the IMU has been established and if it is acting nominally.
* __alaNominal__ is true if connection with the ALA has been established and if it is acting nominally.
* __sendDaqStatus__ is true if the DAQ Status struct should be updated.
* __gpsCounter__ is TBD
* __bmpCounter__ is TBD
* __fsmState__ is TBD
* **_nmeaAddrStart** is used when parsing NMEA strings. It stores the start index of a specific element within the string and is set by the the function [**_findNmeaAddr(int addr)**](###_findNmeaAddr(int-addr)).
*  **_nmeaAddrEnd** is used when parsing NMEA strings. It stores the end index of a specific element within the string and is set by the the function [**_findNmeaAddr(int addr)**](###_findNmeaAddr(int-addr)).
*  **_gpsFile** is a file pointer to the text file that has test data for the GPS. This is only declared if **NDEBUG** is not defined.
*  **_bmpFile** is a file pointer to the text file that has test data for the BMP. This is only declared if **NDEBUG** is not defined.
*  **_imuFile** is a file pointer to the text file that has test data for the IMU. This is only declared if **NDEBUG** is not defined.
*  **_alaFile** is a file pointer to the text file that has test data for the ALA. This is only declared if **NDEBUG** is not defined.
<br><br>




### setup_A()
```c
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
```
The above function is the one-time setup function for _acquisition.c_. This function has different sections, each denoted by their own comment header. <br>
#### Intialize local variables
In this section, all local variables are initialized as needed.
```c
	daqScaling = false;
	daqScaler = DEFAULT_DAQ_SCALER;
```
> DAQ Scaling is disabled on startup. The DAQ Scaler is set to the default value constant which is defined in common.h.
<br><br>

```c
	gpsNominal = false;
	bmpNominal = false;
	imuNominal = false;
	alaNominal = false;
```
> All _"sensor nominal"_ variables are set to false. Each variable will be set to true once a connection to its sensor is established.
<br><br>

```c
	sendDaqStatus = false;
```
> Since __setup_A()__ already updates the DAQ Status interface struct, __sendDaqStatus__ is initially set to false.
<br><br>

```c
	gpsCounter = 0;
	bmpCounter = 0;
	fsmState = ACQUIRE_IMU_BMP_GPS;
```
> Finite state machine variables. TBD
<br><br>

#### Setup sensors
In this section, each sensor is setup by calling their setup functions.
```c
	gpsSetup_A();
	bmpSetup_A();
	imuSetup_A();
	alaSetup_A();
```

#### Unlock initialized interface structs
In this section, the initialized global interface structs used by _Acquisition_ are unlocked, insuring they can be accessed.
```c
	g_daqStatusData.lock = false;
	g_daqScalingData.lock = false;
```
#### Send update
The DAQ Scaling Status struct is updated via the function [__sendUpdate_A()__](###sendUpdate_A()).
```c
	sendUpdate_A();
```
<br>




### loop_A()
TBD
<br><br>




### gpsSetup_A()
```c
void gpsSetup_A() {
	#ifndef NDEBUG
		_gpsFile = setupSensorFile_DS(GPS, &gpsNominal);
	#else
		// TODO: Implement gpsSetup
	#endif
}
```
The above function sets up the GPS sensor. If **NDBEUG** is defined (Debug Mode), **_gpsFile** is set using the function **setupSensorFile_DS** which is declared in the **debugSettings**. If **NDEBUG** is not defined, *TODO*.
<br><br>




### bmpSetup_A()
```c
void bmpSetup_A() {
	#ifndef NDEBUG
		_bmpFile = setupSensorFile_DS(BMP, &bmpNominal);
	#else
		// TODO: Implement bmpSetup
	#endif
}
```
The above function sets up the BMP sensor. If **NDBEUG** is defined (Debug Mode), **_bmpFile** is set using the function **setupSensorFile_DS** which is declared in the **debugSettings**. If **NDEBUG** is not defined, *TODO*.
<br><br>




### imuSetup_A()

```c
void imuSetup_A() {
	#ifndef NDEBUG
		_imuFile = setupSensorFile_DS(IMU, &imuNominal);
	#else
		// TODO: Implement imuSetup
	#endif
}
```
The above function sets up the IMU sensor. If **NDBEUG** is defined (Debug Mode), **_imuFile** is set using the function **setupSensorFile_DS** which is declared in the **debugSettings**. If **NDEBUG** is not defined, *TODO*.
<br><br>




### alaSetup_A()
```c
void alaSetup_A() {
	#ifndef NDEBUG
		_alaFile = setupSensorFile_DS(ALA, &alaNominal);
	#else
		// TODO: Implement alaSetup
	#endif
}
```
The above function sets up the ALA sensor. If **NDBEUG** is defined (Debug Mode), **_alaFile** is set using the function **setupSensorFile_DS** which is declared in the **debugSettings**. If **NDEBUG** is not defined, *TODO*.
<br><br>




### gpsRead_A()
TBD
<br><br>




### bmpRead_A()
TBD
<br><br>




### imuRead_A()
TBD
<br><br>




### checkStatus_A()
```c
void checkStatus_A() {
	// Check for daqScaling update
	while(g_daqScalingData.lock) {
		retryTakeDelay(0);
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
```
The above function checks to see if DAQ Scaling should be enabled/disabled, handles the sensors, and 
<br><br>




### _A()
TBD
<br><br>




### gpsSetup_A())
<br><br><br><br><br><br><br><br><br><br><br><br>

### sendUpdate_M()
```c
void sendUpdate_M() {
	while(g_acquisitionData.lock) {
		retryTakeDelay(0);
	}
	g_acquisitionData.lock = true;
	g_acquisitionData.timeStamp = getTimeStamp();
	g_acquisitionData.batteryVoltage;
	for (ui8 i = 0; i < 4; i++) {
		g_acquisitionData.continuity[i] = continuity[i];
	}
	g_acquisitionData.buttonState = buttonState;
	g_acquisitionData.hardwareDeploymentDisable = hardwareDeploymentDisable;
	g_acquisitionData.hasUpdate = true;
	g_acquisitionData.lock = false;
}
``` 
The above function handles sending the _acquisition.c_ data to other files. To accomplish this, the function stores all local data as global variables which can be accessed by other files. These globals include:
* __g_acquisitionData.batteryVoltage__ is the value of the voltage departing from the battery. It is being detected from _TBD_ on the PCB.
* __g_acquisitionData.continuity[4]__ is a bool array which checks the continuity connection of the 4 parachute deployment charges. It is detected from the _Sense_A-D_ signals on the PCB.
* __g_acquisitionData.buttonState__ detects the state of the switch on the hardware. This switch _TBD_
* __g_acquisitionData.hardwareDeploymentDisable__ detects if the hardware disable terminal block system is enabled or disabled. During actual flight (or when we want parachute charges to deploy), the connection between the terminal block should be severed.
* __g_acquisitionData.hasUpdate__ tells other files if the data has an update from previous access.
* __g_acquisitionData.lock__ prevents multiple files from accessing the global variables at the same time by "locking" the globals while writing or reading.