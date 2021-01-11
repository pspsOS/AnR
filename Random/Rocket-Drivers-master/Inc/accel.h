#ifndef ACCEL_H
#define ACCEL_H

//Includes
#include <math.h>
#include "FreeRTOS.h"
#include "genericSpi.h"

//Variable Build Defines

//Generic Defines
#define ACCEL_STACK_SIZE 256 //Change to fit needs
#define ACCEL_PRIORITY   1	//Change to fit needs
#define ACCEL_RATE       5 / portTICK_RATE_MS //Run at a rate of 200 Hz. The 5 is indicative of a period of 5 ms.

//Structs
typedef struct linearAccel {
    uint8_t stdefl;
    uint8_t offcorr;
    double stAccMinLimit;
    double stAccMaxLimit;
    int16_t testaccel;
} linearAccel;

extern linearAccel lin;

//Prototypes
void accelTask();
void initAccel();

#endif
