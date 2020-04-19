/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPUnivTransBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len))
{
#if 1
	FOREACH_CHANNEL_2SRC(
		{
			tjs_uint32 k = s1[j];
			k -= s2[j];
			k *= *((const tjs_uint16 *)table + rule[i]);
			k >>= 16;
			k <<= 1;
			k += s2[j];
			d[j] = k;
		}, len, src1, src2, dest);
#else
	__m64            v12; // mm4
	__m64            v13; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v12 = _mm_set1_pi16(*((const tjs_uint16 *)table + rule[i]));
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src2[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(v13, _m_psllwi(_m_pmulhw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[i]), _mm_setzero_si64()), v13), v12), 1u)),
				_mm_setzero_si64()));
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPUnivTransBlend_switch_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
#if 1
	FOREACH_CHANNEL_2SRC(
		{
			if (rule[i] >= src1lv)
			{
				d[j] = s1[j];
				continue;
			}
			else if (rule[i] < src2lv)
			{
				d[j] = s2[j];
				continue;
			}
			tjs_uint32 k = s1[j];
			k -= s2[j];
			k *= *((const tjs_uint16 *)table + rule[i]);
			k >>= 16;
			k <<= 1;
			k += s2[j];
			d[j] = k;
		}, len, src1, src2, dest);
#else
	__m64            v14; // mm4
	__m64            v15; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		if (rule[i] >= src1lv)
		{
			dest[i] = src1[i];
			continue;
		}
		else if (rule[i] < src2lv)
		{
			dest[i] = src2[i];
			continue;
		}
		v14 = _mm_set1_pi16(*((const tjs_uint16 *)table + rule[i]));
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(v15, _m_psllwi(_m_pmulhw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[i]), _mm_setzero_si64()), v15), v14), 1u)),
				_mm_setzero_si64()));
	}
	_m_empty();
#endif
}
