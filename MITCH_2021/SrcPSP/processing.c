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
float R = 8.3144598; // R constant (J/(mol K)) of Earth's atmosphere (air)
float M = 0.0289644; // molar mass of Earth's ait (kg/mol)
float L_b;
float press_rto;
float power_val;
float temp_alt_bmp; // Temp in K at sea level
float pres_alt_bmp; // Pressure in Pa at sea level
float new_temp;
float new_pres;

float V_i[3] = {0, 0, 1};
float X_axis[] = {1, 0, 0};
float Y_axis[] = {0, 1, 0};
float Z_axis[] = {0, 0, 1};
float Calb_arr[];

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

		Calb_arr_mag[0] = magX_imu-V_i[0];
		Calb_arr_mag[1] = magY_imu-V_i[1];
		Calb_arr_mag[2] = magZ_imu-V_i[2];

		V = V_i;

		gyX_0 = gyrX_imu;
		gyY_0 = gyrY_imu;
		gyZ_0 = gyrZ_imu;

		time_var = g_bmpData.timeStamp / pow(10,3);
	}
	else {
		new_temp = g_bmpData.temperature;
		new_pres = g_bmpData.pressure;

		new_time = g_bmpData.timeStamp;

		gyX = gyrX_imu - gyX_0;
		gyY = gyrY_imu - gyY_0;
		gyZ = gyrZ_imu - gyZ_0;

		V_mag[0] = -(magX_imu-Calb_arr_mag[0]);
		V_mag[1] = -(magY_imu-Calb_arr_mag[1]);
		V_mag[2] = -(magZ_imu-Calb_arr_mag[2]);

		mag_Xw = Cross_prod_mag(V_mag, X_axis);
		mag_Yw = Cross_prod_mag(V_mag, Y_axis);
		mag_Zw = Cross_prod_mag(V_mag, Z_axis);

		CalcAltBMP_P();

		Ang_xz = atan2(V[0], V[2]);
		Ang_yz = atan2(V[1], V[2]);

		CalcFlightDataBMP_P();

		Ang_xz = Ang_xz_new;
		Ang_yz = Ang_yz_new;

		gyX_0 = gyX;
		gyY_0 = gyY;
		gyZ_0 = gyZ;

		V[0] = V_new[0];
		V[1] = V_new[1];
		V[2] = V_new[2];

		time_var = new_time;
	}

	v_s_alt = sqrt(1.4*R*temp_alt_bmp); // Speed of Sound in m/s at the altitude




}


/* TODO: Implement transmitData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

float Cross_prod_mag(v, w) {
	s_1 = pow(v[2] * w[3] - v[3] * w[2], 2);
	s_2 = pow(v[3] * w[1] - v[1] * w[3], 2);
	s_3 = pow(v[1] * w[2] - v[2] * w[1], 2);

	return sqrt(s_1 + s_2 + s_3);
}


float Cross_prod_fun(v, w) {
	s[0] = v[2] * w[3] - v[3] * w[2];
	s[1] = v[3] * w[1] - v[1] * w[3];
	s[2] = v[1] * w[2] - v[2] * w[1];

	return s;
}

float dot_prod_fun(v, w) {
	return v[0]*w[0] + v[1]*w[1] + v[2]*w[2]
}

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
	delta_time = new_time - time_var;
	L_b = - (new_temp - temp_alt_bmp) / delta_time;

	press_rto = new_pres/pres_alt_bmp;

	// in e-frame
	if (L_b == 0) {
		power_val = - R*L_b / (g*M);

		delta_alt = (pow(press_rto, power_val) - 1) * temp_alt_bmp / L_b;
	}
	else {
		delta_alt = - log(press_rto) * R * temp_alt_bmp / (g * M);
	}

	Calc_Alt += delta_alt; //e-frame

	temp_alt_bmp = new_temp;
	pres_alt_bmp = new_pres;

	// After the addition of the kalman filter, a velocity will be given

	tm_apgee = new_Z_speed / g;

	if (tm_apgee > 0) {
		apgee = (new_Z_speed * tm_apgee) - (0.5 * g * pow(tm_apgee, 2));
	}
	else {
		// free fall
	}
}

void CalcFlightDataBMP_P() {
	delta_time = new_time - time_var;

	Ang_xz_new = Ang_xz + ((gyY + gyY_0)/2) * delta_time;

	Ang_yz_new = Ang_yz + ((gyX + gyX_0)/2) * delta_time;

	Vx_gy = sin(Ang_xz_new) / sqrt(1 + pow(cos(Ang_xz_new), 2) * pow(tan(Ang_yz_new), 2));

	Vy_gy = sin(Ang_yz_new) / sqrt(1 + pow(cos(Ang_yz_new), 2) * pow(tan(Ang_xz_new), 2));

	Vz_gy = sqrt(1 - pow(Vx_gy,2) - pow(Vy_gy,2)) * V_new[2]/abs(V_new[2]);

	if (mag_Xw == 0) {
		V_new[0] = Vx_gy; // Orientation x-comp, e-frame
		Calb_arr_mag[0] = magX_imu-V_new[0];
	}
	else {
		wX_gy = Const/mag_Xw;
		V_new[0] = (V_mag[0] + Vx_gy * wX_gy) / (1+wX_gy); // Orientation x-comp
	}

	if (mag_Yw == 0) {
		V_new[1] = Vy_gy; // Orientation y-comp
		Calb_arr_mag[1] = magX_imu-V_new[1];
	}
	else {
		wY_gy = Const/mag_Yw;
		V_new[1] = (V_mag[1] + Vy_gy * wY_gy) / (1+wY_gy); // Orientation y-comp
	}

	if (mag_Zw == 0) {
		V_new[2] = Vz_gy; // Orientation z-comp
		Calb_arr_mag[2] = magX_imu-V_new[2];
	}
	else {
		wZ_gy = Const/mag_Zw;
		V_new[2] = (V_mag[2] + Vz_gy * wZ_gy) / (1+wZ_gy); // Orientation z-comp
	}

	V1 = V/mag(V);
	V1_new = V_new/mag(V_new);

	v_prod = Cross_prod_fun(V1, V1_new);

	v_matrix = [0, -v_prod[2], v_prod[1]; v_prod[2], 0, -v_prod[0]; -v_prod[1], v_prod[0], 0]; // skew-symmetric cross-product

	// works for all but 180 deg rotations

	Rot_matrix = [1, 0, 0; 0, 1, 0; 0, 0, 1] + v_matrix + pow(v_matrix, 2) * (1-dot_prod_fun(V1, V1_new))/mag(v_prod)^2;

	X_axis = Rot_matrix * X_axis; // e-frame
	Y_axis = Rot_matrix * Y_axis;
	Z_axis = Rot_matrix * Z_axis;
}
