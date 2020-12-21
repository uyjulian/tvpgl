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

TVP_GL_IA32_FUNC_DECL(void, TVPDarkenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v6; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(dest[i]);
		dest[i] = _mm_cvtsi64_si32(_m_psubb(v6, _m_psubusb(v6, _mm_cvtsi32_si64(src[i]))));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPDarkenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v4; // mm7
	__m64         v8; // mm1

	v4 = _mm_set1_pi32(0xFFFFFFu);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _m_por(_mm_cvtsi32_si64(dest[i]), v4);
		dest[i] = _mm_cvtsi64_si32(_m_psubb(v8, _m_pand(_m_psubusb(v8, _mm_cvtsi32_si64(src[i])), v4)));
	}
	_m_empty();
}
