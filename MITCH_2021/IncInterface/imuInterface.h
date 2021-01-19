/*
 * imu.h
 *
 *  Created on: Jan 17, 2021
 *      Author: Jeff
 */

#ifndef IMUINTERFACE_H_
#define IMUINTERFACE_H_

//Includes
#include "genericInterface.h"

#define ACCEL_FS0 (2)  // g
#define ACCEL_FS1 (4)  // g
#define ACCEL_FS2 (8)  // g
#define ACCEL_FS3 (16) // g
#define GYRO_FS0 (250)  // dps
#define GYRO_FS1 (500)  // dps
#define GYRO_FS2 (1000) // dps
#define GYRO_FS3 (2000) // dps
#define MAG_FS (4900) // uT

#define ACCEL_SENSITIVITY ACCEL_FS3
#define GYRO_SENSITIVITY GYRO_FS3
#define MAG_SENSITIVITY MAG_FS

#endif /* IMUINTERFACE_H_ */
