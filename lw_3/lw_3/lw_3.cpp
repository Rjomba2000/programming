#include "time.h"
#include <stdio.h>
#include <string.h>

short toInt(char ch) {
	return ch - 48;
}

int findFirstCharOccurrence(char str[], char looking) {
	int i = 0;
	while ((str[i] != looking) && (str[i] != '\0')) {
		i++;
	}
	if (str[i] != '\0') {
		return i;
	}
	else {
		return -1;
	}
}

time extractTimeFromLog(char log[]) {
	time result;
	short i = findFirstCharOccurrence(log, '[');
	result.day = toInt(log[i + 1]) * 10 + toInt(log[i + 2]);
	char month[4] = {log[i + 4], log[i + 5], log [i + 6]};
	result.month = encodeMonth(month);
	result.year = toInt(log[i + 8]) * 1000 + toInt(log[i + 9]) * 100 + toInt(log[i + 10]) * 10 + toInt(log[i + 11]);
	result.hour = toInt(log[i + 13]) * 10 + toInt(log[i + 14]);
	result.minute = toInt(log[i + 16]) * 10 + toInt(log[i + 17]);
	result.second = toInt(log[i + 19]) * 10 + toInt(log[i + 20]);
	return result;
}

short extractStatusFromLog(char log[]) {
	short counter = 0, i = 0;
	while (counter != 2) {
		if (log[i] == '\"') {
			counter++;
		}
		i++;
	}
	return toInt(log[i + 1]) * 100 + toInt(log[i + 2]) * 10 + toInt(log[i + 3]);
}

void readLineAndMoveFilePointer(FILE *fInput, char str[], fpos_t *ptr) {
	fsetpos(fInput, ptr);
	fgets(str, 400, fInput);
	fgetpos(fInput, ptr);
}

void outputErrorRequests(FILE *fInput, FILE* fOutput) {
	fprintf(fOutput, "\n");
	unsigned errorsCounter = 0;
	while (!feof(fInput)) {
		char inputLog[400];
		fgets(inputLog, 400, fInput);
		if (extractStatusFromLog(inputLog) / 100 == 5) {
			errorsCounter++;
			fprintf(fOutput, "%s", inputLog);
		}
	}
	rewind(fOutput);
	fprintf(fOutput, "%u\n", errorsCounter);
}

void findMaxRequestsCount(FILE *fInput, unsigned long long avaliableInterval) {
	time  leftTime, rightTime, timeStart, timeEnd;
	fpos_t leftIt, rightIt;
	unsigned maxLogCount = 0, logCounter = 1;

	char inputLog[400] = { 0 };
	fseek(fInput, SEEK_SET, 0);
	fgets(inputLog, 400, fInput);
	rightTime = extractTimeFromLog(inputLog);
	leftTime = rightTime;
	timeStart = leftTime;
	timeEnd = leftTime;
	fgetpos(fInput, &leftIt);
	fgetpos(fInput, &rightIt);
	while (!feof(fInput)) {
		if (getTimeInSeconds(rightTime) - getTimeInSeconds(leftTime) <= avaliableInterval) {
			readLineAndMoveFilePointer(fInput, inputLog, &rightIt);
			rightTime = extractTimeFromLog(inputLog);
			logCounter++;
		}
		else {
			readLineAndMoveFilePointer(fInput, inputLog, &leftIt);
			leftTime = extractTimeFromLog(inputLog);
			logCounter--;
		}
		if ((logCounter > maxLogCount) && (getTimeInSeconds(rightTime) - getTimeInSeconds(leftTime) <= avaliableInterval)) {
			maxLogCount = logCounter;
			timeStart = leftTime;
			timeEnd = rightTime;
		}
	}

	printTime(timeStart);
	printf(" - ");
	printTime(timeEnd);
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Wrong number of arguments");
		return 1;
	}
	FILE* fInput;
	if (!(fInput = fopen(argv[1], "r"))) {
		printf("File does not exist");
		return 1;
	}
	FILE* fOutput = fopen(argv[2], "w");
	long long avaliableInterval;
	if (!(avaliableInterval = atoll(argv[3]))) {
		printf("Interval is not number");
		return 1;
	}
	outputErrorRequests(fInput, fOutput);
	fclose(fOutput);
	findMaxRequestsCount(fInput, 10000);
	fclose(fInput);
	return 0;
}
