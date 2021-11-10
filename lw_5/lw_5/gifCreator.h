#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addFrame(FILE* gifFile, char** bitMap, unsigned width, unsigned height);
void createGif(FILE* gifFile, unsigned width, unsigned height);