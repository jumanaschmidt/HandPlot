#include <vector>
#include <morph/Visual.h>
#include <morph/GraphVisual.h>
#include "ftd2xx.h"
#include "LibFT4222.h"
//#include "dongle/driver.h"
//#include "dongle/rundemos.h"




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



int main() 
{

uint32_t locationID = 0;
int ret = setupDevice(&locationID);
printf("\n \n Attempting SPI SetUp...\n \n");

//SPI_Handler(locationID);
//SPI_Demos_Handler(locationID);

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

return 0;
}
