#pragma once
#include <stdio.h>
#include <string.h>
#include "tagTable.h"

unsigned tagCheck(char frame[]);
void getTagsPositions(FILE* fIn, fpos_t tagsPos[]);
void showTag(FILE* fIn, fpos_t tagOffset);
void showAllTags(FILE* fIn, fpos_t tagsPos[]);
void setTag(FILE* fIn, unsigned tagType, char newTag[], fpos_t tagsPos[], char fileName[]);
