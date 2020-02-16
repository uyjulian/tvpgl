/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <immintrin.h>
#include <mmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

void __cdecl TVPPsAlphaBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm2
	__m64         v6; // mm0
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(v8, _m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v6, _mm_setzero_si64()), v8), _m_punpckldq(v9, v9)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAlphaBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm2
	__m64         v11; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v8, _mm_setzero_si64()), v9), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAlphaBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm2
	__m64         v6; // mm0
	__m64         v7; // mm2
	__m64         v8; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v6, _mm_setzero_si64()), v8), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAlphaBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v8, _mm_setzero_si64()), v9), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAddBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_paddusb(v7, v6);
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAddBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2
	__m64         v12; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_paddusb(v7, v8);
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(v12, v12)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAddBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_paddusb(v7, v6);
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsAddBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_paddusb(v7, v8);
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11)), v11)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSubBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm7
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_psubusb(v6, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSubBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2
	__m64         v12; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_psubusb(v8, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(v12, v12)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSubBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm7
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_psubusb(v6, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSubBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_psubusb(v8, _m_pxor(v7, _m_pcmpeqd(_mm_setzero_si64(), _mm_setzero_si64())));
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v11)), v11)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsMulBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm6

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), v8), 8u), v8),
								_m_punpckldq(v9, v9)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsMulBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm6

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), v9), 8u), v9),
								_m_punpckldq(v11, v11)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsMulBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), v8), 8u), v8),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsMulBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psrlwi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), v9), 8u), v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsScreenBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = v5;
			v7  = _m_psrldi(v5, 0x19u);
			v8  = _m_punpcklbw(v6, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(_m_pmullw(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)), _m_punpckldq(v10, v10)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsScreenBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2
	__m64         v11; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(v7, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(_m_pmullw(_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsScreenBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm2
	__m64         v6; // mm1
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm6

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(v6, _mm_setzero_si64());
			v9 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsScreenBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(v7, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(v8, _m_psrlwi(_m_pmullw(v8, v9), 8u)),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsOverlayBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm6
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v6  = _mm_cvtsi32_si64(*v4);
			v7  = v6;
			v8  = _m_psrldi(v6, 0x19u);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _m_punpcklbw(v7, _mm_setzero_si64());
			v11 = v10;
			v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
			v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(
										_m_pand(v12, v13),
										_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v11, v9), 1u), v12), v5))),
									v9),
								_m_punpckldq(v14, v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsOverlayBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm6
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2
	__m64         v15; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v8  = _mm_cvtsi32_si64(*v5);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = v10;
			v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
			v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
			v15 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(
										_m_pand(v12, v13),
										_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v11, v9), 1u), v12), v7))),
									v9),
								_m_punpckldq(v15, v15)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsOverlayBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm6
	__m64         v12; // mm0
	__m64         v13; // mm7

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v6  = _mm_cvtsi32_si64(*v4);
			v7  = v6;
			v8  = _m_psrldi(v6, 0x19u);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _m_punpcklbw(v7, _mm_setzero_si64());
			v11 = v10;
			v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(
										_m_pand(v12, v13),
										_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v11, v9), 1u), v12), v5))),
									v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsOverlayBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm6
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v8  = _mm_cvtsi32_si64(*v5);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = v10;
			v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
			v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(
										_m_pand(v12, v13),
										_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v11, v9), 1u), v12), v7))),
									v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14)), v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsHardLightBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v6  = _mm_cvtsi32_si64(*v4);
			v7  = v6;
			v8  = _m_psrldi(v6, 0x19u);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _m_punpcklbw(v7, _mm_setzero_si64());
			v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
			v12 = _m_paddw(v10, v9);
			v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v14 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v5))),
									v9),
								_m_punpckldq(v14, v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsHardLightBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
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

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v8  = _mm_cvtsi32_si64(*v5);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
			v12 = _m_paddw(v10, v9);
			v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
			v15 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v14));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(
									_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v7))),
									v9),
								_m_punpckldq(v15, v15)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsHardLightBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v6  = _mm_cvtsi32_si64(*v4);
			v7  = v6;
			v8  = _m_psrldi(v6, 0x19u);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _m_punpcklbw(v7, _mm_setzero_si64());
			v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
			v12 = _m_paddw(v10, v9);
			v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
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
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsHardLightBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
		do
		{
			v8  = _mm_cvtsi32_si64(*v5);
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_punpcklbw(v8, _mm_setzero_si64());
			v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
			v12 = _m_paddw(v10, v9);
			v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
			v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
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
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSoftLightBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableSoftLight[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableSoftLight[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableSoftLight[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			v12        = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(v12, v12)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSoftLightBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableSoftLight[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableSoftLight[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableSoftLight[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v14         = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(v14, v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSoftLightBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableSoftLight[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableSoftLight[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableSoftLight[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsSoftLightBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableSoftLight[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableSoftLight[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableSoftLight[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodgeBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableColorDodge[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableColorDodge[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableColorDodge[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			v12        = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(v12, v12)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodgeBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableColorDodge[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableColorDodge[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableColorDodge[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v14         = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(v14, v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodgeBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableColorDodge[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableColorDodge[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableColorDodge[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodgeBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableColorDodge[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableColorDodge[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableColorDodge[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodge5Blend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm0
	__m64         v7;  // mm2
	int           v8;  // edx
	__m64         v9;  // mm2
	int           v10; // ebx
	tjs_uint16    v11; // ax

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*v3));
			v9 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			v10         = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), _m_punpckldq(v9, v9)), 7u), _mm_setzero_si64()));
			HIBYTE(v11) = v10;
			LOBYTE(v11) = v8;
			LOBYTE(v10) = TVPPsTableColorDodge[0][v11];
			HIBYTE(v11) = BYTE1(v10);
			++v3;
			LOBYTE(v11) = BYTE1(v8);
			BYTE1(v10)  = TVPPsTableColorDodge[0][v11];
			v10         = _rotr(v10, 16);
			HIBYTE(v11) = v10;
			LOBYTE(v11) = BYTE2(v8);
			LOBYTE(v10) = TVPPsTableColorDodge[0][v11];
			*(v3 - 1)   = _rotr(v10, 16);
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodge5Blend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm0
	int           v9;  // edx
	__m64         v10; // mm2
	__m64         v11; // mm2
	int           v12; // ebx
	tjs_uint16    v13; // ax

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*v4));
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			v12         = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), _m_punpckldq(v11, v11)), 7u), _mm_setzero_si64()));
			HIBYTE(v13) = v12;
			LOBYTE(v13) = v9;
			LOBYTE(v12) = TVPPsTableColorDodge[0][v13];
			HIBYTE(v13) = BYTE1(v12);
			++v4;
			LOBYTE(v13) = BYTE1(v9);
			BYTE1(v12)  = TVPPsTableColorDodge[0][v13];
			v12         = _rotr(v12, 16);
			HIBYTE(v13) = v12;
			LOBYTE(v13) = BYTE2(v9);
			LOBYTE(v12) = TVPPsTableColorDodge[0][v13];
			*(v4 - 1)   = _rotr(v12, 16);
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodge5Blend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm0
	__m64         v7;  // mm2
	unsigned int  v8;  // edx
	__m64         v9;  // mm2
	int           v10; // ebx
	tjs_uint16    v11; // ax

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*v3));
			v9 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			v10         = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), _m_punpckldq(v9, v9)), 7u), _mm_setzero_si64()));
			HIBYTE(v11) = v10;
			LOBYTE(v11) = v8;
			LOBYTE(v10) = TVPPsTableColorDodge[0][v11];
			HIBYTE(v11) = BYTE1(v10);
			++v3;
			LOBYTE(v11) = BYTE1(v8);
			v8 >>= 16;
			BYTE1(v10)  = TVPPsTableColorDodge[0][v11];
			v10         = _rotr(v10, 16);
			HIBYTE(v11) = v10;
			BYTE1(v10)  = BYTE1(v8);
			LOBYTE(v11) = v8;
			LOBYTE(v10) = TVPPsTableColorDodge[0][v11];
			*(v3 - 1)   = _rotr(v10, 16);
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorDodge5Blend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm0
	unsigned int  v9;  // edx
	__m64         v10; // mm2
	__m64         v11; // mm2
	int           v12; // ebx
	tjs_uint16    v13; // ax

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _mm_cvtsi64_si32(_mm_cvtsi32_si64(*v4));
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			v12         = _mm_cvtsi64_si32(_m_packuswb(_m_psrawi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), _m_punpckldq(v11, v11)), 7u), _mm_setzero_si64()));
			HIBYTE(v13) = v12;
			LOBYTE(v13) = v9;
			LOBYTE(v12) = TVPPsTableColorDodge[0][v13];
			HIBYTE(v13) = BYTE1(v12);
			++v4;
			LOBYTE(v13) = BYTE1(v9);
			v9 >>= 16;
			BYTE1(v12)  = TVPPsTableColorDodge[0][v13];
			v12         = _rotr(v12, 16);
			HIBYTE(v13) = v12;
			BYTE1(v12)  = BYTE1(v9);
			LOBYTE(v13) = v9;
			LOBYTE(v12) = TVPPsTableColorDodge[0][v13];
			*(v4 - 1)   = _rotr(v12, 16);
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorBurnBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableColorBurn[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableColorBurn[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableColorBurn[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			v12        = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(v12, v12)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorBurnBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2
	__m64         v14; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableColorBurn[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableColorBurn[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableColorBurn[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v14         = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(v14, v14)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorBurnBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	int           v7;  // ebx
	int           v8;  // edx
	tjs_uint16    v9;  // ax
	__m64         v10; // mm2
	__m64         v11; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5         = _mm_cvtsi32_si64(*v4);
			v6         = _mm_cvtsi32_si64(*v3);
			v7         = _mm_cvtsi64_si32(v5);
			v8         = _mm_cvtsi64_si32(v6);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = v8;
			LOBYTE(v7) = TVPPsTableColorBurn[0][v9];
			HIBYTE(v9) = BYTE1(v7);
			LOBYTE(v9) = BYTE1(v8);
			BYTE1(v7)  = TVPPsTableColorBurn[0][v9];
			v7         = _rotr(v7, 16);
			HIBYTE(v9) = v7;
			LOBYTE(v9) = BYTE2(v8);
			LOBYTE(v7) = TVPPsTableColorBurn[0][v9];
			v10        = _m_psrldi(v5, 0x19u);
			v11        = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v7, 16)), _mm_setzero_si64()), v11),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsColorBurnBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	int           v9;  // ebx
	int           v10; // edx
	tjs_uint16    v11; // ax
	__m64         v12; // mm1
	__m64         v13; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7          = _mm_cvtsi32_si64(*v5);
			v8          = _mm_cvtsi32_si64(*v4);
			v9          = _mm_cvtsi64_si32(v7);
			v10         = _mm_cvtsi64_si32(v8);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = v10;
			LOBYTE(v9)  = TVPPsTableColorBurn[0][v11];
			HIBYTE(v11) = BYTE1(v9);
			LOBYTE(v11) = BYTE1(v10);
			BYTE1(v9)   = TVPPsTableColorBurn[0][v11];
			v9          = _rotr(v9, 16);
			HIBYTE(v11) = v9;
			LOBYTE(v11) = BYTE2(v10);
			LOBYTE(v9)  = TVPPsTableColorBurn[0][v11];
			v12         = _m_punpcklbw(v8, _mm_setzero_si64());
			v13         = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v12,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_punpcklbw(_mm_cvtsi32_si64(_rotr(v9, 16)), _mm_setzero_si64()), v12),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v13)), v13)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsLightenBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm6
	__m64         v6;  // mm0
	__m64         v7;  // mm6
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm6

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = v5;
			v7  = _m_psrldi(v5, 0x19u);
			v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9  = _m_punpcklbw(v6, _mm_setzero_si64());
			v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(_m_pmullw(_m_psubw(_m_paddb(v9, _m_psubusb(v8, v9)), v8), _m_punpckldq(v10, v10)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsLightenBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm0
	__m64         v12; // mm6

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _m_punpcklbw(v8, _mm_setzero_si64());
			v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_paddb(v11, _m_psubusb(v9, v11)), v9), _m_punpckldq(v12, v12)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsLightenBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm0

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9 = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_paddb(v9, _m_psubusb(v8, v9)), v8),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsLightenBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm0

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _m_punpcklbw(v8, _mm_setzero_si64());
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_paddb(v11, _m_psubusb(v9, v11)), v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDarkenBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm6
	__m64         v6;  // mm0
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = v5;
			v7  = _m_psrldi(v5, 0x19u);
			v8  = _m_punpcklbw(v6, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(_m_pmullw(_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9), _m_punpckldq(v10, v10)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDarkenBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6
	__m64         v11; // mm6

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(v7, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(_m_pmullw(_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDarkenBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm0
	__m64         v9; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(v6, _mm_setzero_si64());
			v9 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDarkenBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm1
	__m64         v10; // mm6

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(v7, _mm_setzero_si64());
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v9,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(_m_psubb(v8, _m_psubusb(v8, v9)), v9),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiffBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1
	__m64         v11; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_paddb(_m_psubusb(v6, v7), _m_psubusb(v7, v6));
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiffBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm2
	__m64         v11; // mm1
	__m64         v12; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_paddb(_m_psubusb(v8, v7), _m_psubusb(v7, v8));
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _m_punpcklbw(v8, _mm_setzero_si64());
			v12 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v11), _m_punpckldq(v12, v12)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiffBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm1
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm0
	__m64         v10; // mm1

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = _mm_cvtsi32_si64(*v3);
			v7  = v5;
			v8  = _m_psrldi(v5, 0x19u);
			v9  = _m_paddb(_m_psubusb(v6, v7), _m_psubusb(v7, v6));
			v10 = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v10,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v10), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v8)), v8)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiffBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm0
	__m64         v10; // mm2
	__m64         v11; // mm1

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _mm_cvtsi32_si64(*v4);
			v9  = _m_paddb(_m_psubusb(v8, v7), _m_psubusb(v7, v8));
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _m_punpcklbw(v8, _mm_setzero_si64());
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v11,
						_m_psrawi(
							_m_pmullw(_m_psubw(_m_punpcklbw(v9, _mm_setzero_si64()), v11), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiff5Blend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm6
	__m64         v6;  // mm0
	__m64         v7;  // mm6
	__m64         v8;  // mm6
	__m64         v9;  // mm1
	__m64         v10; // mm0

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = v5;
			v7  = _m_psrldi(v5, 0x19u);
			v8  = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			v9  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v10 = _m_psrawi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), _m_punpckldq(v8, v8)), 7u);
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v9, v10), _m_psubusw(v10, v9)), _mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiff5Blend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm6
	__m64         v10; // mm6
	__m64         v11; // mm1
	__m64         v12; // mm0

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9));
			v11 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v12 = _m_psrawi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), _m_punpckldq(v10, v10)), 7u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v11, v12), _m_psubusw(v12, v11)), _mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiff5Blend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm6
	__m64         v6; // mm0
	__m64         v7; // mm6
	__m64         v8; // mm1
	__m64         v9; // mm0

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9 = _m_psrawi(_m_pmullw(_m_punpcklbw(v6, _mm_setzero_si64()), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)), 7u);
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v8, v9), _m_psubusw(v9, v8)), _mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsDiff5Blend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm6
	__m64         v8;  // mm0
	__m64         v9;  // mm6
	__m64         v10; // mm1
	__m64         v11; // mm0

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = v7;
			v9  = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v10 = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v11 = _m_psrawi(_m_pmullw(_m_punpcklbw(v8, _mm_setzero_si64()), _m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v9)), v9)), 7u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(_m_packuswb(_m_paddw(_m_psubusw(v10, v11), _m_psubusw(v11, v10)), _mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsExclusionBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3;  // edi
	unsigned int *v4;  // esi
	__m64         v5;  // mm2
	__m64         v6;  // mm6
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5  = _mm_cvtsi32_si64(*v4);
			v6  = v5;
			v7  = _m_psrldi(v5, 0x19u);
			v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9  = _m_punpcklbw(v6, _mm_setzero_si64());
			v10 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7));
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(_m_pmullw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)), _m_punpckldq(v10, v10)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsExclusionBlend_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2
	__m64         v11; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v9  = _m_punpcklbw(v7, _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			v11 = _mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10));
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(_m_pmullw(_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)), _m_punpckldq(v11, v11)), 7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsExclusionBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	unsigned int *v3; // edi
	unsigned int *v4; // esi
	__m64         v5; // mm2
	__m64         v6; // mm6
	__m64         v7; // mm2
	__m64         v8; // mm1
	__m64         v9; // mm6

	if (len > 0)
	{
		v3 = dest;
		v4 = (unsigned int *)src;
		do
		{
			v5 = _mm_cvtsi32_si64(*v4);
			v6 = v5;
			v7 = _m_psrldi(v5, 0x19u);
			v8 = _m_punpcklbw(_mm_cvtsi32_si64(*v3), _mm_setzero_si64());
			v9 = _m_punpcklbw(v6, _mm_setzero_si64());
			++v4;
			++v3;
			*(v3 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v7)), v7)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v4 < (signed int)&src[len]);
	}
	_m_empty();
}

