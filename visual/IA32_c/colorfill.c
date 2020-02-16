/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPFillARGB_mmx_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi
	tjs_uint32 *v4; // ebx

	if (len > 0)
	{
		v3 = dest;
		v4 = &dest[len];
		if (dest < v4)
		{
			do
			{
				*v3 = value;
				++v3;
			} while (v3 < v4);
		}
	}
	_m_empty();
}

void __cdecl TVPFillARGB_sse_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi
	tjs_uint32 *v4; // ebx

	if (len > 0)
	{
		v3 = dest;
		v4 = &dest[len];
		if (dest < v4)
		{
			do
			{
				*v3 = value;
				++v3;
			} while (v3 < v4);
		}
	}
}

void __cdecl TVPFillARGB_NC_sse_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 value)
{
	tjs_uint32 *v3; // edi
	tjs_uint32 *v4; // ebx

	if (len > 0)
	{
		v3 = dest;
		v4 = &dest[len];
		if (dest < v4)
		{
			do
			{
				*v3 = value;
				++v3;
			} while (v3 < v4);
		}
	}
}

void __cdecl TVPFillColor_mmx_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color)
{
	tjs_uint32 *v3; // edi
	tjs_uint32 *v4; // ebx

	if (len > 0)
	{
		v3 = dest;
		v4 = &dest[len];
		if (dest < v4)
		{
			do
			{
				*v3 = color & 0xFFFFFF | *v3 & 0xFF000000;
				++v3;
			} while (v3 < v4);
		}
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_mmx_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
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
	tjs_uint32 *  v15; // ebp
	__m64         v16; // mm3

	if (len > 0)
	{
		v4  = dest;
		v5  = _mm_cvtsi32_si64(0xFF000000);
		v6  = _m_punpckldq(v5, v5);
		v7  = _mm_cvtsi32_si64(0xFFFFFFu);
		v8  = _m_punpckldq(v7, v7);
		v10 = _mm_set1_pi16(opa);
		v11 = _m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color), _mm_setzero_si64()), v10);
		v13 = _mm_set1_pi16((tjs_uint8) ~(tjs_uint8)opa);
		v14 = v13;
		v15 = &dest[len];
		if (dest < v15)
		{
			do
			{
				v16 = _mm_cvtsi32_si64(*v4);
				*v4 = _mm_cvtsi64_si32(
					_m_por(
						_m_pand(
							_m_packuswb(_m_psrlwi(_m_paddusw(_m_pmullw(_m_punpcklbw(v16, _mm_setzero_si64()), v14), v11), 8u), _mm_setzero_si64()),
							v8),
						_m_pand(v16, v6)));
				++v4;
			} while (v4 < v15);
		}
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_d_mmx_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
{
	tjs_int     v4;  // ecx
	tjs_uint32 *v5;  // edi
	tjs_uint32 *v6;  // ebp
	__m64       v7;  // mm7
	tjs_int     v8;  // esi
	__m64       v9;  // mm1
	tjs_uint32  v10; // edx
	__m64       v12; // mm4

	v4 = len;
	if (len > 0)
	{
		v5 = dest;
		v6 = &dest[len];
		v7 = _m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64());
		v8 = opa << 8;
		if (dest < v6)
		{
			do
			{
				v9         = _m_punpcklbw(_mm_cvtsi32_si64(*v5 & 0xFFFFFF), _mm_setzero_si64());
				v10        = *v5 >> 24;
				LOBYTE(v4) = TVPNegativeMulTable[v8 + v10];
				v4 <<= 24;
				v12 = _mm_set1_pi16(TVPOpacityOnOpacityTable[v8 + v10]);
				*v5 = v4 | _mm_cvtsi64_si32(_m_packuswb(_m_psrlwi(_m_paddw(_m_pmullw(_m_psubw(v7, v9), v12), _m_psllwi(v9, 8u)), 8u), _mm_setzero_si64()));
				++v5;
			} while (v5 < v6);
		}
	}
	_m_empty();
}

void __cdecl TVPConstColorAlphaBlend_a_mmx_c(tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)
{
	unsigned int *v4;  // edi
	tjs_uint32 *  v5;  // ebp
	unsigned int  v6;  // esi
	__m64         v8;  // mm7
	__m64         v9;  // mm7
	__m64         v10; // mm6
	__m64         v11; // mm1

	if (len > 0)
	{
		v4  = dest;
		v5  = &dest[len];
		v6  = ((unsigned int)opa >> 7) + opa;
		v8  = _mm_set1_pi16(v6);
		v9  = v8;
		v10 = _m_por(
			_m_psrlwi(_m_pmullw(_m_punpcklbw(_mm_cvtsi32_si64(color & 0xFFFFFF), _mm_setzero_si64()), v9), 8u),
			_m_punpcklbw(_mm_cvtsi32_si64(v6 << 24), _mm_setzero_si64()));
		if (dest < v5)
		{
			do
			{
				v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
				*v4 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v9), 8u)), v10), _mm_setzero_si64()));
				++v4;
			} while (v4 < v5);
		}
	}
	_m_empty();
}
