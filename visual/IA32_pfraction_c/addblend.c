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

void __cdecl TVPAddBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddusb(_mm_cvtsi32_si64(dest[i]), _mm_cvtsi32_si64(src[i])));
	}
	_m_empty();
}

void __cdecl TVPAddBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v4; // mm7

	v4 = _mm_set1_pi32(0xFFFFFFu);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddusb(_mm_cvtsi32_si64(dest[i]), _m_pand(_mm_cvtsi32_si64(src[i]), v4)));
	}
	_m_empty();
}

void __cdecl TVPAddBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5; // mm7
	__m64         v6; // mm7

	v5 = _mm_set1_pi16(opa);
	v6 = _m_psrlqi(v5, 0x10u);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()),
					_m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v6), 8u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
