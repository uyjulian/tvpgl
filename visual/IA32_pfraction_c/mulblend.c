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

static tjs_uint64 TVPMulBlendHDA_mulmask       = 0x0000ffffffffffffull;
static tjs_uint64 TVPMulBlendHDA_100bit        = 0x0100000000000000ull;
static tjs_uint64 TVPMulBlendHDA_fullbit       = 0xffffffffffffffffull;
static tjs_uint64 TVPMulBlend_full_bit_aligned = 0xffffffffffffffffull;

void __cdecl TVPMulBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	tjs_uint32 *  v3; // edi
	unsigned int *v4; // ebp
	tjs_uint32 *  v5; // esi

	v3 = dest;
	v4 = (unsigned int *)src;
	v5 = &dest[len];
	while (v3 < v5)
	{
		*v3 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64()), _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64())),
					8u),
				_mm_setzero_si64()));
		++v3;
		++v4;
	}
	_m_empty();
}

void __cdecl TVPMulBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm6
	__m64         v4; // mm7
	tjs_uint32 *  v5; // edi
	unsigned int *v6; // ebp
	tjs_uint32 *  v7; // esi

	v3 = (__m64)TVPMulBlendHDA_mulmask;
	v4 = (__m64)TVPMulBlendHDA_100bit;
	v5 = dest;
	v6 = (unsigned int *)src;
	v7 = &dest[len];
	while (v5 < v7)
	{
		*v5 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64()),
						_m_por(_m_pand(_m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64()), v3), v4)),
					8u),
				_mm_setzero_si64()));
		++v5;
		++v6;
	}
	_m_empty();
}

void __cdecl TVPMulBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5;  // mm5
	__m64         v6;  // mm5
	__m64         v7;  // mm6
	tjs_uint32 *  v8;  // edi
	unsigned int *v9;  // ebp
	tjs_uint32 *  v10; // esi

	v5  = _mm_set1_pi16(opa);
	v6  = v5;
	v7  = (__m64)TVPMulBlendHDA_fullbit;
	v8  = dest;
	v9  = (unsigned int *)src;
	v10 = &dest[len];
	while (v8 < v10)
	{
		*v8 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(*v8), _mm_setzero_si64()),
						_m_psrlwi(_m_pxor(_m_pmullw(_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v9), v7), _mm_setzero_si64()), v6), v7), 8u)),
					8u),
				_mm_setzero_si64()));
		++v8;
		++v9;
	}
	_m_empty();
}

void __cdecl TVPMulBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5;  // mm5
	__m64         v6;  // mm5
	__m64         v7;  // mm6
	__m64         v8;  // mm7
	tjs_uint32 *  v9;  // edi
	unsigned int *v10; // ebp
	tjs_uint32 *  v11; // esi

	v5  = _mm_set1_pi16(opa);
	v6  = v5;
	v7  = (__m64)TVPMulBlendHDA_mulmask;
	v8  = (__m64)TVPMulBlendHDA_100bit;
	v9  = dest;
	v10 = (unsigned int *)src;
	v11 = &dest[len];
	while (v9 < v11)
	{
		*v9 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_pmullw(
						_m_punpcklbw(_mm_cvtsi32_si64(*v9), _mm_setzero_si64()),
						_m_por(
							_m_pand(
								_m_psrlwi(
									_m_pxor(
										_m_pmullw(
											_m_punpcklbw(
												_m_pxor(
													_mm_cvtsi32_si64(*v10),
													(__m64)TVPMulBlend_full_bit_aligned),
												_mm_setzero_si64()),
											v6),
										(__m64)TVPMulBlend_full_bit_aligned),
									8u),
								v7),
							v8)),
					8u),
				_mm_setzero_si64()));
		++v9;
		++v10;
	}
	_m_empty();
}
