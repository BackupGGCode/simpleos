#ifndef LIB_STDARG_H
#define LIB_STDARG_H

#include <lib/stdtypes.h>

#if defined(_MSC_VER) // Visual Studio
typedef int8_t *va_list;
#define intsizeof(n)    ((sizeof(n) + sizeof(int32_t) - 1) &~(sizeof(int32_t) - 1))
#define va_start(ap, v) (ap = (va_list)&(v) + intsizeof(v))
#define va_arg(ap, t)   (*(t *) ((ap += intsizeof(t)) - intsizeof(t)))
#define va_end(ap)      (ap = (va_list)0)
#else // GCC
typedef __builtin_va_list va_list;
#define va_start(v,l)     __builtin_va_start(v,l)
#define va_arg(v,l)       __builtin_va_arg(v,l)
#define va_end(v)         __builtin_va_end(v)
#define va_copy(d,s)      __builtin_va_copy(d,s)
#endif

#endif
