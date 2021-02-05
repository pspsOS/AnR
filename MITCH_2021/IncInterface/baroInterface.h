#ifndef BARO_H
#define BARO_H

//Includes
#include "genericInterface.h"
//Variable Build Defines (Higher values = more accuracy and higher conversion times)
#define D1_256			0x40
#define	D1_512			0x42
#define D1_1024			0x44
#define	D1_2048			0x46
#define	D1_4096			0x48
#define D2_256			0x50
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

//GPIO Defines
#define BARO_CS_GPIO_Port CS2_GPIO_Port
#define BARO_CS_Pin CS2_Pin

typedef struct MS5607 {
	//Baro Data
		uint16_t senst1;		//C1 on datasheet
		uint16_t offt1; 		//C2 on datasheet
		uint16_t tcs;			//C3 on datasheet
		uint16_t tco;			//C4 on datasheet
		uint16_t tref;			//C5 on datasheet
		uint16_t tempsens;		//C6 on datasheet
		uint32_t digitalPres;	//D1 on datasheet (Only 24 bits will be filled)
		uint32_t digitalTemp;	//D2 on datasheet (Only 24 bits will be filled)
		int32_t	deltaT;			//dT on datasheet (This is a calculated value)
		int32_t temp;			//TEMP on datasheet
		int64_t off;			//OFF on datasheet (This is a calculated value)
		int64_t sens;			//SENS on datasheet (This is a calculated value)
		int32_t pressure;		//P on datasheet (This is a calculated value)
} MS5607_t;

MS5607_t bmp;

//Prototypes
void barometerRead(int32_t *temperature, int32_t *pressure);
void barometerInit(bool *bmpNomPtr);

#endif
