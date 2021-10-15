#include <stdio.h>
#include <string.h>
#include "uint1024_t_functions.h"

void print_help();
void summarize();
void subtract();
void multiply();

int main() {
	print_help();
	short is_finished = 0;
	while (!is_finished) {
		printf("Enter command: ");
		char input[256];
		gets(input);
		if (!strcmp(input, "sum")) {
			summarize();
		}
		else if (!strcmp(input, "subtr")) {
			subtract();
		}
		else if (!strcmp(input, "mult")) {
			multiply();
		}
		else if (!strcmp(input, "end")) {
			is_finished = 1;
		}
		else {
			printf("Command is invalid, please enter again: ");
		}
		printf("\n\n");
	}
	return 0;
}

void print_help() {
	printf("<sum>  to add numbers\n");
	printf("<subtr> to subtract numbers\n");
	printf("<mult> to multipy numbers\n");
	printf("<end> to finish the test\n\n");
}

void summarize() {
	printf("Enter the first term and the second term:\n");
	uint1024_t a, b;
	scanf_value(&a);
	scanf_value(&b);
	printf("Result:\n");
	printf_value(add_op(a, b));
}

void subtract() {
	printf("Enter decrement and subtraction:\n");
	uint1024_t a, b;
	scanf_value(&a);
	scanf_value(&b);
	printf("Result:\n");
	printf_value(subtr_op(a, b));
}

void multiply() {
	printf("Enter the first factor and the second factor:\n");
	uint1024_t a, b;
	scanf_value(&a);
	scanf_value(&b);
	printf("Result:\n");
	printf_value(mult_op(a, b));
}