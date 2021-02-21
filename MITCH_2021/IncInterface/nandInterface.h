#ifndef NAND_H
#define NAND_H

//Includes
#include "genericInterface.h"

//Command Defines
#define R_CELL_ARRAY 	0x13
#define R_BUFFER 		0x03

#define W_ENABLE		0x06
#define W_LOAD			0x02
#define W_LOAD_RAND		0x84
#define W_EXECUTE		0x10

#define GET_FEATURE		0x0F
#define FEATURE_ADDR	0xC0;

//Generic Defines
#define MAX_BLOCK 		2048
#define MAX_PAGE		64
#define MAX_COLUMN		4224

//GPIO Defines
#define NAND_CS_GPIO_Port CS3_GPIO_Port
#define NAND_CS_Pin CS3_Pin

//Prototypes

void nandBufferLoad(int32_t rowAddr); //Loads data from a row to the buffer
void nandBufferRead(int16_t colAddr, int8_t data[], int8_t size); //Reads data from the buffer into the data array

void nandBufferWrite(int16_t colAddr, int8_t data[], int8_t size); //Loads data from the data array to the buffer
void nandBufferExecute(int32_t rowAddr); //Loads the data in the buffer to a row

void nandRead(int32_t rowAddr, uint16_t colAddr, int8_t data[], int8_t size); //Executes nandBufferLoad and nandBufferRead
void nandWrite(int32_t rowAddr, uint16_t colAddr, int8_t data[], int8_t size); //Executes nandBufferWrite and nandBufferExecute

#endif
