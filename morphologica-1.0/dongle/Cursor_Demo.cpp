/*
* 
* @file Cursor_Demo.cpp
* 
* @author	Jumana Schmidt
* @date		2022-07-29
* 
* @brief This demo showcases using input from your mouse along the y-axis to control the degree positions
* on the hand (ie. allowing you to open and close the fingers)
* 
*/

#include "Cursor_Demo.h"

void CursorDemo(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 75;																						// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;

	POINT cursor;
	GetCursorPos(&cursor);																			// get coordinates of your cursor at that moment
	for (int ch = 0; ch < 6; ch++) {
		deg[ch] = cursor.y / CURSORCONTROLRANGE;															// divisor controls mouse's range of effect & sensitivity of motor control (change based on your size of screen)
	}
	deg[5] = -deg[5];
}

/* 
* @brief converts degree outputs from previous function into gear ratio and packages data into TX array 
*/
void LoadTXDegrees_(float* arr, uint8_t* send_data) {
	send_data[0] = 0x11;																					// length of message (17 bits)
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
