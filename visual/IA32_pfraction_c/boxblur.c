/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include <xmmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

static const __m64 c_0000ffffffffffff = (__m64)0x0000ffffffffffffull;
static const __m64 c_ffff000000000000 = (__m64)0xffff000000000000ull;
static const __m64 c_0100000000000000 = (__m64)0x0100000000000000ull;

void __cdecl TVPAddSubVertSum16_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		((__m64 *)dest)[i] = _m_psubd(
			_m_paddd(((__m64 *)dest)[i], _m_punpcklbw(_mm_cvtsi32_si64(addline[i]), _mm_setzero_si64())),
			_m_punpcklbw(_mm_cvtsi32_si64(subline[i]), _mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum16_d_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64         v7;  // mm3
	__m64         v8;  // mm4
	__m64         v9;  // mm5
	__m64         v10; // mm6
	__m64         v11; // mm5
	__m64         v12; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _m_punpcklbw(_mm_cvtsi32_si64(addline[i]), _mm_setzero_si64());
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(subline[i]), _mm_setzero_si64());
		v9  = _m_psrlqi(_m_paddw(_m_psrlwi(v7, 7u), v7), 0x30u);
		v10 = _m_psrlqi(_m_paddw(_m_psrlwi(v8, 7u), v8), 0x30u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		((__m64 *)dest)[i] = _m_psubd(
			_m_paddd(
				((__m64 *)dest)[i],
				_m_psrlwi(
					_m_pmullw(
						v7,
						_m_por(
							_m_pand(v11, (__m64)c_0000ffffffffffff),
							(__m64)c_0100000000000000)),
					8u)),
			_m_psrlwi(
				_m_pmullw(
					v8,
					_m_por(_m_pand(v12, (__m64)c_0000ffffffffffff), (__m64)c_0100000000000000)),
				8u));
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum32_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64         v7; // mm3
	__m64         v8; // mm4
	__m64         v9; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _m_punpcklbw(_mm_cvtsi32_si64(addline[i]), _mm_setzero_si64());
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(subline[i]), _mm_setzero_si64());
		v9  = _m_psubd(_m_paddd(((__m64 *)dest)[(i * 2) + 1], _m_punpckhwd(v7, _mm_setzero_si64())), _m_punpckhwd(v8, _mm_setzero_si64()));
		((__m64 *)dest)[i * 2] = _m_psubd(
			_m_paddd(((__m64 *)dest)[i * 2], _m_punpcklwd(v7, _mm_setzero_si64())),
			_m_punpcklwd(v8, _mm_setzero_si64()));
		((__m64 *)dest)[(i * 2) + 1] = v9;
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum32_d_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64         v7;  // mm3
	__m64         v8;  // mm4
	__m64         v9;  // mm5
	__m64         v10; // mm6
	__m64         v11; // mm5
	__m64         v12; // mm6
	__m64         v13; // mm3
	__m64         v14; // mm4
	__m64         v15; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _m_punpcklbw(_mm_cvtsi32_si64(addline[i]), _mm_setzero_si64());
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(subline[i]), _mm_setzero_si64());
		v9  = _m_psrlqi(_m_paddw(_m_psrlwi(v7, 7u), v7), 0x30u);
		v10 = _m_psrlqi(_m_paddw(_m_psrlwi(v8, 7u), v8), 0x30u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		v13 = _m_psrlwi(_m_pmullw(v7, _m_por(_m_pand(v11, (__m64)c_0000ffffffffffff), (__m64)c_0100000000000000)), 8u);
		v14 = _m_psrlwi(_m_pmullw(v8, _m_por(_m_pand(v12, (__m64)c_0000ffffffffffff), (__m64)c_0100000000000000)), 8u);
		v15 = _m_psubd(_m_paddd(((__m64 *)dest)[(i * 2) + 1], _m_punpckhwd(v13, _mm_setzero_si64())), _m_punpckhwd(v14, _mm_setzero_si64()));
		((__m64 *)dest)[i * 2] = _m_psubd(
			_m_paddd(((__m64 *)dest)[i * 2], _m_punpcklwd(v13, _mm_setzero_si64())),
			_m_punpcklwd(v14, _mm_setzero_si64()));
		((__m64 *)dest)[(i * 2) + 1] = v15;
	}
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg16_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	int         v9;  // esi
	__m64       v11; // mm5
	__m64       v14; // mm1
	__m64       v15; // mm2
	__m64       v16; // mm1
	__m64       v17; // mm2
	__m64       v19; // mm7
	__m64       v22; // mm6
	__m64       v25; // mm5
	__m64       v29; // mm1

	if (n >= 3 && n < 128)
	{
		v19 = _mm_set1_pi16((unsigned int)n >> 1);
		v22 = _mm_set1_pi16(0x10000 / n);
		v25 = *(__m64 *)sum;
		for (tjs_int i = 0; i < len; i += 1)
		{
			v29  = _m_pmulhw(_m_paddw(v25, v19), v22);
			dest[i] = _mm_cvtsi64_si32(_m_packuswb(v29, v29));
			v25  = _m_psubw(_m_paddw(v25, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
		}
		*(__m64 *)sum = v25;
		_m_empty();
	}
	else
	{
		v7  = _mm_set1_pi16((unsigned int)n >> 1);
		v11 = *(__m64 *)sum;
		for (tjs_int i = 0; i < len; i += 1)
		{
			v14  = _m_paddw(v11, v7);
			v15  = v14;
			v16  = _m_punpcklwd(v14, _mm_setzero_si64());
			v17  = _m_punpckhwd(v15, _mm_setzero_si64());
			v9   = 0x10000 / n << 16;
			dest[i] = ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v17, 0x20u)) >> 32 << 24) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v17) >> 32 << 16) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v16, 0x20u)) >> 32 << 8) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v16) >> 32);
			v11  = _m_psubw(_m_paddw(v11, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
		}
		*(__m64 *)sum = v11;
		_m_empty();
	}
}

