#include "argParser.h"

void parseArg(char possibleNames[], char arg[], char name[], char value[]) {
	strcpy(name, "");
	strcpy(value, "");
	if ((strlen(arg) < 4) || (arg[0] != '-') || (arg[1] != '-')) {
		printf("Invalid argument");
		exit(EXIT_FAILURE);
	}
	int nameState = 1;
	for (unsigned i = 2; arg[i] != '\0'; ++i) {
		if (arg[i] == '=') {
			if (strlen(name) == 0) {
				printf("Invalid argument");
				exit(EXIT_FAILURE);
			}
			nameState = 0;
		}
		else {
			if (nameState) {
				name[i - 2] = arg[i];
				name[i - 1] = '\0';
			}
			else {
				value[i - strlen(name) - 3] = arg[i];
				value[i - strlen(name) - 2] = '\0';
			}
		}
	}
	if (strstr(possibleNames, name) == NULL) {
		printf("Invalid argument name");
		exit(EXIT_FAILURE);
	}
}
