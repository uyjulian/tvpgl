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

void __cdecl TVPAddBlend_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	tjs_uint32 *  v3; // edi
	unsigned int *v4; // ebp
	tjs_uint32 *  v5; // esi

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				*v3 = _mm_cvtsi64_si32(_m_paddusb(_mm_cvtsi32_si64(*v3), _mm_cvtsi32_si64(*v4)));
				++v3;
				++v4;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPAddBlend_HDA_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm7
	__m64         v4; // mm7
	tjs_uint32 *  v5; // edi
	unsigned int *v6; // ebp
	tjs_uint32 *  v7; // esi

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
				*v5 = _mm_cvtsi64_si32(_m_paddusb(_mm_cvtsi32_si64(*v5), _m_pand(_mm_cvtsi32_si64(*v6), v4)));
				++v5;
				++v6;
			} while (v5 < v7);
		}
	}
	_m_empty();
}

void __cdecl TVPAddBlend_HDA_o_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5; // mm7
	__m64         v6; // mm7
	tjs_uint32 *  v7; // edi
	unsigned int *v8; // ebp
	tjs_uint32 *  v9; // esi

	if (len > 0)
	{
		v5 = _mm_set1_pi16(opa);
		v6 = _m_psrlqi(v5, 0x10u);
		v7 = dest;
		v8 = (unsigned int *)src;
		v9 = &dest[len];
		if (dest < v9)
		{
			do
			{
				*v7 = _mm_cvtsi64_si32(
					_m_packuswb(
						_m_paddw(
							_m_punpcklbw(_mm_cvtsi32_si64(*v7), _mm_setzero_si64()),
							_m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v8), _mm_setzero_si64()), v6), 8u)),
						_mm_setzero_si64()));
				++v7;
				++v8;
			} while (v7 < v9);
		}
	}
	_m_empty();
}
