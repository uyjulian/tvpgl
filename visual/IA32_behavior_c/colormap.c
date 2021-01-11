/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	FOREACH_CHANNEL_COLOR(
		{
			tjs_int16 k = c[j];
			k -= d[j];
			k *= src[i];
			k >>= 6u;
			k += d[j];
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, color, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_d_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint32 l = (dest[i] >> 24) + (src[i] << 8);
			if (j % 4 == 3)
			{
				d[j] = TVPNegativeMulTable65[l];
				continue;
			}
			tjs_uint16 m = d[j];
			m <<= 8u;
			tjs_uint16 k = c[j];
			k -= d[j];
			k *= TVPOpacityOnOpacityTable65[l];
			k += m;
			k >>= 8u;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, color, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPApplyColorMap65_a_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	FOREACH_CHANNEL_COLOR(
		{
			tjs_uint16 cc = c[j];
			if (j % 4 == 3)
			{
				cc = 0x0100;
			}
			tjs_uint16 k = d[j];
			k *= src[i];
			k >>= 6u;
			k = d[j] - k;
			tjs_uint16 l = src[i];
			l *= cc;
			l >>= 6u;
			l += k;
			if (l & (1 << 15))
			{
				l = 0;
			}
			if (l & (~0xFF))
			{
				l = 0xFF;
			}
			d[j] = l;
		}, len, color, dest);
}
