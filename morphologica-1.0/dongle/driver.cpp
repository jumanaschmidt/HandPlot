/**
 * 
 * @file driver.cpp
 * 
 * @brief RS485/UART Computer Peripheral Driver
 * 
 * @author Jumana Schmidt
 * @author Jesse Cornman
 * @author Jonathan Mullen
 * 
 * @date 2022-07-29
 *
 */

#include "driver.h"

/*
* @brief helper function to find checksum
* @param handDataTX indicates to only calculate part of buffer that is hand data or the entire SPI message to the STM
*/
uint8_t checkSum(uint8_t* arr, uint8_t length, bool handDataTX) {
	int8_t sum = 0;
	if (handDataTX) {
		for (uint8_t b = 1; b < length - 2; b++) { 															// start at first index to ignore length of SPI message byte
			sum += (int8_t)arr[b];
		}
	}
	else {
		for (uint8_t b = 0; b < length - 1; b++) { 															// parse each byte except checksum (last byte)
			sum += (int8_t)arr[b];
		}
	}
	sum = 0x100 - sum;																						// subtract sum of other bytes from zero to get checksum
	return sum;
}

/* 
* @brief STM Initialize Command 
* 
* @attention In order to get a working transmission with a different baud rate on UART, the hand’s baud rate needs to be changed to that same rate. 
* To do so, the user must send a command to the memory index of 001 (see WORLD_API_CONFIG0 on Table 20 of section 4.6.2 on the API documentation). For
* example, if a rate of 921600 is desired, “Wn001:0x00110000” and “CR” after must both be sent in the app’s developer mode terminal. The first command 
* changes the baud rate, and the second calibrates the hand to prepare for a UART transmission with the new rate.
*
*/
void InitSTM(uint8_t* TX, uint16_t UARTbaudDivisor, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 6;
	rxdatasize = 5;
	readtxsize = 6;

	uint8_t byte1 = UARTbaudDivisor >> 8;
	uint8_t byte2 = UARTbaudDivisor & 0x00ff;

	TX[0] = { 0x06 };																													// length of message
	TX[1] = { 0x00 };																													// STM command
	TX[2] = { 0x01 };																													// command ID
	TX[3] = byte1;																														// UART baud rate divisor (little endian)
	TX[4] = byte2;																														// UART baud rate divisor
	TX[5] = checkSum(TX, TX[0], FALSE);																												// checksum
}

/* 
* @brief STM Uninitialize Command 
*/
void unInitSTM(uint8_t* TX, int &txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 5;																														// actually 4 but need to zero pad to receive 5
	rxdatasize = 5;
	readtxsize = 4;

	TX[0] = { 0x04 };																													// length of message
	TX[1] = { 0x00 };																													// STM command
	TX[2] = { 0x02 };																													// command ID
	TX[3] = { 0xFA };																													// checksum
}

/*
* @brief STM Uninitialize Command
*/
void Version(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize) {
	txdatasize = 5;																														// actually 4 but need to zero pad to receive 5
	rxdatasize = 5;
	readtxsize = 4;

	TX[0] = { 0x04 };																													// length of message
	TX[1] = { 0x00 };																													// STM command
	TX[2] = { 0x10 };																													// command ID
	TX[3] = { 0xEC };																													// checksum
}

/*
* @brief get version of library and chip
*/
static void showVersion(DWORD locationId)
{
	FT_STATUS            ftStatus;
	FT_HANDLE            ftHandle = (FT_HANDLE)NULL;
	FT4222_STATUS        ft4222Status;
	FT4222_Version       ft4222Version;

	ftStatus = FT_OpenEx((PVOID)(uintptr_t)locationId,
		FT_OPEN_BY_LOCATION,
		&ftHandle);
	if (ftStatus != FT_OK)
	{
		printf("FT_OpenEx failed (error %d)\n",
			(int)ftStatus);
		return;
	}

	ft4222Status = FT4222_GetVersion(ftHandle,
		&ft4222Version);
	if (FT4222_OK != ft4222Status)
	{
		printf("FT4222_GetVersion failed (error %d)\n",
			(int)ft4222Status);
	}
	else
	{
		printf("  Chip version: %08X, LibFT4222 version: %08X\n",
			(unsigned int)ft4222Version.chipVersion,
			(unsigned int)ft4222Version.dllVersion);
	}

	(void)FT_Close(ftHandle);
}

