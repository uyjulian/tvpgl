/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL(
		{
			if (*sa == 0xff)
			{
				d[j] = s[j];
				continue;
			}
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= *sa;
			k >>= 8;
			if (k > 0xff)
			{
				k = 0xff;
			}
			k += d[j];
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= (tjs_uint32)opa * (tjs_uint64)src[i] >> 32;
			k += d[j] << 8;
			k >>= 8;
			if (k > 0xff)
			{
				k = 0xff;
			}
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPAlphaBlend_d_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	FOREACH_CHANNEL_PIXELBODY(
		{
			tjs_uint16 v7 = (dest[i] >> 24) | ((src[i] >> 16) & 0xFF00);
			if (j % 4 == 3)
			{
				d[j] = TVPNegativeMulTable[v7];
				continue;
			}
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= TVPOpacityOnOpacityTable[v7];
			k += d[j] << 8;
			k >>= 8;
			d[j] = k;
		}, 
		{
			if (!*sa)
			{
				continue;
			}
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL(
		{
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= opa;
			k += d[j] << 8;
			k >>= 8;
			d[j] = k;
		}, len, src, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPConstAlphaBlend_SD_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa))
{
	FOREACH_CHANNEL_2SRC(
		{
			tjs_uint16 k = s2[j];
			k -= s1[j];
			k *= opa;
			k += s1[j] << 8;
			k >>= 8;
			d[j] = k;
		}, len, src1, src2, dest);
}
