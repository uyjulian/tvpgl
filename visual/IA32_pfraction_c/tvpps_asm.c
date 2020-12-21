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

#include <immintrin.h>
#include <mmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPPsAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm2
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(v8, _m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v5, _mm_setzero_si64()), v8), v9), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v7, _mm_setzero_si64()), v9), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm2
	__m64         v7; // mm2
	__m64         v8; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v5, _mm_setzero_si64()), v8), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAlphaBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v7, _mm_setzero_si64()), v9), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAddBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_paddusb(v5, v6);
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAddBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2
	__m64         v12; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_paddusb(v7, v8);
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), v12), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAddBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_paddusb(v5, v6);
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsAddBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_paddusb(v7, v8);
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11)), v11)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSubBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_psubusb(v6, _m_pxor(v5, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSubBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2
	__m64         v12; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_psubusb(v8, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), v12), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSubBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_psubusb(v6, _m_pxor(v5, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSubBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_psubusb(v8, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11)), v11)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsMulBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm6
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), v8), 8u), v8),
							v9),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsMulBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm6

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), v9), 8u), v9),
							v11),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsMulBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm6
	__m64         v7; // mm6
	__m64         v8; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), v8), 8u), v8),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsMulBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm6

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), v9), 8u), v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsScreenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = v5;
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(v6, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(_m_pmullw(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)), v10), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsScreenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2
	__m64         v11; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(v7, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(_m_pmullw(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsScreenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm2
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(v5, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsScreenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(v7, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v5))),
								v9),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2
	__m64         v15; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		v15 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v7))),
								v9),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v5))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v7))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14)), v14)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v5))),
								v9),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2
	__m64         v15; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		v15 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v7))),
								v9),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v7  = v6;
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v7, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v5))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v7))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14)), v14)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

#define CALC_USING_TABLE(x, s, d) (                 \
	(((s >> 24) & 0xff) << 24) |                    \
	(x[(s >> 16) & 0xff][(d >> 16) & 0xff] << 16) | \
	(x[(s >> 8) & 0xff][(d >> 8) & 0xff] << 8) |    \
	(x[(s >> 0) & 0xff][(d >> 0) & 0xff] << 0))

#define CALC_USING_TABLE_HDA(x, s, d) (             \
	(((d >> 24) & 0xff) << 24) |                    \
	(x[(s >> 16) & 0xff][(d >> 16) & 0xff] << 16) | \
	(x[(s >> 8) & 0xff][(d >> 8) & 0xff] << 8) |    \
	(x[(s >> 0) & 0xff][(d >> 0) & 0xff] << 0))

