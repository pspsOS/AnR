#ifndef IMU_H
#define IMU_H

//Variable Build Defines

//Generic Defines
#define RESET                   0x00
#define IMU_STACK_SIZE          256
#define IMU_PRIORITY            1
#define IMU_RATE                5 / portTICK_RATE_MS //Run at a rate of 200 Hz. The 5 is indicative of a period of 5 ms.

//R/W Config
#define READ_BYTE               0x80
#define WRITE_BYTE              0x00

//Address List
//Bank Select
#define REG_BANK_SEL            0x7F

//Bank 0
#define USER_CTRL               0x03
#define LP_CONFIG               0x05
#define PWR_MGMT_1              0x06
#define PWR_MGMT_2              0x07
#define INT_PIN_CFG             0x0F
#define INT_ENABLE              0x10
#define INT_ENABLE_1            0x11
#define INT_ENABLE_2            0x12
#define INT_ENABLE_3            0x13
//#define I2C_MST_STATUS    0x17 //Not used (SPI used instead)
#define INT_STATUS              0x19
#define INT_STATUS_1            0x1A
#define INT_STATUS_2            0x1B
#define INT_STATUS_3            0x1C
#define DELAY_TIMEH             0x28
#define DELAY_TIMEL             0x29
#define ACCEL_XOUT_H            0x2D
#define ACCEL_XOUT_L            0x2E
#define ACCEL_YOUT_H            0x2F
#define ACCEL_YOUT_L            0x30
#define ACCEL_ZOUT_H            0x32
#define ACCEL_ZOUT_L            0x32
#define GYRO_XOUT_H             0x33
#define GYRO_XOUT_L             0x34
#define GYRO_YOUT_H             0x35
#define GYRO_YOUT_L             0x36
#define GYRO_ZOUT_H             0x37
#define GYRO_ZOUT_L             0x38
#define TEMP_OUT_H              0x39
#define TEMP_OUT_L              0x3A
//EXT_SLV_SENS_DATA00 - 23 is only required for external I2C sensor functionality
#define FIFO_EN_1               0x66
#define FIFO_EN_2               0x67
#define FIFO_RST                0x68
#define FIFO_MODE               0x69
#define FIFO_COUNTH             0x70
#define FIFO_COUNTL             0x71
#define FIFO_R_W                0x72
#define DATA_DRY_STATUS         0x74
#define FIFO_CFG                0x76

//Bank 1
#define SELF_TEST_X_GYRO        0x02
#define SELF_TEST_Y_GYRO        0x03
#define SELF_TEST_Z_GYRO        0x04
#define SELF_TEST_X_ACCEL       0x0E
#define SELF_TEST_Y_ACCEL       0x0F
#define SELF_TEST_Z_ACCEL       0x10
#define XA_OFFS_H               0x14
#define XA_OFFS_L               0x15
#define YA_OFFS_H               0x17
#define YA_OFFS_L               0x18
#define ZA_OFFS_H               0x1A
#define ZA_OFFS_L               0x1B
#define TIMEBASE_CORRECTION_PLL 0x28

//Bank 2
#define GYRO_SMPLRT_DIV         0x00
#define GYRO_CONFIG_1           0x01
#define GYRO_CONFIG_2           0x02
#define XG_OFFS_USRH            0x03
#define XG_OFFS_USRL            0x04
#define YG_OFFS_USRH            0x05
#define YG_OFFS_USRL            0x06
#define ZG_OFFS_USRH            0x07
#define ZG_OFFS_USRL            0x08
#define ODR_ALIGN_EN            0x09
#define ACCEL_SMPLRT_DIV_1      0x10
#define ACCEL_SMPLRT_DIV_2      0x11
#define ACCEL_INTEL_CTRL        0x12
#define ACCEL_WOM_THR           0x13
#define ACCEL_CONFIG            0x14
#define ACCEL_CONFIG_2          0x15
#define FSYNC_CONFIG            0x52
#define TEMP_CONFIG             0x53
#define MOD_CTRL_USR            0x54

//Bank 3 Not included (I2C Master/Slave functionality)

//Register Configs
#define BANK_0                  0x00
#define BANK_1                  0x10
#define BANK_2                  0x20
#define BANK_3                  0x30
#define USER_CTRL_DMP_ON        0x90
#define USER_CTRL_DMP_OFF       0x10
#define LP_DUTY_ON              0x70
#define LP_DUTY_OFF             0x00

//Structs
typedef struct imu_t {
    uint16_t accelX;
    uint16_t accelY;
    uint16_t accelZ;
    uint16_t gyroX;
    uint16_t gyroY;
    uint16_t gyroZ;
    uint16_t temp; 
} imu_t;

//Prototypes
void imuTask();
int imuInit(imu_t * imu);
int changeBank(int bank);

#endif
