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

static const __m64 TVPSubBlend_full_bit_one = (__m64)0xffffffffffffffffull;

TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v4; // mm7

	v4 = _mm_set1_pi32(0xFFFFFFFF);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_psubusb(_mm_cvtsi32_si64(dest[i]), _m_pxor(_mm_cvtsi32_si64(src[i]), v4)));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v4; // mm7
	__m64         v6; // mm6

	v4 = _mm_set1_pi32(0xFFFFFFFF);
	v6 = _mm_set1_pi32(0xFFFFFFu);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(_m_psubusb(_mm_cvtsi32_si64(dest[i]), _m_pand(_m_pxor(_mm_cvtsi32_si64(src[i]), v4), v6)));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v5; // mm7
	__m64         v6; // mm7

	v5 = _mm_set1_pi16(opa);
	v6 = _m_psrlqi(v5, 0x10u);
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psubw(
					_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()),
					_m_psrlwi(
						_m_pmullw(
							_m_punpcklbw(
								_m_pxor(
									_mm_cvtsi32_si64(src[i]),
									(__m64)TVPSubBlend_full_bit_one),
								_mm_setzero_si64()),
							v6),
						8u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
