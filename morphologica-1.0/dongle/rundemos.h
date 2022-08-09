#ifndef RUNDEMOS_H
#define RUNDEMOS_H

#include "helpers.h"
#include "driver.h"
#include "read.h"
#include "FingerWave_Demo.h"
#include "Cursor_Demo.h"
#include "VoltageWave_Demo.h"

void OpenHand(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize);
void CloseHand(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize);
int SPI_Demos_Handler(DWORD locationId);

#endif