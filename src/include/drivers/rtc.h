#ifndef DRIVERS_RTC_H
#define DRIVERS_RTC_H

#include <lib/stdtypes.h>
#include <lib/time.h>

// I/O Ports
#define IO_RTC_INDEX 0x70
#define IO_RTC_TARGET 0x71

// Indexed Registers
#define RTC_SEC 0x00
#define RTC_SEC_ALARM 0x01
#define RTC_MIN 0x02
#define RTC_MIN_ALARM 0x03
#define RTC_HOUR 0x04
#define RTC_HOUR_ALARM 0x05
#define RTC_WEEK_DAY 0x06
#define RTC_DAY 0x07
#define RTC_MONTH 0x08
#define RTC_YEAR 0x09
#define RTC_A 0x0a
#define RTC_B 0x0b
#define RTC_C 0x0c
#define RTC_D 0x0d

// General Configuration Registers
#define RTC_A_UIP (1 << 7) // Update In Progress
#define RTC_B_HOURFORM (1 << 1) // Hour Format (0 = 12hr, 1 = 24hr)
#define RTC_B_DM (1 << 2) // Data Mode (0 = BCD, 1 = Binary)

void rtc_get(datetime_t *dt);
void rtc_set(const datetime_t *dt);

#endif
