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

static tjs_uint64 c_0000ffffffffffff = 0x0000ffffffffffffull;
static tjs_uint64 c_ffff000000000000 = 0xffff000000000000ull;
static tjs_uint64 c_0100000000000000 = 0x0100000000000000ull;

void __cdecl TVPAddSubVertSum16_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64 *       v4; // edi
	unsigned int *v5; // eax
	unsigned int *v6; // ebx

	if (len > 0)
	{
		v4 = (__m64 *)dest;
		v5 = (unsigned int *)addline;
		v6 = (unsigned int *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				*v4 = _m_psubd(
					_m_paddd(*v4, _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64())),
					_m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64()));
				++v4;
				++v5;
				++v6;
			} while (v4 < (__m64 *)&dest[4 * len]);
		}
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum16_d_mmx_pfraction_c(tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64 *       v4;  // edi
	unsigned int *v5;  // eax
	unsigned int *v6;  // ebx
	__m64         v7;  // mm3
	__m64         v8;  // mm4
	__m64         v9;  // mm5
	__m64         v10; // mm6
	__m64         v11; // mm5
	__m64         v12; // mm6

	if (len > 0)
	{
		v4 = (__m64 *)dest;
		v5 = (unsigned int *)addline;
		v6 = (unsigned int *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64());
				v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64());
				v9  = _m_psrlqi(_m_paddw(_m_psrlwi(v7, 7u), v7), 0x30u);
				v10 = _m_psrlqi(_m_paddw(_m_psrlwi(v8, 7u), v8), 0x30u);
				v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
				v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
				*v4 = _m_psubd(
					_m_paddd(
						*v4,
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
				++v4;
				++v5;
				++v6;
			} while (v4 < (__m64 *)&dest[4 * len]);
		}
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum32_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64 *       v4; // edi
	unsigned int *v5; // eax
	unsigned int *v6; // ebx
	__m64         v7; // mm3
	__m64         v8; // mm4
	__m64         v9; // mm2

	if (len > 0)
	{
		v4 = (__m64 *)dest;
		v5 = (unsigned int *)addline;
		v6 = (unsigned int *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64());
				v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64());
				v9  = _m_psubd(_m_paddd(v4[1], _m_punpckhwd(v7, _mm_setzero_si64())), _m_punpckhwd(v8, _mm_setzero_si64()));
				*v4 = _m_psubd(
					_m_paddd(*v4, _m_punpcklwd(v7, _mm_setzero_si64())),
					_m_punpcklwd(v8, _mm_setzero_si64()));
				v4[1] = v9;
				v4 += 2;
				++v5;
				++v6;
			} while (v4 < (__m64 *)&dest[4 * len]);
		}
	}
	_m_empty();
}

