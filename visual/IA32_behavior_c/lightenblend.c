/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPLightenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 m = s[j];
			tjs_uint16 k = d[j];
			m = m - k;
			if (m > 0xff)
			{
				m = 0;
			}
			m &= 0xff;
			m = m + k;
			m &= 0xff;
			d[j] = m;
		}, len, src, dest);
}

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPLightenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 m = s[j];
			tjs_uint16 k = d[j];
			m = m - k;
			if (m > 0xff)
			{
				m = 0;
			}
			m &= 0xff;
			m = m + k;
			m &= 0xff;
			d[j] = m;
		}, len, src, dest);
}
