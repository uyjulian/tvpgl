/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPDarkenBlend_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // ebp
	tjs_uint32 *  v5; // esi
	__m64         v6; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				v6  = _mm_cvtsi32_si64(*v3);
				*v3 = _mm_cvtsi64_si32(_m_psubb(v6, _m_psubusb(v6, _mm_cvtsi32_si64(*v4))));
				++v3;
				++v4;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPDarkenBlend_HDA_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm7
	__m64         v4; // mm7
	unsigned int *v5; // edi
	unsigned int *v6; // ebp
	tjs_uint32 *  v7; // esi
	__m64         v8; // mm1

	if (len > 0)
	{
		v3 = _mm_cvtsi32_si64(0xFFFFFFu);
		v4 = _m_punpckldq(v3, v3);
		v5 = dest;
		v6 = (unsigned int *)src;
		v7 = &dest[len];
		if (dest < v7)
		{
			do
			{
				v8  = _m_por(_mm_cvtsi32_si64(*v5), v4);
				*v5 = _mm_cvtsi64_si32(_m_psubb(v8, _m_pand(_m_psubusb(v8, _mm_cvtsi32_si64(*v6)), v4)));
				++v5;
				++v6;
			} while (v5 < v7);
		}
	}
	_m_empty();
}
