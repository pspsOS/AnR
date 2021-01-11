/*
 * imu.c
 *
 * Author: Dawson Moore (12/5/2019)
 */
 
#include "imu.h"

/*
 * Function: imuTask()
 *
 * Gathers the IMU data for the rocket
 *
 */
void imuTask()
{
    imu_t imu;
    imuInit(&imu);
    uint8_t recVal[2];
    uint8_t cmd[2];
    TickType_t time_init = 0;
    while (PSP == GREAT)
    {
        cmd[0] = RESET | READ | ACCEL_XOUT_H;
        cmd[1] = RESET;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | ACCEL_XOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.accelX = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | ACCEL_YOUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | ACCEL_YOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.accelY = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | ACCEL_ZOUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | ACCEL_ZOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.accelZ = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | GYRO_XOUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | GYRO_XOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.gyroX = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | GYRO_YOUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | GYRO_YOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.gyroY = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | GYRO_ZOUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | GYRO_ZOUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.gyroZ = (recVal[0] << 8) | recVal[1];
        
        cmd[0] = RESET | READ | TEMP_OUT_H;
        recieveSPI(cmd, 2, recVal, 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        cmd[0] = RESET | READ | TEMP_OUT_L;
        recieveSPI(cmd, 2, recVal[1], 1, IMU_CS_GPIO_Port, IMU_CS_Pin);
        imu.temp = (recVal[0] << 8) | recVal[1];
        
        vTaskDelayUntil(&time_init, IMU_RATE);
    }
}

/*
 * Function: imuInit(imu_t * imu)
 *
 * Initializes the IMU
 \
 *
 */
int imuInit(imu_t * imu)
{
    /*
        Notes:
        SPI messages are sent MSB first
    */
    /* Things to do:
        Set Gyro sensitivity: GYRO_FS_SEL
        Self-test Response = Sensor Output When Enabled - Sensor Output Without Self Test Enabled
        This self test output must be between the min and max limits
        The FSYNC pin can wake the device from sleep
        Set the sensor to 9-axis mode
        Might want to use the wake on motion interrupt
        
    */
    if (changeBank(0))
    {
        handleHalError();
    }
    uint8_t cmd[2];
    
    //Turn DMP and I2C off 
    cmd[0] = RESET | WRITE_BYTE | USER_CTRL;
    cmd[1] = USER_CTRL_DMP_ON; //Change to DMP_OFF to turn off DMP functionality
    if (sendSpi(cmd, 2))
    {
        handleHalError();
    }
    
    //Turn off duty cycle mode
    cmd[0] = RESET | WRITE_BYTE | LP_CONFIG;
    cmd[1] = LP_DUTY_OFF;
        if (sendSpi(cmd, 2))
    {
        handleHalError();
    }
}

/*
 * Function: enterLowPower(imu_t * imu)
 *
 * Sets the device to low power mode
 *
 */
int enterLowPower(imu_t * imu)
{

}

/*
 * Function: changeBank(int bank, imu_t * imu)
 *
 * Changes the current register bank being used
 *
 */
int changeBank(uint8_t bankVal)
{
    uint8_t cmd[2] = {0};
    cmd[0] = NULL | WRITE_BYTE | REG_BANK_SEL;
    cmd[1] = bankVal << 4;
    
    return sendSpi(cmd, 2, IMU_CS_GPIO_Port, IMU_CS_Pin);
}
