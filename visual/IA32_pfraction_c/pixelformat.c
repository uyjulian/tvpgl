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
#include "tvpgl_ia32_intf_pfraction_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPBLConvert24BitTo32Bit_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = ((buf[(i * 3) + 2] << 16) + (buf[(i * 3) + 1] << 8) + buf[(i * 3) + 0]) | 0xFF000000;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPDither32BitTo16Bit565_c, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs))
{
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	v8 = TVPDitherTable_5_6[yofs & 3];
	v9 = (xofs & 3) << 9;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v10 = (int)v8 + v9;
		dest[i] = *(tjs_uint8 *)(*(tjs_uint8 *)(&src[i]) + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)(&src[i]) + 1) + v10 + 256) | (*(tjs_uint8 *)(*((tjs_uint8 *)(&src[i]) + 2) + v10) << 11);
		v8 = (tjs_uint8(*)[2][256])(v10 - v9);
		v9 = ((tjs_uint16)v9 + 512) & 0x600;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPDither32BitTo16Bit555_c, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs))
{
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	v8 = TVPDitherTable_5_6[yofs & 3];
	v9 = (xofs & 3) << 9;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v10 = (int)v8 + v9;
		dest[i] = *(tjs_uint8 *)(*(tjs_uint8 *)(&src[i]) + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)(&src[i]) + 1) + v10) | (*(tjs_uint8 *)(*((tjs_uint8 *)(&src[i]) + 2) + v10) << 10);
		v8 = (tjs_uint8(*)[2][256])(v10 - v9);
		v9 = ((tjs_uint16)v9 + 512) & 0x600;
	}
}
