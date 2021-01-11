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
	__m64         v11; // mm3
	__m64         v13; // mm0
	__m64         v15; // mm5
	__m64         v16; // mm1
	__m64         v17; // [esp+0h] [ebp-28h]

	v11 = _mm_set1_pi16(opa);
	v13 = _m_por(_mm_cvtsi32_si64(sx), _m_psllqi(_mm_cvtsi32_si64(sy), 0x20u));
	v17 = _m_por(_mm_cvtsi32_si64(stepx), _m_psllqi(_mm_cvtsi32_si64(stepy), 0x20u));
	for (tjs_int i = 0; i < len; i += 1)
	{
		v15 = _m_psradi(v13, 0x10u);
		v16 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(*(const tjs_uint32 *)((char *)&src[_mm_cvtsi64_si32(v15)] + srcpitch * _mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u))));
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v16);
		k = _m_pmullw(k, v11);
		k = _m_paddw(_m_psllwi(v16, 8u), k);
		k = _m_psrlwi(k, 8u);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
		v13 = _m_paddd(v13, v17);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	tjs_uint32 *v12; // eax
	__m64         v14; // mm5
	__m64         v17; // mm7
	__m64         v18; // mm1
	__m64         v19; // mm3
	__m64         v21; // mm1
	__m64         v22; // mm3
	__m64         v23; // mm2
	__m64         v24; // mm3

	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		v12 = (tjs_uint32 *)((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		v14 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		v17 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)v12 + srcpitch)), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(v12[1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v18);
		k = _m_pmullw(k, v14);
		k = _m_psrlwi(k, 8u);
		k = _m_paddb(v18, k);
		__m64 l = _mm_cvtsi32_si64(*(tjs_uint32 *)((char *)v12 + srcpitch + 4));
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v19);
		l = _m_pmullw(l, v14);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v19, l);
		l = _m_psubw(l, k);
		l = _m_pmullw(l, v17);
		l = _m_psrlwi(l, 8u);
		v21 = _m_paddb(k, l);
		v22 = _m_psrlqi(v21, 0x30u);
		v23 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v24 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v22));
		__m64 m = _m_pmullw(v23, v24);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(v23, m);
		m = _m_paddw(v21, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	__m64         v10; // mm6
	tjs_uint32 *v16; // eax
	__m64         v19; // mm5
	__m64         v21; // mm7
	__m64         v22; // mm1
	__m64         v23; // mm3
	__m64         v24; // mm1
	__m64         v25; // mm1
	__m64         v26; // mm3
	__m64         v27; // mm2
	__m64         v28; // mm3

	v10 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		v16 = (tjs_uint32 *)((char *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		v19 = _mm_set1_pi16((tjs_uint32)(tjs_uint16)i2 >> 8);
		v21 = _mm_set1_pi16(((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15));
		v22 = _m_punpcklbw(_mm_cvtsi32_si64(*v16), _mm_setzero_si64());
		v23 = _m_punpcklbw(_mm_cvtsi32_si64(*(tjs_uint32 *)((char *)v16 + srcpitch)), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(v16[1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v22);
		k = _m_pmullw(k, v19);
		k = _m_psrlwi(k, 8u);
		v24 = _m_paddb(v22, k);
		__m64 l = _mm_cvtsi32_si64(*(tjs_uint32 *)((char *)v16 + srcpitch + 4));
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v23);
		l = _m_pmullw(l, v19);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v23, l);
		l = _m_psubw(l, v24);
		l = _m_pmullw(l, v21);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v24, l);
		l = _m_pmullw(l, v10);
		v25 = _m_psrlwi(l, 8u);
		v26  = _m_psrlqi(v25, 0x30u);
		v27  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v28  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v26));
		__m64 m = _m_pmullw(v27, v28);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(v27, m);
		m = _m_paddw(v25, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}
