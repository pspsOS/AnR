/*
 * processing.c
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#include "../IncPSP/processing.h"
#include <math.h>

#ifndef NDEBUG
#include <debugSettings.h>
#include <unistd.h>
#endif

bool bmpDStatus = false;
bool IMUDStatus = false;
bool GPSDStatus = false;
bool Upright_OnPad = false;

float pressure_bmp;
float temperature_bmp;
float alt_gps;
float speed_gps;
float accX_imu;
float accY_imu;
float gyrX_imu;
float gyrY_imu;
float gyrZ_imu;
float magX_imu;
float magY_imu;
float magZ_imu;
float accZ;

float delta_alt;
float Calc_Alt;
float g = 9.81; // Acceleration of gravity in m/s^2
float R = 287; // R constant of Earth's atmosphere (air)
float temp_alt_bmp; // Temp in K at sea level
float pres_alt_bmp; // Pressure in Pa at sea level
float new_temp;
float new_pres;
float dens_alt = 1.225; // Density in kg/m^3 at sea level
float v_s_alt; // Speed of Sound in m/s at the altitude

float temp_g = 288.16; // Ground temperature
float temp_trop_p = 216.66; // Temperature in the tropopause
float trop_p_alt = 11000; // in m

float a_trop = (temp_trop_p - temp_g)/(trop_p_alt - 0);

float time_var; // Local time variable
float new_time; // Local time variable

float vert_speed; // Vertical Speed
float new_vert_speed; // Local Vertical Speed variable

float delta_time;

float Horz_vel_SQRD;

float Speed_Norm;
float Vert_Orientation;

float acclZ_bmp;


/* TODO: Implement getBMPData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getBMPData_P() {

	// Lock
	g_bmpData.lock = true;

	// Update local variables

	pressure_bmp = g_bmpData.pressure;
	temperature_bmp = g_bmpData.temperature;


	// Reset Update
	g_bmpData.hasUpdate = false;
	// Unlock
	g_bmpData.lock = false;

	g_bmpData.timeStamp = getTimeStamp();

}


/* TODO: Implement getIMUData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getIMUData_P() {

	// Lock
	g_imuData.lock = true;

	// Update local variables

	accZ = g_imuData.alaZ;

	if (accZ == 0) {
		accZ = g_imuData.accZ;
	}

	accX_imu = g_imuData.accX;
	accY_imu = g_imuData.accY;
	gyrX_imu = g_imuData.gyrX;
	gyrY_imu = g_imuData.gyrY;
	gyrZ_imu = g_imuData.gyrZ;
	magX_imu = g_imuData.magX;
	magY_imu = g_imuData.magY;
	magZ_imu = g_imuData.magZ;

	// Reset Update
	g_imuData.hasUpdate = false;
	// Unlock
	g_imuData.lock = false;

	g_imuData.timeStamp = getTimeStamp();

}


/* TODO: Implement getGPSData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

bool getGPSData_P() {

	// Lock
	g_gpsData.lock = true;

	// Update local variables

	alt_gps = g_gpsData.alt;
	speed_gps = g_gpsData.speed;

	// Reset Update
	g_gpsData.hasUpdate = false;
	// Unlock
	g_gpsData.lock = false;

	g_gpsData.timeStamp = getTimeStamp();

}


/* TODO: Implement processData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void processData_P() {
	if (g_bmpData.hasUpdate && g_bmpData.lock) {
		getBMPData_P();
	}

	if (g_imuData.hasUpdate && g_imuData.lock) {
		getIMUData_P();
	}

	if (g_gpsData.hasUpdate && g_bmpData.lock) {
		getGPSData_P();
	}

	if (g_bmpData.timeStamp == 0) {

		Upright_OnPad = pointyEndUp_P();

		temp_alt_bmp = g_bmpData.temperature;
		pres_alt_bmp = g_bmpData.pressure;

		if (abs(temp_alt_bmp - temp_g) > threshold_temp) {
			// some raise issue
		}

		if (abs(pres_alt_bmp - (1.01325 * pow(10,5))) > threshold_pres) {
			// some raise issue
		}

		Calc_Alt = 0;
		vert_speed = 0;
		time_var = g_bmpData.timeStamp;
	}
	else {
		new_temp = g_bmpData.temperature;
		new_pres = g_bmpData.pressure;

		new_time = g_bmpData.timeStamp;

		CalcAltBMP_P();
		CalcVelBMP_P();

		time_var = new_time;
	}

	v_s_alt = sqrt(1.4*R*temp_alt_bmp); // Speed of Sound in m/s at the altitude




}


/* TODO: Implement transmitData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void transmitData_P() {

}

/**
 * @brief Evaluate Pointy-End-Up
 * Determines whether the magnitude of the acceleration vector is pointing
 * downwards within the tolerance defined in TAN_THETA_SQUARED
 *
 * @param None
 * @return bool true or false
 *
 * @author Jeff Kaji, Bahaa Elshimy
 * @date 12/29/2020
 */
bool pointyEndUp_P() {
	assert((accX_imu * accX_imu + accY_imu * accY_imu) != 0);

	return ((accZ*accZ / (accX_imu * accX_imu + accY_imu * accY_imu)) > TAN_THETA_SQUARED) && (accZ < 0);
}

void CalcAltBMP_P() {
	delta_alt = (new_temp - temp_alt_bmp) / a_trop;
	Calc_Alt += delta_alt;

	temp_alt_bmp = new_temp;
	pres_alt_bmp = new_pres;
}

void CalcFlightDataBMP_P() {
	delta_time = new_time - time_var;

	Horz_vel_SQRD = pow((accX_imu * delta_time), 2) + pow((accY_imu * delta_time), 2);

	new_vert_speed = delta_alt/delta_time;

	Speed_Norm = sqrt(Horz_vel_SQRD + pow(new_vert_speed, 2));
	Vert_Orientation = arctan(Horz_vel_SQRD/new_vert_speed);



	acclZ_bmp = (new_vert_speed - vert_speed) / delta_time;

	if (abs(acclZ_bmp - accZ) < threshold_check) {

	}


	vert_speed = new_vert_speed;
}
