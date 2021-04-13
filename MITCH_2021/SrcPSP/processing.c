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
float new_Alt = 0;
float Calc_Alt = 0;
float g = 9.80665; // Acceleration of gravity in m/s^2
float R = 8.3144598; // R constant (J/(mol K)) of Earth's atmosphere (air)
float M = 0.0289644; // molar mass of Earth's ait (kg/mol)
float L_b = -0.0065; // K/m
float T_b = 288.15; // K
float P_b = 101325; // Pa
float T_b2 = 216.65; // K
float P_b2 = 22632.1; // Pa
float temp_alt_bmp; // Temp in K at sea level
float pres_alt_bmp; // Pressure in Pa at sea level
float new_temp;
float new_pres;
float v_s_alt;

float V_i[3] = {0, 0, 1};
float X_axis[3] = {1, 0, 0};
float Y_axis[3] = {0, 1, 0};
float Z_axis[3] = {0, 0, 1};
float v_matrix[3][3]; // skew-symmetric cross-product
float Rot_matrix[3][3];

float gyX;
float gyY;
float gyZ;

float Vx_gy;
float Vy_gy;
float Vz_gy;
float V_new[3];

float Calb_arr_mag[3];
float mag_Xw;
float wX_gy;
float mag_Yw;
float wY_gy;
float mag_Zw;
float wZ_gy;

float w_Const = 1;

float V_unit[3];
float V_unit_new[3];

float V_mag[3];
float V[3];

float Ang_xz;
float Ang_yz;

float Ang_xz_new;
float Ang_yz_new;

float gyX_0;
float gyY_0;
float gyZ_0;

float v_matrix_sqrd[3][3];
float mult_skew_mat[3][3];
float skew_const;
float first_add_Rot_matrix[3][3];

float result_mat_mult_Rot_Matrix[3];

float vel_magnitude;

float time_var = 0; // Local time variable
float new_time = 0; // Local time variable

float vel_g[3];

float delta_time = 0;

float v_prod[3];

float vel_e[3];
float calc_x_pos;
float calc_y_pos;
float tm_apgee;
float apgee;

int r1=3;
int c1=3;
int r2=3;
int c2=1;
float result_mat_mult[3][3];

float I_n[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
float t[3][3];
float result_mat_pow[3][3]; // Initialized to result_mat_pow = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}

float determinant = 0;
float result_mat_inv[3][3];

float result_mat_add_sub[3][3];

int i;
int j;

float s_1;
float s_2;
float s_3;
float s[3];

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

// function to multiply two matrices
//matrix_mult_fun(first, second, result, r1, c1, r2, c2);
void matrix_mult_fun_3x3(float first[][3],
		float second[][3],
		float result_mat_mult[][3]) {

   // Initializing elements of matrix mult to 0.
   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
    	  result_mat_mult[i][j] = 0;
      }
   }

   // Multiplying first and second matrices and storing it in result
   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         for (int k = 0; k < 3; ++k) {
        	 result_mat_mult[i][j] += first[i][k] * second[k][j];
         }
      }
   }
}

void matrix_mult_fun_3x3_3x1(float first[][3],
		float second[3],
		float result_mat_mult[3]) {

   // Initializing elements of matrix mult to 0.
   for (int i = 0; i < 3; ++i) {
	   result_mat_mult[i] = 0;
   }

   // Multiplying first and second matrices and storing it in result
   for (int i = 0; i < 3; ++i) {
	   for (int j = 0; j < 3; ++j) {
		   result_mat_mult[i] += first[i][j] * second[j];
	   }
   }
}

// function to display the matrix
//void display(float result[][3], int row, int column) {
//
//   printf("\nOutput Matrix:\n");
//   for (int i = 0; i < row; ++i) {
//      for (int j = 0; j < column; ++j) {
//         printf("%d  ", result[i][j]);
//         if (j == column - 1)
//            printf("\n");
//      }
//   }
//}



//matrix_pow_fun(first, result, n, p); result[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, result must be initialized to this
void matrix_pow_fun(float m[][3], //Matrix n x n to be raised to power p
					float result_mat_pow[][3],
					int n, int p) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == j) {
				result_mat_pow[i][j] = 1;
			}
			else {
				result_mat_pow[i][j] = 0;
			}
		}
	}

	int t[n][n]; // Empty 3x3 matrix
    for (int i = 0; i < p; i++) {
    	for (int b = 0; b < n; b++) {
    		for (int d = 0; d < n; d++) {
    			int sum = 0;
                for (int k = 0; k < n; k++) {
                	sum += result_mat_pow[b][k] * m[k][d];
                }
                t[b][d] = sum;
    		}
    	}
    	for (int b = 0; b < n; b++) {
    		for (int d = 0; d < n; d++) {
    			result_mat_pow[b][d] = t[b][d];
    		}
    	}
    }
}