void __cdecl TVPAddSubVertSum32_d_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	__m64 *       v4;  // edi
	unsigned int *v5;  // eax
	unsigned int *v6;  // ebx
	__m64         v7;  // mm3
	__m64         v8;  // mm4
	__m64         v9;  // mm5
	__m64         v10; // mm6
	__m64         v11; // mm5
	__m64         v12; // mm6
	__m64         v13; // mm3
	__m64         v14; // mm4
	__m64         v15; // mm2

	if (len > 0)
	{
		v4 = (__m64 *)dest;
		v5 = (unsigned int *)addline;
		v6 = (unsigned int *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64());
				v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64());
				v9  = _m_psrlqi(_m_paddw(_m_psrlwi(v7, 7u), v7), 0x30u);
				v10 = _m_psrlqi(_m_paddw(_m_psrlwi(v8, 7u), v8), 0x30u);
				v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
				v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
				v13 = _m_psrlwi(_m_pmullw(v7, _m_por(_m_pand(v11, (__m64)c_0000ffffffffffff), (__m64)c_0100000000000000)), 8u);
				v14 = _m_psrlwi(_m_pmullw(v8, _m_por(_m_pand(v12, (__m64)c_0000ffffffffffff), (__m64)c_0100000000000000)), 8u);
				v15 = _m_psubd(_m_paddd(v4[1], _m_punpckhwd(v13, _mm_setzero_si64())), _m_punpckhwd(v14, _mm_setzero_si64()));
				*v4 = _m_psubd(
					_m_paddd(*v4, _m_punpcklwd(v13, _mm_setzero_si64())),
					_m_punpcklwd(v14, _mm_setzero_si64()));
				v4[1] = v15;
				v4 += 2;
				++v5;
				++v6;
			} while (v4 < (__m64 *)&dest[4 * len]);
		}
	}
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg16_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v8;  // mm7
	int         v9;  // esi
	tjs_uint32 *v10; // edi
	__m64       v11; // mm5
	__m64 *     v12; // ebx
	__m64 *     v13; // ebp
	__m64       v14; // mm1
	__m64       v15; // mm2
	__m64       v16; // mm1
	__m64       v17; // mm2
	__m64       v19; // mm7
	__m64       v20; // mm7
	__m64       v22; // mm6
	__m64       v23; // mm6
	tjs_uint32 *v24; // edi
	__m64       v25; // mm5
	__m64 *     v26; // ebx
	__m64 *     v27; // ebp
	tjs_uint32 *v28; // esi
	__m64       v29; // mm1
	tjs_uint32 *v30; // [esp+0h] [ebp-20h]

	if (n >= 3 && n < 128)
	{
		if (len > 0)
		{
			v19 = _mm_set1_pi16((unsigned int)n >> 1);
			v20 = v19;
			v22 = _mm_set1_pi16(0x10000 / n);
			v23 = v22;
			v24 = dest;
			v25 = *(__m64 *)sum;
			v26 = (__m64 *)add;
			v27 = (__m64 *)sub;
			v28 = &dest[len];
			if (dest < v28)
			{
				do
				{
					v29  = _m_pmulhw(_m_paddw(v25, v20), v23);
					*v24 = _mm_cvtsi64_si32(_m_packuswb(v29, v29));
					v25  = _m_psubw(_m_paddw(v25, *v26), *v27);
					++v26;
					++v27;
					++v24;
				} while (v24 < v28);
			}
			*(__m64 *)sum = v25;
		}
		_m_empty();
	}
	else
	{
		if (len > 0)
		{
			v7  = _mm_set1_pi16((unsigned int)n >> 1);
			v8  = v7;
			v10 = dest;
			v11 = *(__m64 *)sum;
			v12 = (__m64 *)add;
			v13 = (__m64 *)sub;
			v30 = &dest[len];
			if (dest < v30)
			{
				do
				{
					v14  = _m_paddw(v11, v8);
					v15  = v14;
					v16  = _m_punpcklwd(v14, _mm_setzero_si64());
					v17  = _m_punpckhwd(v15, _mm_setzero_si64());
					v9   = 0x10000 / n << 16;
					*v10 = ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v17, 0x20u)) >> 32 << 24) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v17) >> 32 << 16) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v16, 0x20u)) >> 32 << 8) + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v16) >> 32);
					v11  = _m_psubw(_m_paddw(v11, *v12), *v13);
					++v12;
					++v13;
					++v10;
				} while (v10 < v30);
			}
			*(__m64 *)sum = v11;
		}
		_m_empty();
	}
}

