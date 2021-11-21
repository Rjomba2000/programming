#pragma once
#include <stdio.h>

struct gameField {
	unsigned height;
	unsigned width;
	char** bitMap;
} typedef gameField;

void gameStep(gameField field);
void printField(gameField field);