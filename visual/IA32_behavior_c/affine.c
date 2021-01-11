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

TVP_GL_IA32_FUNC_DECL(void, TVPLinTransConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));

		const tjs_uint8 *s = (const tjs_uint8 *)src_start;
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= opa;
			k += (d[j] << 8);
			k >>= 8;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k; 
		}
	}
#if 0
	__m64 opa_m64 = _mm_set1_pi16(opa);
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 k = _mm_cvtsi32_si64(*src_start);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, dest_unpacked); // k -= dest_unpacked
		k = _m_pmullw(k, opa_m64); // k *= opa_m64
		k = _m_paddw(k, _m_psllwi(dest_unpacked, 8u)); // k += (dest_unpacked << 8)
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
#if 1
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		tjs_uint16 sx_m64 = (tjs_uint16)i2 >> 8;
		tjs_uint16 sy_m64 = ((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15);

		const tjs_uint8 *s0 = (const tjs_uint8 *)(src_start    );
		const tjs_uint8 *s1 = (const tjs_uint8 *)(src_start + 1);
		const tjs_uint8 *sp0 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch))    );
		const tjs_uint8 *sp1 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch)) + 1);
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1[j];
			k -= s0[j];
			k *= sx_m64;
			k >>= 8;
			k += s0[j];
			tjs_uint16 l = sp1[j];
			l -= sp0[j];
			l *= sx_m64;
			l >>= 8;
			l += sp0[j];
			l -= k;
			l *= sy_m64;
			l >>= 8;
			l += k;
			vals_tmp[j] = l;
		}
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = vals_tmp[3]; // alpha
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += vals_tmp[j];
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k; 
		}
	}
#endif
#if 0
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 sx_m64 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		__m64 sy_m64 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		__m64 src_start_0 = _m_punpcklbw(_mm_cvtsi32_si64(*src_start), _mm_setzero_si64());
		__m64 src_start_1 = _m_punpcklbw(_mm_cvtsi32_si64(src_start[1]), _mm_setzero_si64());
		__m64 src_start_pitch_0 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch)), _mm_setzero_si64());
		__m64 src_start_pitch_1 = _m_punpcklbw( _mm_cvtsi32_si64(((tjs_uint32 *)((char *)src_start + srcpitch))[1]), _mm_setzero_si64());
		__m64 k = src_start_1;
		k = _m_psubw(k, src_start_0); // k -= src_start_0
		k = _m_pmullw(k, sx_m64); // k *= sx_m64
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src_start_0); // k += src_start_0
		__m64 l = src_start_pitch_1;
		l = _m_psubw(l, src_start_pitch_0); // l -= src_start_pitch_0
		l = _m_pmullw(l, sx_m64); // l *= sx_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src_start_pitch_0); // l += src_start_pitch_0
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, sy_m64); // l *= sy_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 alpha = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(_m_psrlqi(l, 0x30u))); // v21 >> 0x30
		__m64 m = _m_pmullw(dest_unpacked, alpha);
		m = _m_psrlwi(m, 8u); // m >>= 8
		m = _m_psubw(dest_unpacked, m);
		m = _m_paddw(m, l); // m += l
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
#if 1
	tjs_uint16 opa_sevenbit = ((tjs_uint32)opa >> 7) + opa;
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		tjs_uint16 sx_m64 = (tjs_uint16)i2 >> 8;
		tjs_uint16 sy_m64 = ((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15);

		const tjs_uint8 *s0 = (const tjs_uint8 *)(src_start    );
		const tjs_uint8 *s1 = (const tjs_uint8 *)(src_start + 1);
		const tjs_uint8 *sp0 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch))    );
		const tjs_uint8 *sp1 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch)) + 1);
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1[j];
			k -= s0[j];
			k *= sx_m64;
			k >>= 8;
			k += s0[j];
			tjs_uint16 l = sp1[j];
			l -= sp0[j];
			l *= sx_m64;
			l >>= 8;
			l += sp0[j];
			l -= k;
			l *= sy_m64;
			l >>= 8;
			l += k;
			l *= opa_sevenbit;
			l >>= 8;
			vals_tmp[j] = l;
		}
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = vals_tmp[3]; // alpha
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += vals_tmp[j];
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k; 
		}
	}
#endif
#if 0
	__m64 opa_sevenbit = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 sx_m64 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		__m64 sy_m64 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		__m64 src_start_0 = _m_punpcklbw(_mm_cvtsi32_si64(*src_start), _mm_setzero_si64());
		__m64 src_start_1 = _m_punpcklbw(_mm_cvtsi32_si64(src_start[1]), _mm_setzero_si64());
		__m64 src_start_pitch_0 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch)), _mm_setzero_si64());
		__m64 src_start_pitch_1 = _m_punpcklbw(_mm_cvtsi32_si64(((tjs_uint32 *)((char *)src_start + srcpitch))[1]), _mm_setzero_si64());
		__m64 k = src_start_1;
		k = _m_psubw(k, src_start_0); // k -= src_start_0
		k = _m_pmullw(k, sx_m64); // k *= sx_m64
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src_start_0); // k += src_start_0
		__m64 l = src_start_pitch_1;
		l = _m_psubw(l, src_start_pitch_0); // l -= src_start_pitch_0
		l = _m_pmullw(l, sx_m64); // l *= sx_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src_start_pitch_0); // (l += src_start_pitch_0) & 0xff
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, sy_m64); // l *= sy_m64
		l = _m_psrlwi(l, 8u); // l >== 8
		l = _m_paddb(l, k); // l += k
		l = _m_pmullw(l, opa_sevenbit); // l *= opa_sevenbit
		l = _m_psrlwi(l, 8u); // l >>= 8
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 alpha = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(_m_psrlqi(l, 0x30u)));
		__m64 m = _m_pmullw(dest_unpacked, alpha);
		m = _m_psrlwi(m, 8u); // m >>= 8
		m = _m_psubw(dest_unpacked, m); // m = dest_unpacked - m
		m = _m_paddw(m, l); // m += l
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}
