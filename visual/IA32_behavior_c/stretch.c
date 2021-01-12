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
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
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
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
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
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64 v9  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 v12 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		__m64 k = src1_unpacked_1;
		k = _m_psubw(k, src1_unpacked_0); // k -= src1_unpacked_0
		k = _m_pmullw(k, v20); // k *= v20
		k = _m_psrlwi(k, 8u); // k >>= 8
		k = _m_paddb(k, src1_unpacked_0); // k += src1_unpacked_0
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 l = src2_unpacked_1;
		l = _m_psubw(l, src2_unpacked_0); // l -= src2_unpacked_0
		l = _m_pmullw(l, v20); // l *= v20
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, src2_unpacked_0); // k += src2_unpacked_0
		l = _m_psubw(l, k); // l -= k
		l = _m_pmullw(l, v9); // l *= v9
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		l = _m_psubw(l, dest_unpacked); // l -= dest_unpacked
		l = _m_pmullw(l, v12); // l *= v12
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, dest_unpacked); // l += dest_unpacked
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
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
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
		l = _m_pmullw(l, v8); // l *= v8
		l = _m_psrlwi(l, 8u); // l >>= 8
		l = _m_paddb(l, k); // l += k
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
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
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64 blend_y_m64  = _mm_set1_pi16(((tjs_uint32)blend_y >> 7) + blend_y);
	__m64 opa_m64 = _mm_set1_pi16(((tjs_uint32)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		__m64 src1_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src1[ j >> 16     ]), _mm_setzero_si64());
		__m64 src1_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64());
		__m64 src2_unpacked_0 = _m_punpcklbw(_mm_cvtsi32_si64(src2[ j >> 16     ]), _mm_setzero_si64());
		__m64 src2_unpacked_1 = _m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64());
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
		__m64 dest_unpacked = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
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
}
