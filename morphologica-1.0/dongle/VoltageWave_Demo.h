#ifndef VOLTAGEWAVE_H
#define VOLTAGEWAVE_H

#include "helpers.h"
#include "driver.h"

#define K				50																			// speed of PID function in voltage controller demo

void FormatRXData(float* mDeg, float* mVelo, int& txdatasize, int& rxdatasize, int& readtxsize, uint8_t* read_data);
void VoltageControlDemo(float* volt, float* deg, float* mDeg, float* mVelocity);
void LoadTXVoltage(float* arr, uint8_t* send_data);

#endif