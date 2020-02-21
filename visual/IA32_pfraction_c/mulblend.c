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

static const __m64 TVPMulBlendHDA_mulmask       = (__m64)0x0000ffffffffffffull;
static const __m64 TVPMulBlendHDA_100bit        = (__m64)0x0100000000000000ull;
static const __m64 TVPMulBlendHDA_fullbit       = (__m64)0xffffffffffffffffull;
static const __m64 TVPMulBlend_full_bit_aligned = (__m64)0xffffffffffffffffull;

void __cdecl TVPMulBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()), _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64())),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPMulBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm6
	__m64         v4; // mm7

	v3 = (__m64)TVPMulBlendHDA_mulmask;
	v4 = (__m64)TVPMulBlendHDA_100bit;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()),
						_m_por(_m_pand(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v3), v4)),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPMulBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5; // mm5
	__m64         v7; // mm6

	v5 = _mm_set1_pi16(opa);
	v7 = (__m64)TVPMulBlendHDA_fullbit;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()),
						_m_psrlwi(_m_pxor(_m_pmullw(_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(src[i]), v7), _mm_setzero_si64()), v5), v7), 8u)),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPMulBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5;  // mm5
	__m64         v7;  // mm6
	__m64         v8;  // mm7

	v5  = _mm_set1_pi16(opa);
	v7  = (__m64)TVPMulBlendHDA_mulmask;
	v8  = (__m64)TVPMulBlendHDA_100bit;
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64()),
						_m_por(
							_m_pand(
								_m_psrlwi(
									_m_pxor(
										_m_pmullw(
											_m_punpcklbw(
												_m_pxor(
													_mm_cvtsi32_si64(src[i]),
													(__m64)TVPMulBlend_full_bit_aligned),
												_mm_setzero_si64()),
											v5),
										(__m64)TVPMulBlend_full_bit_aligned),
									8u),
								v7),
							v8)),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}
