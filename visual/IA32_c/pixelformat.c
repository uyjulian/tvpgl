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
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPBLConvert24BitTo32Bit_mmx_c(tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)
{
	tjs_uint32 *     v3; // edi
	const tjs_uint8 *v4; // ecx
	tjs_uint32 *     v5; // ebx

	if (len > 0)
	{
		v3 = dest;
		v4 = buf;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				*v3 = ((v4[2] << 16) + (v4[1] << 8) + *v4) | 0xFF000000;
				++v3;
				v4 += 3;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPDither32BitTo16Bit565_mmx_c(tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)
{
	tjs_uint16 *      v5;   // edi
	const tjs_uint32 *v6;   // ebp
	const tjs_uint32 *v7;   // esi
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	if (len > 0)
	{
		v5 = dest;
		v6 = src;
		v7 = &src[len];
		v8 = TVPDitherTable_5_6[yofs & 3];
		v9 = (xofs & 3) << 9;
		if (src < v7)
		{
			do
			{
				v10 = (int)v8 + v9;
				*v5 = *(tjs_uint8 *)(*(tjs_uint8 *)v6 + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)v6 + 1) + v10 + 256) | (*(tjs_uint8 *)(*((tjs_uint8 *)v6 + 2) + v10) << 11);
				++v6;
				++v5;
				v8 = (tjs_uint8(*)[2][256])(v10 - v9);
				v9 = ((tjs_uint16)v9 + 512) & 0x600;
			} while (v6 < v7);
		}
	}
	_m_empty();
}

void __cdecl TVPDither32BitTo16Bit555_mmx_c(tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)
{
	tjs_uint16 *      v5;   // edi
	const tjs_uint32 *v6;   // ebp
	const tjs_uint32 *v7;   // esi
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	if (len > 0)
	{
		v5 = dest;
		v6 = src;
		v7 = &src[len];
		v8 = TVPDitherTable_5_6[yofs & 3];
		v9 = (xofs & 3) << 9;
		if (src < v7)
		{
			do
			{
				v10 = (int)v8 + v9;
				*v5 = *(tjs_uint8 *)(*(tjs_uint8 *)v6 + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)v6 + 1) + v10) | (*(tjs_uint8 *)(*((tjs_uint8 *)v6 + 2) + v10) << 10);
				++v6;
				++v5;
				v8 = (tjs_uint8(*)[2][256])(v10 - v9);
				v9 = ((tjs_uint16)v9 + 512) & 0x600;
			} while (v6 < v7);
		}
	}
	_m_empty();
}
