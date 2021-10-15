#include "stdio.h"
#include "string.h"

typedef enum State {
	InProgress,
	End,
} State;

unsigned CountStrings(FILE *fInput);
unsigned CountBytes(FILE *fInput);
unsigned CountWords(FILE *fInput);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Wrong number of arguments");
		return 1;
	}
	FILE *fInput;
	if (!(fInput = fopen(argv[2], "r"))) {
		printf("File does not exist");
		return 1;
	}
	if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--lines") == 0)) {
		printf("%d\n", CountStrings(fInput));
	}
	else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--bytes") == 0)) {
		printf("%d\n", CountBytes(fInput));
	}
	else if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "--words") == 0)) {
		printf("%d\n", CountWords(fInput));
	}
	else {
		printf("Command does not exist");
		fclose(fInput);
		return 1;
	}
	fclose(fInput);
	return 0;
}

unsigned CountStrings(FILE *fInput) {
	State stringState = End;
	unsigned stringCount = 0;
	char ch = getc(fInput);
	while (!feof(fInput)) {
		if (ch == '\n') {
			stringState = End;
			stringCount += 1;
		}
		else {
			stringState = InProgress;
		}
		ch = getc(fInput);
	}
	if (stringState == InProgress) {
		stringCount += 1;
	}
	return stringCount;
}

unsigned CountBytes(FILE *fInput) {
	unsigned byteCount = 0;
	char ch = getc(fInput);
	while (!feof(fInput)) {
		if (ch == '\n') {
			byteCount += 2;
		}
		else {
			byteCount += 1;
		}
		ch = getc(fInput);
	}
	return byteCount;
}

unsigned CountWords(FILE *fInput) {
	State wordState = End;
	unsigned wordCount = 0;
	char ch = getc(fInput);
	while (!feof(fInput)) {
		if ((ch == ' ') || (ch == '\t') || (ch == '\n')) {
			if (wordState == InProgress) {
				wordCount += 1;
			}
			wordState = End;
		}
		else {
			wordState = InProgress;
		}
		ch = getc(fInput);
	}
	if (wordState == InProgress) {
		wordCount += 1;
	}
	return wordCount;
}
