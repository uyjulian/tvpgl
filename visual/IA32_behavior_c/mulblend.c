/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPMulBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = s[j];
			k *= d[j];
			k >>= 8;
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPMulBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = s[j];
			k ^= 0xff;
			k *= opa;
			k >>= 8;
			k ^= 0xff;
			k *= d[j];
			k >>= 8;
			d[j] = k;
		}, len, src, dest);
}

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPMulBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 k = s[j];
			k *= d[j];
			k >>= 8;
			d[j] = k;
		}, len, src, dest);
}

// Output matching with C version
TVP_GL_IA32_FUNC_DECL(void, TVPMulBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 k = s[j];
			k ^= 0xff;
			k *= opa;
			k >>= 8;
			k ^= 0xff;
			k *= d[j];
			k >>= 8;
			d[j] = k;
		}, len, src, dest);
}
