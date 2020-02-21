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

void __cdecl TVPLightenBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_mm_cvtsi32_si64(src[i]), _mm_cvtsi32_si64(dest[i])), _mm_cvtsi32_si64(dest[i])));
	}
	_m_empty();
}

void __cdecl TVPLightenBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64       v3; // mm7
	__m64       v4; // mm7

	v3 = _mm_cvtsi32_si64(0xFFFFFFu);
	v4 = _m_punpckldq(v3, v3);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_m_pand(*((__m64 *)(&src[i])), v4), _mm_cvtsi32_si64(dest[i])), _mm_cvtsi32_si64(dest[i])));
	}
	_m_empty();
}
