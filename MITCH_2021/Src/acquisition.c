/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include <string.h>
#include "acquisition.h"

char gpsNMEA[80];
byte daqScaling;
byte daqScaler;
byte gpsNominal;
byte bmpNominal;
byte imuNominal;
byte alaNominal;
byte sendDaqStatus;
byte i;
byte j;
byte hasUpdate;

gpsData_t *getGpsData() {
	// Check First Characters = '$G'
	if(!pspStrCmp(gpsNMEA,0,"$G",2)) {
		gpsNominal = False;
		return NULL;
	}

	gpsData_t gpsData;
	gpsData.timeStamp = getTimeStamp();
	strcpy(gpsData.NMEA, gpsNMEA);


	return &gpsData;
}
