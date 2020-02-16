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
	tjs_uint32 *v8;  // edi
	__m64       v9;  // mm0
	__m64       v10; // mm2
	tjs_uint32 *v11; // ebp
	__m64       v12; // mm5

	if (len > 0)
	{
		v8  = dest;
		v9  = _m_por(_mm_cvtsi32_si64(sx), _m_psllqi(_mm_cvtsi32_si64(sy), 0x20u));
		v10 = _m_por(_mm_cvtsi32_si64(stepx), _m_psllqi(_mm_cvtsi32_si64(stepy), 0x20u));
		v11 = &dest[len];
		if (dest < v11)
		{
			do
			{
				v12 = _m_psradi(v9, 0x10u);
				*v8 = *(const tjs_uint32 *)((char *)&src[_mm_cvtsi64_si32(v12)] + srcpitch * _mm_cvtsi64_si32(_m_psrlqi(v12, 0x20u)));
				v9  = _m_paddd(v9, v10);
				++v8;
			} while (v8 < v11);
		}
	}
	_m_empty();
}

void __cdecl TVPLinTransConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
{
	unsigned int *v9;  // edi
	__m64         v10; // mm3
	__m64         v11; // mm3
	__m64         v12; // mm3
	__m64         v13; // mm0
	tjs_uint32 *  v14; // ebp
	__m64         v15; // mm5
	__m64         v16; // mm1
	__m64         v17; // [esp+0h] [ebp-28h]

	if (len > 0)
	{
		v9  = dest;
		v10 = _mm_cvtsi32_si64(opa);
		v11 = _m_punpcklwd(v10, v10);
		v12 = v11;
		v13 = _m_por(_mm_cvtsi32_si64(sx), _m_psllqi(_mm_cvtsi32_si64(sy), 0x20u));
		v17 = _m_por(_mm_cvtsi32_si64(stepx), _m_psllqi(_mm_cvtsi32_si64(stepy), 0x20u));
		v14 = &dest[len];
		if (dest < v14)
		{
			do
			{
				v15 = _m_psradi(v13, 0x10u);
				v16 = _m_punpcklbw(_mm_cvtsi32_si64(*v9), _mm_setzero_si64());
				*v9 = _mm_cvtsi64_si32(
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
									v12)),
							8u),
						_mm_setzero_si64()));
				v13 = _m_paddd(v13, v17);
				++v9;
			} while (v9 < v14);
		}
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransCopy_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
{
	tjs_uint32 *  v8;  // edi
	unsigned int  v9;  // ecx
	unsigned int  v10; // edx
	tjs_uint32 *  v11; // ebp
	unsigned int *v12; // eax
	__m64         v13; // mm5
	__m64         v14; // mm5
	__m64         v15; // mm5
	__m64         v16; // mm7
	__m64         v17; // mm7
	__m64         v18; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm1

	if (len > 0)
	{
		v8  = dest;
		v9  = sx;
		v10 = sy;
		v11 = &dest[len];
		if (dest < v11)
		{
			do
			{
				v12 = (unsigned int *)((char *)&src[v9 >> 16] + srcpitch * (v10 >> 16));
				v13 = _mm_cvtsi32_si64((unsigned int)(tjs_uint16)v9 >> 8);
				v14 = _m_punpcklwd(v13, v13);
				v15 = v14;
				v16 = _mm_cvtsi32_si64(((unsigned int)(tjs_uint16)v10 >> 8) + ((unsigned int)(tjs_uint16)v10 >> 15));
				v17 = _m_punpcklwd(v16, v16);
				v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
				v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch)), _mm_setzero_si64());
				v20 = _m_paddb(v18, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v12[1]), _mm_setzero_si64()), v18), v15), 8u));
				*v8 = _mm_cvtsi64_si32(
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
													v15),
												8u)),
										v20),
									v17),
								8u)),
						_mm_setzero_si64()));
				v9 += stepx;
				v10 += stepy;
				++v8;
			} while (v8 < v11);
		}
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
{
	__m64         v9;  // mm6
	__m64         v10; // mm6
	__m64         v11; // mm6
	unsigned int *v12; // edi
	unsigned int  v13; // ecx
	unsigned int  v14; // edx
	tjs_uint32 *  v15; // ebp
	unsigned int *v16; // eax
	__m64         v17; // mm5
	__m64         v18; // mm5
	__m64         v19; // mm5
	__m64         v20; // mm7
	__m64         v21; // mm7
	__m64         v22; // mm1
	__m64         v23; // mm3
	__m64         v24; // mm1
	__m64         v25; // mm2

	if (len > 0)
	{
		v9  = _mm_cvtsi32_si64(((unsigned int)opa >> 7) + opa);
		v10 = _m_punpcklwd(v9, v9);
		v11 = v10;
		v12 = dest;
		v13 = sx;
		v14 = sy;
		v15 = &dest[len];
		if (dest < v15)
		{
			do
			{
				v16  = (unsigned int *)((char *)&src[v13 >> 16] + srcpitch * (v14 >> 16));
				v17  = _mm_cvtsi32_si64((unsigned int)(tjs_uint16)v13 >> 8);
				v18  = _m_punpcklwd(v17, v17);
				v19  = v18;
				v20  = _mm_cvtsi32_si64(((unsigned int)(tjs_uint16)v14 >> 8) + ((unsigned int)(tjs_uint16)v14 >> 15));
				v21  = _m_punpcklwd(v20, v20);
				v22  = _m_punpcklbw(_mm_cvtsi32_si64(*v16), _mm_setzero_si64());
				v23  = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v16 + srcpitch)), _mm_setzero_si64());
				v24  = _m_paddb(v22, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v16[1]), _mm_setzero_si64()), v22), v19), 8u));
				v25  = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
				*v12 = _mm_cvtsi64_si32(
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
									v11),
								8u)),
						_mm_setzero_si64()));
				v13 += stepx;
				v14 += stepy;
				++v12;
			} while (v12 < v15);
		}
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransAdditiveAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)
{
	unsigned int *v8;  // edi
	unsigned int  v9;  // ecx
	unsigned int  v10; // edx
	tjs_uint32 *  v11; // ebp
	unsigned int *v12; // eax
	__m64         v13; // mm5
	__m64         v14; // mm5
	__m64         v15; // mm5
	__m64         v16; // mm7
	__m64         v17; // mm7
	__m64         v18; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm1
	__m64         v21; // mm1
	__m64         v22; // mm3
	__m64         v23; // mm2
	__m64         v24; // mm3

	if (len > 0)
	{
		v8  = dest;
		v9  = sx;
		v10 = sy;
		v11 = &dest[len];
		if (dest < v11)
		{
			do
			{
				v12 = (unsigned int *)((char *)&src[v9 >> 16] + srcpitch * (v10 >> 16));
				v13 = _mm_cvtsi32_si64((unsigned int)(tjs_uint16)v9 >> 8);
				v14 = _m_punpcklwd(v13, v13);
				v15 = v14;
				v16 = _mm_cvtsi32_si64(((unsigned int)(tjs_uint16)v10 >> 8) + ((unsigned int)(tjs_uint16)v10 >> 15));
				v17 = _m_punpcklwd(v16, v16);
				v18 = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
				v19 = _m_punpcklbw(_mm_cvtsi32_si64(*(unsigned int *)((char *)v12 + srcpitch)), _mm_setzero_si64());
				v20 = _m_paddb(v18, _m_psrlwi(_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(v12[1]), _mm_setzero_si64()), v18), v15), 8u));
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
											v15),
										8u)),
								v20),
							v17),
						8u));
				v22 = _m_psrlqi(v21, 0x30u);
				v23 = _m_punpcklbw(_mm_cvtsi32_si64(*v8), _mm_setzero_si64());
				v24 = _m_punpcklwd(v22, v22);
				*v8 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v21, _m_psubw(v23, _m_psrlwi(_m_pmullw(v23, v24), 8u))), _mm_setzero_si64()));
				v9 += stepx;
				v10 += stepy;
				++v8;
			} while (v8 < v11);
		}
	}
	_m_empty();
}