void __cdecl TVPDoBoxBlurAvg16_d_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v8;  // mm7
	int         v9;  // esi
	tjs_uint32 *v10; // edi
	__m64       v11; // mm5
	__m64 *     v12; // ebp
	__m64       v13; // mm1
	__m64       v14; // mm2
	__m64       v15; // mm1
	__m64       v16; // mm2
	int         v17; // edx
	__m64       v19; // mm7
	__m64       v20; // mm7
	__m64       v22; // mm6
	__m64       v23; // mm6
	tjs_uint32 *v24; // edi
	__m64       v25; // mm5
	__m64 *     v26; // ebx
	__m64 *     v27; // ebp
	tjs_uint32 *v28; // esi
	__m64       v29; // mm1
	tjs_uint8 * v30; // ecx
	tjs_uint32 *v31; // [esp+0h] [ebp-20h]

	if (n >= 3 && n < 128)
	{
		if (len > 0)
		{
			v19 = _mm_set1_pi16((unsigned int)n >> 1);
			v20 = v19;
			v22 = _mm_set1_pi16(0x10000 / n);
			v23 = v22;
			v24 = dest;
			v25 = *(__m64 *)sum;
			v26 = (__m64 *)add;
			v27 = (__m64 *)sub;
			v28 = &dest[len];
			if (dest < v28)
			{
				do
				{
					v29  = _m_packuswb(_m_pmulhw(_m_paddw(v25, v20), v23), _mm_setzero_si64());
					v30  = &TVPDivTable[((unsigned int)_mm_cvtsi64_si32(v29) >> 16) & 0xFF00];
					*v24 = (v30[((unsigned int)_mm_cvtsi64_si32(v29) >> 16) & 0xFF] << 16) + (v30[(tjs_uint16)_mm_cvtsi64_si32(v29) >> 8] << 8) + v30[(tjs_uint8)_mm_cvtsi64_si32(v29)] + (_mm_cvtsi64_si32(v29) & 0xFF000000);
					v25  = _m_psubw(_m_paddw(v25, *v26), *v27);
					++v26;
					++v27;
					++v24;
				} while (v24 < v28);
			}
			*(__m64 *)sum = v25;
		}
		_m_empty();
	}
	else
	{
		if (len > 0)
		{
			v7  = _mm_set1_pi16((unsigned int)n >> 1);
			v8  = v7;
			v9  = 0x10000 / n << 16;
			v10 = dest;
			v11 = *(__m64 *)sum;
			v12 = (__m64 *)sub;
			v31 = &dest[len];
			if (dest < v31)
			{
				do
				{
					v13  = _m_paddw(v11, v8);
					v14  = v13;
					v15  = _m_punpcklwd(v13, _mm_setzero_si64());
					v16  = _m_punpckhwd(v14, _mm_setzero_si64());
					v17  = (unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v16, 0x20u)) >> 32;
					*v10 = (TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v16) >> 32)] << 16) + (TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u)) >> 32)] << 8) + TVPDivTable[256 * v17 + ((unsigned int)v9 * (tjs_uint64)(unsigned int)_mm_cvtsi64_si32(v15) >> 32)] + (v17 << 24);
					v11  = _m_psubw(_m_paddw(v11, *(__m64 *)add), *v12);
					add += 4;
					++v12;
					++v10;
				} while (v10 < v31);
			}
			*(__m64 *)sum = v11;
		}
		_m_empty();
	}
}

void __cdecl TVPDoBoxBlurAvg32_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len)
{
	__m64       v6;  // mm7
	__m64       v7;  // mm7
	int         v8;  // esi
	tjs_uint32 *v9;  // edi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64 *     v12; // ebx
	__m64 *     v13; // ebp
	__m64       v14; // mm3
	__m64       v15; // mm4
	tjs_uint32 *v16; // [esp+0h] [ebp-20h]

	if (len > 0)
	{
		v6  = _mm_cvtsi32_si64((unsigned int)n >> 1);
		v7  = _m_punpckldq(v6, v6);
		v9  = dest;
		v10 = *(__m64 *)sum;
		v11 = *((__m64 *)sum + 1);
		v12 = (__m64 *)add;
		v13 = (__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v14 = _m_paddd(v10, v7);
				v15 = _m_paddd(v11, v7);
				v8  = 0x100000000ll / n;
				*v9 = ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u))) >> 32 << 24) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v15)) >> 32 << 16) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u))) >> 32 << 8) + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v14)) >> 32);
				v10 = _m_psubd(_m_paddd(v10, *v12), *v13);
				v11 = _m_psubd(_m_paddd(v11, v12[1]), v13[1]);
				v12 += 2;
				v13 += 2;
				++v9;
			} while (v9 < v16);
		}
		*(__m64 *)sum       = v10;
		*((__m64 *)sum + 1) = v11;
	}
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg32_d_mmx_pfraction_c(tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len)
{
	__m64       v6;  // mm7
	__m64       v7;  // mm7
	int         v8;  // esi
	tjs_uint32 *v9;  // edi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64 *     v12; // ebp
	__m64       v13; // mm3
	__m64       v14; // mm4
	int         v15; // eax
	int         v16; // ebx
	tjs_uint32 *v17; // [esp+0h] [ebp-20h]

	if (len > 0)
	{
		v6  = _mm_cvtsi32_si64((unsigned int)n >> 1);
		v7  = _m_punpckldq(v6, v6);
		v8  = 0x100000000ll / n;
		v9  = dest;
		v10 = *(__m64 *)sum;
		v11 = *((__m64 *)sum + 1);
		v12 = (__m64 *)sub;
		v17 = &dest[len];
		if (dest < v17)
		{
			do
			{
				v13 = _m_paddd(v10, v7);
				v14 = _m_paddd(v11, v7);
				v15 = _mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u));
				v16 = (tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 8;
				*v9 = (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v14)) >> 32)] << 16) + (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(_m_psrlqi(v13, 0x20u))) >> 32)] << 8) + TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)_mm_cvtsi64_si32(v13)) >> 32)] + ((tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 24);
				v10 = _m_paddd(_m_psubd(v10, *v12), *(__m64 *)add);
				v11 = _m_paddd(_m_psubd(v11, v12[1]), *((__m64 *)add + 1));
				add += 4;
				v12 += 2;
				++v9;
			} while (v9 < v17);
		}
		*(__m64 *)sum       = v10;
		*((__m64 *)sum + 1) = v11;
	}
	_m_empty();
}

