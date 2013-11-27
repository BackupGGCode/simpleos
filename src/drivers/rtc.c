#include <drivers/rtc.h>
#include <lib/stdtypes.h>
#include <lib/x86.h>
#include <lib/time.h>
#include <drivers/pit.h>
#include <drivers/screen/textmode.h>

static uint8_t rtc_read(uint8_t addr)
{
	outportb(IO_RTC_INDEX, addr);
	return inportb(IO_RTC_TARGET);
}

static void rtc_write(uint8_t addr, uint8_t val)
{
	outportb(IO_RTC_INDEX, addr);
	outportb(IO_RTC_TARGET, val);
}

static uint8_t bcd_to_bin(uint8_t val)
{
	return (val & 0xf) + (val >> 4) * 10;
}

static uint8_t bin_to_bcd(uint8_t val)
{
	return ((val / 10) << 4) + (val % 10);
}

void rtc_get(datetime_t *dt)
{
	// Wait if update is in progress
	if (rtc_read(RTC_A) & RTC_A_UIP) {
		pit_wait(3);
	}

	// Read Registers
	uint8_t sec = rtc_read(RTC_SEC);
	uint8_t min = rtc_read(RTC_MIN);
	uint8_t hour = rtc_read(RTC_HOUR);
	uint8_t weekDay = rtc_read(RTC_WEEK_DAY);
	uint8_t day = rtc_read(RTC_DAY);
	uint8_t month = rtc_read(RTC_MONTH);
	uint16_t year = rtc_read(RTC_YEAR);

	// Get Data configuration
	uint8_t regb = rtc_read(RTC_B);

	// BCD conversion
	if (~regb & RTC_B_DM) {
		sec = bcd_to_bin(sec);
		min = bcd_to_bin(min);
		hour = bcd_to_bin(hour);
		day = bcd_to_bin(day);
		month = bcd_to_bin(month);
		year = bcd_to_bin(year);
	}

	// Century support
	year += 2000;

	// Week day conversion: Sunday as the first day of the week (0-6)
	weekDay--;

	// Write results
	dt->sec = sec;
	dt->min = min;
	dt->hour = hour;
	dt->day = day;
	dt->month = month;
	dt->year = year;
	dt->week_day = weekDay;
	dt->tz_offset = LOCAL_TIME_ZONE;
}

void rtc_set(const datetime_t *dt)
{
	uint8_t sec = dt->sec;
	uint8_t min = dt->min;
	uint8_t hour = dt->hour;
	uint8_t day = dt->day;
	uint8_t month = dt->month;
	uint8_t year = dt->year - 2000;
	uint8_t week_day = dt->week_day + 1;

	// Validate data
	if (sec >= 60 || min >= 60 || hour >= 24 || day > 31 || month > 12 || year >= 100 || week_day > 7) {
		printf("rtc_set: bad data\n");
		return;
	}

	// Get data configuration
	uint8_t regb = rtc_read(RTC_B);

	// BCD conversion
	if (~regb & RTC_B_DM) {
		sec = bin_to_bcd(sec);
		min = bin_to_bcd(min);
		hour = bin_to_bcd(hour);
		day = bin_to_bcd(day);
		month = bin_to_bcd(month);
		year = bin_to_bcd(year);
	}

	// Wait if update is in progress
	if (rtc_read(RTC_A) & RTC_A_UIP) {
		pit_wait(3);
	}

	// Write registers
	rtc_write(RTC_SEC, sec);
	rtc_write(RTC_MIN, min);
	rtc_write(RTC_HOUR, hour);
	rtc_write(RTC_WEEK_DAY, week_day);
	rtc_write(RTC_DAY, day);
	rtc_write(RTC_MONTH, month);
	rtc_write(RTC_YEAR, year);
}