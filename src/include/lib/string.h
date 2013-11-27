#ifndef LIB_STRING_H
#define LIB_STRING_H

#include <lib/stdtypes.h>
#include <lib/stddef.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, int8_t value, size_t count);
void *memsetw(void *dest, uint16_t value, size_t count);
int32_t memcmp(const void *s1, const void *s2, size_t n);
int8_t *itoa(int32_t value, int8_t *result, int32_t base);
int32_t strcmp(const int8_t *str1, const int8_t *str2);
int32_t strncmp(const int8_t *str1, const int8_t *str2, int32_t count);
int8_t *strcpy(int8_t *dest, const int8_t *src);
int8_t *strncpy(int8_t *dest, const int8_t *src, int32_t count);
int8_t *strcat(int8_t *dest, const int8_t *src);
int32_t strlen(const int8_t *src);
int32_t isspace(int8_t c);
int32_t isupper(int8_t c);
int32_t islower(int8_t c);
int32_t isdigit(int8_t c);
int32_t isalpha(int8_t c);
int8_t toupper(int8_t c);
int8_t tolower(int8_t c);

#endif
