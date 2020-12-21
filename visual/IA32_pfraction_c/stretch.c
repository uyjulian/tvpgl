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

TVP_GL_IA32_FUNC_DECL(void, TVPStretchCopy_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		dest[i] = src[j >> 16];
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64         v9;  // mm7
	__m64         v12; // mm1

	v9  = _mm_set1_pi16(opa);
	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		v12 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(
					_m_paddw(
						_m_psllwi(v12, 8u),
						_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src[j >> 16]), _mm_setzero_si64()), v12), v9)),
					8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	__m64         v8;  // mm3
	__m64         v9;  // mm5
	__m64         v10; // mm4
	__m64         v11; // mm5

	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		v8  = _mm_cvtsi32_si64(src[j >> 16]);
		v9  = _m_psrlqi(v8, 0x18u);
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_psrlwi(_m_paddw(_m_pmullw(_m_psubw(_m_punpcklbw(v8, _mm_setzero_si64()), v10), v11), _m_psllwi(v10, 8u)), 8u),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	__m64         v8;  // mm4
	__m64         v9;  // mm2
	__m64         v10; // mm2
	__m64         v11; // mm1

	for (tjs_int i = 0, j = srcstart; i < len; i += 1, j += srcstep)
	{
		v8  = _mm_cvtsi32_si64(src[j >> 16]);
		v9  = _m_psrlqi(v8, 0x18u);
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(_m_paddusb(_m_packuswb(_m_psubw(v11, _m_psrlwi(_m_pmullw(v11, v10), 8u)), _mm_setzero_si64()), v8));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPFastLinearInterpV2_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src0, const tjs_uint32 *src1))
{
	for (tjs_int i = 0; i < len; i += 1)
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

TVP_GL_IA32_FUNC_DECL(void, TVPFastLinearInterpH2F_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src))
{
	for (tjs_int i = 0; i < destlen; i += 1)
	{
		dest[i] = *src;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPFastLinearInterpH2B_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src))
{
	for (tjs_int i = 0; i < destlen; i += 1)
	{
		dest[i] = *src;
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
	__m64        v8;  // mm7
	__m64        v13; // mm1
	__m64        v15; // mm3
	__m64        v16; // mm5
	__m64        v18; // mm1

	v8  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		v16 = _mm_set1_pi16((tjs_uint16)j >> 8);
		v18 = _m_paddb(
			v13,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64()), v13), v16),
				8u));
		dest[i] = _mm_cvtsi64_si32(
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
											_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64()), v15),
											v16),
										8u)),
								v18),
							v8),
						8u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64         v9;  // mm7
	__m64         v12; // mm6
	__m64         v17; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm5
	__m64         v22; // mm1
	__m64         v23; // mm2

	v9  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v12 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		v22 = _m_paddb(
			v17,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64()), v17), v20),
				8u));
		v23  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
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
															_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64()), v19),
															v20),
														8u)),
												v22),
											v9),
										8u)),
								v23),
							v12),
						8u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
	__m64         v8;  // mm7
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
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		v13 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		v15 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		v16 = _mm_set1_pi16((tjs_uint16)j >> 8);
		v17 = v16;
		v18 = _m_paddb(
			v13,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64()), v13), v17),
				8u));
		v19 = _m_paddb(
			v18,
			_m_psrlwi(
				_m_pmullw(
					_m_psubw(
						_m_paddb(
							v15,
							_m_psrlwi(
								_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64()), v15), v17),
								8u)),
						v18),
					v8),
				8u));
		v20  = _m_psrlqi(v19, 0x30u);
		v21  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v22  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v20));
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v19, _m_psubw(v21, _m_psrlwi(_m_pmullw(v21, v22), 8u))), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	__m64         v9;  // mm7
	__m64         v12; // mm6
	__m64         v17; // mm1
	__m64         v19; // mm3
	__m64         v20; // mm5
	__m64         v22; // mm1
	__m64         v23; // mm1
	__m64         v24; // mm3
	__m64         v25; // mm2
	__m64         v26; // mm3

	v9  = _mm_set1_pi16(((unsigned int)blend_y >> 7) + blend_y);
	v12 = _mm_set1_pi16(((unsigned int)opa >> 7) + opa);
	for (tjs_int i = 0, j = srcstart; i < destlen; i += 1, j += srcstep)
	{
		v17 = _m_punpcklbw(_mm_cvtsi32_si64(src1[j >> 16]), _mm_setzero_si64());
		v19 = _m_punpcklbw(_mm_cvtsi32_si64(src2[j >> 16]), _mm_setzero_si64());
		v20 = _mm_set1_pi16((tjs_uint16)j >> 8);
		v22 = _m_paddb(
			v17,
			_m_psrlwi(
				_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src1[(j >> 16) + 1]), _mm_setzero_si64()), v17), v20),
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
										_m_pmullw(_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(src2[(j >> 16) + 1]), _mm_setzero_si64()), v19), v20),
										8u)),
								v22),
							v9),
						8u)),
				v12),
			8u);
		v24  = _m_psrlqi(v23, 0x30u);
		v25  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v26  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v24));
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(v23, _m_psubw(v25, _m_psrlwi(_m_pmullw(v25, v26), 8u))), _mm_setzero_si64()));
	}
	_m_empty();
}
