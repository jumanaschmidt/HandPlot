#include <vector>
#include <morph/Visual.h>
#include <morph/GraphVisual.h>
#include "ftd2xx.h"
#include "LibFT4222.h"
//#include "dongle/driver.h"
//#include "dongle/rundemos.h"

#define DATASIZE 128

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
* @brief sinusoidal function and data size allocation 
*/
void FingerWave(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize, int t) {
	txdatasize = 75;																						// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;
	
	float ft = t * 5;																			// speed value control how fast the fingers wave

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


int SPI_Demos_Handler(DWORD locationId) {

	FT_HANDLE ftHandle = NULL;
	FT_STATUS ftStatus;
	FT4222_STATUS ft4222Status;

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK)
		return -100;

	ft4222Status = FT4222_SPIMaster_Init(ftHandle, SPI_IO_SINGLE, CLK_DIV_64, CLK_IDLE_LOW, CLK_LEADING, 0x01);								// CLK_DIV determines SPI frequency
	if (ft4222Status != FT4222_OK)
		return -200;

	printf("SPI INIT SUCCESS\n");


	int txsize = 0;
	int rxsize = 0;
	int txactualsize = 0;																													// actual size of overwritten data (for reading purposes since data is zeropadded)
	uint16 sizeTransferred;

	uint8 sendData[DATASIZE] = { 0 };																										// TX buffer
	uint8 readData[DATASIZE] = { 0 };																										// RX buffer

	float degrees[6] = { 0 };
	float time = 0;
	while (1)
	{
		FingerWave(degrees, txsize, rxsize, txactualsize, time);
		LoadTXDegrees(degrees, sendData);
		ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);
		time += 0.1;
	}

	FT4222_UnInitialize(ftHandle);
	FT_Close(ftHandle);

	return 0;
}


int main() 
{

DWORD locationID = 0;
int ret = setupDevice(&locationID);
printf("\n \n Attempting SPI SetUp...\n \n");

//SPI_Handler(locationID);
printf("return val %d", SPI_Demos_Handler(locationID));

printf("\nDone\n");


morph::Visual v(1024, 768, "Made with morph::GraphVisual");

// x holds data for the x axis
std::vector<double> x(14, 0.0);
// y holds data for the y axis.
std::vector<double> y(14, 0.0);
// Populate x and y (we'll raise x to the power 3)
double val = -0.5;
for (unsigned int i = 0; i < 14; ++i) {
    x[i] = val;
    y[i] = val * val * val;
    val += 0.1;
}

// GraphVisual with offset within the scene of 0,0,0
auto gv = new morph::GraphVisual<double> (v.shaderprog, v.tshaderprog, {0,0,0});
// The setdata function passes our data into the GraphVisual object
gv->setdata (x, y);
// finalize() makes GraphVisual compute the vertices of the OpenGL model
gv->finalize();

// Add the GraphVisual OpenGL model to the Visual scene
v.addVisualModel (gv);
// Render the scene on the screen until user quits with 'x'
v.keepOpen();
// Note: When v goes out of scope, gv will be deallocated (so no 'delete')

return ret;
}
