/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

void __cdecl TVPBLConvert24BitTo32Bit_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)
{
	tjs_uint32 *     v3; // edi
	const tjs_uint8 *v4; // ecx

	v3 = dest;
	v4 = buf;
	while (v3 < &dest[len])
	{
		*v3 = ((v4[2] << 16) + (v4[1] << 8) + *v4) | 0xFF000000;
		++v3;
		v4 += 3;
	}
	_m_empty();
}

void __cdecl TVPDither32BitTo16Bit565_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)
{
	tjs_uint16 *      v5;   // edi
	const tjs_uint32 *v6;   // ebp
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	v5 = dest;
	v6 = src;
	v8 = TVPDitherTable_5_6[yofs & 3];
	v9 = (xofs & 3) << 9;
	while (v6 < &src[len])
	{
		v10 = (int)v8 + v9;
		*v5 = *(tjs_uint8 *)(*(tjs_uint8 *)v6 + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)v6 + 1) + v10 + 256) | (*(tjs_uint8 *)(*((tjs_uint8 *)v6 + 2) + v10) << 11);
		++v6;
		++v5;
		v8 = (tjs_uint8(*)[2][256])(v10 - v9);
		v9 = ((tjs_uint16)v9 + 512) & 0x600;
	}
	_m_empty();
}

void __cdecl TVPDither32BitTo16Bit555_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)
{
	tjs_uint16 *      v5;   // edi
	const tjs_uint32 *v6;   // ebp
	tjs_uint8(*v8)[2][256]; // edx
	int v9;                 // ecx
	int v10;                // edx

	v5 = dest;
	v6 = src;
	v8 = TVPDitherTable_5_6[yofs & 3];
	v9 = (xofs & 3) << 9;
	while (v6 < &src[len])
	{
		v10 = (int)v8 + v9;
		*v5 = *(tjs_uint8 *)(*(tjs_uint8 *)v6 + v10) | 32 * *(tjs_uint8 *)(*((tjs_uint8 *)v6 + 1) + v10) | (*(tjs_uint8 *)(*((tjs_uint8 *)v6 + 2) + v10) << 10);
		++v6;
		++v5;
		v8 = (tjs_uint8(*)[2][256])(v10 - v9);
		v9 = ((tjs_uint16)v9 + 512) & 0x600;
	}
	_m_empty();
}
