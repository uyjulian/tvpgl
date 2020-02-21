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
#include "tvpgl_ia32_intf_pfraction_c.h"

void __cdecl TVPMakeAlphaFromKey_cmovcc_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 key)
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
