/*
*
* @file Run_Demos.cpp.cpp
*
* @author	Jumana Schmidt
* @date		2022-07-29
*
* @brief This file runs all the demos, including Finger Wave, Cursor Demo, and Voltage Wave.
* To run any demo, go to main.cpp and uncomment SPI_Demos_Handler(locationID); and comment
* out SPI_Handler(locationID);. Then run the program and select the number corresponding to
* the desired demo.
*
* @attention Windows is required to run the demo code unless the user changes how the delay is
* proccessed within the while loop to their respective OS
*
*/


#include "rundemos.h"

/*
* @brief Simple UART Hand Command Example (OPEN)
*/
void OpenHand(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 75;																													// sending 17 real populated bytes but zeropadding so we can receive 72
	rxdatasize = 75;
	readtxsize = 17;

	TX[0] = { 0x11 };																													// length of message (17 bits)
	TX[1] = { 0x50 };																													// hand address
	TX[2] = { 0x10 };																													// position data
	/* Index */
	TX[3] = { 0x0E };
	TX[4] = { 0x82 };
	/* Middle */
	TX[5] = { 0x0E };
	TX[6] = { 0x82 };
	/* Ring */
	TX[7] = { 0x0E };
	TX[8] = { 0x82 };
	/* Pinky */
	TX[9] = { 0x0E };
	TX[10] = { 0x82 };
	/* Thumb Flexor */
	TX[11] = { 0x0E };
	TX[12] = { 0x82 };
	/* Thumb Rotator */
	TX[13] = { 0xF1 };
	TX[14] = { 0x7E };

	TX[15] = checkSum(TX, TX[0], TRUE);																				// checksum for just hand data (indices 1-14)
	TX[16] = checkSum(TX, TX[0], FALSE);																					// checksum for whole message (indices 0-16)
}

/*
* @brief Simple UART Hand Command Example (CLOSE)
*/
void CloseHand(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 75;																														// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;

	TX[0] = { 0x11 };																														// length of message (17 bits)
	TX[1] = { 0x50 };																														// hand address
	TX[2] = { 0x10 };																														// position data
	/* Index */
	TX[3] = { 0x04 };
	TX[4] = { 0x44 };
	/* Middle */
	TX[5] = { 0x04 };
	TX[6] = { 0x44 };
	/* Ring */
	TX[7] = { 0x04 };
	TX[8] = { 0x44 };
	/* Pinky */
	TX[9] = { 0x04 };
	TX[10] = { 0x44 };
	/* Thumb Flexor */
	TX[11] = { 0x04 };
	TX[12] = { 0x44 };
	/* Thumb Rotator */
	TX[13] = { 0xFB };
	TX[14] = { 0xBC };

	TX[15] = checkSum(TX, TX[0], TRUE);																				// checksum for just hand data (indices 1-14)
	TX[16] = checkSum(TX, TX[0], FALSE);																					// checksum for whole message (indices 0-16)
}


