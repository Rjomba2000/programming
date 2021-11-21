#include "gameOfLife.h"

char isCellExists(gameField field, int i, int j) {
	return (i >= 0) && (i < field.height) && (j >= 0) && (j < field.width);
}

unsigned neighboursNum(gameField field, int i, int j) {
	unsigned result = 0;
	for (int i2 = i - 1; i2 <= i + 1; ++i2) {
		for (int j2 = j - 1; j2 <= j + 1; ++j2) {
			if (((i2 != i) || (j2 != j)) && isCellExists(field, i2, j2) && (field.bitMap[i2][j2] <= 0)) {
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
			if (!field.bitMap[i][j]) {
				if ((neighboursCount == 2) || (neighboursCount == 3)) {
					field.bitMap[i][j] = -1;
				}
			}
			else {
				if (neighboursCount == 3) {
					field.bitMap[i][j] = 2;
				}
			}
		}
	}
	for (int i = 0; i < field.height; ++i) {
		for (int j = 0; j < field.width; ++j) {
			if ((field.bitMap[i][j] == 2) || (field.bitMap[i][j] == -1)) {
				field.bitMap[i][j] = 0;
			}
			else {
				field.bitMap[i][j] = 1;
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