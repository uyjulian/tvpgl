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
#include <xmmintrin.h>
#include "tvpgl_ia32_intf_behavior_c.h"

static const __m64 c_0000ffffffffffff = (__m64)0x0000ffffffffffffull;
static const __m64 c_ffff000000000000 = (__m64)0xffff000000000000ull;
static const __m64 c_0100000000000000 = (__m64)0x0100000000000000ull;

TVP_GL_IA32_FUNC_DECL(void, TVPDoBoxBlurAvg32_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len))
{
#if 0
	// XXX: this doesn't match the MMX version
	tjs_uint half_n = n >> 1;

	tjs_uint32 v8 = 0x100000000ll / n;
	tjs_uint32 sum_tmp[4];
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum_tmp[i] = sum[i];
	}
	for (tjs_int i = 0; i < len; i += 1)
	{
		tjs_uint64 acc = 0 +
		    (((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[0] + half_n)) >> 32) <<  0) +
		    (((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[1] + half_n)) >> 32) <<  8) +
		    (((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[2] + half_n)) >> 32) << 16) +
		    (((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[3] + half_n)) >> 32) << 24);

		dest[i] = acc;

		for (tjs_int j = 0; j < 4; j += 1)
		{
			tjs_uint64 cur_sum = sum_tmp[j];
			cur_sum += add[(i * 4) + j];
			cur_sum -= sub[(i * 4) + j];
			sum_tmp[j] = cur_sum;
		}
	}
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum[i] = sum_tmp[i];
	}
#else
	__m64       v7;  // mm7
	tjs_int32   v8;  // esi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64       v14; // mm3
	__m64       v15; // mm4

	v7  = _mm_set1_pi32((tjs_uint32)n >> 1);
	v10 = *(__m64 *)sum;
	v11 = *((__m64 *)sum + 1);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v14 = _m_paddd(v10, v7);
		v15 = _m_paddd(v11, v7);
		v8  = 0x100000000ll / n;
		dest[i] = ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u))) >> 32 << 24) + 
		          ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(v15)) >> 32 << 16) + 
		          ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u))) >> 32 << 8) + 
		          ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(v14)) >> 32);
		v10 = _m_psubd(_m_paddd(v10, ((__m64 *)add)[i * 2]), ((__m64 *)sub)[i * 2]);
		v11 = _m_psubd(_m_paddd(v11, ((__m64 *)add)[(i * 2) + 1]), ((__m64 *)sub)[(i * 2) + 1]);
	}
	*(__m64 *)sum       = v10;
	*((__m64 *)sum + 1) = v11;
	_m_empty();
#endif
}

TVP_GL_IA32_FUNC_DECL(void, TVPDoBoxBlurAvg32_d_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len))
{
#if 0
	// XXX: this doesn't match the MMX version
	tjs_uint half_n = n >> 1;

	tjs_uint32 v8 = 0x100000000ll / n;
	tjs_uint32 sum_tmp[4];
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum_tmp[i] = sum[i];
	}
	for (tjs_int i = 0; i < len; i += 1)
	{
		tjs_uint32 v15 = (sum_tmp[3] + half_n);
		tjs_uint32 v16 = (tjs_uint64)(v8 * (tjs_uint64)v15) >> 32 << 8;
		tjs_uint64 acc = 0 +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[0] + half_n)) >> 32)] <<  0) +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[1] + half_n)) >> 32)] <<  8) +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (sum_tmp[2] + half_n)) >> 32)] << 16) +
		    (((tjs_uint64)((tjs_uint64)v8 * v15) >> 32) << 24);

		dest[i] = acc;

		for (tjs_int j = 0; j < 4; j += 1)
		{
			tjs_uint64 cur_sum = sum_tmp[j];
			cur_sum += add[(i * 4) + j];
			cur_sum -= sub[(i * 4) + j];
			sum_tmp[j] = cur_sum;
		}
	}
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum[i] = sum_tmp[i];
	}
#else
	__m64       v7;  // mm7
	tjs_int32   v8;  // esi
	__m64       v10; // mm1
	__m64       v11; // mm2
	__m64       v13; // mm3
	__m64       v14; // mm4
	tjs_int32   v15; // eax
	tjs_int32   v16; // ebx

	v7  = _mm_set1_pi32((tjs_uint32)n >> 1);
	v8  = 0x100000000ll / n;
	v10 = *(__m64 *)sum;
	v11 = *((__m64 *)sum + 1);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v13 = _m_paddd(v10, v7);
		v14 = _m_paddd(v11, v7);
		v15 = _mm_cvtsi64_si32(_m_psrlqi(v14, 0x20u));
		v16 = (tjs_uint64)(v8 * (tjs_uint64)v15) >> 32 << 8;
		dest[i] = (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(v14)) >> 32)] << 16) + 
		          (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(_m_psrlqi(v13, 0x20u))) >> 32)] << 8) + 
		           TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_uint64)_mm_cvtsi64_si32(v13)) >> 32)] + 
		                             ((tjs_uint64)(v8 * (tjs_uint64)v15) >> 32 << 24);
		v10 = _m_paddd(_m_psubd(v10, ((__m64 *)sub)[i * 2]), ((__m64 *)add)[i * 2]);
		v11 = _m_paddd(_m_psubd(v11, ((__m64 *)sub)[(i * 2) + 1]), ((__m64 *)add)[(i * 2) + 1]);
	}
	*(__m64 *)sum       = v10;
	*((__m64 *)sum + 1) = v11;
	_m_empty();
#endif
}
