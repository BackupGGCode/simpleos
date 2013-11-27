#include <lib/string.h>
#include <lib/stdtypes.h>

void *memcpy(void *dest, const void *src, size_t count)
{
	const int8_t *sp = (const int8_t *)src;
	int8_t *dp = (int8_t *)dest;

	for (; count != 0; count--) {
		*dp++ = *sp++;
	}

	return dest;
}

void *memset(void *dest, int8_t value, size_t count)
{
	int8_t *temp = (int8_t *)dest;

	for (; count != 0; count--) {
		*temp++ = value;
	}

	return dest;
}

void *memsetw(void *dest, uint16_t value, size_t count)
{
	uint16_t *temp = (uint16_t *)dest;

	for (; count != 0; count--) {
		*temp++ = value;
	}

	return dest;
}

int32_t memcmp(const void *s1, const void *s2, size_t n)
{
	if (n) {
		register const uint8_t *p1 = s1, *p2 = s2;

		do {
			if (*p1++ != *p2++) {
				return (*--p1 - *--p2);
			}
		} while (--n);
	}

	return 0;
}

int8_t *itoa(int32_t value, int8_t *result, int32_t base)
{
	if (base < 2 || base > 36) {
		*result = '\0';

		return result;
	}

	int8_t *ptr = result;
	int8_t *ptr1 = result;
	int8_t tmp_char;
	int32_t tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	if (tmp_value < 0) {
		*ptr++ = '-';
	}

	*ptr-- = '\0';

	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

int32_t strcmp(const int8_t *str1, const int8_t *str2)
{
	int32_t i = 0;
	int32_t failed = 0;

	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			failed = 1;
			break;
		}

		i++;
	}

	if ((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0')) {
		failed = 1;
	}

	return failed;
}

int32_t strncmp(const int8_t *str1, const int8_t *str2, int32_t count)
{
	int32_t i = 0;
	int32_t failed = 0;

	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			failed = 1;
			break;
		}

		i++;
		count--;
	}

	if (count && ((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))) {
		failed = 1;
	}

	return failed;
}


int8_t *strcpy(int8_t *dest, const int8_t *src)
{
	while (*src) {
		*dest++ = *src++;
	}

	*dest = '\0';

	return dest;
}

int8_t *strncpy(int8_t *dest, const int8_t *src, int32_t count)
{
	while (*src && count) {
		*dest++ = *src++;
		count--;
	}

	*dest = '\0';

	return dest;
}

int8_t *strcat(int8_t *dest, const int8_t *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}

int32_t strlen(const int8_t *src)
{
	int32_t i = 0;

	while (*src++) {
		i++;
	}

	return i;
}

int32_t isspace(int8_t c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

int32_t isupper(int8_t c)
{
	return (c >= 'A' && c <= 'Z');
}

int32_t islower(int8_t c)
{
	return (c >= 'a' && c <= 'z');
}

int32_t isdigit(int8_t c)
{
	return (c >= '0' && c <= '9');
}

int32_t isalpha(int8_t c)
{
	return isupper(c) || islower(c) || isdigit(c);
}

int8_t toupper(int8_t c)
{
	if (islower(c)) {
		return 'A' + (c - 'a');
	} else {
		return c;
	}
}

int8_t tolower(int8_t c)
{
	if (isupper(c)) {
		return 'a' + (c - 'A');
	} else {
		return c;
	}
}
