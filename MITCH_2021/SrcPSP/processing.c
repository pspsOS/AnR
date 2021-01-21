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
#else
#include "imuInterface.h"
#endif

bool bmpDStatus = false;
bool IMUDStatus = false;
bool GPSDStatus = false;
bool Upright_OnPad = false;

float pressure_bmp = 0;
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
float a_trop;

float time_var; // Local time variable
float new_time; // Local time variable

float avg_Z_speed;
float avg_Z_accl;

float delta_Z_speed;
float Z_accl;
float new_Z_accl;

float Vert_vel;
float tm_apgee;

float apgee;

float Z_speed; // y-axis Speed, e-frame
float new_Z_speed; // Local y-axis Speed variable, e-frame

float delta_time;

float Horz_vel; // Horizontal velocity relative to accelerometer, g-frame

float Speed_Norm; // g-frame
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

	return true;
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
		accZ = g_imuData.accel_zout / ACCEL_SENSITIVITY; // Set to imu z acceleration if error with analog sensor
	}

	accX_imu = g_imuData.accel_xout / ACCEL_SENSITIVITY;
	accY_imu = g_imuData.accel_yout / ACCEL_SENSITIVITY;
	gyrX_imu = g_imuData.gyro_xout / GYRO_SENSITIVITY;
	gyrY_imu = g_imuData.gyro_yout / GYRO_SENSITIVITY;
	gyrZ_imu = g_imuData.gyro_zout / GYRO_SENSITIVITY;
	magX_imu = g_imuData.mag_xout / MAG_SENSITIVITY;
	magY_imu = g_imuData.mag_yout / MAG_SENSITIVITY;
	magZ_imu = g_imuData.mag_zout / MAG_SENSITIVITY;

	// Reset Update
	g_imuData.hasUpdate = false;
	// Unlock
	g_imuData.lock = false;

	g_imuData.timeStamp = getTimeStamp();

	return true;
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

	return true;
}


/* TODO: Implement processData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void processData_P() {
	if (g_bmpData.hasUpdate && !g_bmpData.lock) {
		getBMPData_P();
	}

	if (g_imuData.hasUpdate && g_imuData.lock) {
		getIMUData_P();
	}

	if (g_gpsData.hasUpdate && g_bmpData.lock) {
		getGPSData_P();
	}

	//g_launchTime

	if (g_bmpData.timeStamp == 0) {

		Upright_OnPad = pointyEndUp_P();

		temp_alt_bmp = g_bmpData.temperature;
		pres_alt_bmp = g_bmpData.pressure;

		//if (abs(temp_alt_bmp - temp_g) > threshold_temp) {
			// some raise issue
		//}

		//if (abs(pres_alt_bmp - (1.01325 * pow(10,5))) > threshold_pres) {
			// some raise issue
		//}

		Calc_Alt = 0;
		Z_speed = 0; // e-frame
		Z_accl = 0;
		time_var = g_bmpData.timeStamp / pow(10,3);
	}
	else {
		new_temp = g_bmpData.temperature;
		new_pres = g_bmpData.pressure;

		new_time = g_bmpData.timeStamp;

		CalcAltBMP_P();
		CalcFlightDataBMP_P();

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
//	if((accX_imu * accX_imu + accY_imu * accY_imu) == 0) asdfasf = 0.000001;

	return ((accZ*accZ / (accX_imu * accX_imu + accY_imu * accY_imu)) > TAN_THETA_SQUARED) && (accZ < 0); // g-rame
}

void CalcAltBMP_P() {
	a_trop = (temp_trop_p - temp_g)/(trop_p_alt);
	delta_alt = (new_temp - temp_alt_bmp) / a_trop; // e-frame
	Calc_Alt += delta_alt; //e-frame

	temp_alt_bmp = new_temp;
	pres_alt_bmp = new_pres;
}

void CalcFlightDataBMP_P() {
	delta_time = new_time - time_var;

	avg_Z_speed = delta_alt/delta_time; // e-frame

	new_Z_speed = (2 * avg_Z_speed) - Z_speed;

	delta_Z_speed = new_Z_speed - Z_speed;

	avg_Z_accl = delta_Z_speed / delta_time;

	new_Z_accl = (2 * avg_Z_accl) - Z_accl;

	if (new_Z_accl != Z_accl) {
		//change in flight status
	}

	tm_apgee = new_Z_speed / g;

	if (tm_apgee > 0) {
		apgee = (new_Z_speed * tm_apgee) - (0.5 * g * pow(tm_apgee, 2));
	}
	else {
		// free fall
	}

	acclZ_bmp = (new_Z_speed - Z_speed) / delta_time; // e-frame

	Horz_vel = sqrt(pow((accX_imu * delta_time), 2) + pow((accY_imu * delta_time), 2)); // g-frame
	Vert_vel = accZ * delta_time; // g-frame

	Speed_Norm = sqrt(pow(Horz_vel, 2) + pow(Vert_vel, 2)); // g-frame
	Vert_Orientation = atan(Horz_vel/Vert_vel);

	//if (abs(acclZ_bmp - accZ) < threshold_check) {
//
	//}


	Z_speed = new_Z_speed; //e-frame
	Z_accl = new_Z_accl; //e-frame
}