void __cdecl TVPDoBoxBlurAvg16_d_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	int         v9;  // esi
	__m64       v11; // mm5
	__m64       v13; // mm1
	__m64       v14; // mm2
	__m64       v15; // mm1
	__m64       v16; // mm2
	int         v17; // edx
	__m64       v19; // mm7
	__m64       v22; // mm6
	__m64       v25; // mm5
	__m64       v29; // mm1
	tjs_uint8 * v30; // ecx

	if (n >= 3 && n < 128)
	{
		v19 = _mm_set1_pi16((unsigned int)n >> 1);
		v22 = _mm_set1_pi16(0x10000 / n);
		v25 = *(__m64 *)sum;
		for (tjs_int i = 0; i < len; i += 1)
		{
			v29  = _m_packuswb(_m_pmulhw(_m_paddw(v25, v19), v22), _mm_setzero_si64());
			v30  = &TVPDivTable[((unsigned int)_mm_cvtsi64_si32(v29) >> 16) & 0xFF00];
			dest[i] = (v30[((unsigned int)_mm_cvtsi64_si32(v29) >> 16) & 0xFF] << 16) + (v30[(tjs_uint16)_mm_cvtsi64_si32(v29) >> 8] << 8) + v30[(tjs_uint8)_mm_cvtsi64_si32(v29)] + (_mm_cvtsi64_si32(v29) & 0xFF000000);
			v25  = _m_psubw(_m_paddw(v25, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
		}
		*(__m64 *)sum = v25;
		_m_empty();
	}
	else
	{
		v7  = _mm_set1_pi16((unsigned int)n >> 1);
		v9  = 0x10000 / n << 16;
		v11 = *(__m64 *)sum;
		for (tjs_int i = 0; i < len; i += 1)
		{
			v13  = _m_paddw(v11, v7);
			v14  = v13;
			v15  = _m_punpcklwd(v13, _mm_setzero_si64());
			v16  = _m_punpckhwd(v14, _mm_setzero_si64());
			v17  = (unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v16, 0x20u)) >> 32;
			dest[i] = (TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v16) >> 32)] << 16) + (TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u)) >> 32)] << 8) + TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v15) >> 32)] + (v17 << 24);
			v11  = _m_psubw(_m_paddw(v11, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
		}
		*(__m64 *)sum = v11;
		_m_empty();
	}
}

