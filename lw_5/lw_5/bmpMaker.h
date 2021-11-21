#pragma once
#include <stdio.h>

#define OFFSET_HEX 0x0A
#define WIDTH_HEX 0x12
#define HEIGHT_HEX 0x16

unsigned getValueFromBmp(FILE* fInput, unsigned offset, unsigned bytesCount);
char** createBitMap(unsigned height, unsigned width);
void deleteBitMap(unsigned char** bitMap, unsigned height);
void fillBitMap(FILE* fInput, unsigned char** bitMap, unsigned picHeight, unsigned picWidth);
void fillBmpFile(FILE* exampleFile, FILE* resFile, char** bitMap, unsigned picWidth, unsigned picHeight);