//----- (00000E54) --------------------------------------------------------
void __cdecl TVPDoBoxBlurAvg16_sse_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v8;  // mm7
	__m64       v10; // mm6
	__m64       v11; // mm6
	tjs_uint32 *v12; // edi
	__m64       v13; // mm5
	__m64 *     v14; // ebx
	__m64 *     v15; // ebp
	tjs_uint32 *v16; // esi
	__m64       v17; // mm1

	if (len > 0)
	{
		v7  = _mm_set1_pi16((unsigned int)n >> 1);
		v8  = v7;
		v10 = _mm_set1_pi16(0x10000 / n);
		v11 = v10;
		v12 = dest;
		v13 = *(__m64 *)sum;
		v14 = (__m64 *)add;
		v15 = (__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v17  = _m_pmulhuw(_m_paddw(v13, v8), v11);
				*v12 = _mm_cvtsi64_si32(_m_packuswb(v17, v17));
				v13  = _m_psubw(_m_paddw(v13, *v14), *v15);
				++v14;
				++v15;
				++v12;
			} while (v12 < v16);
		}
		*(__m64 *)sum = v13;
	}
	_m_empty();
}

void __cdecl TVPDoBoxBlurAvg16_d_sse_pfraction_c(tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 *add, const tjs_uint16 *sub, tjs_int n, tjs_int len)
{
	__m64       v7;  // mm7
	__m64       v8;  // mm7
	__m64       v10; // mm6
	__m64       v11; // mm6
	tjs_uint32 *v12; // edi
	__m64       v13; // mm5
	__m64 *     v14; // ebx
	__m64 *     v15; // ebp
	tjs_uint32 *v16; // esi
	__m64       v17; // mm1
	tjs_uint8 * v18; // ecx

	if (len > 0)
	{
		v7  = _mm_set1_pi16((unsigned int)n >> 1);
		v8  = v7;
		v10 = _mm_set1_pi16(0x10000 / n);
		v11 = v10;
		v12 = dest;
		v13 = *(__m64 *)sum;
		v14 = (__m64 *)add;
		v15 = (__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v17  = _m_packuswb(_m_pmulhuw(_m_paddw(v13, v8), v11), _mm_setzero_si64());
				v18  = &TVPDivTable[((unsigned int)_mm_cvtsi64_si32(v17) >> 16) & 0xFF00];
				*v12 = (v18[((unsigned int)_mm_cvtsi64_si32(v17) >> 16) & 0xFF] << 16) + (v18[(tjs_uint16)_mm_cvtsi64_si32(v17) >> 8] << 8) + v18[(tjs_uint8)_mm_cvtsi64_si32(v17)] + (_mm_cvtsi64_si32(v17) & 0xFF000000);
				v13  = _m_psubw(_m_paddw(v13, *v14), *v15);
				++v14;
				++v15;
				++v12;
			} while (v12 < v16);
		}
		*(__m64 *)sum = v13;
	}
	_m_empty();
}
