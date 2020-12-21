/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#ifdef _WIN32
#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC target("mmx")
#define __MMX__
#endif
#ifdef __clang__
#pragma clang attribute push (__attribute__((target("mmx"))), apply_to=function)
#define __MMX__
#endif
#endif

#include <mmintrin.h>
#include <stdbool.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPMakeAlphaFromKey_cmovcc_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 key))
{
	int          v6; // eax
	int          v7; // ebx
	unsigned int v8; // eax

	for (tjs_int i = 0; i < len; i += 1)
	{
		v6 = dest[i] & 0xFFFFFF;
		v7 = v6;
		v8 = v6 | 0xFF000000;
		if (v8 == (key & 0xFFFFFF | 0xFF000000))
			v8 = v7;
		dest[i] = v8;
	}
}