TVP_GL_IA32_FUNC_DECL(void, TVPPsSoftLightBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableSoftLight, src[i], dest[i])), _mm_setzero_si64()), v11),
							v12),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSoftLightBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableSoftLight, src[i], dest[i])), _mm_setzero_si64()), v12),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSoftLightBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableSoftLight, src[i], dest[i])), _mm_setzero_si64()), v11),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsSoftLightBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableSoftLight, src[i], dest[i])), _mm_setzero_si64()), v12),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodgeBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorDodge, src[i], dest[i])), _mm_setzero_si64()), v11),
							v12),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodgeBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorDodge, src[i], dest[i])), _mm_setzero_si64()), v12),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodgeBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorDodge, src[i], dest[i])), _mm_setzero_si64()), v11),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodgeBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorDodge, src[i], dest[i])), _mm_setzero_si64()), v12),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodge5Blend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v7;  // mm2
	__m64         v9;  // mm2
	int           v10; // ebx

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5 = _mm_cvtsi32_si64(src[i]);
		v7 = _m_psrldi(v5, 0x19u);
		v9 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		v10 = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), v9), 7u), _mm_setzero_si64()));
		dest[i] = CALC_USING_TABLE(TVPPsTableColorDodge, v10, dest[i]);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodge5Blend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v10; // mm2
	__m64         v11; // mm2
	int           v12; // ebx

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		v12 = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), v11), 7u), _mm_setzero_si64()));
		dest[i] = CALC_USING_TABLE(TVPPsTableColorDodge, v12, dest[i]);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodge5Blend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v7;  // mm2
	__m64         v9;  // mm2
	int           v10; // ebx

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v9  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		v10 = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), v9), 7u), _mm_setzero_si64()));
		dest[i] = CALC_USING_TABLE_HDA(TVPPsTableColorDodge, v10, dest[i]);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorDodge5Blend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v10; // mm2
	__m64         v11; // mm2
	int           v12; // ebx

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		v12 = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), v11), 7u), _mm_setzero_si64()));
		dest[i] = CALC_USING_TABLE_HDA(TVPPsTableColorDodge, v12, dest[i]);
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorBurnBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorBurn, src[i], dest[i])), _mm_setzero_si64()), v11),
							v12),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorBurnBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorBurn, src[i], dest[i])), _mm_setzero_si64()), v12),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorBurnBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v10 = _m_psrldi(v5, 0x19u);
		v11 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorBurn, src[i], dest[i])), _mm_setzero_si64()), v11),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsColorBurnBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v12; // mm1
	__m64         v13; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v12 = _m_punpcklbw(v8, _mm_setzero_si64());
		v13 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v12,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(CALC_USING_TABLE(TVPPsTableColorBurn, src[i], dest[i])), _mm_setzero_si64()), v12),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsLightenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm6
	__m64         v7;  // mm6
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v5, _mm_setzero_si64());
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(_m_pmullw(_m_psubw(_m_paddb(v9, _m_psubusb(v8, v9)), v8), v10), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsLightenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm0
	__m64         v12; // mm6

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _m_punpcklbw(v7, _mm_setzero_si64());
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_paddb(v11, _m_psubusb(v9, v11)), v9), v12),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsLightenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm0

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = v5;
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_paddb(v9, _m_psubusb(v8, v9)), v8),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsLightenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm0

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _m_punpcklbw(v7, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_paddb(v11, _m_psubusb(v9, v11)), v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDarkenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm6
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(v5, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(_m_pmullw(_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9), v10), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDarkenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm6

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(v7, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(_m_pmullw(_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDarkenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm6
	__m64         v7; // mm6
	__m64         v8; // mm0
	__m64         v9; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(v5, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDarkenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(v7, _mm_setzero_si64());
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiffBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_paddb(_m_psubusb(v6, v5), _m_psubusb(v5, v6));
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiffBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_paddb(_m_psubusb(v8, v7), _m_psubusb(v7, v8));
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _m_punpcklbw(v8, _mm_setzero_si64());
		v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v11), v12), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiffBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v6  = _mm_cvtsi32_si64(dest[i]);
		v8  = _m_psrldi(v5, 0x19u);
		v9  = _m_paddb(_m_psubusb(v6, v5), _m_psubusb(v5, v6));
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v10,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiffBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm2
	__m64         v11; // mm1

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _mm_cvtsi32_si64(dest[i]);
		v9  = _m_paddb(_m_psubusb(v8, v7), _m_psubusb(v7, v8));
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _m_punpcklbw(v8, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v11,
					_m_psrawi(
						_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v11), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiff5Blend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm6
	__m64         v7;  // mm6
	__m64         v8;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm0

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_psrawi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), v8), 7u);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v9, v10), _m_psubusw(v10, v9)), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiff5Blend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm6
	__m64         v10; // mm6
	__m64         v11; // mm1
	__m64         v12; // mm0

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
		v11 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v12 = _m_psrawi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), v10), 7u);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v11, v12), _m_psubusw(v12, v11)), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiff5Blend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm6
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm0

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_psrawi(_m_pmullw(_m_punpcklbw(v5, _mm_setzero_si64()), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)), 7u);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v8, v9), _m_psubusw(v9, v8)), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsDiff5Blend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v9;  // mm6
	__m64         v10; // mm1
	__m64         v11; // mm0

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v11 = _m_psrawi(_m_pmullw(_m_punpcklbw(v7, _mm_setzero_si64()), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9)), v9)), 7u);
		dest[i] = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v10, v11), _m_psubusw(v11, v10)), _mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsExclusionBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm2
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v5, _mm_setzero_si64());
		v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(_m_pmullw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)), v10), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsExclusionBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2
	__m64         v11; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v7, _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(_m_pmullw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)), v11), 7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsExclusionBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5; // mm2
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm6

	for (tjs_int i = 0; i < len; i += 1)
	{
		v5  = _mm_cvtsi32_si64(src[i]);
		v7  = _m_psrldi(v5, 0x19u);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v5, _mm_setzero_si64());
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsExclusionBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	for (tjs_int i = 0; i < len; i += 1)
	{
		v7  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v9  = _m_punpcklbw(v7, _mm_setzero_si64());
		v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v8,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
