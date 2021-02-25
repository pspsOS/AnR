#include <imuInterface.h>
#include <common.h>

ui8 currentBank = 0;


void imuInit(bool* imuNomPtr) {
	nomPtr[IMU] = imuNomPtr;
	imuSetBank(BANK_2);
	imuWrite8(GYRO_CONFIG_1, GYRO_DLPCFG_DEFAULT | GYRO_SCALE_1000 | ENABLE_GYRO_DLPF);
	imuWrite8(ACCEL_CONFIG_1, ACCEL_DLPCFG_DEFAULT | ACCEL_SCALE_16 | ENABLE_ACCEL_DLPF);
}

void imuGetAccel(ui8 data[]){
	ui8 cmd = (0x01 << 7) + ACCEL_XOUT_H;

	imuSetBank(BANK_0);

	if (recieveSPI(&cmd, 1, data, 6, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(IMU);
		return;
	}

}

void imuGetGyro(ui8 data[]){
	ui8 cmd = (0x01 << 7) + GYRO_XOUT_H;

	imuSetBank(BANK_0);

	if (recieveSPI(&cmd, 1, data, 6, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(IMU);
		return;
	}

}

void imuGetMag(ui8 data[]){
	ui8 cmd = (0x01 << 7) + EXT_SLV_SENS_DATA_00;

	imuSetBank(BANK_3);

	imuWrite8(I2C_SLV0_ADDR, I2C_READ | MAG_ID);
	imuWrite8(I2C_SLV0_REG, HXL);
	imuWrite8(I2C_SLV0_CTRL, MAG_I2C_ENABLE);

	imuSetBank(BANK_0);

	if (recieveSPI(&cmd, 1, data, 6, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(IMU);
		return;
	}

}

ui16 imuRead16(ui8 addr){
	// Variables
	ui8 cmd = (0x01 << 7) + addr;
	ui8 data[2] = {0,0};

	if (recieveSPI(&cmd, 1, data, 2, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(IMU);
		return 0x00;
	}

	return (data[0] << 8) + data[1];
}

ui8 imuRead8(ui8 addr){
	// Variables
	ui8 cmd = (0x01 << 7) + addr;
	ui8 data;

	if (recieveSPI(&cmd, 1, &data, 1, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS))
	{
		handleHalError(IMU);
		return 0x00;
	}

	return data;
}

void imuWrite8(ui8 addr, ui8 data){

	ui8 cmd[2] = {addr, data};

	if (sendSPI(&cmd[0], 2, IMU_CS_GPIO_Port, IMU_CS_Pin, SENSORS_SPI_BUS)){
		handleHalError(IMU);
		return;
	}

}

void imuSetBank(ui8 bank){
	if (currentBank != bank){
		imuWrite8(REG_BANK_SEL, bank);
		currentBank = bank;
	}
}