void __cdecl TVPPsExclusionBlend_HDA_o_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
{
	unsigned int *v4;  // edi
	unsigned int *v5;  // esi
	__m64         v6;  // mm4
	__m64         v7;  // mm2
	__m64         v8;  // mm1
	__m64         v9;  // mm6
	__m64         v10; // mm2

	if (len > 0)
	{
		v4 = dest;
		v5 = (unsigned int *)src;
		v6 = _mm_cvtsi32_si64(opa);
		do
		{
			v7  = _mm_cvtsi32_si64(*v5);
			v8  = _m_punpcklbw(_mm_cvtsi32_si64(*v4), _mm_setzero_si64());
			v9  = _m_punpcklbw(v7, _mm_setzero_si64());
			v10 = _m_psrldi(_m_pmullw(_m_psrldi(v7, 0x19u), v6), 8u);
			++v5;
			++v4;
			*(v4 - 1) = _mm_cvtsi64_si32(
				_m_packuswb(
					_m_paddw(
						v8,
						_m_psrawi(
							_m_pmullw(
								_m_psubw(v9, _m_psrlwi(_m_pmullw(v9, v8), 7u)),
								_m_punpckldq(_mm_set1_pi16((tjs_uint16)_mm_cvtsi64_si32(v10)), v10)),
							7u)),
					_mm_setzero_si64()));
		} while ((signed int)v5 < (signed int)&src[len]);
	}
	_m_empty();
}