void matrix_inverse_fun(float mat[][3], float result_mat_inv[][3]) {
	//finding determinant
	determinant = 0;
	for(i = 0; i < 3; i++)
		determinant = determinant + (mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]));

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			result_mat_inv[i][j] = ((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))/ determinant;
		}
	}
//	printf("\nInverse of matrix is: \n");
//	for(i = 0; i < 3; i++){
//		for(j = 0; j < 3; j++){
//			printf("%f\t", result[i][j]);
//		}
//		printf("\n");
//	}
}

void matrix_add_sub_fun(float first[][3],
                      float second[][3],
					  float result_mat_add_sub[][3],
					  int mode) { // mode = 1: add, mode = -1: subtract second from first
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			if (mode == 1) {
				result_mat_add_sub[i][j] = first[i][j] + second[i][j];
			}
			else {
				result_mat_add_sub[i][j] = first[i][j] - second[i][j];
			}
		}
	}
}

float Cross_prod_mag(float v[3], float w[3]) {
	s_1 = pow(v[1] * w[2] - v[2] * w[1], 2);
	s_2 = pow(v[2] * w[0] - v[0] * w[2], 2);
	s_3 = pow(v[0] * w[1] - v[1] * w[0], 2);

	return sqrt(s_1 + s_2 + s_3);
}


void Cross_prod_fun(float v[3], float w[3], float s[3]) {
	s[0] = v[1] * w[2] - v[2] * w[1];
	s[1] = v[2] * w[0] - v[0] * w[2];
	s[2] = v[0] * w[1] - v[1] * w[0];
}

float dot_prod_fun(float v[3], float w[3]) {
	return v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
}


/* TODO: Implement processData
 *
 * Author: Vishnu Vijay, Bahaa Elshimy
 * Date: 12/29/20
 */

