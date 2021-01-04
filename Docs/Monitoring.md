# Monitoring
This document was last edited on *4 Jan 2021*. <br/>

### Description
This is a documentation for the code supporting the task _Monitoring_. <br>
Created for use by Purdue Space Program Solids. <br><br>
[View][1] the _Task Flow Chart_. <br>

### Required Files:
* [monitoring.h](../MITCH_2021/IncPSP/monitoring.h)
* [monitoring.c](../MITCH_2021/SrcPSP/monitoring.c)


[1]: https://drive.google.com/file/d/1L-hbviqYfiP9NwsLlXdcOAHZ8kLTNxf0/view?usp=sharing "Task Flow Chart (Read Only)"

## monitoring.h
_monitoring.h_ is a header file. A header file is a file with extension _.h_ which contains C function declarations and macro definitions to be shared between several source files. <br>
```c
void setup_M();
void loop_M();
void checkBatteryVoltage();
void checkContinuity();
void checkButtonState();
void checkHardwareDeploymentDisable_M();
void sendUpdate_M();
``` 
The above is the main contents of _monitoring.h_. This declares all the functions created in _monitoring.c_.

## monitoring.c
_monitoring.c_ is a source file. A source file is a file with extension _.c_ that contains program instructions. <br> <br>
_monitoring.c_ gathers data for the voltage of the battery, the continuity of the parachute deployment charges, the state of the button, and the state of the hardware charge disable which it stores as global variables for other files to access.

```c
float batteryVoltage; // Voltage of battery (V)
bool continuity[4]; // Continuity status of parachute deployment charges
bool buttonState; // State of button (on/off)
bool hardwareDeploymentDisable; // Terminal block disables deployment in hardware
```
The above declares the local variables. Listed below are the current variables:
* __batteryVoltage__ is the value of the voltage departing from the battery. It is being detected from _TBD_ on the PCB.
* __continuity[4]__ is a bool array which checks the continuity connection of the 4 parachute deployment charges. It is detected from the _Sense_A-D_ signals on the PCB.
* __buttonState__ detects the state of the switch on the hardware. This switch _TBD_
* __hardwareDeploymentDisable__ detects if the hardware disable terminal block system is enabled or disabled. During actual flight (or when we want parachute charges to deploy), the connection between the terminal block should be severed.


### setup_M()
```c
void setup_M() {
	//initialize variables
	#ifndef NDEBUG
		_monitoringFile = setupMonitoringFile_DS();
	#else
		// TODO: Implement Monitoring Setup
	#endif
	batteryVoltage = 7.4;
	for (ui8 i = 0; i < 4; i++) {
		continuity[i] = true;
	}
	buttonState = false;
	hardwareDeploymentDisable = false;
}
```
The above function is the one-time setup function for _monitoring.c_. This function only initializes local variables.


### loop_M()
```c
void loop_M() {
	#ifndef NDEBUG
		if(!simulateMonitoring) {
			if(notifyWhenReadAborted)
				print("Monitoring read aborted.\n");
			return;
		}
		fscanf(_monitoringFile, "%f", &batteryVoltage);
		fscanf(_monitoringFile, "%d", &continuity[0]);
		fscanf(_monitoringFile, "%d", &continuity[1]);
		fscanf(_monitoringFile, "%d", &continuity[2]);
		fscanf(_monitoringFile, "%d", &continuity[3]);
		fscanf(_monitoringFile, "%d", &buttonState);
		fscanf(_monitoringFile, "%d", &hardwareDeploymentDisable);
	#else
		checkBatteryVoltage_M();
		checkContinuity_M();
		checkButtonState_M();
		checkHardwareDeploymentDisable_M();
	#endif
	sendUpdate_M();
}
```
The above function is the flight loop function which will run continuously during flight. When in debug mode, the function will read data from a _.txt_ file. When not in debug mode, the function will call other functions to gather the necessary monitoring data.


### checkBatteryVoltage_M()
```c
void checkBatteryVoltage_M() {
	// TODO: Implement battery voltage reading from hardware
}
``` 
_TBD_


### checkContinuity_M()
```c
void checkContinuity_M() {
	for (ui8 i = 0; i < 4; i++) {
		// TODO: Implement continuity reading from hardware
	}
}
``` 
_TBD_


### checkButtonState_M()
```c
void checkButtonState_M() {
	// TODO: Implement button state reading from hardware
}
``` 
_TBD_


### checkHardwareDeploymentDisable_M()
```c
void checkHardwareDeploymentDisable_M() {
	// TODO: Implement hardware deployment disable reading from hardware
}
``` 
_TBD_


### sendUpdate_M()
```c
void sendUpdate_M() {
	while(g_monitoringData.lock) {
		retryTakeDelay(0);
	}
	g_monitoringData.lock = true;
	g_monitoringData.timeStamp = getTimeStamp();
	g_monitoringData.batteryVoltage;
	for (ui8 i = 0; i < 4; i++) {
		g_monitoringData.continuity[i] = continuity[i];
	}
	g_monitoringData.buttonState = buttonState;
	g_monitoringData.hardwareDeploymentDisable = hardwareDeploymentDisable;
	g_monitoringData.hasUpdate = true;
	g_monitoringData.lock = false;
}
``` 
The above function handles sending the _monitoring.c_ data to other files. To accomplish this, the function stores all local data as global variables which can be accessed by other files. These globals include:
* __g_monitoringData.batteryVoltage__ is the value of the voltage departing from the battery. It is being detected from _TBD_ on the PCB.
* __g_monitoringData.continuity[4]__ is a bool array which checks the continuity connection of the 4 parachute deployment charges. It is detected from the _Sense_A-D_ signals on the PCB.
* __g_monitoringData.buttonState__ detects the state of the switch on the hardware. This switch _TBD_
* __g_monitoringData.hardwareDeploymentDisable__ detects if the hardware disable terminal block system is enabled or disabled. During actual flight (or when we want parachute charges to deploy), the connection between the terminal block should be severed.
* __g_monitoringData.hasUpdate__ tells other files if the data has an update from previous access.
* __g_monitoringData.lock__ prevents multiple files from accessing the global variables at the same time by "locking" the globals while writing or reading.