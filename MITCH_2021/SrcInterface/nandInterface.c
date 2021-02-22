#include <nandInterface.h>
#include <common.h>

spiLock_t* nandSpiLock;

void nandInit(bool* nandNomPtr) {
	nomPtr[NAND] = nandNomPtr;
	nandSpiLock = registerSpiLock();
	setSpiLock(NAND_CS_GPIO_Port, NAND_CS_Pin, nandSpiLock);
	*nandNomPtr = true;

	setFeature(FEATURE_ADDR_A, 0x00);
	setFeature(FEATURE_ADDR_B, 0x10);
}



/**
 * @brief Load data into the buffer from the cell array
 *
 * @param rowAddr: The 17 bit row address
 *
 * @author Mark Batistich
 * @date 01/21/2021
 */


void nandBufferLoad(uint32_t rowAddr){
//#if CS3_PIN != FAKE_PIN
	// Variables
	uint8_t cmd[4];  // Command sent to device
	uint8_t feature; //feature byte
	bool oip = true; //operation in progress

	//Load data from cell array into buffer
	cmd[0] = R_CELL_ARRAY;
	cmd[3] = rowAddr;
	cmd[2] = rowAddr >> 8;
	cmd[1] = rowAddr >> 16;
	if (sendSPI(&cmd[0], 4, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}

	do{
		feature = getFeature(FEATURE_ADDR_C);
		oip = getBit(feature, 0);
	}while(oip);
//#endif
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
//#if CS3_PIN != FAKE_PIN
	// Variables
	uint8_t cmd[4];       // Command sent to device

	//Read data from the buffer into the data array
	cmd[0] = R_BUFFER;
	cmd[2] = colAddr;
	cmd[1] = colAddr >> 8;
	cmd[3] = 0x00; //dummy byte
	if (recieveSPI(&cmd[0], 4, data, size, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS))
	{
		handleHalError(NAND);
		return;
	}
//#endif
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

	cmd[0] = W_LOAD_RAND;
	cmd[2] = colAddr;
	cmd[1] = colAddr >> 8;

	lockSpi(nandSpiLock);

	if (sendSPI(&cmd[0], 3, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}

	if (sendSPI(&data[0], size, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}
	unlockSpi(nandSpiLock);
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
	uint8_t feature = 0x00; //feature byte
	uint8_t oip = 1; //operation in progress
	uint8_t prg_f = 0; //program fail
	uint8_t eccs1 = 0;
	uint8_t eccs0 = 0;

	//Load data from cell array into buffer
	cmd[0] = W_EXECUTE;
	cmd[3] = rowAddr;
	cmd[2] = rowAddr >> 8;
	cmd[1] = rowAddr >> 16;
	if (sendSPI(&cmd[0], 4, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}

	/*for (int i=0; i<5; i++){
		feature = getFeature(FEATURE_ADDR_C);
		oip = getBit(feature, 0);
		prg_f = getBit(feature, 3);
		eccs1 = getBit(feature, 5);
		eccs0 = getBit(feature, 4);

		printf("prg_f: %d, eccs1: %d, eccs0: %d, oip: %d\n\r", prg_f, eccs1, eccs0, oip);

		if (!oip) i = 5;
		//printf("Exiting For Loop");
	}*/
	for(int i=0;i<10;i++){
		feature = getFeature(FEATURE_ADDR_C);
		oip = getBit(feature, 0);
		if(!oip) i=10;
	}

	/*do{
		feature = getFeature(FEATURE_ADDR_C);
		oip = getBit(feature, 0);
		prg_f = getBit(feature, 3);
		eccs1 = getBit(feature, 5);
		eccs0 = getBit(feature, 4);

		printf("prg_f: %d, eccs1: %d, eccs0: %d, oip: %d \n\r", prg_f, eccs1, eccs0, oip);

	}while(oip);*/
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
	//eraseBlock(rowAddr);
	nandBufferExecute(rowAddr);
	writeDisable();
	return;
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
		handleHalError(NAND);
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
		handleHalError(NAND);
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
	uint8_t feature = 0x00; //feature byte

	// Send Command
	cmd[0] = GET_FEATURE;
	cmd[1] = featureAddr;
	if (recieveSPI(&cmd[0], 2, &feature, 1, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
	}
	return feature;
}

/**
 * @brief Sets a feature at a specific address
 *
 * @param featureAddr: address of the feature
 * @param featureVal: Value to set
 *
 * @author Mark Batistich
 * @date 01/24/2021
 */
void setFeature(uint8_t featureAddr, uint8_t featureVal){
	// Variables
	uint8_t cmd[3];  // Command sent to device

	// Send Command
	cmd[0] = SET_FEATURE;
	cmd[1] = featureAddr;
	cmd[2] = featureVal;
	if (sendSPI(&cmd[0], 3, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}
}

/**
 * @brief Erase a block
 *
 * @param rowAddr: block to erase
 *
 * @author Mark Batistich
 * @date 2/6/2021
 */
void eraseBlock(uint32_t rowAddr){
	// Variables
	uint8_t cmd[4];  // Command sent to device
	uint8_t feature; //feature byte
	bool oip = true; //operation in progress

	//Load data from cell array into buffer
	cmd[0] = BLOCK_ERASE;
	cmd[3] = rowAddr;
	cmd[2] = rowAddr >> 8;
	cmd[1] = rowAddr >> 16;

	writeEnable();
	if (sendSPI(&cmd[0], 4, NAND_CS_GPIO_Port, NAND_CS_Pin, STORAGE_SPI_BUS)){
		handleHalError(NAND);
		return;
	}

	do{
		feature = getFeature(FEATURE_ADDR_C);
		oip = getBit(feature, 0);
	}while(oip);
	writeDisable();
}

void eraseAll(){
	for (int i=0; i<MAX_BLOCK; i++){
		eraseBlock(i << 6);
	}
}
