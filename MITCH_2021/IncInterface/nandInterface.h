#ifndef NAND_H
#define NAND_H

//Includes
#include "genericInterface.h"
#include "../IncPSP/gpio.h"
//Command Defines
#define R_CELL_ARRAY 	0x13
#define R_BUFFER 		0x03

#define W_ENABLE		0x06
#define W_DISABLE		0x04
#define W_LOAD			0x02
#define W_LOAD_RAND		0x84
#define W_EXECUTE		0x10

#define GET_FEATURE 0x0F
#define OP_FEATURE_ADDR	0xC0

//Generic Defines
#define MAX_BLOCK 		2048
#define MAX_PAGE		64
#define MAX_COLUMN		4224

//GPIO Defines



//Prototypes

void nandBufferLoad(uint32_t rowAddr); //Loads data from a row to the buffer
void nandBufferRead(uint16_t colAddr, uint8_t data[], uint8_t size); //Reads data from the buffer into the data array

void nandBufferWrite(uint16_t colAddr, uint8_t data[], uint8_t size); //Loads data from the data array to the buffer
void nandBufferExecute(uint32_t rowAddr); //Loads the data in the buffer to a row

void nandRead(uint32_t rowAddr, uint16_t colAddr, uint8_t data[], uint8_t size); //Executes nandBufferLoad and nandBufferRead
void nandWrite(uint32_t rowAddr, uint16_t colAddr, uint8_t data[], uint8_t size); //Executes nandBufferWrite and nandBufferExecute

void writeEnable(); //Enables writing on the nand
void writeDisable(); //Disables writing on the nand

uint8_t getFeature(uint8_t featureAddr); //Gets a feature from the nand

#endif
