#include <nandInterface.h>
#include <common.h>

/**
 * @brief Load data into the buffer from the cell array
 *
 * @param rowAddr: The 17 bit row address
 *
 * @author Mark Batistich
 * @date 01/21/2021
 */
void nandBufferLoad(uint32_t rowAddr){
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

	do{
		feature = getFeature(OP_FEATURE_ADDR);
		oip = getBit(feature, 0);
	}while(oip);

}

/**
 * @brief Load data into the data array from the buffer
 *
 * @param data: The array to which data is read to or written from
 * @param size: The number of bytes to read/write
 * @param colAddr: The 13 bit column address
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void nandBufferRead(uint16_t colAddr, uint8_t data[], uint8_t size){
	// Variables
	uint8_t cmd[3];       // Command sent to device

	//Read data from the buffer into the data array
	cmd[0] = R_BUFFER;
	cmd[2] = colAddr;
	cmd[1] = colAddr >> 8;
	if (recieveSPI(&cmd[0], 3, data, size, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS))
	{
		handleHalError(BMP);
		return;
	}
}

/**
 * @brief Write data into the buffer from the data array
 *
 * @param data: The array to which data is read to or written from
 * @param size: The number of bytes to read/write
 * @param colAddr: The 13 bit column address
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void nandBufferWrite(uint16_t colAddr, uint8_t data[], uint8_t size){
	// Variables
	uint8_t cmd[3];       // Command sent to device

	//Read data from the buffer into the data array
	cmd[0] = W_LOAD_RAND;
	cmd[2] = colAddr;
	cmd[1] = colAddr >> 8;
	if (sendSPI(&cmd[0], 3, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return;
	}
}

/**
 * @brief Write data from the buffer to the cell array
 *
 * @param rowAddr: The 17 bit row address
 *
 * @author Mark Batistich
 * @date 01/24 /2021
 */
void nandBufferExecute(uint32_t rowAddr){
	// Variables
	uint8_t cmd[4];  // Command sent to device
	uint8_t feature; //feature byte
	bool oip = false; //operation in progress
	//bool prg_f = false; //program fail

	//Load data from cell array into buffer
	cmd[0] = W_EXECUTE;
	cmd[3] = rowAddr;
	cmd[2] = rowAddr >> 8;
	cmd[1] = rowAddr >> 16;
	if (sendSPI(&cmd[0], 4, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return;
	}
	do{
		feature = getFeature(OP_FEATURE_ADDR);
		oip = getBit(feature, 0);
		//prg_f = getBit(feature, 3);
	}while(oip);
}

/**
 * @brief Performs all reading commands
 *
 * @param rowAddr: The 17 bit row address
 * @param data: The array to which data is read to or written from
 * @param size: The number of bytes to read/write
 * @param colAddr: The 13 bit column address
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void nandRead(uint32_t rowAddr, uint16_t colAddr, uint8_t data[], uint8_t size){
	nandBufferLoad(rowAddr);
	nandBufferRead(colAddr, data, size);
}

/**
 * @brief Performs all writing commands
 *
 * @param rowAddr: The 17 bit row address
 * @param data: The array to which data is read to or written from
 * @param size: The number of bytes to read/write
 * @param colAddr: The 13 bit column address
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void nandWrite(uint32_t rowAddr, uint16_t colAddr, uint8_t data[], uint8_t size){
	nandBufferLoad(rowAddr);
	writeEnable();
	nandBufferWrite(colAddr, data, size);
	nandBufferExecute(rowAddr);
	writeDisable();
}

/**
 * @brief Enables writing
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void writeEnable(){
	// Variables
	uint8_t cmd;  // Command sent to device

	// Send Command
	cmd = W_ENABLE;
	if (sendSPI(&cmd, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return;
	}
}

/**
 * @brief Disables writing
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void writeDisable(){
	// Variables
	uint8_t cmd;  // Command sent to device

	// Send Command
	cmd = W_DISABLE;
	if (sendSPI(&cmd, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return;
	}
}

/**
 * @brief Gets a feature of a specific address
 *
 * @param featureAddr: address of the feature to get
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
uint8_t getFeature(uint8_t featureAddr){
	// Variables
	uint8_t cmd[2];  // Command sent to device
	uint8_t feature; //feature byte

	// Send Command
	cmd[0] = GET_FEATURE;
	cmd[1] = featureAddr;
	if (recieveSPI(&cmd[0], 2, &feature, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(BMP);
		return 0x00;
	}
	return feature;
}
