
#ifdef _WIN32
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC target("sse2")
#define __SSE2__
#endif
#ifdef __clang__
#pragma clang attribute push (__attribute__((target("sse2"))), apply_to=function)
#define __SSE2__
#endif
#endif


#ifdef __SSE2__
#pragma warning(push)
#pragma warning(disable:4305)
#define USE_SSE2
#include "sse_mathfun.h"
#undef USE_SSE2
#pragma warning(pop)
#endif
