#ifndef LIB_TIME_H
#define LIB_TIME_H

#include <lib/stdtypes.h>

#define LOCAL_TIME_ZONE 0 * 60; // Time zone offset in minutes

typedef uint32_t time_t; // Expiry: 2038

typedef struct datetime {
	int sec;      // [0, 59]
	int min;      // [0, 59]
	int hour;     // [0, 23]
	int day;      // [1, 31]
	int month;    // [1, 12]
	int year;     // [1970, 2038]
	int week_day;  // [0, 6] Sunday = 0
	int year_day;  // [0, 365]
	int tz_offset; // offset in minutes
} datetime_t;

#endif
