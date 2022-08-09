#include "driver.h"

int main(void)
{
	DWORD locationID = 0;
	int ret = setupDevice(&locationID);

	printf("\n \n Attempting SPI SetUp...\n \n");

	//SPI_Handler(locationID);
	SPI_Demos_Handler(locationID);

	printf("\nDone\n");


	return ret;
}