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
#include "tvpgl_ia32_intf_pfraction_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPFillARGB_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = value;
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPFillARGB_sse_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = value;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPFillARGB_NC_sse_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = value;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPFillColor_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = color & 0xFFFFFF | dest[i] & 0xFF000000;
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstColorAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	__m64         v6;  // mm6
	__m64         v8;  // mm7
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v13; // mm2
	__m64         v16; // mm3

	v6  = _mm_set1_pi32(0xFF000000);
	v8  = _mm_set1_pi32(0xFFFFFFu);
	v10 = _mm_set1_pi16(opa);
	v11 = _m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color), _mm_setzero_si64()), v10);
	v13 = _mm_set1_pi16((tjs_uint8) ~(tjs_uint8)opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v16 = _mm_cvtsi32_si64(dest[i]);
		dest[i] = _mm_cvtsi64_si32(
			_m_por(
				_m_pand(
					_m_packuswb(_m_psrlwi(_m_paddusw(_m_pmullw(_m_punpcklbw(v16, _mm_setzero_si64()), v13), v11), 8u), _mm_setzero_si64()),
					v8),
				_m_pand(v16, v6)));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstColorAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	__m64       v7;  // mm7
	tjs_int     v8;  // esi
	__m64       v9;  // mm1
	tjs_uint32  v10; // edx
	__m64       v12; // mm4

	v7 = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
	v8 = opa << 8;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v9         = _m_punpcklbw(_mm_cvtsi32_si64(dest[i] & 0xFFFFFF), _mm_setzero_si64());
		v10        = dest[i] >> 24;
		v12 = _mm_set1_pi16(TVPOpacityOnOpacityTable[v8 + v10]);
		dest[i] = (TVPNegativeMulTable[v8 + v10] << 24) | _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(_m_pmullw(_m_psubw(v7, v9), v12), _m_psllwi(v9, 8u)), 8u), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstColorAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	unsigned int  v6;  // esi
	__m64         v8;  // mm7
	__m64         v10; // mm6
	__m64         v11; // mm1

	v6  = ((unsigned int)opa >> 7) + opa;
	v8  = _mm_set1_pi16(v6);
	v10 = _m_por(
		_m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64()), v8), 8u),
		_m_punpcklbw(_mm_cvtsi32_si64(v6 << 24), _mm_setzero_si64()));
	for (tjs_int i = 0; i < len; i += 1)
	{
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v8), 8u)), v10), _mm_setzero_si64()));
	}
	_m_empty();
}
