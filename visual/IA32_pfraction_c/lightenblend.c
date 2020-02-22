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

TVP_GL_IA32_FUNC_DECL(void, TVPLightenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_mm_cvtsi32_si64(src[i]), _mm_cvtsi32_si64(dest[i])), _mm_cvtsi32_si64(dest[i])));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPLightenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64       v4; // mm7

	v4 = _mm_set1_pi32(0xFFFFFFu);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_m_pand(*((__m64 *)(&src[i])), v4), _mm_cvtsi32_si64(dest[i])), _mm_cvtsi32_si64(dest[i])));
	}
	_m_empty();
}
