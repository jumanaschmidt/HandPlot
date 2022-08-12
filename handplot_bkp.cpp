/*
 * Visualize a graph on which points are added with time.
 */
#include <morph/Visual.h>
#include <morph/ColourMap.h>
#include <morph/GraphVisual.h>
#include <morph/Scale.h>
#include <morph/Vector.h>
#include <morph/vVector.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>

#include "ftd2xx.h"
#include "LibFT4222.h"
#include <chrono>
//#include "dongle/driver.h"
//#include "dongle/rundemos.h"

#define DATASIZE 128

typedef union {
	uint8_t bytes[24];
	int16_t vals[12];
}motordata_t;

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
void FingerWave(float* deg, int& txdatasize, int& rxdatasize, int& readtxsize, double time) {
	txdatasize = 75;																						// sending 17 real populated bytes but zeropadding so we can receive 72 + length & format
	rxdatasize = 75;
	readtxsize = 17;
	
	double ft = time * 6;
	for (int ch = 0; ch < 6; ch++)										// parse through each finger channel & set new associated degree value
	{
		deg[ch] = (0.5 * sin((ft + (float)ch) + 0.5) * 45 + 15);				// sinusoidal function on all fingers
	}
	deg[5] = -deg[5];					// negate thumb rotator
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

int main (int argc, char** argv)
{
	DWORD locationID = 0;
	int ret = setupDevice(&locationID);
	printf("\n \n Attempting SPI SetUp...\n \n");

	FT_HANDLE ftHandle = NULL;
	FT_STATUS ftStatus;
	FT4222_STATUS ft4222Status;

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK)
		return -100;

	ft4222Status = FT4222_SPIMaster_Init(ftHandle, SPI_IO_SINGLE, CLK_DIV_256, CLK_IDLE_LOW, CLK_LEADING, 0x01);
	if (ft4222Status != FT4222_OK)
		return -200;

	printf("SPI INIT SUCCESS\n");

	int txsize = 0;
	int rxsize = 0;
	int txactualsize = 0;
	uint16 sizeTransferred;
	uint8 sendData[DATASIZE] = { 0 };		
	uint8 readData[DATASIZE] = { 0 };
	float degrees[6] = { 0 };


	int rtn = -1;

	morph::Visual v(1024, 768, "Graph", {-0.8,-0.8}, {.1,.1,.1}, 2.0f, 0.01f);
	v.zNear = 0.001;
	v.showCoordArrows = true;
	v.backgroundWhite();
	v.lightingEffects();

	try {
	 morph::vVector<float> time(1,0);
        morph::vVector<float> position(1,0);
	morph::GraphVisual<float>* gv = new morph::GraphVisual<float> (v.shaderprog, v.tshaderprog, {0,0,0});

	gv->setsize (1.33, 1);			// Optionally change the physical size of the graph and range of the axes
	// Optionally change the range of the axes
	//gv->setlimits (-1, 1, -1, 1);

	// Set the graphing policy
	//gv->policy = morph::stylepolicy::lines; // markers, lines, both, allcolour
	//gv->axisstyle = morph::axisstyle::twinax;
	// We 'prepare' two datasets, but won't fill them with data yet. However, we do give the data legend label here.
	//gv->prepdata ("Third power", morph::axisside::left);
	gv->xlabel = "Time";
	gv->ylabel = "Position";
	//gv->setdata(time, position, "HandPlot", morph::axisside::left);
	gv->prepdata ("Plot", morph::axisside::left);
	//gv->prepdata ("Position", morph::axisside::right);
	gv->finalize();

	// Add the GraphVisual (as a VisualModel*)
	v.addVisualModel (gv);

	size_t idx = 0;
	v.render();
	auto start = std::chrono::high_resolution_clock::now();
	while (v.readyToFinish == false) {
	    glfwWaitEventsTimeout (0.018);
	    auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> t = finish - start;
		FingerWave(degrees, txsize, rxsize, txactualsize, t.count());
		LoadTXDegrees(degrees, sendData);
		ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], txsize, &sizeTransferred, 1);
		motordata_t posdata;
		for (int i = 0; i < 24; i++) {
			posdata.bytes[i] = readData[i + 3];
		}
		for (int ch = 0; ch < 6; ch++) {
			degrees[ch] = ((float)posdata.vals[ch * 2]) * 150.f / 32767.f;
		}
		
		float x = t.count();
		float y = degrees[3];
		time[idx] = t.count();
		position[idx] = degrees[3];
		//printf("idx: %d \n", idx);
		//printf("sec: %f \n", time[idx]);
		//printf("pos: %f \n", position[idx]);
		gv->append (x, y, 0);
		//gv->completeAppend();
		//printf("%f", gv->graphDataCoords.size());
		gv->update(time, position, idx);
		
		//glfwPollEvents();
                v.render();
                ++idx;
	}

	} catch (const std::exception& e) {
	std::cerr << "Caught exception: " << e.what() << std::endl;
	rtn = -1;
	}

	return rtn;
}
