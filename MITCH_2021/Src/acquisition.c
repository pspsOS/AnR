/*
 * acquisition.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include <string.h>
#include "acquisition.h"


gpsData_t g_gpsData;

char gpsNMEA[MAX_NMEA];
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

void getGpsData() {
	// Check First Characters = '$G'
	if (!(strncmp(gpsNMEA[0], "$G", 2))) {
		gpsNominal = FALSE;
		return NULL;
	}

	g_gpsData.timeStamp = getTimeStamp();
	strncpy(gpsData.NMEA, gpsNMEA, strlen(gpsNMEA));

	return;
}
