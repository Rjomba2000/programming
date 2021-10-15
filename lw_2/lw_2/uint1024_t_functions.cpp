#include <stdio.h>
#include "uint1024_t_functions.h"

#define DECIMAL_RANK_COUNT 308
#define STORED_RANK_COUNT 64

uint1024_t from_uint(uint16_t x) {
	uint1024_t result = { 0 };
	result.rank[0] = x;
	return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
	uint1024_t result = { 0 };
	for (int i = 0; i < STORED_RANK_COUNT; i++) {
		if (x.rank[i] + y.rank[i] + result.rank[i] > UINT16_MAX) {
			result.rank[i + 1]++;
		}
		result.rank[i] += x.rank[i] + y.rank[i];
	}
	return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
	uint1024_t result = { 0 };
	short carry_rank = 0;
	for (int i = 0; i < STORED_RANK_COUNT; i++) {
		result.rank[i] = x.rank[i] - carry_rank - y.rank[i];
		if (x.rank[i] - carry_rank < y.rank[i]) {
			carry_rank = 1;
		}
		else {
			carry_rank = 0;
		}
	}
	return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
	uint1024_t result = { 0 };
	for (int i = 0; i < STORED_RANK_COUNT; i++) {
		uint1024_t buffer = { 0 };
		for (int j = 0; j < STORED_RANK_COUNT; j++) {
			uint32_t rank_mult = x.rank[j] * y.rank[i] + buffer.rank[j + i];
			buffer.rank[i + j] = rank_mult % (UINT16_MAX + 1);
			if (rank_mult > UINT16_MAX) {
				buffer.rank[i + j + 1] = rank_mult / (UINT16_MAX + 1);
			}
		}
		result = add_op(result, buffer);
	}
	return result;
}

short is_null(uint16_t x[], short rankCount) {
	for (int i = 0; i < rankCount; i++) {
		if (x[i] != 0) {
			return 0;
		}
	}
	return 1;
}

uint16_t get_remainder_and_divide(uint16_t dividend[], int rank_count, int initial_number_system, int finite_number_system) {
	uint32_t add_rank = 0;
	for (int i = rank_count - 1; i >= 0; i--) {
		uint16_t current_rank = dividend[i];
		dividend[i] = (add_rank * initial_number_system + current_rank) / finite_number_system;
		add_rank = (add_rank * initial_number_system + current_rank) % finite_number_system;
	}
	return add_rank;
}

void printf_value(uint1024_t x) {
	int i = -1;
	uint16_t print_buffer[DECIMAL_RANK_COUNT] = { 0 };
	if (is_null(x.rank, STORED_RANK_COUNT)) {
		printf("0");
	}
	else {
		while (!is_null(x.rank, STORED_RANK_COUNT)) {
			i++;
			print_buffer[i] = get_remainder_and_divide(x.rank, 64, UINT16_MAX + 1, 10);
		}
		for (int j = i; j >= 0; j--) {
			printf("%u", print_buffer[j]);
		}
	}
}

short is_number(char ch) {
	if ((ch >= '0') && (ch <= '9')) {
		return 1;
	}
	else {
		return 0;
	}
}

short to_int(char ch) {
	return ch - 48;
}

void swap(uint16_t *x, uint16_t *y) {
	uint16_t buffer = *x;
	*x = *y;
	*y = buffer;
}

void swap_array(uint16_t arr[], int left, int right) {
	for (int i = left, j = right - 1; i < j; i++, j--) {
		swap(&arr[i], &arr[j]);
	}
}

void read_number_into_buffer(uint16_t buffer[], int *rank_count) {
	char ch;
	scanf("%c", &ch);
	while (!is_number(ch)) {
		scanf("%c", &ch);
	}
	*rank_count = 0;
	while (is_number(ch)) {
		buffer[*rank_count] = to_int(ch);
		scanf("%c", &ch);
		*rank_count += 1;
	}
}

void zero_array(uint16_t arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = 0;
	}
}

void scanf_value(uint1024_t *x) {
	uint16_t scan_buffer[DECIMAL_RANK_COUNT] = { 0 };
	int rank_count = 0;
	read_number_into_buffer(scan_buffer, &rank_count);
	swap_array(scan_buffer, 0, rank_count);
	zero_array(x->rank, STORED_RANK_COUNT);
	int i = 0;
	while (!is_null(scan_buffer, DECIMAL_RANK_COUNT)) {
		x->rank[i] = get_remainder_and_divide(scan_buffer, DECIMAL_RANK_COUNT, 10, UINT16_MAX + 1);
		i++;
	}
}