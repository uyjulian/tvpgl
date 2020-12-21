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
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPAdjustGamma_a_mmx_c(tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp)
{
	tjs_uint32 * v3;  // edi
	tjs_uint32 * v4;  // esi
	unsigned int v5;  // eax
	__m64        v6;  // mm2
	__m64        v9;  // mm1
	__m64        v10; // mm3
	__m64        v11; // mm3
	__m64        v12; // mm6
	unsigned int v13; // eax

	v3 = dest;
	v4 = &dest[len];
	if (dest < v4)
	{
		do
		{
			while (1)
			{
				v5 = *v3;
				if (*v3 < 0xFF000000)
					break;
				*v3 = (temp->R[(v5 >> 16) & 0xFF] << 16) | (temp->G[BYTE1(v5)] << 8) | temp->B[(tjs_uint8)v5] | 0xFF000000;
			_TVPAdjustGamma_a_mmx_pfraction_a_ptransp:
				++v3;
				if (v3 >= v4)
					goto _TVPAdjustGamma_a_mmx_pfraction_a_pexit;
			}
			if (!v5)
				goto _TVPAdjustGamma_a_mmx_pfraction_a_ptransp;
			v6  = _m_punpcklbw(_mm_cvtsi32_si64(v5), _mm_setzero_si64());
			v9  = _mm_set1_pi16(TVPRecipTable256_16[v5 >> 24]);
			v10 = _mm_set1_pi16(v5 >> 24);
			v11 = v10;
			v12 = _mm_cvtsi32_si64(v5 & 0xFF000000);
			v13 = _mm_cvtsi64_si32(_m_packuswb(_m_por(_m_psrlwi(_m_pmullw(v9, v6), 8u), _m_psrlwi(_m_pcmpgtw(v6, v11), 8u)), _mm_setzero_si64()));
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_por(
					_m_packuswb(
						_m_paddw(
							_m_psrlwi(
								_m_pmullw(
									_m_punpcklbw(
										_mm_cvtsi32_si64((temp->R[(v13 >> 16) & 0xFF] << 16) | (temp->G[BYTE1(v13)] << 8) | (unsigned int)temp->B[(tjs_uint8)v13]),
										_mm_setzero_si64()),
									_m_paddw(v11, _m_psrlwi(v11, 7u))),
								8u),
							_m_psubusb(v6, v11)),
						_mm_setzero_si64()),
					v12));
		} while (v3 < v4);
	}
_TVPAdjustGamma_a_mmx_pfraction_a_pexit:
	_m_empty();
}
