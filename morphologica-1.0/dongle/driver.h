#ifndef DRIVER_H
#define DRIVER_H

/* FTDI libraries */
#include "ftd2xx.h"
#include "LibFT4222.h"

/* Program Libraries */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

/* Project Files */
#include "helpers.h"
#include "read.h"
#include "rundemos.h"

#define DATASIZE		128
#define TOUCHLENGTH		30																					// length of touch data (from API Reply Variant 1)

uint8_t checkSum(uint8_t* arr, uint8_t length, bool handDataTX);

void InitSTM(uint8_t* TX, uint16_t UARTbaudDivisor, int& txdatasize, int& rxdatasize, int& readtxsize);
void unInitSTM(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize);
void Version(uint8_t* TX, int& txdatasize, int& rxdatasize, int& readtxsize);

static void showVersion(DWORD locationId);
int setupDevice(DWORD* locationID);
int SPI_Handler(DWORD locationId);

#endif