/*
* @brief sets up SPI device
*/
int setupDevice(DWORD* locationID)
{
	printf("Searching for Devices...   ");
	FT_STATUS ftStatus;
	DWORD numDevs = 0;

	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus != FT_OK)
	{
		printf("FT_CreateDeviceInfoList failed (error code %d)\n",
			(int)ftStatus);
		return -10;
	}

	if (numDevs == 0)
	{
		printf("No devices found\n");
		return -20;
	}

	printf("Found %d device(s)\n", numDevs);
	FT_DEVICE_LIST_INFO_NODE* devInfo = NULL;
	memset(&devInfo, 0, sizeof(devInfo));
	if (devInfo == NULL)
	{
		printf("Memory Allocation Failed");
		free(devInfo);
		return -30;
	}

	ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
	if (ftStatus != FT_OK)
	{
		printf("FT_GetDeviceInfoList failed (error code %d)\n",
			(int)ftStatus);
		free(devInfo);
		return -40;
	}

	int modeThreeCount = 0;
	int i;

	for (i = 0; i < (int)numDevs; i++)
	{
		if (devInfo[i].Type == FT_DEVICE_4222H_3)
		{
			/* In mode 3, the FT4222H presents a single interface */
			printf("Device %d: '%s' Mode 3\n",
				i,
				devInfo[i].Description);
			*locationID = devInfo[i].LocId;
			printf("loc id is %d \n", devInfo[i].LocId);
			modeThreeCount++;
			break;
		}
	}

	if (modeThreeCount == 0)
	{
		printf("No mode FT4222 Found");
		free(devInfo);
		return -50;
	}

	showVersion(*locationID);

	free(devInfo);
	return 0;
}

/*
* @brief sets TX & RX buffer and sends them on SPI bus to STM
* 
* @attention select preferred method of viewing data by uncommenting or commenting ReadData...() functions (see read.cpp for more information)
* @attention print statements can cause delay in transmission (generally not noticeable if at higher baud rate)
* 
*/
int SPI_Handler(DWORD locationId)
{
	/* SPI Initialization */
	FT_HANDLE ftHandle = NULL;
	FT_STATUS ftStatus;
	FT4222_STATUS ft4222Status;

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK)
		return -100;

	ft4222Status = FT4222_SPIMaster_Init(ftHandle, SPI_IO_SINGLE, CLK_DIV_512, CLK_IDLE_LOW, CLK_LEADING, 0x01);
	if (ft4222Status != FT4222_OK)
		return -200;

	printf("SPI INIT SUCCESS\n");

	int txsize = 0;
	int rxsize = 0;
	int txactualsize = 0;																													// actual size of overwritten data (for reading purposes since data is zeropadded)
	uint16 sizeTransferred;

	uint8 sendData[DATASIZE] = { 0 };																										// TX buffer
	uint8 readData[DATASIZE] = { 0 };																										// RX buffer

	uint16 touchData[TOUCHLENGTH] = { 0 };
	float degrees[6] = { 0 };

	uint16_t UARTdivisor = 0x0008;																											// divisor that sets baud rate


	printf("\nAttempting SPI SetUp...\n \n");
	printf("Type Number Corresponding to Desired Command: \n \n");
	printf(" ********************** \n");
	printf(" ** 0 ~ Init STM     ** \n");
	printf(" ** 1 ~ UnInit STM   ** \n");
	printf(" ** 2 ~ Get Version  ** \n");
	printf(" ********************** \n \n");


	int input;																																// keyboard input to select what to send over SPI
	std::cin >> input;
	
	while(1)
	{
		/* Commands that Set TX */
		switch (input) {
		case 0:
			InitSTM(sendData, UARTdivisor, txsize, rxsize, txactualsize);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			ReadRawTXData(sendData, txactualsize);
			ReadRawRXData(readData, rxsize);
			break;
		case 1:
			unInitSTM(sendData, txsize, rxsize, txactualsize);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			ReadRawTXData(sendData, txactualsize);
			ReadRawRXData(readData, rxsize);
			break;
		case 2:
			Version(sendData, txsize, rxsize, txactualsize);
			ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);

			ReadRawTXData(sendData, txactualsize);
			ReadRawRXData(readData, rxsize);
			break;
		
		default:
			printf("Invalid Command");
			return 0;
		}
	}

	FT4222_UnInitialize(ftHandle);
	FT_Close(ftHandle);

	return 0;

}