void processData_P() {

	print("Works");


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

		vel_e[0] = 0;
		vel_e[1] = 0;
		vel_e[2] = 0;

		Calb_arr_mag[0] = magX_imu-V_i[0];
		Calb_arr_mag[1] = magY_imu-V_i[1];
		Calb_arr_mag[2] = magZ_imu-V_i[2];

		V[0] = V_i[0];
		V[1] = V_i[1];
		V[2] = V_i[2];

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

		CalcAltBMP_P(new_pres, new_time);

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

void CalcAltBMP_P(float new_pres, float new_time) {
	delta_time = new_time - time_var;

	// in e-frame
	if (Calc_Alt < 11000) {
		new_Alt = ((pow((new_pres/P_b), -(R*L_b/(g*M))) * T_b) - T_b)/L_b;
	}
	else {
		new_Alt = (-log(new_pres/P_b2) * R * T_b2/(g*M)) + 11000;
	}

	delta_alt = new_Alt - Calc_Alt;

	temp_alt_bmp = new_temp;
	pres_alt_bmp = new_pres;

	//printf("%f \n", new_temp_alt_bmp);

	// Temp Mock vel:
	// vel_g is the velocity calculated from the barometric data and corresponds only to the vertical component of the velocity in e
	vel_g[0] = 0;
	vel_g[1] = 0;
	vel_g[2] = delta_alt/delta_time;

	tm_apgee = (vel_g[2] / g);

	if (tm_apgee > 0) {
		apgee = Calc_Alt + pow(vel_g[2], 2) / (2*g);
	}

	//printf(", %f, %f, %f\n", new_Alt, vel_g[2], tm_apgee);

	Calc_Alt = new_Alt;
	time_var = new_time;

	// After the addition of the kalman filter, a velocity will be given
}

void CalcFlightDataBMP_P() {
	delta_time = new_time - time_var;

	Ang_xz_new = Ang_xz + ((gyY + gyY_0)/2) * delta_time;

	Ang_yz_new = Ang_yz + ((gyX + gyX_0)/2) * delta_time;

	Vx_gy = sin(Ang_xz_new) / sqrt(1 + pow(cos(Ang_xz_new), 2) * pow(tan(Ang_yz_new), 2));

	Vy_gy = sin(Ang_yz_new) / sqrt(1 + pow(cos(Ang_yz_new), 2) * pow(tan(Ang_xz_new), 2));

	Vz_gy = sqrt(1 - pow(Vx_gy,2) - pow(Vy_gy,2)) * V_new[2]/abs(V_new[2]); // V_new[2]/abs(V_new[2]) = 1 or -1

	if (mag_Xw == 0) {
		V_new[0] = Vx_gy; // Orientation x-comp, e-frame
		Calb_arr_mag[0] = magX_imu-V_new[0];
	}
	else {
		wX_gy = w_Const/mag_Xw;
		V_new[0] = (V_mag[0] + Vx_gy * wX_gy) / (1+wX_gy); // Orientation x-comp
	}

	if (mag_Yw == 0) {
		V_new[1] = Vy_gy; // Orientation y-comp
		Calb_arr_mag[1] = magX_imu-V_new[1];
	}
	else {
		wY_gy = w_Const/mag_Yw;
		V_new[1] = (V_mag[1] + Vy_gy * wY_gy) / (1+wY_gy); // Orientation y-comp
	}

	if (mag_Zw == 0) {
		V_new[2] = Vz_gy; // Orientation z-comp
		Calb_arr_mag[2] = magX_imu-V_new[2];
	}
	else {
		wZ_gy = w_Const/mag_Zw;
		V_new[2] = (V_mag[2] + Vz_gy * wZ_gy) / (1+wZ_gy); // Orientation z-comp
	}

	for(i = 0; i < 3; i++){
		V_unit[i] = V[i]/sqrt(pow(V[0], 2) + pow(V[1], 2) + pow(V[2], 2));
		V_unit_new[i] = V_new[i]/sqrt(pow(V_new[0], 2) + pow(V_new[1], 2) + pow(V_new[2], 2));
	}

	for(i = 0; i < 3; i++){
		s[i] = v_prod[i];
	}
	Cross_prod_fun(V_unit, V_unit_new, &s[0]);

	// skew-symmetric cross-product

	v_matrix[0][0] = 0;
	v_matrix[0][1]= -v_prod[2];
	v_matrix[0][2] = v_prod[1];
	v_matrix[1][0] = v_prod[2];
	v_matrix[1][1] = 0;
	v_matrix[1][2] = -v_prod[0];
	v_matrix[2][0] = -v_prod[1];
	v_matrix[2][1] = v_prod[0];
	v_matrix[2][2] = 0;

	// works for all but 180 deg rotations

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			result_mat_pow[i][j] = v_matrix_sqrd[i][j];
		}
	}
	matrix_pow_fun(v_matrix, result_mat_pow, 3, 2);

	skew_const = (1-dot_prod_fun(V_unit, V_unit_new))/pow(sqrt(pow(v_prod[0], 2) + pow(v_prod[1], 2) + pow(v_prod[2], 2)), 2);

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			result_mat_add_sub[i][j] = first_add_Rot_matrix[i][j];
		}
	}
	matrix_add_sub_fun(I_n, v_matrix, result_mat_add_sub, 1);

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			result_mat_add_sub[i][j] = Rot_matrix[i][j];
		}
	}

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			mult_skew_mat[i][j] = v_matrix_sqrd[i][j] * skew_const;
		}
	}
	matrix_add_sub_fun(first_add_Rot_matrix, mult_skew_mat, result_mat_add_sub, 1);

	Rot_matrix[0][0] = 1 + v_matrix[0][0] + v_matrix_sqrd[0][0] * skew_const;
	Rot_matrix[0][1] = v_matrix[0][1] + v_matrix_sqrd[0][1] * skew_const;
	Rot_matrix[0][2] = v_matrix[0][2] + v_matrix_sqrd[0][2] * skew_const;
	Rot_matrix[1][0] = v_matrix[1][0] + v_matrix_sqrd[1][0] * skew_const;
	Rot_matrix[1][1] = 1 + v_matrix[1][1] + v_matrix_sqrd[1][1] * skew_const;
	Rot_matrix[1][2] = v_matrix[1][2] + v_matrix_sqrd[1][2] * skew_const;
	Rot_matrix[2][0] = v_matrix[2][0] + v_matrix_sqrd[2][0] * skew_const;
	Rot_matrix[2][1] = v_matrix[2][1] + v_matrix_sqrd[2][1] * skew_const;
	Rot_matrix[2][2] = 1 + v_matrix[2][2] + v_matrix_sqrd[2][2] * skew_const;

	for(i = 0; i < 3; i++){
		result_mat_mult_Rot_Matrix[i] = X_axis[i]; // e-frame
	}
	matrix_mult_fun_3x3_3x1(Rot_matrix, X_axis, result_mat_mult_Rot_Matrix);

	for(i = 0; i < 3; i++){
		result_mat_mult_Rot_Matrix[i] = Y_axis[i]; // e-frame
	}
	matrix_mult_fun_3x3_3x1(Rot_matrix, Y_axis, result_mat_mult_Rot_Matrix);

	for(i = 0; i < 3; i++){
		result_mat_mult_Rot_Matrix[i] = Z_axis[i]; // e-frame
	}
	matrix_mult_fun_3x3_3x1(Rot_matrix, Z_axis, result_mat_mult_Rot_Matrix);

	vel_magnitude = vel_g[2] / V_unit_new[2];

	vel_e[0] = vel_magnitude * V_unit_new[0];
	vel_e[1] = vel_magnitude * V_unit_new[1];
	vel_e[2] = vel_g[2];

	calc_x_pos += vel_e[0] * delta_time;
	calc_y_pos += vel_e[1] * delta_time;
}
