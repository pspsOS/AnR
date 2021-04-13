/*
 * processing.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_PROCESSING_H_
#define INC_PROCESSING_H_

#include "../IncPSP/common.h"

/* Function prototypes */

bool getBMPData_P();
bool getIMUData_P();
bool getGPSData_P();
void processData_P();
void transmitData_P();

bool pointyEndUp_P();
void CalcAltBMP_P(float new_pres, float new_time);
void CalcFlightDataBMP_P();
void matrix_mult_fun_3x3(float first[][3],
		float second[][3],
		float result_mat_mult[][3]);
void matrix_mult_fun_3x3_3x1(float first[][3],
		float second[3],
		float result_mat_mult[3]);
void matrix_pow_fun(float m[][3], //Matrix n x n to be raised to power p
					float result_mat_pow[][3],
					int n, int p);
void matrix_inverse_fun(float mat[][3], float result_mat_inv[][3]);
void matrix_add_sub_fun(float first[][3],
                      float second[][3],
					  float result_mat_add_sub[][3],
					  int mode);
float Cross_prod_mag(float v[3], float w[3]);
void Cross_prod_fun(float v[3], float w[3], float *s);
float dot_prod_fun(float v[3], float w[3]);

#endif /* INC_PROCESSING_H_ */
