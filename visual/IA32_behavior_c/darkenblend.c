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
#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPDarkenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
#if 1
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 m = s[j];
			tjs_uint16 k = d[j] | 0xff;
			m = k - m;
			if (m > 0xff)
			{
				m = 0;
			}
			m &= 0xff;
			m = k - m;
			m &= 0xff;
			d[j] = m;
		}, len, src, dest);
#endif
#if 0
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 m = _mm_cvtsi32_si64(src[i]);
		__m64 k = _mm_cvtsi32_si64(dest[i] | 0xFFFFFFu);
		m = _m_psubusb(k, m);
		m = _m_pand(m, _mm_set1_pi32(0xFFFFFFu));
		m = _m_psubb(k, m);
		dest[i] = _mm_cvtsi64_si32(m);
	}
	_m_empty();
#endif
}
