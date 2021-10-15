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

unsigned extractTimeFromLog(char log[]) {
	short i = findFirstCharOccurrence(log, '[');
	short days = toInt(log[i + 1]) * 10 + toInt(log[i + 2]);
	short hours = toInt(log[i + 13]) * 10 + toInt(log[i + 14]);
	short minutes = toInt(log[i + 16]) * 10 + toInt(log[i + 17]);
	short seconds = toInt(log[i + 19]) * 10 + toInt(log[i + 20]);
	return (((days - 1) * 24 + hours) * 60 + minutes) * 60 + seconds;
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

void printTime(unsigned seconds, char month[], short year) {
	short days = seconds / 24 / 60 / 60;
	seconds -= days * 24 * 60 * 60;
	short hours = seconds / 60 / 60;
	seconds -= hours * 60 * 60;
	short minutes = seconds / 60;
	seconds -= minutes * 60;
	printf("%02d/%s/%d/%02d:%02d:%02u", days + 1, month, year, hours, minutes, seconds);
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

void findMaxRequestsCount(FILE *fInput, unsigned maxLogCount) {
	unsigned leftTime, rightTime, avaliableInterval = 1000, timeStart = 0, timeEnd = 0;
	maxLogCount = 100;
	fpos_t leftIt, rightIt;
	char inputLog[400] = { 0 };
	fseek(fInput, SEEK_SET, 0);
	fgets(inputLog, 400, fInput);
	unsigned logCounter = 1;
	rightTime = extractTimeFromLog(inputLog);
	leftTime = rightTime;
	fgetpos(fInput, &leftIt);
	fgetpos(fInput, &rightIt);
	while (!feof(fInput)) {
		if (rightTime - leftTime <= avaliableInterval) {
			readLineAndMoveFilePointer(fInput, inputLog, &rightIt);
			rightTime = extractTimeFromLog(inputLog);
			logCounter++;
		}
		else {
			readLineAndMoveFilePointer(fInput, inputLog, &leftIt);
			leftTime = extractTimeFromLog(inputLog);
			logCounter--;
		}
		if ((logCounter > maxLogCount) && (rightTime - leftTime <= avaliableInterval)) {
			maxLogCount = logCounter;
			timeStart = leftTime;
			timeEnd = rightTime;
		}
	}
	char month[] = "Jul";
	printTime(timeStart, month, 1995);
	printf(" - ");
	printTime(timeEnd, month, 1995);
}

int main() {
	FILE *fInput = fopen("access_log_Jul95", "r");
	FILE *fOutput = fopen("result.txt", "w");
	outputErrorRequests(fInput, fOutput);
	fclose(fOutput);
	unsigned maxLogCount = 0;
	findMaxRequestsCount(fInput, maxLogCount);
	fclose(fInput);
	return 0;
}