void __cdecl TVPInterpLinTransAdditiveAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)
{
	__m64         v9;  // mm6
	__m64         v10; // mm6
	__m64         v11; // mm6
	unsigned int *v12; // edi
	unsigned int  v13; // ecx
	unsigned int  v14; // edx
	tjs_uint32 *  v15; // ebp
	unsigned int *v16; // eax
	__m64         v17; // mm5
	__m64         v18; // mm5
	__m64         v19; // mm5
	__m64         v20; // mm7
	__m64         v21; // mm7
	__m64         v22; // mm1
	__m64         v23; // mm3
	__m64         v24; // mm1
	__m64         v25; // mm1
	__m64         v26; // mm3
	__m64         v27; // mm2
	__m64         v28; // mm3

	if (len > 0)
	{
		v9  = _mm_cvtsi32_si64(((unsigned int)opa >> 7) + opa);
		v10 = _m_punpcklwd(v9, v9);
		v11 = v10;
		v12 = dest;
		v13 = sx;
		v14 = sy;
		v15 = &dest[len];
		if (dest < v15)
		{
			do
			{
				v16 = (unsigned int *)((char *)&src[v13 >> 16] + srcpitch * (v14 >> 16));
				v17 = _mm_cvtsi32_si64((unsigned int)(tjs_uint16)v13 >> 8);
				v18 = _m_punpcklwd(v17, v17);
				v19 = v18;
				v20 = _mm_cvtsi32_si64(((unsigned int)(tjs_uint16)v14 >> 8) + ((unsigned int)(tjs_uint16)v14 >> 15));
				v21 = _m_punpcklwd(v20, v20);
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
						v11),
					8u);
				v26  = _m_psrlqi(v25, 0x30u);
				v27  = _m_punpcklbw(_mm_cvtsi32_si64(*v12), _mm_setzero_si64());
				v28  = _m_punpcklwd(v26, v26);
				*v12 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v25, _m_psubw(v27, _m_psrlwi(_m_pmullw(v27, v28), 8u))), _mm_setzero_si64()));
				v13 += stepx;
				v14 += stepy;
				++v12;
			} while (v12 < v15);
		}
	}
	_m_empty();
}
