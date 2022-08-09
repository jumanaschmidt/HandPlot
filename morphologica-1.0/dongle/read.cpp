/*
*
* @file Read.cpp
*
* @author	Jumana Schmidt
* @date		July 2022
*
* @brief This file allows you to be able to visualize the data being sent to and received by the hand in various 
* ways on the terminal. 
* 
* @attention These print statements can cause delay in transmission (generally not noticeable if at higher baud rate)
* 
* @note Data is collected in different indexes from the API's reply variant because the dongle's firmware 
* adds to RX data buffer from the hand for different purposes. So, the hand sends 72 bytes of data to the 
* STM, but the STM adds three bytes, shifting where the position data, touch data, etc is.
* 
*/


#include "read.h"

/* 
* @brief displays data being sent to the hand in raw hexadecimal format 
*/
void ReadRawTXData(uint8_t* TX, int length) {
	for (int i = 0; i < length; i++) {
		printf("%02X ", TX[i]);
	}
	printf("\n");
}

/* 
* @brief displays data being sent to the hand in raw hexadecimal format 
*/
void ReadRawRXData(uint8_t* RX, int length) {
	for (int i = 0; i < length; i++) {
		printf("%02X ", RX[i]);
	}
	printf("\n");
}


/* 
* @brief displays data being received from the hand in processed/readable format on one line 
*/
void ReadHandData(uint8_t* RX, uint16_t* touchdata, int touchdatalength) {
	printf("\n********Hand Data******** \n \n");
	printf("Position Data:	%f %f %f %f %f %f \n", getDegrees(RX[1], RX[2]), getDegrees(RX[5], RX[6]), getDegrees(RX[9], RX[10]), getDegrees(RX[13], RX[14]), getDegrees(RX[17], RX[18]), getDegrees(RX[21], RX[22]));
	printf("Velocity Data:	%d %d %d %d %d %d \n", getDecimal(RX[4]), getDecimal(RX[8]), getDecimal(RX[12]), getDecimal(RX[16]), getDecimal(RX[20]), getDecimal(RX[24]));
	printf("Current Data:	%f %f %f %f %f %f \n", getCurrent(getDecimal(RX[3])), getCurrent(getDecimal(RX[7])), getCurrent(getDecimal(RX[11])), getCurrent(getDecimal(RX[15])), getCurrent(getDecimal(RX[19])), getCurrent(getDecimal(RX[23])));
	printf("Touch Data: ");
	unpack_8bit_into_12bit(&RX[25], touchdata, touchdatalength);
	for (int i = 0; i < touchdatalength; i++) {
		printf("%d ", touchdata[i]);
	}
	printf("\n");
}


/* 
* @brief displays data being received from the hand in processed/readable format on one line 
*/
void ReadHandDataOneLine(uint8_t* RX, uint16_t* touchdata, int length) {
	printf("\n********Hand Data******** \n \n");
	printf("%f %f %f %f %f %f %d %d %d %d %d %d %f %f %f %f %f %f ", getDegrees(RX[1], RX[2]), getDegrees(RX[5], RX[6]), getDegrees(RX[9], RX[10]), getDegrees(RX[13], RX[14]), getDegrees(RX[17], RX[18]), getDegrees(RX[21], RX[22]), getDecimal(RX[4]), getDecimal(RX[8]), getDecimal(RX[12]), getDecimal(RX[16]), getDecimal(RX[20]), getDecimal(RX[24]), getCurrent(getDecimal(RX[3])), getCurrent(getDecimal(RX[7])), getCurrent(getDecimal(RX[11])), getCurrent(getDecimal(RX[15])), getCurrent(getDecimal(RX[19])), getCurrent(getDecimal(RX[23])));
	unpack_8bit_into_12bit(&RX[25], touchdata, 30);
	for (int i = 0; i < length; i++) {
		printf("%d ", touchdata[i]);
	}
	printf("\n");
}

/* 
* @brief displays position/degree data being received from the hand in processed/readable format 
*/
void ReadHandPositionData(uint8_t* RX) {
	float measureddegrees[6] = { 0 };
	motordata_t posdata;
	for (int i = 0; i < 24; i++) {
		posdata.bytes[i] = RX[i + 3];
	}
	for (int ch = 0; ch < 6; ch++) {
		measureddegrees[ch] = ((float)posdata.vals[ch * 2]) * 150.f / 32767.f;
	}
	printf("\n********Position Data******** \n \n");
	printf("Index: %f \n", measureddegrees[0]);
	printf("Middle: %f \n", measureddegrees[1]);
	printf("Ring: %f \n", measureddegrees[2]);
	printf("Pinky: %f \n", measureddegrees[3]);
	printf("Thumb Flexor: %f \n", measureddegrees[4]);
	printf("Thumb Rotator: %f \n \n", measureddegrees[5]);
	printf("\n");
}

/* 
* @brief displays velocity data being received from the hand in processed/readable format 
*/
void ReadHandVelocityData(uint8_t* RX) {
	float measuredvelocity[6] = { 0 };
	motordata_t velocitydata;
	for (int i = 0; i < 24; i++) {
		velocitydata.bytes[i] = RX[i + 4];
	}
	for (int ch = 0; ch < 6; ch++) {
		if (ch == 5) {
			measuredvelocity[ch] = ((float)velocitydata.vals[ch * 2] / (4.f * 162.45));
		}
		else {
			measuredvelocity[ch] = ((float)velocitydata.vals[ch * 2] / (4.f * 649.f));
		}
	}

	printf("\n********Velocity Data******** \n \n");
	printf("Index: %f \n", measuredvelocity[0]);
	printf("Middle: %f \n", measuredvelocity[1]);
	printf("Ring: %f \n", measuredvelocity[2]);
	printf("Pinky: %f \n", measuredvelocity[3]);
	printf("Thumb Flexor: %f \n", measuredvelocity[4]);
	printf("Thumb Rotator: %f \n \n", measuredvelocity[5]);
	printf("\n");
}

/* 
* @brief displays touch sensor data being received from the hand in processed/readable format 
*/
void ReadHandTouchData(uint8_t* RX, uint16_t* touchdata, int touchdatalength) {
	unpack_8bit_into_12bit(&RX[25], touchdata, touchdatalength);
	printf("\n********Touch Sensor Data******** \n \n");
	for (int i = 0; i < touchdatalength; i++) {
		printf(" %d ", touchdata[i]);
	}
	printf("\n");
}