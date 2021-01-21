/*
 * main.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Jeff
 */

#ifndef __MAIN_H
#define __MAIN_H

#define GPS_FREQ (1)        //   1 Hz
#define BMP_MULTIPLIER (4) //  10 Hz
#define IMU_MULTIPLIER (4) // 100 Hz

#define portTICK_RATE_MS 1000 // Used for task timing. Is actually defined in FreeRTOS.h
#define ACQUISITION_TASK_DELAY2 1

#endif /* __MAIN_H */
