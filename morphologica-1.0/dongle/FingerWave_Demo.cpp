/*
*
* @file FingerWave_Demo.cpp
*
* @author	Jumana Schmidt
* @date		2022-07-29
*
* @brief This demo demonstrates a 'finger wave' that uses degree outputs from a sinusoidal function
* to control finger position on the hand.
*
*/

#include "FingerWave_Demo.h"

/* 
* @brief sinusoidal function and data size allocation 
*/
void FingerWave(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 75;																						// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;

	SYSTEMTIME st;
	GetSystemTime(&st);
	float t = ((float)st.wMilliseconds) / 1000.f + ((float)st.wSecond);										// using ms/1000 to update enough & adding a second so values don't go back to zero
	float ft = t * WAVESPEEDVAL;																			// speed value control how fast the fingers wave

	for (int ch = 0; ch < 6; ch++)																			// parse through each finger channel & set new associated degree value
	{
		deg[ch] = (0.5 * sin(ft + (float)ch) + 0.5) * 45 + 15;												// sinusoidal function on all fingers
	}
	deg[5] = -deg[5];																						// negate thumb rotator
}

/* 
* @brief converts degree outputs from previous function into gear ratio and packages data into TX array 
*/
void LoadTXDegrees(float* arr, uint8_t* send_data) {
	send_data[0] = 0x11;																					// length of message (17)
	send_data[1] = 0x50;																					// hand address
	send_data[2] = 0x10;																					// position control mode

	int sdidx = 3;																							// counter to keep track of sendData index values (starts @ 3 so it ignores first 3 values)
	for (int i = 0; i < 6; i++) {

		float pdigital = arr[i] * 32767.f / 150.f;															// ratio equation to get position values from degrees
		int16_t hexp = (int16_t)pdigital;

		send_data[sdidx + 1] = hexp >> 8;																	// split message into bytes
		send_data[sdidx] = hexp & 0xff;
		sdidx += 2;																							// increment to next set of two bytes
	}

	send_data[15] = checkSum(send_data, send_data[0], TRUE);							// checksum for just data being sent directly to the hand
	send_data[16] = checkSum(send_data, send_data[0], FALSE);								// checksum for entire SPI message
}

