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
			k &= 0xFFFF;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, src1, src2, dest);
#else
	__m64            v12; // mm4
	__m64            v13; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v12 = _mm_set1_pi16(*((const tjs_uint16 *)table + rule[i]));
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src2[i]), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(src1[i]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v13);
		k = _m_pmulhw(k, v12);
		k = _m_psllwi(k, 1u);
		k = _m_paddw(v13, k);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPUnivTransBlend_switch_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
#if 0
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
#elif 0
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
#else
	__m64            v14; // mm4
	__m64            v15; // mm1
	tjs_int ze = 1;
	tjs_int ze2 = 0;

	for (tjs_int i = 0; i < len; i += 1)
	{
		if (ze2 == 1)
		{
			ze2 = 0;
			dest[i] = src1[i];
			continue;
		}
		if (ze2 == 2)
		{
			ze2 = 0;
			dest[i] = src2[i];
			continue;
		}
		if (ze)
		{
			if ((rule[i + 1] >= src1lv) && (rule[i] < src1lv))
			{
				ze = 0;
			}
			else if (rule[i + 1] >= src1lv)
			{
				dest[i] = src1[i];
				ze2 = 1;
				continue;
			}
			else if (rule[i + 1] >= src2lv)
			{
				ze = 0;
			}
			else if (rule[i] >= src2lv)
			{
				ze = 0;
			}
			else
			{
				dest[i] = src2[i];
				ze2 = 2;
				continue;
			}
		}
		else
		{
			ze = 1;
		}
		v14 = _mm_set1_pi16(*((const tjs_uint16 *)table + rule[i]));
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[i]), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(src1[i]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v15);
		k = _m_pmulhw(k, v14);
		k = _m_psllwi(k, 1u);
		k = _m_paddw(v15, k);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}
