#include "bmpMaker.h"

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

void fillBitMap(FILE* fInput, unsigned char** bitMap, unsigned picHeight, unsigned picWidth) {
	unsigned bitOffset = getValueFromBmp(fInput, OFFSET_HEX, 4);
	fseek(fInput, bitOffset, SEEK_SET);
	int scanLength = ((picWidth - 1) / 32 + 1) * 4;
	for (int i = picHeight - 1; i >= 0; --i) {
		int bitCount = 0;
		for (int j = 0; j < scanLength; ++j) {
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

void copyPartOfFile(FILE* fIn, FILE* fOut, unsigned start, unsigned length) {
	fseek(fIn, start, SEEK_SET);
	for (int i = 0; i < length; ++i) {
		fputc(fgetc(fIn), fOut);
	}
}

void fillBmpFile(FILE* exampleFile, FILE* resFile, char** bitMap, unsigned picWidth, unsigned picHeight) {
	copyPartOfFile(exampleFile, resFile, 0, getValueFromBmp(exampleFile, OFFSET_HEX, 4));
	for (int i = picHeight - 1; i >= 0; --i) {
		for (int j = 0; j < picWidth; j += 32) {
			unsigned char printedCh = 0, factor = 128;
			for (int frameCnt = 0; frameCnt <= 32; ++frameCnt) {
				if ((frameCnt % 8 == 0) && (frameCnt != 0)) {
					fputc(printedCh, resFile);
					factor = 128;
					printedCh = 0;
				}
				unsigned char bit;
				if (j + frameCnt >= picWidth) {
					bit = 0;
				}
				else {
					bit = bitMap[i][j + frameCnt];
				}
				printedCh += bit * factor;
				factor /= 2;
			}
		}
	}
}