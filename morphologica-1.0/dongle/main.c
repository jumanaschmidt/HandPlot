//#include <windows.h>
#include "ftd2xx.h"
#include "libft4222.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>



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

    // Get version of library and chip.    
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

int setupDevice(DWORD *locationID)
{
	printf("Searching for Devices...   ");
	FT_STATUS ftStatus;
	DWORD numDevs = 0;

	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if(ftStatus != FT_OK)
	{
		printf("FT_CreateDeviceInfoList failed (error code %d)\n", 
               (int)ftStatus);
        return -10;
	}

	if(numDevs == 0)
	{
		printf("No devices found\n");
		return -20;
	}

	printf("Found %d device(s)\n", numDevs);
	FT_DEVICE_LIST_INFO_NODE *devInfo = NULL;
	devInfo = calloc((size_t) numDevs, sizeof(FT_DEVICE_LIST_INFO_NODE));
	if(devInfo == NULL)
	{
		printf("Memory Allocation Failed");
		free(devInfo);
		return -30;
	}

	ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
	if(ftStatus != FT_OK)
	{
		printf("FT_GetDeviceInfoList failed (error code %d)\n",
               (int)ftStatus);
		free(devInfo);
        return -40;
	}

	int modeThreeCount = 0;
	int i;
	//DWORD locationID;
	for (i = 0; i < (int)numDevs; i++)
	{
		if(devInfo[i].Type == FT_DEVICE_4222H_3)
		{
			// In mode 3, the FT4222H presents a single interface.  
            printf("Device %d: '%s' Mode 3\n",
                   i,
                   devInfo[i].Description);
            *locationID = devInfo[i].LocId; 
            printf("loc id is %d \n", devInfo[i].LocId);
            modeThreeCount++;          
           	break;
		}
	}

	if(modeThreeCount == 0)
	{
		printf("No mode FT4222 Found");
		free(devInfo);
		return -50;
	}

	showVersion(*locationID);	




	free(devInfo);
	return 0;
}


int spiToggleLed(DWORD locationId)
{
	FT_HANDLE ftHandle = NULL;
	FT_STATUS ftStatus;
	FT4222_STATUS ft4222Status;

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK)
		return -100;

	ft4222Status = FT4222_SPIMaster_Init(ftHandle, SPI_IO_SINGLE, CLK_DIV_512, CLK_IDLE_LOW,
CLK_LEADING, 0x01);
	if(ft4222Status != FT4222_OK)
		return -200;
	
	printf("SPI INIT SUCCESS\n");

	uint8 sendData[4] = {0x0};
	uint8 readData[4] = {0x0};
	uint16 sizeTransferred;


	uint8 toggle = 0;
	for(int j = 0; j < 10; j++)
	{
		printf("Toggle LED\n");
		ft4222Status = FT4222_SPIMaster_SingleReadWrite(ftHandle, &readData[0], &sendData[0], 4, &sizeTransferred, 1);

		if((ft4222Status != FT4222_OK) || (sizeTransferred != 4))
		{
			printf("Transfer Failed  %d   -  %d\n", ft4222Status, sizeTransferred);
		}
		else
		{
			printf("Received: 0x%02x %02x %02x %02x \n", readData[0], readData[1], readData[2], readData[3]);
		}
		toggle = (toggle == 0) ? 1 : 0;
		uint8_t newVal = 0;
		if(toggle == 1)
			newVal = 0x3A;

		for(int i = 0; i < 4; i++)
			sendData[i] = newVal;


		sleep(1);
	}

	FT4222_UnInitialize(ftHandle);
	FT_Close(ftHandle);

	return 0;

}

int main(void)
{
	printf("Hello World\n");
	DWORD locationID = 0;
	int ret = setupDevice(&locationID);
	printf("Attempting SPI SetUp\n");
	spiToggleLed(locationID);
	
	printf("Done\n");


	return ret;
}