/*
* @brief sets TX & RX buffer and sends them on SPI bus to STM
*
* @attention select preferred method of viewing data by uncommenting or commenting ReadData...() functions (see read.cpp for more information)
* @attention print statements can cause delay in transmission
* @attention if the hand is not moving, try changing the clock division in FT4222_SPIMaster_Init()
*
*/
int SPI_Demos_Handler(DWORD locationId) {

	FT_HANDLE ftHandle = NULL;
	FT_STATUS ftStatus;
	FT4222_STATUS ft4222Status;

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK)
		return -100;

	ft4222Status = FT4222_SPIMaster_Init(ftHandle, SPI_IO_SINGLE, CLK_NONE, CLK_IDLE_LOW, CLK_LEADING, 0x01);								// CLK_DIV determines SPI frequency
	if (ft4222Status != FT4222_OK)
		return -200;

	printf("SPI INIT SUCCESS\n");

	printf("Enter the Number Corresponding to the Desired Command: \n \n");
	printf(" ********************** \n");
	printf(" ** 0 ~ Open Hand    ** \n");
	printf(" ** 1 ~ Close Hand   ** \n");
	printf(" ** 2 ~ Finger Wave  ** \n");
	printf(" ** 3 ~ Cursor Demo  ** \n");
	printf(" ** 4 ~ Voltage Wave ** \n");
	printf(" ********************** \n \n");

	int txsize = 0;
	int rxsize = 0;
	int txactualsize = 0;																													// actual size of overwritten data (for reading purposes since data is zeropadded)
	uint16 sizeTransferred;

	uint8 sendData[DATASIZE] = { 0 };																										// TX buffer
	uint8 readData[DATASIZE] = { 0 };																										// RX buffer

	uint16 touchData[TOUCHLENGTH] = { 0 };
	float degrees[6] = { 0 };

	uint16_t UARTdivisor = 0x0008;																											// used to change UART baud rate

	int input;																																// kyeboard input to select what to send over SPI
	std::cin >> input;
	while (1)
	{
		uint64_t startTime = ns();																											// get nanosecond count for delay

		/* Commands that Set TX */
		switch (input) {
		case 0:
			OpenHand(sendData, txsize, rxsize, txactualsize);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			//ReadRawTXData(sendData, txactualsize);
			//ReadRawRXData(readData, rxsize);
			//ReadHandDataOneLine(readData, touchData, TOUCHLENGTH);
			ReadHandData(readData, touchData, TOUCHLENGTH);
			//ReadHandPositionData(readData);
			//ReadHandVelocityData(readData);
			//ReadHandTouchData(readData, touchData, TOUCHLENGTH);
			break;
		case 1:
			CloseHand(sendData, txsize, rxsize, txactualsize);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			//ReadRawTXData(sendData, txactualsize);
			//ReadRawRXData(readData, rxsize);
			//ReadHandDataOneLine(readData, touchData, TOUCHLENGTH);
			ReadHandData(readData, touchData, TOUCHLENGTH);
			//ReadHandPositionData(readData);
			//ReadHandVelocityData(readData);
			//ReadHandTouchData(readData, touchData, TOUCHLENGTH);
			break;
		case 2:
			FingerWave(degrees, txsize, rxsize, txactualsize);
			LoadTXDegrees(degrees, sendData);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			//ReadRawTXData(sendData, txactualsize);
			//ReadRawRXData(readData, rxsize);
			//ReadHandDataOneLine(readData, touchData, TOUCHLENGTH);
			ReadHandData(readData, touchData, TOUCHLENGTH);
			//ReadHandPositionData(readData);
			//ReadHandVelocityData(readData);
			//ReadHandTouchData(readData, touchData, TOUCHLENGTH);
			break;
		case 3:
			CursorDemo(degrees, txsize, rxsize, txactualsize);
			LoadTXDegrees(degrees, sendData);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			//ReadRawTXData(sendData, txactualsize);
			//ReadRawRXData(readData, rxsize);
			//ReadHandDataOneLine(readData, touchData, TOUCHLENGTH);
			ReadHandData(readData, touchData, TOUCHLENGTH);
			//ReadHandPositionData(readData);
			//ReadHandVelocityData(readData);
			//ReadHandTouchData(readData, touchData, TOUCHLENGTH);
			break;
		case 4:
			float voltage[6];
			float measuredDegrees[6];
			float measuredVelocity[6];

			FingerWave(degrees, txsize, rxsize, txactualsize);																			// can use either fingerwave or cursor demo as input/desired theta into PID
			//CursorDemo(degrees);

			if (isRXCheckSumCorrect(readData, readData[0]))
			{
				FormatRXData(measuredDegrees, measuredVelocity, txsize, rxsize, txactualsize, readData);								// gets measured data from RX data from hand
				VoltageControlDemo(voltage, degrees, measuredDegrees, measuredVelocity);												// PID equation
				LoadTXVoltage(voltage, sendData);
			}
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			//ReadRawTXData(sendData, txactualsize);
			//ReadRawRXData(readData, rxsize);
			//ReadHandDataOneLine(readData, touchData, TOUCHLENGTH);
			ReadHandData(readData, touchData, TOUCHLENGTH);
			//ReadHandPositionData(readData);
			//ReadHandVelocityData(readData);
			//ReadHandTouchData(readData, touchData, TOUCHLENGTH);
			break;
		default:
			printf("Invalid Command");
			return 0;
		}

		while (ns() <= startTime + 15000) {}																																// delay
	}

	FT4222_UnInitialize(ftHandle);
	FT_Close(ftHandle);

	return 0;
}
