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

void __cdecl TVPAdjustGamma_a_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp)
{
	__m64        v6;  // mm2
	__m64        v9;  // mm1
	__m64        v10; // mm3
	__m64        v12; // mm6
	unsigned int v13; // eax

	for (tjs_int i = 0; i < len; i += 1)
	{
		if (!dest[i])
		{
			continue;
		}
		if (dest[i] >= 0xFF000000)
		{
			dest[i] = (temp->R[(dest[i] >> 16) & 0xFF] << 16) | (temp->G[(dest[i] >> 8) & 0xFF] << 8) | temp->B[dest[i] & 0xFF] | 0xFF000000;
			continue;
		}
		v6  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _mm_set1_pi16(TVPRecipTable256_16[dest[i] >> 24]);
		v10 = _mm_set1_pi16(dest[i] >> 24);
		v12 = _mm_cvtsi32_si64(dest[i] & 0xFF000000);
		v13 = _mm_cvtsi64_si32(_m_packuswb(_m_por(_m_psrlwi(_m_pmullw(v9, v6), 8u), _m_psrlwi(_m_pcmpgtw(v6, v10), 8u)), _mm_setzero_si64()));
		dest[i] = _mm_cvtsi64_si32(
			_m_por(
				_m_packuswb(
					_m_paddw(
						_m_psrlwi(
							_m_pmullw(
								_m_punpcklbw(
									_mm_cvtsi32_si64((temp->R[(v13 >> 16) & 0xFF] << 16) | (temp->G[(v13 >> 8) & 0xFF] << 8) | temp->B[v13 & 0xFF]),
									_mm_setzero_si64()),
								_m_paddw(v10, _m_psrlwi(v10, 7u))),
							8u),
						_m_psubusb(v6, v10)),
					_mm_setzero_si64()),
				v12));
	}
	_m_empty();
}
