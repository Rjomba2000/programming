#include "time.h"

unsigned getMonthDays(Months month) {
	short monthArr[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return monthArr[month];
}

unsigned long long getTimeInSeconds(time t) {
	unsigned long long seconds = (t.year - MIN_YEAR - 1) / 4 * 366 + (t.year - MIN_YEAR - 1) * 365;
	for (Months month = JAN; month < t.month; ++month) {
		seconds += getMonthDays(t.month);
	}
	if ((t.year % 4 == 0) && (t.month > FEB)) {
		seconds++;
	}
	seconds = (seconds + t.day - 1) * 24;
	seconds = (seconds + t.hour) * 60;
	seconds = (seconds + t.minute) * 60;
	seconds += t.second;
	return seconds;
}

Months encodeMonth(char str[]) {
	char* monthArr[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	for (Months m = JAN; m <= DEC; ++m) {
		if (!strcmp(str, monthArr[m])) {
			return m;
		}
	}
}

void decodeMonth(char str[], Months month) {
	char* monthArr[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	strcpy(str, monthArr[month]);
}

void printTime(time tOut) {
	char month[4];
	decodeMonth(month, tOut.month);
	printf("%02d/%s/%d/%02d:%02d:%02u", tOut.day, month, tOut.year, tOut.hour, tOut.minute, tOut.second);
}