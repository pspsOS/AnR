#include <nandInterface.h>
//#include <common.h>
#ifndef getBit
#define getBit(A, X) ((((A >> X) & 0x01) == 0x01) ? (0x01) : (0x00))
#define setBit(A, X, V) (A & ~(0x01 << X) | (V << X))
#endif
/**
 * @brief Load data into the buffer from the cell array
 *
 * @param data: The array to which data is read to or written from
 * @param size: The number of bytes to read/write
 * @param rowAddr: The 17 bit row address
 * @param colAddr: The 13 bit column address
 *
 * @author Mark Batistich
 * @date 01/21/2021
 */

void nandBufferLoad(int32_t rowAddr){
#if CS3_PIN != FAKE_PIN
	// Variables
	uint8_t cmd[4];  // Command sent to device
	uint8_t feature; //feature byte
	bool oip = false; //operation in progress

	//Load data from cell array into buffer
	cmd[0] = R_CELL_ARRAY;
	cmd[3] = rowAddr;
	cmd[2] = rowAddr >> 8;
	cmd[1] = rowAddr >> 16;
	if (sendSPI(&cmd[0], 4, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return;
	}

	cmd[0] = GET_FEATURE;
	cmd[1] = FEATURE_ADDR;
	do{
		if (recieveSPI(&cmd[0], 2, &feature, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS))
		{
			handleHalError(BMP);
			return;
		}
		oip = getBit(feature, 0);
	}while(oip);
#endif
}
void nandBufferRead(int16_t colAddr, int8_t data[], int8_t size){
#if CS3_PIN != FAKE_PIN
	// Variables
	uint8_t cmd[3];       // Command sent to device

	//Read data from the buffer into the data array
	cmd[0] = R_BUFFER;
	cmd[2] = colAddr;
	cmd[1] = colAddr >> 8;
	if (recieveSPI(&cmd[0], 3, (uint8_t*) data, size, CS3_GPIO_Port, CS3_Pin, STORAGE_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
#endif
}

//void nandBufferWrite(int16_t colAddr, int8_t data[], int8_t size); //Loads data from the data array to the buffer
//void nandBufferExecute(int32_t rowAddr); //Loads the data in the buffer to a row

void nandRead(int32_t rowAddr, uint16_t colAddr, int8_t data[], int8_t size){
	nandBufferLoad(rowAddr);
	nandBufferRead(colAddr, data, size);
}

/*void nandWrite(int32_t rowAddr, uint16_t colAddr, int8_t data[], int8_t size){
	nandBufferWrite(colAddr, data, size);
	nandBufferRead(rowAddr);
}*/


