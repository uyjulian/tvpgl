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

void __cdecl TVPApplyColorMap65_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
{
	unsigned int *   v4;  // edi
	const tjs_uint8 *v5;  // ebp
	tjs_uint32 *     v6;  // edx
	__m64            v7;  // mm7
	__m64            v9;  // mm4
	__m64            v10; // mm1

	v4 = dest;
	v5 = src;
	v6 = &dest[len];
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(color), _mm_setzero_si64());
	while (v4 < v6)
	{
		v9  = _mm_set1_pi16(*v5);
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
		*v4 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v10, _m_psrawi(_m_pmullw(_m_psubw(v7, v10), v9), 6u)), _mm_setzero_si64()));
		++v4;
		++v5;
	}
	_m_empty();
}

void __cdecl TVPApplyColorMap65_d_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
{
	tjs_uint32 *     v4;  // edi
	const tjs_uint8 *v5;  // ebp
	tjs_uint32 *     v6;  // esi
	__m64            v7;  // mm7
	int              v8;  // eax
	__m64            v9;  // mm1
	__m64            v11; // mm4

	v4 = dest;
	v5 = src;
	v6 = &dest[len];
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
	while (v4 < v6)
	{
		v8  = (*v4 >> 24) + (*v5 << 8);
		v9  = _m_punpcklbw(_m_psrlqi(_m_psllqi(_mm_cvtsi32_si64(*v4), 0x28u), 0x28u), _mm_setzero_si64());
		v11 = _mm_set1_pi16(TVPOpacityOnOpacityTable65[v8]);
		*v4 = (TVPNegativeMulTable65[v8] << 24) | _mm_cvtsi64_si32(
													  _m_packuswb(
														  _m_psrlwi(
															  _m_paddw(_m_psllwi(v9, 8u), _m_pmullw(_m_psubw(v7, v9), v11)),
															  8u),
														  _mm_setzero_si64()));
		++v5;
		++v4;
	}
	_m_empty();
}

void __cdecl TVPApplyColorMap65_a_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
{
	unsigned int *   v4;  // edi
	const tjs_uint8 *v5;  // ebp
	tjs_uint32 *     v6;  // esi
	__m64            v7;  // mm7
	__m64            v9;  // mm3
	__m64            v10; // mm3
	__m64            v11; // mm1

	v4 = dest;
	v5 = src;
	v6 = &dest[len];
	v7 = _m_por(_m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64()), _m_psllqi(_mm_cvtsi32_si64(0x100u), 0x30u));
	while (v4 < v6)
	{
		v9  = _mm_set1_pi16(*v5);
		v10 = v9;
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
		*v4 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v10), 6u)), _m_psrlwi(_m_pmullw(v10, v7), 6u)),
				_mm_setzero_si64()));
		++v5;
		++v4;
	}
	_m_empty();
}
