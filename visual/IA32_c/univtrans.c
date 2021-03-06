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
#include "tvpgl_ia32_intf_c.h"

void __cdecl TVPUnivTransBlend_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len)
{
	tjs_uint32 *     v6;  // edi
	unsigned int *   v7;  // ecx
	unsigned int *   v8;  // ebx
	const tjs_uint8 *v9;  // ebp
	tjs_uint32 *     v10; // esi
	__m64            v12; // mm4
	__m64            v13; // mm1

	if (len > 0)
	{
		v6  = dest;
		v7  = (unsigned int *)src1;
		v8  = (unsigned int *)src2;
		v9  = rule;
		v10 = &dest[len];
		if (dest < v10)
		{
			do
			{
				v12 = _mm_set1_pi16(*(const tjs_uint32 *)((char *)table + 2 * *v9));
				v13 = _m_punpcklbw(_mm_cvtsi32_si64(*v8), _mm_setzero_si64());
				*v6 = _mm_cvtsi64_si32(
					_m_packuswb(
						_m_paddw(v13, _m_psllwi(_m_pmulhw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v7), _mm_setzero_si64()), v13), v12), 1u)),
						_mm_setzero_si64()));
				++v9;
				++v7;
				++v6;
				++v8;
			} while (v6 < v10);
		}
	}
	_m_empty();
}

void __cdecl TVPUnivTransBlend_switch_mmx_c(tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv)
{
	tjs_uint32 *     v8;  // edi
	unsigned int *   v9;  // ecx
	unsigned int *   v10; // ebx
	const tjs_uint8 *v11; // ebp
	tjs_uint32 *     v12; // esi
	__m64            v14; // mm4
	__m64            v15; // mm1

	if (len > 0)
	{
		v8  = dest;
		v9  = (unsigned int *)src1;
		v10 = (unsigned int *)src2;
		v11 = rule;
		v12 = &dest[len];
		if (dest < v12)
		{
			tjs_int ze = 1;
			do
			{
				if (ze)
				{
					if ((v11[1] >= src1lv) && (v11[0] < src1lv))
					{
						ze = 0;
					}
					else if (v11[1] >= src1lv)
					{
						*v8 = *v9;
						++v11;
						++v9;
						++v8;
						++v10;
						*v8 = *v9;
						++v11;
						++v9;
						++v8;
						++v10;
						continue;
					}
					else if (v11[1] >= src2lv)
					{
						ze = 0;
					}
					else if (v11[0] >= src2lv)
					{
						ze = 0;
					}
					else
					{
						*v8 = *v10;
						++v11;
						++v9;
						++v8;
						++v10;
						*v8 = *v10;
						++v11;
						++v9;
						++v8;
						++v10;
						continue;
					}
				}
				else
				{
					ze = 1;
				}
				v14 = _mm_set1_pi16(*(const tjs_uint32 *)((char *)table + 2 * *v11));
				v15 = _m_punpcklbw(_mm_cvtsi32_si64(*v10), _mm_setzero_si64());
				*v8 = _mm_cvtsi64_si32(
					_m_packuswb(
						_m_paddw(v15, _m_psllwi(_m_pmulhw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(*v9), _mm_setzero_si64()), v15), v14), 1u)),
						_mm_setzero_si64()));
				++v11;
				++v9;
				++v8;
				++v10;
			} while (v8 < v12);
		}
	}
	_m_empty();
}
