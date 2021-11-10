#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gifCreator.h"

int findInDict(char dictionary[][512], unsigned last, char* looking) {
	for (int i = 0; i < last; ++i) {
		if (!strcmp(looking, dictionary[i])) {
			return i;
		}
	}
	return -1;
}

char* makeStream(char** bitMap, unsigned width, unsigned height) {
	char* res = (char*)malloc(width * height * sizeof(char));
	int counter = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j, ++counter) {
			res[counter] = bitMap[i][j];
		}
	}
	return res;
}

void fillBuffer(char buffer[], unsigned value, char size) {
	unsigned divider = 1;
	for (int i = 0; i < size - 1; ++i) {
		divider *= 2;
	}
	for (int i = 0; i < size; ++i) {
		buffer[i] = value / divider;
		value = value % divider;
		divider = divider / 2;
	}
}

void addBuffer(char bitArr[], char buffer[], long index, char size) {
	for (int i = 0; i < size; ++i) {
		bitArr[index - size + i] = buffer[i];
	}
}

unsigned power(unsigned value, unsigned degree) {
	unsigned res = 1;
	for (int i = 0; i < degree; ++i) {
		res *= value;
	}
	return res;
}

void printBytes(FILE* fOut, unsigned value, char size) {
	for (int i = 0; i < size; ++i) {
		fprintf(fOut, "%c", value % 0xFF);
		value /= 0xFF;
	}
}

void addFrame(FILE* gifFile, char** bitMap, unsigned width, unsigned height) {
	fprintf(gifFile, "%c%c%c%c%c%c%c%c", 0x21, 0xF9, 0x04, 0x05, 0x0A, 0x00, 0xFF, 0x00);
	fprintf(gifFile, "%c%c%c%c%c", 0x2C, 0x00, 0x00, 0x00, 0x00);
	printBytes(gifFile, width, 2);
	printBytes(gifFile, height, 2);
	fprintf(gifFile, "%c%c", 0x00, 0x00);
	
	char* stream = makeStream(bitMap, width, height);

	char dictionary[1024][512];

	strcpy(dictionary[0], "0");
	strcpy(dictionary[1], "1");
	unsigned codeArr[4096];
	codeArr[0] = 4;
	unsigned codeInd = 1;
	unsigned index = 6;
	unsigned memInd;
	if (!stream[0]) {
		strcpy(dictionary[index], "0");
		memInd = 0;
	}
	else {
		strcpy(dictionary[index], "1");
		memInd = 1;
	}

	for (int i = 1; i < height * width; ++i) {
		if (!stream[i]) {
			strcat(dictionary[index], "0");
		}
		else {
			strcat(dictionary[index], "1");
		}
		unsigned strPos = findInDict(dictionary, index, dictionary[index]);
		if (strPos != -1) {
			memInd = strPos;
		}
		else {
			codeArr[codeInd] = memInd;
			codeInd++;
			index++;
			if (!stream[i]) {
				strcpy(dictionary[index], "0");
				memInd = 0;
			}
			else {
				strcpy(dictionary[index], "1");
				memInd = 1;
			}
		}
	}
	unsigned strPos = findInDict(dictionary, index, dictionary[index]);
	if (strPos != -1) {
		codeArr[codeInd] = strPos;
	}
	else {
		codeArr[codeInd] = memInd;
	}
	codeArr[codeInd + 1] = 5;

	unsigned long bitCount = 0;
	char bitArr[100000] = { 1 };
	long ind = 100000;
	char size = 3;
	unsigned temp = 0;
	for (int i = 0; i < codeInd + 2; ++i) {
		if (i == temp + power(2, size - 1)) {
			temp += power(2, size - 1);
			size++;
		}
		char buffer[12];
		fillBuffer(buffer, codeArr[i], size);
		bitCount += size;
		addBuffer(bitArr, buffer, ind, size);
		ind -= size;
	}

	unsigned long byteCount = bitCount / 8 + 1;
	if (byteCount >= 0xFE) {
		fprintf(gifFile, "%c", 0xFF);
	}
	else {
		fprintf(gifFile, "%c", byteCount);
	}
	unsigned cnt = 0;
	for (long i = 100000; i >= 100000 - bitCount; i -= 8) {
		if (cnt == 0xFD) {
			if (byteCount >= 0xFE) {
				fprintf(gifFile, "%c", 0xFE);
			}
			else {
				fprintf(gifFile, "%c", byteCount);
			};
			byteCount -= 0xFE;
			cnt = 0;
		}
		unsigned num = 0, factor = 128;
		for (long j = i - 8; j < i; ++j) {
			num += bitArr[j] * factor;
			factor /= 2;
		}
		cnt++;
		fprintf(gifFile, "%c", num);
	}
	free(stream);
	fprintf(gifFile, "%c", 0x00);
}

void createGif(FILE* gifFile, unsigned width, unsigned height) {
	fprintf(gifFile, "%c%c%c%c%c%c", 0x47, 0x49, 0x46, 0x38, 0x39, 0x61);
	printBytes(gifFile, width, 2);
	printBytes(gifFile, height, 2);
	fprintf(gifFile, "%c%c%c%c%c%c%c%c%c", 0x80, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00);
	fprintf(gifFile, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x21, 0xFF, 0x0B, 0x4E, 0x45, 0x54, 0x53, 0x43, 0x41, 0x50, 0x45, 0x32, 0x2E, 0x30, 0x03, 0x01, 0x00, 0x00, 0x00);
}

void finishGifCreation(FILE* gifFile) {
	fprintf(gifFile, "%c", 0x3B);
}