/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = *sa;
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += s[j];
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint opacity = ((tjs_uint)opa >> 7) + opa;

	FOREACH_CHANNEL(
		{
			tjs_uint16 k = *sa;
			k *= opacity;
			k >>= 8;
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			tjs_uint16 m = s[j];
			m *= opacity;
			m >>= 8;
			k += m;
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			if (j % 4 == 3)
			{
				continue;
			}
			tjs_uint16 k = *sa;
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += s[j];
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPAdditiveAlphaBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = *sa;
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += s[j];
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, src, dest);
}
