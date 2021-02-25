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

//Addresses
//Bank 0
#define ACCEL_XOUT_H 0x2D
#define ACCEL_YOUT_H 0x2F
#define ACCEL_ZOUT_H 0x31
#define GYRO_XOUT_H 0x33
#define GYRO_YOUT_H 0x35
#define GYRO_ZOUT_H 0x37
#define TEMP_OUT_H 0x39
#define REG_BANK_SEL 0x7F
#define EXT_SLV_SENS_DATA_00 0x3B

//Bank 2
#define GYRO_CONFIG_1 0x01
#define ACCEL_CONFIG_1 0x14

//Bank 3
#define I2C_SLV0_ADDR 0x03
#define I2C_SLV0_REG 0x04
#define I2C_SLV0_CTRL 0x05
#define I2C_SLV0_DO 0x06

//accel and gyro reg values
#define BANK_0 0x00
#define BANK_1 0x08
#define BANK_2 0x10
#define BANK_3 0x18
#define ENABLE_GYRO_DLPF 0x01
#define GYRO_SCALE_250 0x00
#define GYRO_SCALE_500 0x02
#define GYRO_SCALE_1000 0x04
#define GYRO_SCALE_2000 0x06
#define GYRO_DLPCFG_DEFAULT 0x00
#define ENABLE_ACCEL_DLPF 0x01
#define ACCEL_SCALE_2 0x00
#define ACCEL_SCALE_4 0x02
#define ACCEL_SCALE_8 0x04
#define ACCEL_SCALE_16 0x06
#define ACCEL_DLPCFG_DEFAULT 0x00

//mag reg addr
#define ST1 0x10
#define ST2 0x18
#define CNTL2 0x31
#define CNTL3 0x31

#define HXL 0x11
#define HXH 0x12
#define HYL 0x13
#define HYH 0x14
#define HZL 0x15
#define HZH 0x16

//mag reg values
#define MAG_ID 0x09
#define I2C_READ 0x80
#define I2C_WRITE 0x00
#define MAG_I2C_ENABLE 0xC6




//Prototypes
void imuInit(bool* imuNomPtr);
ui16 imuRead16(ui8 addr);
ui8 imuRead8(ui8 addr);
void imuWrite8(ui8 addr, ui8 data);
void imuSetBank(ui8 bank);
void imuGetAccel(ui8 data[]);
void imuGetGyro(ui8 data[]);
void imuGetMag(ui8 data[]);


#endif /* IMUINTERFACE_H_ */
