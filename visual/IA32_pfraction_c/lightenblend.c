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

void __cdecl TVPLightenBlend_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	tjs_uint32 *  v3; // edi
	unsigned int *v4; // ebp

	v3 = (tjs_uint32 *)dest;
	v4 = (unsigned int *)src;
	while (v3 < &dest[len])
	{
		*v3 = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_mm_cvtsi32_si64(*v4), _mm_cvtsi32_si64(*v3)), _mm_cvtsi32_si64(*v3)));
		v3  = (tjs_uint32 *)((char *)v3 + 4);
		++v4;
	}
	_m_empty();
}

void __cdecl TVPLightenBlend_HDA_mmx_pfraction_c(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
{
	__m64       v3; // mm7
	__m64       v4; // mm7
	tjs_uint32 *v5; // edi
	__m64 *     v6; // ebp

	v3 = _mm_cvtsi32_si64(0xFFFFFFu);
	v4 = _m_punpckldq(v3, v3);
	v5 = (tjs_uint32 *)dest;
	v6 = (__m64 *)src;
	while (v5 < &dest[len])
	{
		*v5 = _mm_cvtsi64_si32(_m_paddb(_m_psubusb(_m_pand(*v6, v4), _mm_cvtsi32_si64(*v5)), _mm_cvtsi32_si64(*v5)));
		v5  = (tjs_uint32 *)((char *)v5 + 4);
		v6  = (__m64 *)((char *)v6 + 4);
	}
	_m_empty();
}
