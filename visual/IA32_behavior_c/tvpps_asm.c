/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

#define TVP_GL_IA32_BLEND_FUNC_BODY(funcname, channelbody, hda) \
	TVP_GL_IA32_FUNC_DECL(void, funcname, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	{ \
		FOREACH_CHANNEL( \
			{ \
				if (hda && (j % 4 == 3)) \
				{ \
					continue; \
				} \
				tjs_uint16 sevenbit = src[i] >> 0x19u; \
				channelbody; \
			}, len, src, dest); \
	}

#define TVP_GL_IA32_BLEND_FUNC_BODY_OPACITY(funcname, channelbody, hda) \
	TVP_GL_IA32_FUNC_DECL(void, funcname, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	{ \
		FOREACH_CHANNEL( \
			{ \
				if (hda && (j % 4 == 3)) \
				{ \
					continue; \
				} \
				tjs_uint16 sevenbit = src[i] >> 0x19u; \
				sevenbit *= opa; \
				sevenbit >>= 8u; \
				channelbody; \
			}, len, src, dest); \
	}

#define TVP_GL_IA32_BLEND_FUNC(funcname, channelbody) \
	TVP_GL_IA32_BLEND_FUNC_BODY        (TVPPs##funcname##Blend_c,       channelbody, 0) \
	TVP_GL_IA32_BLEND_FUNC_BODY_OPACITY(TVPPs##funcname##Blend_o_c,     channelbody, 0) \
	TVP_GL_IA32_BLEND_FUNC_BODY        (TVPPs##funcname##Blend_HDA_c,   channelbody, 1) \
	TVP_GL_IA32_BLEND_FUNC_BODY_OPACITY(TVPPs##funcname##Blend_HDA_o_c, channelbody, 1)

#define TVP_GL_IA32_BLEND_FUNC_OPACITY_ONLY(funcname, channelbody) \
	TVP_GL_IA32_BLEND_FUNC_BODY_OPACITY(TVPPs##funcname##Blend_o_c,     channelbody, 0) \
	TVP_GL_IA32_BLEND_FUNC_BODY_OPACITY(TVPPs##funcname##Blend_HDA_o_c, channelbody, 1)

#define TVP_GL_IA32_ALPHABLEND(channel, destchannel, sevenbit) \
	{ \
		channel -= destchannel; \
		channel *= sevenbit; \
		channel >>= 7; \
		channel += destchannel; \
		destchannel = channel; \
	}

TVP_GL_IA32_BLEND_FUNC(Alpha, {
	tjs_uint16 k = s[j];
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Add, {
	tjs_uint16 k = s[j];
	k += d[j];
	if (k > 0xff)
	{
		k = 0xff;
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Sub, {
	tjs_uint16 k = s[j];
	k ^= 0xff;
	k = d[j] - k;
	if (k > 0xff)
	{
		k = 0;
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Mul, {
	tjs_uint16 k = s[j];
	k *= d[j];
	k >>= 8u;
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Screen, {
	tjs_uint16 k = s[j];
	k *= d[j];
	k >>= 8u;
	tjs_uint16 l = s[j];
	l -= k;
	l *= sevenbit;
	l >>= 7;
	l += d[j];
	d[j] = l;
});

// FIXME: some channel shows up as wrong on regular/HDA versions (not opacity version) on test harness w/ IA32 (asm) only
TVP_GL_IA32_BLEND_FUNC(Overlay, {
	tjs_uint16 k = s[j];
	tjs_uint16 l = s[j];
	l *= d[j];
	l >>= 7;
	if (0x80 > d[j])
	{
		k = l;
	}
	else
	{
		k += d[j];
		k <<= 1;
		k -= l;
		k -= 0xff;
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

// FIXME: some channel shows up as wrong on regular/HDA versions (not opacity version) on test harness w/ IA32 (asm) only
TVP_GL_IA32_BLEND_FUNC(HardLight, {
	tjs_uint16 k = s[j];
	tjs_uint16 l = s[j];
	l *= d[j];
	l >>= 7u;
	if (0x80u > s[j])
	{
		k = l;
	}
	else
	{
		k += d[j];
		k <<= 1u;
		k -= l;
		k -= 0xff;
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(SoftLight, {
	tjs_uint16 k = TVPPsTableSoftLight[s[j]][d[j]];
	if (j % 4 == 3)
	{
		k = s[j];
	}
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(ColorDodge, {
	tjs_uint16 k = TVPPsTableColorDodge[s[j]][d[j]];
	if (j % 4 == 3)
	{
		k = s[j];
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(ColorDodge5, {
	tjs_uint16 k = s[j];
	k *= sevenbit;
	k >>= 7;
	if (j % 4 != 3)
	{
		k = TVPPsTableColorDodge[k][d[j]];
	}
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(ColorBurn, {
	tjs_uint16 k = TVPPsTableColorBurn[s[j]][d[j]];
	if (j % 4 == 3)
	{
		k = s[j];
	}
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Lighten, {
	tjs_uint16 k = d[j];
	k -= s[j];
	if (k > 0xff)
	{
		k = 0;
	}
	k += s[j];
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Darken, {
	tjs_uint16 k = s[j];
	k -= d[j];
	if (k > 0xff)
	{
		k = 0;
	}
	k = s[j] - k;
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Diff, {
	tjs_uint16 k = s[j];
	k -= d[j];
	if (k > 0xff)
	{
		k = 0;
	}
	tjs_uint16 l = d[j];
	l -= s[j];
	if (l > 0xff)
	{
		l = 0;
	}
	k += l;
	TVP_GL_IA32_ALPHABLEND(k, d[j], sevenbit);
});

TVP_GL_IA32_BLEND_FUNC(Diff5, {
	tjs_uint16 k = s[j];
	k *= sevenbit;
	k >>= 7;
	tjs_uint16 l = k;
	l -= d[j];
	if (l > 0xff)
	{
		l = 0;
	}
	tjs_uint16 m = d[j];
	m -= k;
	if (m > 0xff)
	{
		m = 0;
	}
	l += m;
	d[j] = l;
});

TVP_GL_IA32_BLEND_FUNC(Exclusion, {
	tjs_uint16 k = s[j];
	k *= d[j];
	k >>= 7;
	tjs_uint16 l = s[j];
	l -= k;
	l *= sevenbit;
	l >>= 7;
	l += d[j];
	d[j] = l;
});
