#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <errno.h>
#include "bmpMaker.h"
#include "gameOfLife.h"
#include "argParser.h"

void finishWithErrorMessage(char message[]) {
	printf("%s", message);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		finishWithErrorMessage("Wrong number of arguments");
	}

	char argName[255] = "", argValue[255] = "";
	parseArg("input", argv[1], argName, argValue);
	FILE* bmpInputFile;
	if (!(bmpInputFile = fopen(argValue, "rb"))) {
		finishWithErrorMessage("Can't open the file");
	}

	char filePath[255];
	parseArg("output", argv[2], argName, filePath);
	if (_mkdir(filePath) == -1) {
		if (errno == ENOENT) {
			finishWithErrorMessage("Can't create a directory along that path");
		}
	}

	unsigned picWidth = getValueFromBmp(bmpInputFile, WIDTH_HEX, 4);
	unsigned picHeight = getValueFromBmp(bmpInputFile, HEIGHT_HEX, 4);
	gameField field = { picHeight, picWidth, createBitMap(picHeight, picWidth) };
	fillBitMap(bmpInputFile, field.bitMap, picHeight, picWidth);

	int genNumber = -1, dumpFreq = 1, genNumFlag = 1, dumpFreqFlag = 1;
	for (int i = 3; i < argc; ++i) {
		parseArg("max_iterdump_freq", argv[i], argName, argValue);
		if (!strcmp(argName, "max_iter")) {
			if (!(genNumber = atoi(argValue)) && genNumFlag) {
				finishWithErrorMessage("Wrong format of max_iter");
			}
			genNumFlag = 0;
		}
		else if (dumpFreqFlag) {
			if (!(dumpFreq = atoi(argValue))) {
				finishWithErrorMessage("Wrong format of dump_freq");
			}
			dumpFreqFlag = 0;
		}
		else {
			finishWithErrorMessage("Repeating argument");
		}
	}

	for (int i = 0; i < genNumber; ++i) {
		Sleep(100 / dumpFreq);
		char newFileName[255];
		sprintf(newFileName, "%s/%d.bmp", filePath, i);
		FILE* resFile = fopen(newFileName, "wb");
		fillBmpFile(bmpInputFile, resFile, field.bitMap, field.width, field.height);
		gameStep(field);
		fclose(resFile);
	}

	deleteBitMap(field.bitMap, picHeight);
	fclose(bmpInputFile);
}
