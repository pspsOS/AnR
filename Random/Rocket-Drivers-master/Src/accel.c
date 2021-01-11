/*
 * accel.c
 *
 * Author: Dawson Moore (11/22/2019)
 */

#include "accel.h"

/*
 * Function: accelTask()
 *
 * Gathers the linear acceleration of the rocket
 *
 */
void accelTask()
{
    initAccel();
    TickType_t time_init = 0;
	uint8_t dataIn[2];
	uint8_t cmd;
	while (PSP == GREAT)
	{
        //Get the current acceleration value as an unsiged 12 bit integer with the 0g point being 512 LSB
        cmd[0] = 0x30; //0x300D 
        cmd[1] = 0x0D;
        recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
        rocket.accel = (0x00 | ((0x03 & dataIn[0]) << 8) | ((0xC0 & dataIn[0]) >> 6) | (dataIn[1] << 2)) - 512; //This is now centered around 0

        vTaskDelayUntil(&time_init, ACCEL_RATE);
    }
}

/*
 * Function: accelInit()
 *
 * Initializes the linear accelerometer
 * 
 * Note: Register Write: P | 00 | AAAAA | 00000000
 *       Register Read:  P | 10 | AAAAA | DDDDDDDD
 *
 */
void initAccel()
{
    uint8_t dataIn[2];
	uint8_t cmd[2];

    //Gather STDEFL
	cmd[0] = 0x04; //0000 | 00100 | 00000000 | 0x8400
    cmd[1] = 0x00;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    lin.stdefl = dataIn[1];
    //Check for whichever issues you'd like here

    //Set self test in AXISCFG (set low pass too)
    cmd[0] = 0x4C; //010 | 01100 | 10000010 | 0x4C82 This will set the low pass to 400 Hz, 4 pole (last 4 bits)
    cmd[1] = 0x82;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    //Again, check for issues

    //If arming is necessary, set the arming parameters here
    //I'm not using it as of now.

    //Check DEVSTAT for errors
    cmd[0] = 0x94; //100 | 10100 | 00000000 | 0x9400
    cmd[1] = 0x00;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    if (dataIn[1] & 0x4A)
    {
        //The sensor has either IDE, MISO, or OFFSET error(s)
        //Launch should be aborted and debug should be used to find exact issue
        //So do some error handling here
    }

    //Get offset correction (OFFCORR)
    cmd[0] = 0x16; //000 | 10110 | 00000000 | 0x1600
    cmd[1] = 0x00;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    lin.offcorr = dataIn[1];

    //Figure out the max and min limits based on the STDEFL register for self test
    lin.stAccMinLimit = floor((363 + (lin.stdefl / 255) * (605 - 363)) * 0.9); //The STmin and STmax values need to be validated for launch config
    lin.stAccMaxLimit = ciel((363 + (lin.stdefl / 255) * (605 - 363)) * 1.1); //The STmin and STmax values need to be validated for launch config

    //Get the current acceleration for self test
    cmd[0] = 0x30; //0x300D Note: This one is unsigned since the self test exerts a positive acceleration
    cmd[1] = 0x0D;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    lin.testaccel = (0x00 | ((0x03 & dataIn[0]) << 8) | ((0xC0 & dataIn[0]) >> 6) | (dataIn[1] << 2)) - 512; //-512 zeros the g load. Unsigned mode has 0g set at 512, so this reverses it (might need to check)
    if (lin.testaccel > lin.stAccMaxLimit || lin.testaccel < lin.stAccMinLimit)
    {
        //We failed the self test. . . Uh oh. . .
        //Do error handling and abort launch
    }

    //Toggle the self test off to give us some extra time before beginning normal read
    cmd[0] = 0xCC; //110 | 01100 | 00000010 | 0xCC02 Turns self test off (endinit does too, but this is more likely to be reliable)
    cmd[1] = 0x02;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    //Again, check for issues

    //Write DEVCFG (endinit)
    cmd[0] = 0xCB; //110 | 01011 | 10111000 | 0xCBB8 We can no longer write configurations except to DEVCTL (initialization ended)
    cmd[1] = 0xB8;
    recieveSPI(cmd, 2, dataIn, 2, LIN_ACCEL_CS_GPIO_Port, LIN_ACCEL_CS_Pin);
    //Last issue check. We should be good to go now
}