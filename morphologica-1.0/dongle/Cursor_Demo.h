#ifndef CURSOR_H
#define CURSOR_H

#define CURSORCONTROLRANGE			8											// divisor  that controls mouse's range of effect & sensitivity of motor control (change based on your size of screen)

#include "helpers.h"
#include "driver.h"

void CursorDemo(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize);
void LoadTXDegrees_(float* arr, uint8_t* send_data);

#endif