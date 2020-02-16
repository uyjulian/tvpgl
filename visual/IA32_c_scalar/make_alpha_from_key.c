/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include <stdbool.h>
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPMakeAlphaFromKey_cmovcc_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 key)
{
	tjs_uint32 * v3; // edi
	bool         v4; // sf
	int          v5; // ecx
	int          v6; // eax
	int          v7; // ebx
	unsigned int v8; // eax

	v3 = dest;
	v5 = len - 1;
	v4 = len - 1 < 0;
	while (!v4)
	{
		v6 = *v3 & 0xFFFFFF;
		v7 = v6;
		++v3;
		v8 = v6 | 0xFF000000;
		if (v8 == (key & 0xFFFFFF | 0xFF000000))
			v8 = v7;
		v4        = --v5 < 0;
		*(v3 - 1) = v8;
	}
}
