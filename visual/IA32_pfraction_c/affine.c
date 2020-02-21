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

void __cdecl TVPLinTransCopy_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
{
	__m64       v9;  // mm0
	__m64       v10; // mm2
	__m64       v12; // mm5

	v9  = _m_por(_mm_cvtsi32_si64(sx), _m_psllqi(_mm_cvtsi32_si64(sy), 0x20u));
	v10 = _m_por(_mm_cvtsi32_si64(stepx), _m_psllqi(_mm_cvtsi32_si64(stepy), 0x20u));
	for (tjs_int i = 0; i < len; i += 1)
	{
		v12 = _m_psradi(v9, 0x10u);
		dest[i] = *(const tjs_uint32 *)((char *)&src[_mm_cvtsi64_si32(v12)] + srcpitch * _mm_cvtsi64_si32(_m_psrlqi(v12, 0x20u)));
		v9  = _m_paddd(v9, v10);
	}
	_m_empty();
}

void __cdecl TVPLinTransConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
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
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_paddw(
						_m_psllwi(v16, 8u),
						_m_pmullw(
							_m_psubw(
								_m_punpcklbw(
									_mm_cvtsi32_si64(*(const tjs_uint32 *)((char *)&src[_mm_cvtsi64_si32(v15)] + srcpitch * _mm_cvtsi64_si32(_m_psrlqi(v15, 0x20u)))),
									_mm_setzero_si64()),
								v16),
							v11)),
					8u),
				_mm_setzero_si64()));
		v13 = _m_paddd(v13, v17);
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransCopy_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
{
	unsigned int  v9;  // ecx
	unsigned int  v10; // edx
	unsigned int *v12; // eax
	__m64         v14; // mm5
	__m64         v17; // mm7
	__m64         v18; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm1

	v9  = sx;
	v10 = sy;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v12 = (unsigned int *)((char *)&src[v9 >> 16] + srcpitch * (v10 >> 16));
		v14 = _mm_set1_pi16((unsigned int)(tjs_uint16)v9 >> 8);
		v17 = _mm_set1_pi16(((unsigned int)(tjs_uint16)v10 >> 8) + ((unsigned int)(tjs_uint16)v10 >> 15));
		v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch)), _mm_setzero_si64());
		v20 = _m_paddb(v18, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v12[1]), _mm_setzero_si64()), v18), v14), 8u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddb(
					v20,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v19,
									_m_psrlwi(
										_m_pmullw(
											_m_psubw(
												_m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch + 4)), _mm_setzero_si64()),
												v19),
											v14),
										8u)),
								v20),
							v17),
						8u)),
				_mm_setzero_si64()));
		v9 += stepx;
		v10 += stepy;
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
{
	__m64         v10; // mm6
	unsigned int  v13; // ecx
	unsigned int  v14; // edx
	unsigned int *v16; // eax
	__m64         v18; // mm5
	__m64         v19; // mm5
	__m64         v21; // mm7
	__m64         v22; // mm1
	__m64         v23; // mm3
	__m64         v24; // mm1
	__m64         v25; // mm2

	v10 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	v13 = sx;
	v14 = sy;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v16  = (unsigned int *)((char *)&src[v13 >> 16] + srcpitch * (v14 >> 16));
		v18  = _mm_set1_pi16((unsigned int)(tjs_uint16)v13 >> 8);
		v19  = v18;
		v21  = _mm_set1_pi16(((unsigned int)(tjs_uint16)v14 >> 8) + ((unsigned int)(tjs_uint16)v14 >> 15));
		v22  = _m_punpcklbw(_mm_cvtsi32_si64(*v16), _mm_setzero_si64());
		v23  = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + srcpitch)), _mm_setzero_si64());
		v24  = _m_paddb(v22, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v16[1]), _mm_setzero_si64()), v22), v19), 8u));
		v25  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddb(
					v25,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v24,
									_m_psrlwi(
										_m_pmullw(
											_m_psubw(
												_m_paddb(
													v23,
													_m_psrlwi(
														_m_pmullw(
															_m_psubw(
																_m_punpcklbw(
																	_mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + srcpitch + 4)),
																	_mm_setzero_si64()),
																v23),
															v19),
														8u)),
												v24),
											v21),
										8u)),
								v25),
							v10),
						8u)),
				_mm_setzero_si64()));
		v13 += stepx;
		v14 += stepy;
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransAdditiveAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
{
	unsigned int  v9;  // ecx
	unsigned int  v10; // edx
	unsigned int *v12; // eax
	__m64         v14; // mm5
	__m64         v17; // mm7
	__m64         v18; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm1
	__m64         v21; // mm1
	__m64         v22; // mm3
	__m64         v23; // mm2
	__m64         v24; // mm3

	v9  = sx;
	v10 = sy;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v12 = (unsigned int *)((char *)&src[v9 >> 16] + srcpitch * (v10 >> 16));
		v14 = _mm_set1_pi16((unsigned int)(tjs_uint16)v9 >> 8);
		v17 = _mm_set1_pi16(((unsigned int)(tjs_uint16)v10 >> 8) + ((unsigned int)(tjs_uint16)v10 >> 15));
		v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch)), _mm_setzero_si64());
		v20 = _m_paddb(v18, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v12[1]), _mm_setzero_si64()), v18), v14), 8u));
		v21 = _m_paddb(
			v20,
			_m_psrlwi(
				_m_pmullw(
					_m_psubw(
						_m_paddb(
							v19,
							_m_psrlwi(
								_m_pmullw(
									_m_psubw(
										_m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch + 4)), _mm_setzero_si64()),
										v19),
									v14),
								8u)),
						v20),
					v17),
				8u));
		v22 = _m_psrlqi(v21, 0x30u);
		v23 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v24 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v22));
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v21, _m_psubw(v23, _m_psrlwi(_m_pmullw(v23, v24), 8u))), _mm_setzero_si64()));
		v9 += stepx;
		v10 += stepy;
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransAdditiveAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
{
	__m64         v10; // mm6
	unsigned int  v13; // ecx
	unsigned int  v14; // edx
	unsigned int *v16; // eax
	__m64         v18; // mm5
	__m64         v19; // mm5
	__m64         v21; // mm7
	__m64         v22; // mm1
	__m64         v23; // mm3
	__m64         v24; // mm1
	__m64         v25; // mm1
	__m64         v26; // mm3
	__m64         v27; // mm2
	__m64         v28; // mm3

	v10 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	v13 = sx;
	v14 = sy;
	for (tjs_int i = 0; i < len; i += 1)
	{
		v16 = (unsigned int *)((char *)&src[v13 >> 16] + srcpitch * (v14 >> 16));
		v18 = _mm_set1_pi16((unsigned int)(tjs_uint16)v13 >> 8);
		v19 = v18;
		v21 = _mm_set1_pi16(((unsigned int)(tjs_uint16)v14 >> 8) + ((unsigned int)(tjs_uint16)v14 >> 15));
		v22 = _m_punpcklbw(_mm_cvtsi32_si64(*v16), _mm_setzero_si64());
		v23 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + srcpitch)), _mm_setzero_si64());
		v24 = _m_paddb(v22, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v16[1]), _mm_setzero_si64()), v22), v19), 8u));
		v25 = _m_psrlwi(
			_m_pmullw(
				_m_paddb(
					v24,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v23,
									_m_psrlwi(
										_m_pmullw(
											_m_psubw(
												_m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + srcpitch + 4)), _mm_setzero_si64()),
												v23),
											v19),
										8u)),
								v24),
							v21),
						8u)),
				v10),
			8u);
		v26  = _m_psrlqi(v25, 0x30u);
		v27  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v28  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v26));
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v25, _m_psubw(v27, _m_psrlwi(_m_pmullw(v27, v28), 8u))), _mm_setzero_si64()));
		v13 += stepx;
		v14 += stepy;
	}
	_m_empty();
}
