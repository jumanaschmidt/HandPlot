#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

int getDecimal(uint8_t byte);
float getDegrees(uint8_t byte1, uint8_t byte2);
float getCurrent(int Idigi);
void unpack_8bit_into_12bit(uint8_t* arr, uint16_t* vals, int valsize);
uint64_t ns();
int isRXCheckSumCorrect(uint8_t* arr, uint8_t length);

typedef union {
	uint8_t bytes[24];
	int16_t vals[12];
}motordata_t;

#endif
