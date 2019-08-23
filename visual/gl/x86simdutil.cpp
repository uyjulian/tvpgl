
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4305)
#endif
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC target("sse2")
#endif
#ifdef __clang__
#pragma clang attribute push (__attribute__((target("sse2"))), apply_to=function)
#endif
#define USE_SSE2
#include "sse_mathfun.h"
#undef USE_SSE2
#ifdef __clang__
#pragma clang attribute pop
#endif
#ifdef __GNUC__
#pragma GCC pop_options
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif
