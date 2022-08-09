/*
*
* @file VoltageWave_Demo.cpp
*
* @author	Jumana Schmidt
* @date		2022-07-29
*
* @brief This demo controls the hand using varying voltage values calculated by a proportional-integral-derivative (PID) function
* to allow smoother and more accurate movements. The function takes any varying position inputs of degree value, but either 
* Finger Wave or Cursor Demo can be selected in driver.cpp as such inputs. The PID function also uses measured position and 
* velocity data from the hand to have precise control, which showcases the dongle's ability for high-frequency communication.
*
* @note In FormatRXData(), data is collected in different indexes from the API's reply variant because the dongle's firmware 
* adds to RX data buffer from the hand for different purposes. So, the hand sends 72 bytes of data to the STM, but the STM
* adds three bytes, shifting where the position data, touch data, etc is.
* 
*/

#include "VoltageWave_Demo.h"

/* 
* @brief processes incoming data from the hand to be able to be used in the PID function 
*/
void FormatRXData(float* mDeg, float* mVelo, int& txdatasize, int& rxdatasize, int& readtxsize, uint8_t* read_data) {
	txdatasize = 75;																										// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;

	motordata_t velodata;
	for (int i = 0; i < 24; i++) {
		velodata.bytes[i] = read_data[i + 4];																				   
	}
	for (int ch = 0; ch < 6; ch++) {
		if (ch == 5) {
			mVelo[ch] = ((float)velodata.vals[ch * 2] / (4.f * 162.45));
		}
		else {
			mVelo[ch] = ((float)velodata.vals[ch * 2] / (4.f * 649.f));
		}
	}

	motordata_t posdata;
	for (int i = 0; i < 24; i++) {
		posdata.bytes[i] = read_data[i + 3];
	}
	for (int ch = 0; ch < 6; ch++) {
		mDeg[ch] = ((float)posdata.vals[ch * 2]) * 150.f / 32767.f;
	}

}

/* 
* @brief Actual proportional-integral-derrivative equation
*
* @note PID equation: u = voltage = K(thetad - measuredtheta) - D(finger velocity) + I(integral(thetad - measuredtheta)) 
*/
void VoltageControlDemo(float* volt, float* deg, float* mDeg, float* mVelocity) {
	for (int ch = 0; ch < 6; ch++) {
		volt[ch] = (float)(K * (deg[ch] - mDeg[ch]) - mVelocity[ch]);
	}
}

/* 
* @brief packages data into TX arra 
*/
void LoadTXVoltage(float* arr, uint8_t* send_data) {
	send_data[0] = 17;																						// length of SPI message
	send_data[1] = 0x50;																					// hand address
	send_data[2] = 0x40;																					// voltage control mode

	int sdidx = 3;																							// counter to keep track of sendData index values (starts @ 3 so it ignores first 3 values)
	for (int i = 0; i < 6; i++) {
		int16_t hexv = (int16_t)arr[i];

		send_data[sdidx + 1] = hexv >> 8;																	// split message into bytes
		send_data[sdidx] = hexv & 0xff;
		sdidx += 2;																							// increment to next set of two bytes
	}

	send_data[15] = checkSum(send_data, send_data[0], TRUE);							// checksum for just data being sent directly to the hand
	send_data[16] = checkSum(send_data, send_data[0], FALSE);								// checksum for entire SPI message
}