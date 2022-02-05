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
TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = s[j];
			k ^= 0xff;
			k = d[j] - k;
			if (k > 0xff)
			{
				k = 0;
			}
			d[j] = k;
		}, len, src, dest);
}

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 k = s[j];
			k ^= 0xff;
			k = d[j] - k;
			if (k > 0xff)
			{
				k = 0;
			}
			d[j] = k;
		}, len, src, dest);
}

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPSubBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 k = s[j];
			k ^= 0xff;
			if (j % 4 != 3)
			{
				k *= opa;
				k >>= 8;
			}
			else
			{
				k = 0;
			}
			k = d[j] - k;
			if (k > 0xff)
			{
				k = 0;
			}
			d[j] = k;
		}, len, src, dest);
}
