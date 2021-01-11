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
	__m64 v9  = _mm_set1_pi16(opa);
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 v12 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = _mm_cvtsi32_si64(src[j >> 16]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v12);
		k = _m_pmullw(k, v9);
		k = _m_paddw(_m_psllwi(v12, 8u), k);
		k = _m_psrlwi(k, 8u);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 v8  = _mm_cvtsi32_si64(src[j >> 16]);
		__m64 v9  = _m_psrlqi(v8, 0x18u);
		__m64 v10 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		__m64 k = _m_punpcklbw(v8, _mm_setzero_si64());
		k = _m_psubw(k, v10);
		k = _m_pmullw(k, v11);
		k = _m_paddw(k, _m_psllwi(v10, 8u));
		k = _m_psrlwi(k, 8u);
		k = _m_packuswb(k, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		__m64 v8  = _mm_cvtsi32_si64(src[j >> 16]);
		__m64 v9  = _m_psrlqi(v8, 0x18u);
		__m64 v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		__m64 v11 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 k = _m_pmullw(v11, v10);
		k = _m_psrlwi(k, 8u);
		k = _m_psubw(v11, k);
		k = _m_packuswb(k, _mm_setzero_si64());
		k = _m_paddusb(k, v8);
		dest[i] = _mm_cvtsi64_si32(k);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64 v9  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 v12 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		__m64 v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		__m64 v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = _mm_cvtsi32_si64(src1[(j >> 16) + 1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v17);
		k = _m_pmullw(k, v20);
		k = _m_psrlwi(k, 8u);
		__m64 v22 = _m_paddb(v17, k);
		__m64 v23 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 l = _mm_cvtsi32_si64(src2[(j >> 16) + 1]);
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v19);
		l = _m_pmullw(l, v20);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v19, l);
		l = _m_psubw(l, v22);
		l = _m_pmullw(l, v9);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v22, l);
		l = _m_psubw(l, v23);
		l = _m_pmullw(l, v12);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v23, l);
		l = _m_packuswb(l, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(l);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
	__m64 v8  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 v13 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		__m64 v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		__m64 v17 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = _mm_cvtsi32_si64(src1[(j >> 16) + 1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v13);
		k = _m_pmullw(k, v17);
		k = _m_psrlwi(k, 8u);
		__m64 v18 = _m_paddb(v13, k);
		__m64 l = _mm_cvtsi32_si64(src2[(j >> 16) + 1]);
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v15);
		l = _m_pmullw(l, v17);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v15, l);
		l = _m_psubw(l, v18);
		l = _m_pmullw(l, v8);
		l = _m_psrlwi(l, 8u);
		__m64 v19 = _m_paddb(v18, l);
		__m64 v20 = _m_psrlqi(v19, 0x30u);
		__m64 v21 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v22 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v20));
		__m64 m = _m_pmullw(v21, v22);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(v21, m);
		m = _m_paddw(v19, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64 v9  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 v12 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		__m64 v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		__m64 v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = _mm_cvtsi32_si64(src1[(j >> 16) + 1]);
		k = _m_punpcklbw(k, _mm_setzero_si64());
		k = _m_psubw(k, v17);
		k = _m_pmullw(k, v20);
		k = _m_psrlwi(k, 8u);
		__m64 v22 = _m_paddb(v17, k);
		__m64 l = _mm_cvtsi32_si64(src2[(j >> 16) + 1]);
		l = _m_punpcklbw(l, _mm_setzero_si64());
		l = _m_psubw(l, v19);
		l = _m_pmullw(l, v20);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v19, l);
		l = _m_psubw(l, v22);
		l = _m_pmullw(l, v9);
		l = _m_psrlwi(l, 8u);
		l = _m_paddb(v22, l);
		l = _m_pmullw(l, v12);
		__m64 v23 = _m_psrlwi(l, 8u);
		__m64 v24 = _m_psrlqi(v23, 0x30u);
		__m64 v25 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 v26 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v24));
		__m64 m = _m_pmullw(v25, v26);
		m = _m_psrlwi(m, 8u);
		m = _m_psubw(v25, m);
		m = _m_paddw(v23, m);
		m = _m_packuswb(m, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
}
