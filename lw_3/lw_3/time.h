#pragma once
#include <stdio.h>
#include <string.h>

#define MIN_YEAR 1900

enum Months {
	JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} typedef Months;

struct time {
	short year;
	Months month;
	short day;
	short hour;
	short minute;
	short second;
} typedef time;

unsigned getMonthDays(Months month);
unsigned long long getTimeInSeconds(time t);
Months encodeMonth(char str[]);
void decodeMonth(char str[], Months month);
void printTime(time tOut);