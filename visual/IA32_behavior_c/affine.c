/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPLinTransConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((const tjs_uint8 *)&src[i2 >> 16] + srcpitch * (i3 >> 16));

		const tjs_uint8 *s = (const tjs_uint8 *)src_start;
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s[j];
			k -= d[j];
			k *= opa;
			k += (d[j] << 8);
			k >>= 8;
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
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((const tjs_uint8 *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		tjs_uint16 sx_m64 = (tjs_uint16)i2 >> 8;
		tjs_uint16 sy_m64 = ((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15);

		const tjs_uint8 *s0 = (const tjs_uint8 *)(src_start    );
		const tjs_uint8 *s1 = (const tjs_uint8 *)(src_start + 1);
		const tjs_uint8 *sp0 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch))    );
		const tjs_uint8 *sp1 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch)) + 1);
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1[j];
			k -= s0[j];
			k *= sx_m64;
			k >>= 8;
			k += s0[j];
			tjs_uint16 l = sp1[j];
			l -= sp0[j];
			l *= sx_m64;
			l >>= 8;
			l += sp0[j];
			l -= k;
			l *= sy_m64;
			l >>= 8;
			l += k;
			vals_tmp[j] = l;
		}
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = vals_tmp[3]; // alpha
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += vals_tmp[j];
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
}

TVP_GL_IA32_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint16 opa_sevenbit = ((tjs_uint32)opa >> 7) + opa;
	for (tjs_int i = 0, i2 = sx, i3 = sy; i < len; i += 1, i2 += stepx, i3 += stepy)
	{
		const tjs_uint32 *src_start = ((const tjs_uint8 *)&src[i2 >> 16] + srcpitch * (i3 >> 16));
		tjs_uint16 sx_m64 = (tjs_uint16)i2 >> 8;
		tjs_uint16 sy_m64 = ((tjs_uint32)(tjs_uint16)i3 >> 8) + ((tjs_uint32)(tjs_uint16)i3 >> 15);

		const tjs_uint8 *s0 = (const tjs_uint8 *)(src_start    );
		const tjs_uint8 *s1 = (const tjs_uint8 *)(src_start + 1);
		const tjs_uint8 *sp0 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch))    );
		const tjs_uint8 *sp1 = (const tjs_uint8 *)(((tjs_uint32 *)((const tjs_uint8 *)src_start + srcpitch)) + 1);
		tjs_uint8 *d = (tjs_uint8 *)&dest[i];
		tjs_uint8 vals_tmp[4];
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = s1[j];
			k -= s0[j];
			k *= sx_m64;
			k >>= 8;
			k += s0[j];
			tjs_uint16 l = sp1[j];
			l -= sp0[j];
			l *= sx_m64;
			l >>= 8;
			l += sp0[j];
			l -= k;
			l *= sy_m64;
			l >>= 8;
			l += k;
			l *= opa_sevenbit;
			l >>= 8;
			vals_tmp[j] = l;
		}
		for (tjs_int j = 0; j < 4; j += 1)
		{		
			tjs_uint16 k = vals_tmp[3]; // alpha
			k *= d[j];
			k >>= 8;
			k = d[j] - k;
			k += vals_tmp[j];
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
}
