/*
 * debugStorage.c
 *
 *  Created on: Jan 30, 2021
 *      Author: 19rya
 */
#include "debugStorage.h"

void writeToFile(uint8_t pointer[], uint8_t size){
	int i;
	for(i = 0; i < size; i++){
		fwrite(pointer[i], size, 1, 0);
	}
}

