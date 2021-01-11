/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPDarkenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
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
}
