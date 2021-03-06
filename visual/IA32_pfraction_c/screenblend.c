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

static const __m64 TVPScreenMulBlend_full_bit_aligned = (__m64)0xffffffffffffffffull;
static const __m64 TVPScreenBlendHDA_alphamask        = (__m64)0xff000000ff000000ull;
static const __m64 TVPScreenBlendHDA_mulmask          = (__m64)0x00ffffff00ffffffull;
static const __m64 TVPScreenBlendHDA_mul_100bit       = (__m64)0x0100000000000000ull;

TVP_GL_IA32_FUNC_DECL(void, TVPScreenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v3; // mm6

	v3 = (__m64)TVPScreenMulBlend_full_bit_aligned;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_pxor(
				_m_packuswb(
					_m_psrlwi(
						_m_pmullw(
							_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(dest[i]), v3), _mm_setzero_si64()),
							_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(src[i]), v3), _mm_setzero_si64())),
						8u),
					_mm_setzero_si64()),
				v3));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPScreenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v3; // mm6
	__m64         v4; // mm7

	v3 = (__m64)TVPScreenBlendHDA_mul_100bit;
	v4 = (__m64)TVPScreenBlendHDA_mulmask;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_pxor(
				_m_packuswb(
					_m_psrlwi(
						_m_pmullw(
							_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(dest[i]), v4), _mm_setzero_si64()),
							_m_por(_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(src[i]), v4), _mm_setzero_si64()), v3)),
						8u),
					_mm_setzero_si64()),
				v4));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPScreenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v5; // mm5
	__m64         v7; // mm6

	v5 = _mm_set1_pi16(opa);
	v7 = (__m64)TVPScreenMulBlend_full_bit_aligned;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_pxor(
				_m_packuswb(
					_m_psrlwi(
						_m_pmullw(
							_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(dest[i]), v7), _mm_setzero_si64()),
							_m_psrlwi(_m_pxor(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v5), v7), 8u)),
						8u),
					_mm_setzero_si64()),
				v7));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPScreenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v5;  // mm5
	__m64         v7;  // mm6
	__m64         v8;  // mm7

	v5  = _mm_set1_pi16(opa);
	v7  = (__m64)TVPScreenBlendHDA_alphamask;
	v8  = (__m64)TVPScreenBlendHDA_mulmask;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_por(
				_m_pand(
					_m_pxor(
						_m_packuswb(
							_m_psrlwi(
								_m_pmullw(
									_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(dest[i]), v8), _mm_setzero_si64()),
									_m_por(
										_m_pand(
											_m_psrlwi(
												_m_pxor(
													_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v5),
													(__m64)TVPScreenMulBlend_full_bit_aligned),
												8u),
											v7),
										v8)),
								8u),
							_mm_setzero_si64()),
						v8),
					v8),
				_m_pand(_mm_cvtsi32_si64(dest[i]), v7)));
	}
	_m_empty();
}
