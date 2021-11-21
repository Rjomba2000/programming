#include "tagEditor.h"

void readBytes(FILE* fIn, unsigned bytesCount) {
	for (unsigned i = 0; i < bytesCount; ++i) {
		char ch;
		ch = fgetc(fIn);
	}
}

int isEmptyFrame(char frame[]) {
	if (!frame[0] && !frame[1] && !frame[2] && !frame[3]) {
		return 1;
	}
	return 0;
}

unsigned tagCheck(char frame[]) {
	for (unsigned i = 0; i < TAG_TYPES_COUNT; ++i) {
		if (!strcmp(frame, tagTable[i][0])) {
			return i;
		}
	}
	return TAG_TYPES_COUNT;
}

void getTagsPositions(FILE* fIn, fpos_t tagsPos[]) {
	rewind(fIn);
	readBytes(fIn, 8);
	char frame[5];
	frame[1] = fgetc(fIn);
	frame[2] = fgetc(fIn);
	frame[3] = fgetc(fIn);
	frame[4] = '\0';
	while (!isEmptyFrame(frame)) {
		frame[0] = frame[1];
		frame[1] = frame[2];
		frame[2] = frame[3];
		frame[3] = fgetc(fIn);
		unsigned tag = tagCheck(frame);
		if (tag != TAG_TYPES_COUNT) {
			fgetpos(fIn, &tagsPos[tag]);
		}
	}
}

void showTag(FILE* fIn, fpos_t tagOffset) {
	fsetpos(fIn, &tagOffset);
	readBytes(fIn, 3);
	unsigned size = fgetc(fIn);
	readBytes(fIn, 3);
	for (unsigned i = 1; i < size; ++i) {
		char ch = fgetc(fIn);
		printf("%c", ch);
	}
	printf("\n");
}

void showAllTags(FILE* fIn, fpos_t tagsPos[]) {
	for (unsigned i = 0; i < TAG_TYPES_COUNT; ++i) {
		if (tagsPos[i]) {
			printf("%s: ", tagTable[i][1]);
			showTag(fIn, tagsPos[i]);
		}
	}
}

void copyChars(FILE* fIn, FILE* fOut, unsigned long charsCount) {
	for (unsigned long i = 0; i < charsCount; ++i) {
		char ch = fgetc(fIn);
		fprintf(fOut, "%c", ch);
	}
}

void copyUntilEof(FILE* fIn, FILE* fOut) {
	while (!feof(fIn)) {
		char ch = fgetc(fIn);
		fprintf(fOut, "%c", ch);
	}
}

void setTag(FILE* fIn, unsigned tagType, char newTag[], fpos_t tagsPos[], char fileName[]) {
	rewind(fIn);
	FILE* fOut = fopen(".mp3", "wb");
	if (tagsPos[tagType]) {
		copyChars(fIn, fOut, tagsPos[tagType]);
		fprintf(fOut, "%c%c%c%c%c%c%c%s", 0x00, 0x00, 0x00, strlen(newTag) + 1, 0x00, 0x00, 0x00, newTag);
		readBytes(fIn, 3);
		unsigned size = fgetc(fIn);
		readBytes(fIn, 3);
		for (unsigned i = 1; i < size; ++i) {
			char ch = fgetc(fIn);
		}
	}
	else {
		copyChars(fIn, fOut, 10);
		fprintf(fOut, "%s", tagTable[tagType][0]);
		fprintf(fOut, "%c%c%c%c%c%c%c%s", 0x00, 0x00, 0x00, strlen(newTag) + 1, 0x00, 0x00, 0x00, newTag);
	}
	copyUntilEof(fIn, fOut);
	fclose(fOut);
	fclose(fIn);
	remove(fileName);
	rename(".mp3", fileName);
	fIn = fopen(fileName, "rb");
}
