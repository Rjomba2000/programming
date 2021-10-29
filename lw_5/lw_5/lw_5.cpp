#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "gifenc.h"

#define OFFSET_HEX 0x0A
#define WIDTH_HEX 0x12
#define HEIGHT_HEX 0x16

struct gameField {
	unsigned height;
	unsigned width;	
	char** bitMap;
} typedef gameField;

unsigned readBytes(FILE* fInput, unsigned bytesCount) {
	unsigned res = 0, factor = 1;
	for (int i = 0; i < bytesCount; ++i) {
		unsigned char ch;
		fscanf(fInput, "%c", &ch);
		res += ch * factor;
		factor *= 256;
	}
	return res;
}

unsigned getValueFromBmp(FILE* fInput, unsigned offset, unsigned bytesCount) {
	fseek(fInput, offset, SEEK_SET);
	return readBytes(fInput, bytesCount);
}

char** createBitMap(unsigned height, unsigned width) {
	char **bitMap = (char**)malloc(height * sizeof(char*));
	for (int i = 0; i < height; ++i) {
		bitMap[i] = (char*)malloc(width * sizeof(char));
	}
	return bitMap;
}

void deleteBitMap(unsigned char** bitMap, unsigned height) {
	for (int i = 0; i < height; ++i) {
		free(bitMap[i]);
	}
	free(bitMap);
}

void fillBitMap(FILE* fInput, unsigned char** bitMap, unsigned bitOffset, unsigned picHeiht, unsigned picWidth) {
	fseek(fInput, bitOffset, SEEK_SET);
	int scanLength = ((picWidth - 1) / 32 + 1) * 4;
	for (int i = picHeiht - 1; i >= 0; --i) {
		int bitCount = 0;
		for (int j = 0; j < scanLength; j++) {
			unsigned char ch = readBytes(fInput, 1);
			unsigned char divider = 128;
			for (int k = 0; k < 8; ++k, ++bitCount) {
				if (bitCount < picWidth) {
					bitMap[i][bitCount] = ch / divider;
					ch = ch % divider;
					divider /= 2;
				}
			}
		}
	}
}

char isCellExists(gameField field, int i, int j) {
	return (i >= 0) && (i < field.height) && (j >= 0) && (j < field.width);
}

unsigned neighboursNum(gameField field, int i, int j) {
	unsigned result = 0;
	for (int i2 = i - 1; i2 <= i + 1; ++i2) {
		for (int j2 = j - 1; j2 <= j + 1; ++j2) {
			if (((i2 != i) || (j2 != j)) && isCellExists(field, i2, j2) && (field.bitMap[i2][j2] > 0)) {
				result++;
			}
		}
	}
	return result;
}

void gameStep(gameField field) {
	for (int i = 0; i < field.height; ++i) {
		for (int j = 0; j < field.width; ++j) {
			unsigned neighboursCount = neighboursNum(field, i, j);
			if (field.bitMap[i][j] == 1) {
				if ((neighboursCount == 2) || (neighboursCount == 3)) {
					field.bitMap[i][j] = 2;
				}
			}
			else {
				if (neighboursCount == 3) {
					field.bitMap[i][j] = -1;
				}
			}
		}
	}

	for (int i = 0; i < field.height; ++i) {
		for (int j = 0; j < field.width; ++j) {
			if ((field.bitMap[i][j] == 2) || (field.bitMap[i][j] == -1)) {
				field.bitMap[i][j] = 1;
			}
			else {
				field.bitMap[i][j] = 0;
			}
		}
	}
}

void printField(gameField field) {
	for (int i = 0; i < field.height; ++i) {
		for (int j = 0; j < field.width; ++j) {
			if (field.bitMap[i][j]) {
				printf("#");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

int main() {
	FILE* bmpInputFile;

	bmpInputFile = fopen("field.bmp", "r");

	unsigned bitOffset = getValueFromBmp(bmpInputFile, OFFSET_HEX, 4);
	unsigned picWidth = getValueFromBmp(bmpInputFile, WIDTH_HEX, 4);
	unsigned picHeight = getValueFromBmp(bmpInputFile, HEIGHT_HEX, 4);

	gameField field = {picHeight, picWidth, createBitMap(picHeight, picWidth) };

	fillBitMap(bmpInputFile, field.bitMap, bitOffset, picHeight, picWidth);


	//COORD position = { 0,0 };
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ge_GIF *gif = ge_new_gif(
		"example.gif",
		picWidth, picHeight,
		(uint8_t[]) {
			0x00, 0x00, 0x00,
			0xFF, 0xFF, 0xFF,
		},
		1,
		-1,
		0
	);

	printField(field);

	//SetConsoleCursorPosition(hConsole, position);
	for (int k = 0; k < 1000; ++k) {
		//printField(field);
		int counter = 0;
		for (int i = 0; i < picHeight; ++i) {
			for (int j = 0; j < picWidth; ++j) {
				gif->frame[counter] = !field.bitMap[i][j];
				counter++;
			}
		}

		ge_add_frame(gif, 10);

		gameStep(field);
	}

	ge_close_gif(gif);
	deleteBitMap(field.bitMap, picHeight);
	fclose(bmpInputFile);
}
