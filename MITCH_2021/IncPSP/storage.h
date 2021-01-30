/*
 * storage.h
 *
 *  Created on: Dec 13, 2020
 *      Author: vishv
 */

#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#include "common.h"

bool storageSetup();
void storageWrite();

void storeDaqStatus();
void storeDaqScaling();
void storeGpsData();
void storeBmpData();
void storeImuData();
void storeProcessedData();
void storeMonitoringData();

#endif /* INC_STORAGE_H_ */
