/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPConstColorAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	FOREACH_CHANNEL_COLOR(
		{
			if (j % 4 == 3)
			{
				d[j] = TVPNegativeMulTable[(opa << 8) + *da];
				continue;
			}
			tjs_uint16 k = c[j];
			k -= d[j];
			k *= TVPOpacityOnOpacityTable[(opa << 8) + *da];
			tjs_uint16 l = d[j];
			l <<= 8;
			k += l;
			k >>= 8;
			d[j] = k;
		}, len, color, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstColorAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint16 k = d[j];
			tjs_uint16 l = d[j];
			l *= ((unsigned int)opa >> 7) + opa;
			l >>= 8u;
			k -= l;
			tjs_uint16 m = c[j];
			if (j % 4 == 3)
			{
				m = ((unsigned int)opa >> 7) + opa;
			}
			else
			{
				m *= ((unsigned int)opa >> 7) + opa;
				m >>= 8u;
			}
			k += m;
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, color, dest);
}
