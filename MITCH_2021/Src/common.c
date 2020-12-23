/*
 * common.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Jeff
 */

int i;

byte pspStrCmp(char *str, byte start, char *compareWith, byte length) {
	for(i = 0; i < length; i++)
		if(str[start + i] != compareWith[i])
			return False;
	return True;
}

// TODO: Implement getTimeStamp()
uint32_t getTimeStamp(void) {
	return 0;
}
