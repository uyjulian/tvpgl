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
	unsigned int *    v3; // edi
	const tjs_uint32 *v4; // ebp
	tjs_uint32 *      v5; // esi
	unsigned int      v6; // eax
	__m64             v8; // mm4
	__m64             v9; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = src;
		v5 = &dest[len];
		if (dest < v5)
		{
			do
			{
				v6 = *v4;
				if (*v4 < 0xFF000000)
				{
					v8  = _mm_set1_pi16(v6 >> 24);
					v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
					*v3 = _mm_cvtsi64_si32(
						_m_packuswb(
							_m_paddb(v9, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v6), _mm_setzero_si64()), v9), v8), 8u)),
							_mm_setzero_si64()));
				}
				else
				{
					*v3 = v6;
				}
				++v3;
				++v4;
			} while (v3 < v5);
		}
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4; // edi
	unsigned int *v5; // ebp
	tjs_uint32 *  v6; // esi
	__m64         v8; // mm4
	__m64         v9; // mm1

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = &dest[len];
		if (dest < v6)
		{
			do
			{
				v8  = _mm_set1_pi16((unsigned int)opa * (tjs_uint64)*v5 >> 32);
				v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
				*v4 = _mm_cvtsi64_si32(
					_m_packuswb(
						_m_psrlwi(
							_m_paddw(_m_psllwi(v9, 8u), _m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64()), v9), v8)),
							8u),
						_mm_setzero_si64()));
				++v4;
				++v5;
			} while (v4 < v6);
		}
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	int          v3; // ecx
	unsigned int v4; // eax
	__m64        v6; // mm2
	__m64        v7; // mm4
	unsigned int v8; // eax
	__m64        v9; // mm1

	if (len - 1 >= 0)
	{
		v3 = len - 1;
		do
		{
			v4       = src[v3];
			v6       = _mm_cvtsi32_si64(v4);
			v7       = _mm_set1_pi16(v4 >> 24);
			v8       = dest[v3];
			v9       = _m_punpcklbw(_mm_cvtsi32_si64(v8), _mm_setzero_si64());
			dest[v3] = v8 & 0xFF000000 | _mm_cvtsi64_si32(
											 _m_packuswb(
												 _m_psrlwi(
													 _m_paddw(
														 _m_psllwi(v9, 8u),
														 _m_pmullw(_m_psubw(_m_punpcklbw(v6, _mm_setzero_si64()), v9), v7)),
													 8u),
												 _mm_setzero_si64())) &
											 0xFFFFFF;
			--v3;
		} while (v3 >= 0);
	}
	_m_empty();
}

void __cdecl TVPAlphaBlend_d_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	tjs_uint32 *      v3;  // edi
	const tjs_uint32 *v4;  // ebp
	__m64             v5;  // mm7
	tjs_uint32 *      v6;  // esi
	int               v7;  // eax
	__m64             v8;  // mm1
	__m64             v10; // mm4

	if (len > 0)
	{
		v3 = dest;
		v4 = src;
		v5 = _mm_cvtsi32_si64(0xFFFFFFu);
		v6 = &dest[len];
		if (dest < v6)
		{
			do
			{
				v7  = (*v3 >> 24) + ((*v4 >> 16) & 0xFF00);
				v8  = _m_punpcklbw(_m_pand(_mm_cvtsi32_si64(*v3), v5), _mm_setzero_si64());
				v10 = _mm_set1_pi16(TVPOpacityOnOpacityTable[v7]);
				*v3 = (TVPNegativeMulTable[v7] << 24) | _mm_cvtsi64_si32(
															_m_packuswb(
																_m_psrlwi(
																	_m_paddw(
																		_m_psllwi(v8, 8u),
																		_m_pmullw(
																			_m_psubw(
																				_m_punpcklbw(_m_pand(_mm_cvtsi32_si64(*v4), v5), _mm_setzero_si64()),
																				v8),
																			v10)),
																	8u),
																_mm_setzero_si64()));
				++v3;
				++v4;
			} while (v3 < v6);
		}
	}
	_m_empty();
}

void __cdecl TVPConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	tjs_int v5; // ebx
	__m64   v6; // mm4
	__m64   i;  // mm4
	__m64   v8; // mm1
	__m64   v9; // mm2

	if (len > 0)
	{
		v5 = 0;
		v6 = _mm_set1_pi16(opa);
		for (i = v6;
			 v5 < len;
			 dest[v5 - 1] = _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(_m_psllwi(v8, 8u), _m_pmullw(v9, i)), 8u), _mm_setzero_si64())))
		{
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(dest[v5]), _mm_setzero_si64());
			v9 = _m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src[v5]), _mm_setzero_si64()), v8);
			++v5;
		}
	}
	_m_empty();
}

void __cdecl TVPConstAlphaBlend_SD_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)
{
	tjs_int v6;  // ebx
	__m64   v7;  // mm4
	__m64   i;   // mm4
	__m64   v9;  // mm1
	__m64   v10; // mm1

	if (len > 0)
	{
		v6 = 0;
		v7 = _mm_set1_pi16(opa);
		for (i = v7; v6 < len; dest[v6 - 1] = _mm_cvtsi64_si32(_m_packuswb(v10, _mm_setzero_si64())))
		{
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(src1[v6]), _mm_setzero_si64());
			v10 = _m_psrlwi(
				_m_paddw(_m_psllwi(v9, 8u), _m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[v6]), _mm_setzero_si64()), v9), i)),
				8u);
			++v6;
		}
	}
	_m_empty();
}
