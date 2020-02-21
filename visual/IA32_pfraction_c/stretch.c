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

void __cdecl TVPStretchCopy_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)
{
	tjs_uint32 * v5; // edi
	unsigned int v6; // ebx
	tjs_uint32 * v7; // ebp

	v5 = dest;
	v6 = srcstart;
	v7 = &dest[len];
	while (v5 < v7)
	{
		*v5 = _mm_cvtsi64_si32(_mm_cvtsi32_si64(src[v6 >> 16]));
		v6 += srcstep;
		++v5;
	}
	_m_empty();
}

void __cdecl TVPStretchConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)
{
	unsigned int *v6;  // edi
	unsigned int  v7;  // ebx
	__m64         v9;  // mm7
	tjs_uint32 *  v10; // ebp
	__m64         v11; // mm7
	__m64         v12; // mm1

	v6  = dest;
	v7  = srcstart;
	v9  = _mm_set1_pi16(opa);
	v10 = &dest[len];
	v11 = v9;
	while (v6 < v10)
	{
		v12 = _m_punpcklbw(_mm_cvtsi32_si64(*v6), _mm_setzero_si64());
		*v6 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_paddw(
						_m_psllwi(v12, 8u),
						_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src[v7 >> 16]), _mm_setzero_si64()), v12), v11)),
					8u),
				_mm_setzero_si64()));
		v7 += srcstep;
		++v6;
	}
	_m_empty();
}

void __cdecl TVPStretchAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)
{
	unsigned int *v5;  // edi
	unsigned int  v6;  // ebx
	tjs_uint32 *  v7;  // ebp
	__m64         v8;  // mm3
	__m64         v9;  // mm5
	__m64         v10; // mm4
	__m64         v11; // mm5

	v5 = dest;
	v6 = srcstart;
	v7 = &dest[len];
	while (v5 < v7)
	{
		v8  = _mm_cvtsi32_si64(src[v6 >> 16]);
		v9  = _m_psrlqi(v8, 0x18u);
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64());
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		*v5 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(_m_paddw(_m_pmullw(_m_psubw(_m_punpcklbw(v8, _mm_setzero_si64()), v10), v11), _m_psllwi(v10, 8u)), 8u),
				_mm_setzero_si64()));
		v6 += srcstep;
		++v5;
	}
	_m_empty();
}

void __cdecl TVPStretchAdditiveAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)
{
	unsigned int *v5;  // edi
	unsigned int  v6;  // ebx
	tjs_uint32 *  v7;  // ebp
	__m64         v8;  // mm4
	__m64         v9;  // mm2
	__m64         v10; // mm2
	__m64         v11; // mm1

	v5 = dest;
	v6 = srcstart;
	v7 = &dest[len];
	while (v5 < v7)
	{
		v8  = _mm_cvtsi32_si64(src[v6 >> 16]);
		v9  = _m_psrlqi(v8, 0x18u);
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v5), _mm_setzero_si64());
		*v5 = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v10), 8u)), _mm_setzero_si64()), v8));
		v6 += srcstep;
		++v5;
	}
	_m_empty();
}

void __cdecl TVPFastLinearInterpV2_mmx_pfraction_c(tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src0, const tjs_uint32 *src1)
{
	tjs_int i; // ecx

	for (i = 0; i < len; ++i)
	{
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_paddw(
						_m_punpcklbw(_mm_cvtsi32_si64(src0[i]), _mm_setzero_si64()),
						_m_punpcklbw(_mm_cvtsi32_si64(src1[i]), _mm_setzero_si64())),
					1u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

void __cdecl TVPFastLinearInterpH2F_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)
{
	tjs_int i; // ecx

	for (i = 0; i < destlen; ++i)
	{
		dest[i] = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*src));
	}
	_m_empty();
}

void __cdecl TVPFastLinearInterpH2B_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)
{
	tjs_int i; // ecx

	for (i = 0; i < destlen; ++i)
	{
		dest[i] = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*src));
	}
	_m_empty();
}

void __cdecl TVPInterpStretchCopy_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)
{
	__m64        v8;  // mm7
	__m64        v9;  // mm7
	tjs_uint32 * v10; // edi
	unsigned int v11; // ebx
	tjs_uint32 * v12; // ebp
	__m64        v13; // mm1
	__m64        v15; // mm3
	__m64        v16; // mm5
	__m64        v17; // mm5
	__m64        v18; // mm1

	v8  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v9  = v8;
	v10 = dest;
	v11 = srcstart;
	v12 = &dest[destlen];
	while (v10 < v12)
	{
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src1[v11 >> 16]), _mm_setzero_si64());
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[v11 >> 16]), _mm_setzero_si64());
		v16 = _mm_set1_pi16((tjs_uint16)v11 >> 8);
		v17 = v16;
		v18 = _m_paddb(
			v13,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(v11 >> 16) + 1]), _mm_setzero_si64()), v13), v17),
				8u));
		*v10 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddb(
					v18,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v15,
									_m_psrlwi(
										_m_pmullw(
											_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(v11 >> 16) + 1]), _mm_setzero_si64()), v15),
											v17),
										8u)),
								v18),
							v9),
						8u)),
				_mm_setzero_si64()));
		v11 += srcstep;
		++v10;
	}
	_m_empty();
}

