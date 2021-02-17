/*
 * debugStorage.c
 *
 *  Created on: Jan 30, 2021
 *      Author: 19rya
 */
#include "debugStorage.h"
/**
 * @brief Writes to file
 * Writes the pointer to a file
 *
 * @author Ryan Horvath
 * @date 2/11/21
 */
int8_t writeToDebugStorage(uint8_t *bytePointer, uint8_t streamSize){
	FILE *fp = NULL;
	fp = fopen("output.txt","a");
	if (fp == NULL) {
		return FAILED_FILE_WRITE;
	}
	fwrite(bytePointer, sizeof(uint8_t), streamSize, fp);
	fclose(fp);
	fp = NULL;
	return SUCCESSFUL_FILE_WRITE;
}

