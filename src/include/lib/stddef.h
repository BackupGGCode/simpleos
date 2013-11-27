#ifndef LIB_STDDEF_H
#define LIB_STDDEF_H

#if defined(_MSC_VER) // Visual Studio
#define PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
#define PACKED( __Declaration__ ) __Declaration__ __attribute__((__packed__)) // GCC
#endif

#define NULL ((void *)0)

typedef unsigned long size_t;

#endif
