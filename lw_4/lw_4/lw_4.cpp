#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "tagEditor.h"
#include "argParser.h" 

void finishWithErrorMessage(char* message) {
	printf("%s", message);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");

	if (argc < 3) {
		finishWithErrorMessage("Wrong number of arguments");
	}

	char argName[100] = "", fileName[100] = "";
	parseArg("filepath", argv[1], argName, fileName);
	FILE *mp3File;
	if (!(mp3File = fopen(fileName, "rb"))) {
		finishWithErrorMessage("Can not open the file");
	}

	char argValue[100] = "";
	parseArg("showgetset", argv[2], argName, argValue);
	fpos_t tagsPos[TAG_TYPES_COUNT] = { 0 };
	getTagsPositions(mp3File, tagsPos);
	if (!strcmp(argName, "show") && (argc == 3)) {
		showAllTags(mp3File, tagsPos);
	}
	else if (!strcmp(argName, "get") && (argc == 3)) {
		unsigned tag = tagCheck(argValue);
		if ((tag != TAG_TYPES_COUNT) && (tagsPos[tag] != 0)) {
			showTag(mp3File, tagsPos[tag]);
		}
		else {
			finishWithErrorMessage("Tag doesn't exist");
		}
	}
	else if (!strcmp(argName, "set") && (argc == 4)) {
		unsigned tag = tagCheck(argValue);
		char tagValue[100];
		if (tag != TAG_TYPES_COUNT) {
			parseArg("value", argv[3], argName, tagValue);
			setTag(mp3File, tag, tagValue, tagsPos, fileName);
		}
		else {
			finishWithErrorMessage("Tag doesn't exist");
		}
	}
	else {
		finishWithErrorMessage("Unknown set of arguments");
	}
	fclose(mp3File);
	return 0;
}
