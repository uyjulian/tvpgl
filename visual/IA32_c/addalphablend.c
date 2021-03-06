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

static tjs_uint64 mask0000ffffffffffff = 0x0000ffffffffffffull;
static tjs_uint64 mask00ffffff00ffffff = 0x00ffffff00ffffffull;

void __cdecl TVPAdditiveAlphaBlend_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *    v3; // edi
	const tjs_uint32 *v4; // ebp
	tjs_uint32 *      v5; // esi
	__m64             v7; // mm2
	__m64             v8; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				v7  = _mm_set1_pi16(*v4 >> 24);
				v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
				*v3 = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v7), 8u)), _mm_setzero_si64()), _mm_cvtsi32_si64(*v4)));
				++v4;
				++v3;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPAdditiveAlphaBlend_o_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // ebp
	__m64         v7;  // mm7
	__m64         v8;  // mm7
	tjs_uint32 *  v9;  // esi
	__m64         v10; // mm2
	__m64         v11; // mm4
	__m64         v12; // mm1
	__m64         v13; // mm1

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v7 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
		v8 = v7;
		v9 = &dest[len];
		if (dest < v9)
		{
			do
			{
				v10 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
				v11 = _m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64()), v8), 8u);
				v12 = _m_psrlqi(v11, 0x30u);
				v13 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v12));
				*v4 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v10, _m_psrlwi(_m_pmullw(v10, v13), 8u)), v11), _mm_setzero_si64()));
				++v5;
				++v4;
			} while (v4 < v9);
		}
	}
	_m_empty();
}

void __cdecl TVPAdditiveAlphaBlend_HDA_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *    v3; // edi
	const tjs_uint32 *v4; // ebp
	tjs_uint32 *      v5; // esi
	__m64             v7; // mm2
	__m64             v8; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				v7  = _mm_set1_pi16(*v4 >> 24);
				v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
				*v3 = _mm_cvtsi64_si32(
					_m_paddusb(
						_m_packuswb(
							_m_psubw(v8, _m_pand(_m_psrlwi(_m_pmullw(v8, v7), 8u), (__m64)mask0000ffffffffffff)),
							_mm_setzero_si64()),
						_m_pand(_mm_cvtsi32_si64(*v4), (__m64)mask00ffffff00ffffff)));
				++v4;
				++v3;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPAdditiveAlphaBlend_a_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // ebp
	tjs_uint32 *  v5; // esi
	__m64         v6; // mm3
	__m64         v7; // mm4
	__m64         v8; // mm4
	__m64         v9; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				v6  = _mm_cvtsi32_si64(*v4);
				v7  = _m_psrlqi(v6, 0x18u);
				v8  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
				v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
				*v3 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 8u)), _m_punpcklbw(v6, _mm_setzero_si64())), _mm_setzero_si64()));
				++v4;
				++v3;
			} while (v3 < v5);
		}
	}
	_m_empty();
}