void __cdecl TVPInterpStretchConstAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)
{
	__m64         v9;  // mm7
	__m64         v10; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm6
	unsigned int *v14; // edi
	unsigned int  v15; // ebx
	tjs_uint32 *  v16; // ebp
	__m64         v17; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm5
	__m64         v21; // mm5
	__m64         v22; // mm1
	__m64         v23; // mm2

	v9  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v10 = v9;
	v12 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	v13 = v12;
	v14 = dest;
	v15 = srcstart;
	v16 = &dest[destlen];
	while (v14 < v16)
	{
		v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[v15 >> 16]), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[v15 >> 16]), _mm_setzero_si64());
		v20 = _mm_set1_pi16((tjs_uint16)v15 >> 8);
		v21 = v20;
		v22 = _m_paddb(
			v17,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(v15 >> 16) + 1]), _mm_setzero_si64()), v17), v21),
				8u));
		v23  = _m_punpcklbw(_mm_cvtsi32_si64(*v14), _mm_setzero_si64());
		*v14 = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddb(
					v23,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v22,
									_m_psrlwi(
										_m_pmullw(
											_m_psubw(
												_m_paddb(
													v19,
													_m_psrlwi(
														_m_pmullw(
															_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(v15 >> 16) + 1]), _mm_setzero_si64()), v19),
															v21),
														8u)),
												v22),
											v10),
										8u)),
								v23),
							v13),
						8u)),
				_mm_setzero_si64()));
		v15 += srcstep;
		++v14;
	}
	_m_empty();
}

void __cdecl TVPInterpStretchAdditiveAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)
{
	__m64         v8;  // mm7
	__m64         v9;  // mm7
	unsigned int *v10; // edi
	unsigned int  v11; // ebx
	tjs_uint32 *  v12; // ebp
	__m64         v13; // mm1
	__m64         v15; // mm3
	__m64         v16; // mm5
	__m64         v17; // mm5
	__m64         v18; // mm1
	__m64         v19; // mm1
	__m64         v20; // mm3
	__m64         v21; // mm2
	__m64         v22; // mm3

	v8  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v9  = v8;
	v10 = dest;
	v11 = srcstart;
	v12 = &dest[destlen];
	while (v10 < v12)
	{
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src1[v11 >> 16]), _mm_setzero_si64());
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[v11 >> 16]), _mm_setzero_si64());
		v16 = _mm_set1_pi16((tjs_uint16)v11 >> 8);
		v17 = v16;
		v18 = _m_paddb(
			v13,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(v11 >> 16) + 1]), _mm_setzero_si64()), v13), v17),
				8u));
		v19 = _m_paddb(
			v18,
			_m_psrlwi(
				_m_pmullw(
					_m_psubw(
						_m_paddb(
							v15,
							_m_psrlwi(
								_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(v11 >> 16) + 1]), _mm_setzero_si64()), v15), v17),
								8u)),
						v18),
					v9),
				8u));
		v20  = _m_psrlqi(v19, 0x30u);
		v21  = _m_punpcklbw(_mm_cvtsi32_si64(*v10), _mm_setzero_si64());
		v22  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v20));
		*v10 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v19, _m_psubw(v21, _m_psrlwi(_m_pmullw(v21, v22), 8u))), _mm_setzero_si64()));
		v11 += srcstep;
		++v10;
	}
	_m_empty();
}

void __cdecl TVPInterpStretchAdditiveAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)
{
	__m64         v9;  // mm7
	__m64         v10; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm6
	unsigned int *v14; // edi
	unsigned int  v15; // ebx
	tjs_uint32 *  v16; // ebp
	__m64         v17; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm5
	__m64         v21; // mm5
	__m64         v22; // mm1
	__m64         v23; // mm1
	__m64         v24; // mm3
	__m64         v25; // mm2
	__m64         v26; // mm3

	v9  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v10 = v9;
	v12 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	v13 = v12;
	v14 = dest;
	v15 = srcstart;
	v16 = &dest[destlen];
	while (v14 < v16)
	{
		v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[v15 >> 16]), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[v15 >> 16]), _mm_setzero_si64());
		v20 = _mm_set1_pi16((tjs_uint16)v15 >> 8);
		v21 = v20;
		v22 = _m_paddb(
			v17,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(v15 >> 16) + 1]), _mm_setzero_si64()), v17), v21),
				8u));
		v23 = _m_psrlwi(
			_m_pmullw(
				_m_paddb(
					v22,
					_m_psrlwi(
						_m_pmullw(
							_m_psubw(
								_m_paddb(
									v19,
									_m_psrlwi(
										_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(v15 >> 16) + 1]), _mm_setzero_si64()), v19), v21),
										8u)),
								v22),
							v10),
						8u)),
				v13),
			8u);
		v24  = _m_psrlqi(v23, 0x30u);
		v25  = _m_punpcklbw(_mm_cvtsi32_si64(*v14), _mm_setzero_si64());
		v26  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v24));
		*v14 = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v23, _m_psubw(v25, _m_psrlwi(_m_pmullw(v25, v26), 8u))), _mm_setzero_si64()));
		v15 += srcstep;
		++v14;
	}
	_m_empty();
}
