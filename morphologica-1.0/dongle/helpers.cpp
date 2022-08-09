/*
*
* @file helpers.cpp
*
* @author	Jumana Schmidt
* @date		2022-07-29
*
* @brief This file includes several different helper functions and libraries necessary for many different
* demos and the basic driver itself.
*
*/

#include "helpers.h"

int getDecimal(uint8_t byte) {
	return (int)byte;
}

float getDegrees(uint8_t byte1, uint8_t byte2) {
	int16_t word = ((uint16_t)byte1 << 8) | ((uint16_t)byte2);
	return ((float)word * 150.f) / 32767.f;																	// convert degress input equation from PSYONIC API documentation
}

float getCurrent(int Idigi) {
	return ((float)Idigi * 0.540) / 7000.f;																	// convert current input equation from PSYONIC API documentation
}

/*
* @brief function to help unpack touch sensor data in little endian format (used for upnpacking touch data)
*/
void unpack_8bit_into_12bit(uint8_t* arr, uint16_t* vals, int valsize)
{
	for (int i = 0; i < valsize; i++)
		vals[i] = 0;																						//clear the buffer before loading it with |=
	for (int bidx = valsize * 12 - 4; bidx >= 0; bidx -= 4)
	{
		int validx = bidx / 12;
		int arridx = bidx / 8;
		int shift_val = (bidx % 8);
		vals[validx] |= ((arr[arridx] >> shift_val) & 0x0F) << (bidx % 12);
	}
}

/*
* @brief get nanosecond count (used for delay in rundemos.h)
*/
uint64_t ns() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	uint64_t t = (((uint64_t)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
	return t;
}

/* 
* @brief checks if input data has a correct checksum
*/
int isRXCheckSumCorrect(uint8_t* arr, uint8_t length) {
	uint8_t ORsum = 0;
	int8_t sum = 0;
	for (uint8_t b = 0; b < length; b++) { 																	// parse each byte
		sum += (int8_t)arr[b];																				// add up each byte
		ORsum |= arr[b];
	}

	if (sum == 0x00 && ORsum != 0) {																		// check if all of bytes' sum equals zero & makes sure data isn't just all zeros
		return 1;																							// good data
	}
	else {
		return 0;																							// bad data
	}
}
