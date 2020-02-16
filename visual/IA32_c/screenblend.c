/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_c.h"

static tjs_uint64 TVPScreenMulBlend_full_bit_aligned = 0xffffffffffffffffull;
static tjs_uint64 TVPScreenBlendHDA_alphamask        = 0xff000000ff000000ull;
static tjs_uint64 TVPScreenBlendHDA_mulmask          = 0x00ffffff00ffffffull;
static tjs_uint64 TVPScreenBlendHDA_mul_100bit       = 0x0100000000000000ull;

void __cdecl TVPScreenBlend_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm6
	tjs_uint32 *  v4; // edi
	unsigned int *v5; // ebp
	tjs_uint32 *  v6; // esi

	if (len > 0)
	{
		v3 = (__m64)TVPScreenMulBlend_full_bit_aligned;
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = &dest[len];
		if (dest < v6)
		{
			do
			{
				*v4 = _mm_cvtsi64_si32(
					_m_pxor(
						_m_packuswb(
							_m_psrlwi(
								_m_pmullw(
									_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v4), v3), _mm_setzero_si64()),
									_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v5), v3), _mm_setzero_si64())),
								8u),
							_mm_setzero_si64()),
						v3));
				++v4;
				++v5;
			} while (v4 < v6);
		}
	}
	_m_empty();
}

void __cdecl TVPScreenBlend_HDA_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64         v3; // mm6
	__m64         v4; // mm7
	tjs_uint32 *  v5; // edi
	unsigned int *v6; // ebp
	tjs_uint32 *  v7; // esi

	if (len > 0)
	{
		v3 = (__m64)TVPScreenBlendHDA_mul_100bit;
		v4 = (__m64)TVPScreenBlendHDA_mulmask;
		v5 = dest;
		v6 = (unsigned int *)src;
		v7 = &dest[len];
		if (dest < v7)
		{
			do
			{
				*v5 = _mm_cvtsi64_si32(
					_m_pxor(
						_m_packuswb(
							_m_psrlwi(
								_m_pmullw(
									_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v5), v4), _mm_setzero_si64()),
									_m_por(_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v6), v4), _mm_setzero_si64()), v3)),
								8u),
							_mm_setzero_si64()),
						v4));
				++v5;
				++v6;
			} while (v5 < v7);
		}
	}
	_m_empty();
}

void __cdecl TVPScreenBlend_o_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5;  // mm5
	__m64         v6;  // mm5
	__m64         v7;  // mm6
	tjs_uint32 *  v8;  // edi
	unsigned int *v9;  // ebp
	tjs_uint32 *  v10; // esi

	if (len > 0)
	{
		v5  = _mm_set1_pi16(opa);
		v6  = v5;
		v7  = (__m64)TVPScreenMulBlend_full_bit_aligned;
		v8  = dest;
		v9  = (unsigned int *)src;
		v10 = &dest[len];
		if (dest < v10)
		{
			do
			{
				*v8 = _mm_cvtsi64_si32(
					_m_pxor(
						_m_packuswb(
							_m_psrlwi(
								_m_pmullw(
									_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v8), v7), _mm_setzero_si64()),
									_m_psrlwi(_m_pxor(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v9), _mm_setzero_si64()), v6), v7), 8u)),
								8u),
							_mm_setzero_si64()),
						v7));
				++v8;
				++v9;
			} while (v8 < v10);
		}
	}
	_m_empty();
}

void __cdecl TVPScreenBlend_HDA_o_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v5;  // mm5
	__m64         v6;  // mm5
	__m64         v7;  // mm6
	__m64         v8;  // mm7
	tjs_uint32 *  v9;  // edi
	unsigned int *v10; // ebp
	tjs_uint32 *  v11; // esi

	if (len > 0)
	{
		v5  = _mm_set1_pi16(opa);
		v6  = v5;
		v7  = (__m64)TVPScreenBlendHDA_alphamask;
		v8  = (__m64)TVPScreenBlendHDA_mulmask;
		v9  = dest;
		v10 = (unsigned int *)src;
		v11 = &dest[len];
		if (dest < v11)
		{
			do
			{
				*v9 = _mm_cvtsi64_si32(
					_m_por(
						_m_pand(
							_m_pxor(
								_m_packuswb(
									_m_psrlwi(
										_m_pmullw(
											_m_punpcklbw(_m_pxor(_mm_cvtsi32_si64(*v9), v8), _mm_setzero_si64()),
											_m_por(
												_m_pand(
													_m_psrlwi(
														_m_pxor(
															_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(*v10), _mm_setzero_si64()), v6),
															(__m64)TVPScreenMulBlend_full_bit_aligned),
														8u),
													v7),
												v8)),
										8u),
									_mm_setzero_si64()),
								v8),
							v8),
						_m_pand(_mm_cvtsi32_si64(*v9), v7)));
				++v9;
				++v10;
			} while (v9 < v11);
		}
	}
	_m_empty();
}