void __cdecl TVPDoBoxBlurAvg32_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len)
{
	__m64       v6;  // mm7
	__m64       v7;  // mm7
	int         v8;  // esi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64       v14; // mm3
	__m64       v15; // mm4

	v6  = _mm_cvtsi32_si64((unsigned int)n >> 1);
	v7  = _m_punpckldq(v6, v6);
	v10 = *(__m64 *)sum;
	v11 = *((__m64 *)sum + 1);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v14 = _m_paddd(v10, v7);
		v15 = _m_paddd(v11, v7);
		v8  = 0x100000000ll / n;
		dest[i] = ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u))) >> 32 << 24) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v15)) >> 32 << 16) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u))) >> 32 << 8) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v14)) >> 32);
		v10 = _m_psubd(_m_paddd(v10, ((__m64 *)add)[i * 2]), ((__m64 *)sub)[i * 2]);
		v11 = _m_psubd(_m_paddd(v11, ((__m64 *)add)[(i * 2) + 1]), ((__m64 *)sub)[(i * 2) + 1]);
	}
	*(__m64 *)sum       = v10;
	*((__m64 *)sum + 1) = v11;
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg32_d_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len)
{
	__m64       v6;  // mm7
	__m64       v7;  // mm7
	int         v8;  // esi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64       v13; // mm3
	__m64       v14; // mm4
	int         v15; // eax
	int         v16; // ebx

	v6  = _mm_cvtsi32_si64((unsigned int)n >> 1);
	v7  = _m_punpckldq(v6, v6);
	v8  = 0x100000000ll / n;
	v10 = *(__m64 *)sum;
	v11 = *((__m64 *)sum + 1);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v13 = _m_paddd(v10, v7);
		v14 = _m_paddd(v11, v7);
		v15 = _mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u));
		v16 = (tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 8;
		dest[i] = (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v14)) >> 32)] << 16) + (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v13, 0x20u))) >> 32)] << 8) + TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v13)) >> 32)] + ((tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 24);
		v10 = _m_paddd(_m_psubd(v10, ((__m64 *)sub)[i * 2]), ((__m64 *)add)[i * 2]);
		v11 = _m_paddd(_m_psubd(v11, ((__m64 *)sub)[(i * 2) + 1]), ((__m64 *)add)[(i * 2) + 1]);
	}
	*(__m64 *)sum       = v10;
	*((__m64 *)sum + 1) = v11;
	_m_empty();
}

//----- (00000E54) --------------------------------------------------------
void __cdecl TVPDoBoxBlurAvg16_sse_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v10; // mm6
	__m64       v13; // mm5
	__m64       v17; // mm1

	v7  = _mm_set1_pi16((unsigned int)n >> 1);
	v10 = _mm_set1_pi16(0x10000 / n);
	v13 = *(__m64 *)sum;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v17  = _m_pmulhuw(_m_paddw(v13, v7), v10);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(v17, v17));
		v13  = _m_psubw(_m_paddw(v13, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
	}
	*(__m64 *)sum = v13;
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg16_d_sse_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v10; // mm6
	__m64       v13; // mm5
	__m64       v17; // mm1
	tjs_uint8 * v18; // ecx

	v7  = _mm_set1_pi16((unsigned int)n >> 1);
	v10 = _mm_set1_pi16(0x10000 / n);
	v13 = *(__m64 *)sum;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v17  = _m_packuswb(_m_pmulhuw(_m_paddw(v13, v7), v10), _mm_setzero_si64());
		v18  = &TVPDivTable[((unsigned int)_mm_cvtsi64_si32(v17) >> 16) & 0xFF00];
		dest[i] = (v18[((unsigned int)_mm_cvtsi64_si32(v17) >> 16) & 0xFF] << 16) + (v18[(tjs_uint16)_mm_cvtsi64_si32(v17) >> 8] << 8) + v18[(tjs_uint8)_mm_cvtsi64_si32(v17)] + (_mm_cvtsi64_si32(v17) & 0xFF000000);
		v13  = _m_psubw(_m_paddw(v13, ((__m64 *)add)[i]), ((__m64 *)sub)[i]);
	}
	*(__m64 *)sum = v13;
	_m_empty();
}
