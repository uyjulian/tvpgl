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

static const __m64 mask0000ffffffffffff = (__m64)0x0000ffffffffffffull;
static const __m64 mask00ffffff00ffffff = (__m64)0x00ffffff00ffffffull;

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64             v7; // mm2
	__m64             v8; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_set1_pi16(src[i] >> 24);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v7), 8u)), _mm_setzero_si64()), _mm_cvtsi32_si64(src[i])));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v7;  // mm7
	__m64         v10; // mm2
	__m64         v11; // mm4
	__m64         v12; // mm1
	__m64         v13; // mm1

	v7 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v11 = _m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v7), 8u);
		v12 = _m_psrlqi(v11, 0x30u);
		v13 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v12));
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v10, _m_psrlwi(_m_pmullw(v10, v13), 8u)), v11), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64             v7; // mm2
	__m64             v8; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_set1_pi16(src[i] >> 24);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_paddusb(
				_m_packuswb(
					_m_psubw(v8, _m_pand(_m_psrlwi(_m_pmullw(v8, v7), 8u), (__m64)mask0000ffffffffffff)),
					_mm_setzero_si64()),
				_m_pand(_mm_cvtsi32_si64(src[i]), (__m64)mask00ffffff00ffffff)));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v6; // mm3
	__m64         v7; // mm4
	__m64         v8; // mm4
	__m64         v9; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrlqi(v6, 0x18u);
		v8  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 8u)), _m_punpcklbw(v6, _mm_setzero_si64())), _mm_setzero_si64()));
	}
	_m_empty();
}
