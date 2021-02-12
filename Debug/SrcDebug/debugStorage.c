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
void writeToFile(uint8_t pointer[], uint8_t size){
	FILE *fp;
	fp = fopen("output.txt","w+");
	int i;
	for(i = 0; i < size; i++){
		fwrite(pointer[i], sizeof(pointer), 1, 0);
	}
	fclose(fp);
}

