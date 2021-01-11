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

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	FOREACH_CHANNEL_COLOR(
		{
			tjs_int16 k = c[j];
			k -= d[j];
			k *= src[i];
			k >>= 6u;
			k += d[j];
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, color, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_d_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
#if 0
	// XXX: this doesn't match the MMX version
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint16 l = src[i];
			l <<= 8;
			l += *da;
			if (j % 4 == 3)
			{
				d[j] = TVPNegativeMulTable65[l];
				continue;
			}
			tjs_uint16 k = c[j];
			k -= d[j];
			k *= TVPOpacityOnOpacityTable65[l];
			tjs_uint16 m = d[j];
			m <<= 8u;
			k += m;
			k >>= 8u;
			k += d[j];
			d[j] = k;
		}, len, color, dest);
#else
	__m64            v7;  // mm7
	tjs_int32        v8;  // eax
	__m64            v9;  // mm1
	__m64            v11; // mm4

	v7 = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = (dest[i] >> 24) + (src[i] << 8);
		__m64 k = _mm_cvtsi32_si64(dest[i]);
		k = _m_psllqi(k, 0x28u);
		k = _m_psrlqi(k, 0x28u);
		v9  = _m_punpcklbw(k, _mm_setzero_si64());
		v11 = _mm_set1_pi16(TVPOpacityOnOpacityTable65[v8]);
		__m64 l = _m_psllwi(v9, 8u);
		__m64 m = _m_psubw(v7, v9);
		m = _m_pmullw(m, v11);
		m = _m_paddw(l, m);
		m = _m_psrlwi(m, 8u);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = (TVPNegativeMulTable65[v8] << 24) | _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_a_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
#if 0
	// XXX: this doesn't match the MMX version
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint16 k = c[j];
			if (j % 4 == 3)
			{
				k = 1;
			}
			k *= src[i];
			k >>= 6u;
			tjs_uint16 l = src[i];
			l *= d[j];
			l >>= 6u;
			tjs_uint16 m = d[j];
			m -= l;
			k += m;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, color, dest);
#else
	__m64            v7;  // mm7
	__m64            v9;  // mm3
	__m64            v11; // mm1

	v7 = _m_por(_m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64()), _m_psllqi(_mm_cvtsi32_si64(0x100u), 0x30u));
	for (tjs_int i = 0; i < len; i += 1)
	{
		v9  = _mm_set1_pi16(src[i]);
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = _m_pmullw(v11, v9);
		k = _m_psrlwi(k, 6u);
		k = _m_psubw(v11, k);
		__m64 l = _m_pmullw(v9, v7);
		l = _m_psrlwi(l, 6u);
		l = _m_paddw(k, l);
		l = _m_packuswb(l, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(l);
	}
	_m_empty();
#endif
}
