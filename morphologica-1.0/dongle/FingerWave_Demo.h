#ifndef FINGERWAVE_H
#define FINGERWAVE_H

#include "helpers.h"
#include "driver.h"

#define WAVESPEEDVAL	6																					// speed of fingerwave demo

void FingerWave(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize);
void LoadTXDegrees(float* arr, uint8_t* send_data);

#endif