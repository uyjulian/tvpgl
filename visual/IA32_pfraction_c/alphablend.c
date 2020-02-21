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

void __cdecl TVPAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int      v6; // eax
	__m64             v8; // mm4
	__m64             v9; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v6 = src[i];
		if (src[i] < 0xFF000000)
		{
			v8  = _mm_set1_pi16(v6 >> 24);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
			dest[i] = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddb(v9, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v6), _mm_setzero_si64()), v9), v8), 8u)),
					_mm_setzero_si64()));
		}
		else
		{
			dest[i] = v6;
		}
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64         v8; // mm4
	__m64         v9; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_set1_pi16((unsigned int)opa * (tjs_uint64)src[i] >> 32);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_paddw(_m_psllwi(v9, 8u), _m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v9), v8)),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int v4; // eax
	__m64        v6; // mm2
	__m64        v7; // mm4
	unsigned int v8; // eax
	__m64        v9; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v4       = src[i];
		v6       = _mm_cvtsi32_si64(v4);
		v7       = _mm_set1_pi16(v4 >> 24);
		v8       = dest[i];
		v9       = _m_punpcklbw(_mm_cvtsi32_si64(v8), _mm_setzero_si64());
		dest[i] = v8 & 0xFF000000 | _mm_cvtsi64_si32(
										 _m_packuswb(
											 _m_psrlwi(
												 _m_paddw(
													 _m_psllwi(v9, 8u),
													 _m_pmullw(_m_psubw(_m_punpcklbw(v6, _mm_setzero_si64()), v9), v7)),
												 8u),
											 _mm_setzero_si64())) &
										 0xFFFFFF;
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_d_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64             v5;  // mm7
	int               v7;  // eax
	__m64             v8;  // mm1
	__m64             v10; // mm4

	v5 = _mm_cvtsi32_si64(0xFFFFFFu);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = (dest[i] >> 24) + ((src[i] >> 16) & 0xFF00);
		v8  = _m_punpcklbw(_m_pand(_mm_cvtsi32_si64(dest[i]), v5), _mm_setzero_si64());
		v10 = _mm_set1_pi16(TVPOpacityOnOpacityTable[v7]);
		dest[i] = (TVPNegativeMulTable[v7] << 24) | _mm_cvtsi64_si32(
													_m_packuswb(
														_m_psrlwi(
															_m_paddw(
																_m_psllwi(v8, 8u),
																_m_pmullw(
																	_m_psubw(
																		_m_punpcklbw(_m_pand(_mm_cvtsi32_si64(src[i]), v5), _mm_setzero_si64()),
																		v8),
																	v10)),
															8u),
														_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	__m64   v6; // mm4
	__m64   v8; // mm1
	__m64   v9; // mm2

	v6 = _mm_set1_pi16(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9 = _m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64()), v8);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(_m_psllwi(v8, 8u), _m_pmullw(v9, v6)), 8u), _mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPConstAlphaBlend_SD_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)
{
	__m64   v7;  // mm4
	__m64   v9;  // mm1
	__m64   v10; // mm1

	v7 = _mm_set1_pi16(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(src1[i]), _mm_setzero_si64());
		v10 = _m_psrlwi(
			_m_paddw(_m_psllwi(v9, 8u), _m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[i]), _mm_setzero_si64()), v9), v7)),
			8u);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(v10, _mm_setzero_si64()));
	}
	_m_empty();
}
