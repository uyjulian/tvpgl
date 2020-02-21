/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

void __cdecl TVPFillARGB_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi

	v3 = dest;
	while (v3 < &dest[len])
	{
		*v3 = value;
		++v3;
	}
	_m_empty();
}

void __cdecl TVPFillARGB_sse_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi

	v3 = dest;
	while (v3 < &dest[len])
	{
		*v3 = value;
		++v3;
	}
}

void __cdecl TVPFillARGB_NC_sse_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi

	v3 = dest;
	while (v3 < &dest[len])
	{
		*v3 = value;
		++v3;
	}
}

void __cdecl TVPFillColor_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color)
{
	tjs_uint32 *v3; // edi

	v3 = dest;
	while (v3 < &dest[len])
	{
		*v3 = color & 0xFFFFFF | *v3 & 0xFF000000;
		++v3;
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
{
	unsigned int *v4;  // edi
	__m64         v5;  // mm6
	__m64         v6;  // mm6
	__m64         v7;  // mm7
	__m64         v8;  // mm7
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2
	__m64         v16; // mm3

	v4  = dest;
	v5  = _mm_cvtsi32_si64(0xFF000000);
	v6  = _m_punpckldq(v5, v5);
	v7  = _mm_cvtsi32_si64(0xFFFFFFu);
	v8  = _m_punpckldq(v7, v7);
	v10 = _mm_set1_pi16(opa);
	v11 = _m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color), _mm_setzero_si64()), v10);
	v13 = _mm_set1_pi16((tjs_uint8) ~(tjs_uint8)opa);
	v14 = v13;
	while (v4 < &dest[len])
	{
		v16 = _mm_cvtsi32_si64(*v4);
		*v4 = _mm_cvtsi64_si32(
			_m_por(
				_m_pand(
					_m_packuswb(_m_psrlwi(_m_paddusw(_m_pmullw(_m_punpcklbw(v16, _mm_setzero_si64()), v14), v11), 8u), _mm_setzero_si64()),
					v8),
				_m_pand(v16, v6)));
		++v4;
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_d_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
{
	tjs_int     v4;  // ecx
	tjs_uint32 *v5;  // edi
	__m64       v7;  // mm7
	tjs_int     v8;  // esi
	__m64       v9;  // mm1
	tjs_uint32  v10; // edx
	__m64       v12; // mm4

	v4 = len;
	v5 = dest;
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
	v8 = opa << 8;
	while (v5 < &dest[len])
	{
		v9         = _m_punpcklbw(_mm_cvtsi32_si64(*v5 & 0xFFFFFF), _mm_setzero_si64());
		v10        = *v5 >> 24;
		LOBYTE(v4) = TVPNegativeMulTable[v8 + v10];
		v4 <<= 24;
		v12 = _mm_set1_pi16(TVPOpacityOnOpacityTable[v8 + v10]);
		*v5 = v4 | _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(_m_pmullw(_m_psubw(v7, v9), v12), _m_psllwi(v9, 8u)), 8u), _mm_setzero_si64()));
		++v5;
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_a_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int  v6;  // esi
	__m64         v8;  // mm7
	__m64         v9;  // mm7
	__m64         v10; // mm6
	__m64         v11; // mm1

	v4  = dest;
	v6  = ((unsigned int)opa >> 7) + opa;
	v8  = _mm_set1_pi16(v6);
	v9  = v8;
	v10 = _m_por(
		_m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64()), v9), 8u),
		_m_punpcklbw(_mm_cvtsi32_si64(v6 << 24), _mm_setzero_si64()));
	while (v4 < &dest[len])
	{
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
		*v4 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v9), 8u)), v10), _mm_setzero_si64()));
		++v4;
	}
	_m_empty();
}
