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
	__m64 opa_m64 = _mm_set1_pi16(opa);
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 k = _mm_cvtsi32_si64(*src_start);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, dest_unpacked);
		k = _m_pmullw(k, opa_m64);
		k = _m_paddw(_m_psllwi(dest_unpacked, 8u), k);
		k = _m_psrlwi(k, 8u);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 v14 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		__m64 v17 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		__m64 v18 = _m_punpcklbw(_mm_cvtsi32_si64(*src_start), _mm_setzero_si64());
		__m64 v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch)), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(src_start[1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v18);
		k = _m_pmullw(k, v14);
		k = _m_psrlwi(k, 8u);
		k = _m_paddb(v18, k);
		__m64 l = _mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch + 4));
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v19);
		l = _m_pmullw(l, v14);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v19, l);
		l = _m_psubw(l, k);
		l = _m_pmullw(l, v17);
		l = _m_psrlwi(l, 8u);
		__m64 v21 = _m_paddb(k, l);
		__m64 v22 = _m_psrlqi(v21, 0x30u);
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v24 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v22));
		__m64 m = _m_pmullw(dest_unpacked, v24);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(dest_unpacked, m);
		m = _m_paddw(v21, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	__m64 opa_sevenbit = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		__m64 v19 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		__m64 v21 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		__m64 v22 = _m_punpcklbw(_mm_cvtsi32_si64(*src_start), _mm_setzero_si64());
		__m64 v23 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch)), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(src_start[1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v22);
		k = _m_pmullw(k, v19);
		k = _m_psrlwi(k, 8u);
		__m64 v24 = _m_paddb(v22, k);
		__m64 l = _mm_cvtsi32_si64(*(tjs_uint32 *)((char *)src_start + srcpitch + 4));
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v23);
		l = _m_pmullw(l, v19);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v23, l);
		l = _m_psubw(l, v24);
		l = _m_pmullw(l, v21);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v24, l);
		l = _m_pmullw(l, opa_sevenbit);
		__m64 v25 = _m_psrlwi(l, 8u);
		__m64 v26 = _m_psrlqi(v25, 0x30u);
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v28 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v26));
		__m64 m = _m_pmullw(dest_unpacked, v28);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(dest_unpacked, m);
		m = _m_paddw(v25, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}
