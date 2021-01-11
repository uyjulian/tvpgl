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
#if 1
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint32 l = (dest[i] >> 24) + (src[i] << 8);
			if (j % 4 == 3)
			{
				d[j] = TVPNegativeMulTable65[l];
				continue;
			}
			tjs_uint16 m = d[j];
			m <<= 8u;
			tjs_uint16 k = c[j];
			k -= d[j];
			k *= TVPOpacityOnOpacityTable65[l];
			k += m;
			k >>= 8u;
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
	__m64 color_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		tjs_uint32 l = (dest[i] >> 24) + (src[i] << 8);
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i] & 0xFFFFFF), _mm_setzero_si64());
		__m64 m = dest_unpacked; 
		m = _m_psllwi(m, 8u); // m <<= 8
		__m64 k = color_unpacked;
		k = _m_psubw(k, dest_unpacked); // k -= d[j]
		k = _m_pmullw(k, _mm_set1_pi16(TVPOpacityOnOpacityTable65[l])); // k *= TVPOpacityOnOpacityTable65[l]
		k = _m_paddw(k, m); // k += m
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = (TVPNegativeMulTable65[l] << 24) | _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_a_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
#if 1
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint16 cc = c[j];
			if (j % 4 == 3)
			{
				cc = 0x0100;
			}
			tjs_uint16 k = d[j];
			k *= src[i];
			k >>= 6u;
			k = d[j] - k;
			tjs_uint16 l = src[i];
			l *= cc;
			l >>= 6u;
			l += k;
			if (l & (1 << 15))
			{
				l = 0;
			}
			if (l & (~0xFF))
			{
				l = 0xFF;
			}
			d[j] = l;
		}, len, color, dest);
#else
	__m64 color_unpacked = _m_por(_m_punpcklbw(_mm_cvtsi32_si64((color & 0xFFFFFF) | 0x01000000), _mm_setzero_si64()), _m_psllqi(_mm_cvtsi32_si64(0x100u), 0x30u));
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 src_m64 = _mm_set1_pi16(src[i]);
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = dest_unpacked;
		k = _m_pmullw(k, src_m64); // k *= src[i]
		k = _m_psrlwi(k, 6u); // k >>= 6
		k = _m_psubw(dest_unpacked, k); // k = dest_unpacked - k
		__m64 l = src_m64;
		l = _m_pmullw(l, color_unpacked); // l *= c[j]
		l = _m_psrlwi(l, 6u); // l >== 6
		l = _m_paddw(l, k); // l += k
		l = _m_packuswb(l, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(l);
	}
	_m_empty();
#endif
}
