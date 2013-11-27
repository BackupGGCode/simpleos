#ifndef LIB_VSPRINTF_H
#define LIB_VSPRINTF_H

#include <lib/stdtypes.h>
#include <lib/stdarg.h>

int32_t vsprintf(int8_t *buf, const int8_t *fmt, va_list args);
int32_t sprintf(int8_t *buf, const int8_t *fmt, ...);

#endif
