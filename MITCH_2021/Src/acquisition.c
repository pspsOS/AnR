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


// Setup
/* TODO: Implement gpsSetup
 * Established connection with GPS.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool gpsSetup() {

}


/* TODO: Implement bmpSetup
 * Established connection with Barometric Pressure Sensor.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool bmpSetup() {

}


/* TODO: Implement imuSetup
 * Established connection with IMU.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool imuSetup() {

}


/* TODO: Implement alaSetup
 * Established connection with Analog Linear Accelerometer.
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bool alaSetup() {

}

// Loop


gpsData_t getGpsData();


/* TODO: Implement gpsRead
 * Read data from GPS
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
gpsData_t gpsRead() {

}


/* TODO: Implement bmpRead
 * Read data from BMP
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
bmpData_t bmpRead() {

}


/* TODO: Implement imuRead
 * Read data from IMU and ALA
 *
 * Author: Jeff Kaji
 * Date: 12/23/2020
 */
imuData_t imuRead();

void checkStatus();

//--------------------
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
