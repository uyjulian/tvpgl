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

TVP_GL_IA32_FUNC_DECL(void, TVPStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	for (tjs_int i = 0, i2 = srcstart; i < len; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s = (const tjs_uint8 *)&src[i2 >> 16];
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
	__m64 opa_m64  = _mm_set1_pi16(opa);
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 src_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(src[j >> 16]), _mm_setzero_si64());
		__m64 k = src_unpacked;
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

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	for (tjs_int i = 0, i2 = srcstart; i < len; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s = (const tjs_uint8 *)&src[i2 >> 16];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint16 alpha = src[i2 >> 16] >> 24;
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= alpha;
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
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v11 = _mm_set1_pi16(src[j >> 16] >> 24);
		__m64 src_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(src[j >> 16]), _mm_setzero_si64());
		__m64 k = src_unpacked;
		k = _m_psubw(k, dest_unpacked); // k -= dest_unpacked
		k = _m_pmullw(k, v11); // k *= v11
		k = _m_paddw(k, _m_psllwi(dest_unpacked, 8u)); // k += (dest_unpacked << 8)
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	for (tjs_int i = 0, i2 = srcstart; i < len; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s = (const tjs_uint8 *)&src[i2 >> 16];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint16 alpha = src[i2 >> 16] >> 24;
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = d[j];
			k *= alpha;
			k >>= 8;
			k = d[j] - k;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			k += s[j];
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
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 v10 = _mm_set1_pi16(src[j >> 16] >> 24);
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = dest_unpacked;
		k = _m_pmullw(k, v10); // k *= v10
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_psubw(dest_unpacked, k); // k = dest_unpacked - k
		k = _m_packuswb(k, _mm_setzero_si64());
		k = _m_paddusb(k, _mm_cvtsi32_si64(src[j >> 16])); // k += src[j >> 16] (saturated)
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
#if 0
	// XXX: doesn't match MMX version…
	tjs_uint16 blend_y_m64 = ((tjs_uint32)blend_y >> 7) + blend_y;
	tjs_uint16 opa_m64 = ((tjs_uint32)opa >> 7) + opa;
	for (tjs_int i = 0, i2 = srcstart; i < destlen; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s1_0 = (const tjs_uint8 *)&src1[(i2 >> 16) + 0];
		const tjs_uint8 *s1_1 = (const tjs_uint8 *)&src1[(i2 >> 16) + 1];
		const tjs_uint8 *s2_0 = (const tjs_uint8 *)&src2[(i2 >> 16) + 0];
		const tjs_uint8 *s2_1 = (const tjs_uint8 *)&src2[(i2 >> 16) + 1];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint16 v20 = (tjs_uint16)i2 >> 8;
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1_1[j];
			k -= s1_0[j];
			k *= v20;
			k >>= 8;
			k += s1_0[j];
			tjs_uint16 l = s2_1[j];
			l -= s2_0[j];
			l *= v20;
			l >>= 8;
			l += s2_0[j];
			l -= k;
			l *= blend_y_m64;
			l >>= 8;
			l += k;
			l -= d[j];
			l *= opa_m64;
			l >>= 8;
			l += d[j];
			
			if (l & (1 << 15))
			{
				l = 0;
			}
			if (l & (~0xFF))
			{
				l = 0xFF;
			}
			d[j] = l; 
		}
	}
#endif
#if 1
	__m64 blend_y_m64  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 opa_m64 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = src1_unpacked_1;
		k = _m_psubw(k, src1_unpacked_0); // k -= src1_unpacked_0
		k = _m_pmullw(k, v20); // k *= v20
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src1_unpacked_0); // k += src1_unpacked_0
		__m64 l = src2_unpacked_1;
		l = _m_psubw(l, src2_unpacked_0); // l -= src2_unpacked_0
		l = _m_pmullw(l, v20); // l *= v20
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src2_unpacked_0); // k += src2_unpacked_0
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, blend_y_m64); // l *= blend_y_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		l = _m_psubw(l, dest_unpacked); // l -= dest_unpacked
		l = _m_pmullw(l, opa_m64); // l *= opa_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, dest_unpacked); // l += dest_unpacked
		l = _m_packuswb(l, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(l);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
#if 1
	tjs_uint32 blend_y_m64 = ((tjs_uint32)blend_y >> 7) + blend_y;
	for (tjs_int i = 0, i2 = srcstart; i < destlen; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s1_0 = (const tjs_uint8 *)&src1[(i2 >> 16) + 0];
		const tjs_uint8 *s1_1 = (const tjs_uint8 *)&src1[(i2 >> 16) + 1];
		const tjs_uint8 *s2_0 = (const tjs_uint8 *)&src2[(i2 >> 16) + 0];
		const tjs_uint8 *s2_1 = (const tjs_uint8 *)&src2[(i2 >> 16) + 1];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint16 v27 = (tjs_uint16)i2 >> 8;
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1_1[j];
			k -= s1_0[j];
			k *= v27;
			k >>= 8;
			k += s1_0[j];
			tjs_uint16 l = s2_1[j];
			l -= s2_0[j];
			l *= v27;
			l >>= 8;
			l += s2_0[j];
			l -= k;
			l *= blend_y_m64;
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
	__m64 blend_y_m64  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v17 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = src1_unpacked_1;
		k = _m_psubw(k, src1_unpacked_0); // k -= src1_unpacked_0
		k = _m_pmullw(k, v17); // k *= v17
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src1_unpacked_0); // k += src1_unpacked_0
		__m64 l = src2_unpacked_1;
		l = _m_psubw(l, src2_unpacked_0); // l -= src2_unpacked_0
		l = _m_pmullw(l, v17); // l *= v17
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src2_unpacked_0); // l += src2_unpacked_0
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, blend_y_m64); // l *= blend_y_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		__m64 v22 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(_m_psrlqi(l, 0x30u)));
		__m64 m = dest_unpacked;
		m = _m_pmullw(m, v22); // m *= v22
		m = _m_psrlwi(m, 8u); // m >>= 8
		m = _m_psubw(dest_unpacked, m); // m = dest_unpacked - m
		m = _m_paddw(m, l); // m += l
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
#if 0
	// XXX: doesn't match MMX version…
	tjs_uint16 blend_y_m64 = ((tjs_uint32)blend_y >> 7) + blend_y;
	tjs_uint16 opa_m64 = ((tjs_uint32)opa >> 7) + opa;
	for (tjs_int i = 0, i2 = srcstart; i < destlen; i += 1, i2 += srcstep)
	{
		const tjs_uint8 *s1_0 = (const tjs_uint8 *)&src1[(i2 >> 16) + 0];
		const tjs_uint8 *s1_1 = (const tjs_uint8 *)&src1[(i2 >> 16) + 1];
		const tjs_uint8 *s2_0 = (const tjs_uint8 *)&src2[(i2 >> 16) + 0];
		const tjs_uint8 *s2_1 = (const tjs_uint8 *)&src2[(i2 >> 16) + 1];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint16 v20 = (tjs_uint16)i2 >> 8;
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1_1[j];
			k -= s1_0[j];
			k *= v20;
			k >>= 8;
			k += s1_0[j];
			tjs_uint16 l = s2_1[j];
			l -= s2_0[j];
			l *= v20;
			l >>= 8;
			l += s2_0[j];
			l -= k;
			l *= blend_y_m64;
			l >>= 8;
			l += k;
			l *= opa_m64;
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
#if 1
	__m64 blend_y_m64  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 opa_m64 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = src1_unpacked_1;
		k = _m_psubw(k, src1_unpacked_0); // k -= src1_unpacked_0
		k = _m_pmullw(k, v20); // k *= v20
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src1_unpacked_0); // k += src1_unpacked_0
		__m64 l = src2_unpacked_1;
		l = _m_psubw(l, src2_unpacked_0); // l -= src2_unpacked_0
		l = _m_pmullw(l, v20); // l *= v20
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src2_unpacked_0); // l += src2_unpacked_0
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, blend_y_m64); // l *= blend_y_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		l = _m_pmullw(l, opa_m64); // l *= opa_m64
		l = _m_psrlwi(l, 8u); // l >>= 8
		__m64 v26 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(_m_psrlqi(l, 0x30u)));
		__m64 m = dest_unpacked;
		m = _m_pmullw(m, v26); // m *= v26
		m = _m_psrlwi(m, 8u); // m >>= 8
		m = _m_psubw(dest_unpacked, m); // m = dest_unpacked - m
		m = _m_paddw(m, l); // m += l
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}
