/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPUnivTransBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len))
{
	FOREACH_CHANNEL_2SRC(
		{
			tjs_uint32 k = s1[j];
			k -= s2[j];
			k *= *((const tjs_uint16 *)table + rule[i]);
			k >>= 16;
			k <<= 1;
			k += s2[j];
			k &= 0xFFFF;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}, len, src1, src2, dest);
}

TVP_GL_IA32_FUNC_DECL(void, TVPUnivTransBlend_switch_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
#if 0
	FOREACH_CHANNEL_2SRC(
		{
			if (rule[i] >= src1lv)
			{
				d[j] = s1[j];
				continue;
			}
			else if (rule[i] < src2lv)
			{
				d[j] = s2[j];
				continue;
			}
			tjs_uint32 k = s1[j];
			k -= s2[j];
			k *= *((const tjs_uint16 *)table + rule[i]);
			k >>= 16;
			k <<= 1;
			k += s2[j];
			d[j] = k;
		}, len, src1, src2, dest);
#else
	tjs_int ze = 1;
	tjs_int ze2 = 0;

	for (tjs_int i = 0; i < len; i += 1)
	{
		if (ze2 == 1)
		{
			ze2 = 0;
			dest[i] = src1[i];
			continue;
		}
		if (ze2 == 2)
		{
			ze2 = 0;
			dest[i] = src2[i];
			continue;
		}
		if (ze)
		{
			if ((rule[i + 1] >= src1lv) && (rule[i] < src1lv))
			{
				ze = 0;
			}
			else if (rule[i + 1] >= src1lv)
			{
				dest[i] = src1[i];
				ze2 = 1;
				continue;
			}
			else if (rule[i + 1] >= src2lv)
			{
				ze = 0;
			}
			else if (rule[i] >= src2lv)
			{
				ze = 0;
			}
			else
			{
				dest[i] = src2[i];
				ze2 = 2;
				continue;
			}
		}
		else
		{
			ze = 1;
		}
		const tjs_uint8 *s1 = (const tjs_uint8 *)&src1[i];
		const tjs_uint8 *s2 = (const tjs_uint8 *)&src2[i];
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		for (tjs_int j = 0; j < 4; j += 1)
		{
			tjs_uint32 k = s1[j];
			k -= s2[j];
			k *= *((const tjs_uint16 *)table + rule[i]);
			k >>= 16;
			k <<= 1;
			k += s2[j];
			k &= 0xFFFF;
			if (k & (1 << 15))
			{
				k = 0;
			}
			if (k & (~0xFF))
			{
				k = 0xFF;
			}
			d[j] = k;
		}
	}
#endif
}
