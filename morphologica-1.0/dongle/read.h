#ifndef READ_H
#define READ_H

#include "helpers.h"

void ReadRawTXData(uint8_t* TX, int length);
void ReadRawRXData(uint8_t* RX, int length);
void ReadHandData(uint8_t* RX, uint16_t* touchdata, int touchdatalength);
void ReadHandDataOneLine(uint8_t* RX, uint16_t* touchdata, int length);
void ReadHandPositionData(uint8_t* RX);
void ReadHandVelocityData(uint8_t* RX);
void ReadHandTouchData(uint8_t* RX, uint16_t* touchdata, int touchdatalength);

#endif