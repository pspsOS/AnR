#ifndef BARO_H
#define BARO_H

//Includes
#include "FreeRTOS.h"
#include "genericSpi.h"

//Variable Build Defines (Higher values = more accuracy and higher conversion times)
#define D1_256			0x40
#define	D1_512			0x42
#define D1_1024			0x44
#define	D1_2048			0x46
#define	D1_4096			0x48
#define D2_512			0x50
#define D2_512			0x52
#define D2_1024			0x54
#define D2_2048			0x56
#define D2_4069			0x58
#define CONV_T_256		1 //Conversion times are worst case rounded up
#define CONV_T_512		2
#define CONV_T_1024		3
#define CONV_T_2048 	5
#define CONV_T_4096 	10

//Generic Defines
#define BARO_STACK_SIZE 256 //Change to fit needs
#define BARO_PRIORITY   1	//Change to fit needs
#define BARO_RATE       5 / portTICK_RATE_MS //Run at a rate of 200 Hz. The 5 is indicative of a period of 5 ms.

//Prototypes
void barometerTask();
void barometerInit();

#endif
