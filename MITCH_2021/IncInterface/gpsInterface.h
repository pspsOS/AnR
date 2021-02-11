/*
 * gpsInterface.h
 *
 *  Created on: Feb 1, 2021
 *      Author: Jeff
 */

#ifndef GPSINTERFACE_H_
#define GPSINTERFACE_H_

#include "genericInterface.h"

void gpsInit(bool* gpsNominal);
void gpsLoadString(char* gpsNmea);

#endif
