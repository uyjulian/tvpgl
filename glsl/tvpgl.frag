#version 300 es
// TVPGL ubershader, based on C version of code.
precision mediump float;
in vec2 v_texCoord;
uniform sampler2D s_tex0;
uniform sampler2D s_tex1;
uniform float a_hda;
uniform float a_method;
uniform float a_opa;
out vec4 fragmentColor;

#define bmCopy 0u
#define bmCopyOnAlpha 1u
#define bmAlpha 2u
#define bmAlphaOnAlpha 3u
#define bmAdd 4u
#define bmSub 5u
#define bmMul 6u
#define bmDodge 7u
#define bmDarken 8u
#define bmLighten 9u
#define bmScreen 10u
#define bmAddAlpha 11u
#define bmAddAlphaOnAddAlpha 12u
#define bmAddAlphaOnAlpha 13u
#define bmAlphaOnAddAlpha 14u
#define bmCopyOnAddAlpha 15u
#define bmPsNormal 16u
#define bmPsAdditive 17u
#define bmPsSubtractive 18u
#define bmPsMultiplicative 19u
#define bmPsScreen 20u
#define bmPsOverlay 21u
#define bmPsHardLight 22u
#define bmPsSoftLight 23u
#define bmPsColorDodge 24u
#define bmPsColorDodge5 25u
#define bmPsColorBurn 26u
#define bmPsLighten 27u
#define bmPsDarken 28u
#define bmPsDifference 29u
#define bmPsDifference5 30u
#define bmPsExclusion 31u

highp uint TVPOpacityOnOpacityCalcTable(in highp uint a, in highp uint b)
{
	float c;
	highp uint ci;

	if (a != 0u)
	{
		float at = (float(a)/255.0);
		float bt = (float(b)/255.0);
		c = bt / at;
		c /= ( (1.0 - bt + c) );
		ci = uint(c*255.0);
		if(ci>=256u) ci = 255u; /* will not overflow... */
	}
	else
	{
		ci = 255u;
	}
	return ci & 0xffu;
}

highp uint TVPNegativeMulCalcTable(in highp uint a, in highp uint b)
{
	return ( 255u - (255u-a)*(255u-b)/ 255u ) & 0xffu;
}

highp uint TVPRecipCalcTable256(in highp uint i)
{
	if (i == 0u)
	{
		return 65536u;
	}
	return 65536u/i;
}

void TVPCopyColor(inout highp uint dest, in highp uint src)
{
	dest = (dest & 0xff000000u) + (src & 0x00ffffffu);
}

void TVPConstAlphaBlend(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
}

void TVPConstAlphaBlend_HDA(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
}

void TVPCopyOpaqueImage(inout highp uint dest, in highp uint src)
{
	highp uint t1 = src;
	t1 |= 0xff000000u;;
	dest = t1;
}

void TVPConstAlphaBlend_d(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint alpha = TVPOpacityOnOpacityCalcTable(d>>24u, opa);
	highp uint d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * alpha >> 8u)) & 0xff00ffu) +
		(TVPNegativeMulCalcTable(d>>24u, opa)<<24u);
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 | ((d + ((s - d) * alpha >> 8u)) & 0xff00u);
}

void TVPAlphaBlend(inout highp uint dest, in highp uint src)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint sopa = s >> 24u;
	highp uint d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
}

void TVPAlphaBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint sopa = s >> 24u;
	highp uint d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u); /* hda */
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
}

void TVPAlphaBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint sopa = ((s >> 24u) * opa) >> 8u;
	highp uint d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
}

void TVPAlphaBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint sopa = ((s >> 24u) * opa) >> 8u;
	highp uint d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
}

void TVPAlphaBlend_d(inout highp uint dest, in highp uint src)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint destalpha = TVPNegativeMulCalcTable((d>>24u), (s >> 16u))<<24u;
	highp uint sopa = TVPOpacityOnOpacityCalcTable((d>>24u), (s >> 16u));
	highp uint d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
}

void TVPAlphaBlend_do(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = src;
	highp uint d = dest;
	highp uint destalpha = TVPNegativeMulCalcTable((d>>24u), ( (s>>24u)*opa) >> 8u)<<24u;
	highp uint sopa = TVPOpacityOnOpacityCalcTable((d>>24u), ( (s>>24u)*opa) >> 8u);
	highp uint d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
}

void TVPAddBlend(inout highp uint dest, in highp uint src)
{
	highp uint tmp = 0u;
	tmp = (  ( src & dest ) + ( ((src^dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	dest= (src + dest - tmp) | tmp;
}

void TVPAddBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint tmp = 0u;
	tmp = (  ( src & dest ) + ( ((src^dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	dest= (((src + dest - tmp) | tmp) & 0xffffffu) | (dest & 0xff000000u) ;
}

void TVPAddBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ( ((src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (src&0xff00ffu) * opa >> 8u)&0xff00ffu);
	highp uint tmp = (  ( s & dest ) + ( ((s^dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	dest= (s + dest - tmp) | tmp;
}

void TVPAddBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ( ((src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (src&0xff00ffu) * opa >> 8u)&0xff00ffu);
	highp uint tmp = (  ( s & dest ) + ( ((s^dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	dest= (((s + dest - tmp) | tmp) & 0xffffffu) + (dest & 0xff000000u) ;
}

void TVPSubBlend(inout highp uint dest, in highp uint src)
{
	highp uint tmp = (  ( src & dest ) + ( ((src ^ dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest = (src + dest - tmp) & tmp;
}

void TVPSubBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint s = src | 0xff000000u;
	highp uint tmp = (  ( s & dest ) + ( ((s ^ dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest = (s + dest - tmp) & tmp;
}

void TVPSubBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ~src;
	s = ~ (( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu) );
	highp uint tmp = (  ( s & dest ) + ( ((s ^ dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest = (s + dest - tmp) & tmp;
}

void TVPSubBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ~src;
	s = 0xff000000u | ~ (( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu) );
	highp uint tmp = (  ( s & dest ) + ( ((s ^ dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest = (s + dest - tmp) & tmp;
}

void TVPMulBlend(inout highp uint dest, in highp uint src)
{
	highp uint tmp  = (dest & 0xffu) * (src & 0xffu) & 0xff00u;
	tmp |= ((dest & 0xff00u) >> 8u) * (src & 0xff00u) & 0xff0000u;
	tmp |= ((dest & 0xff0000u) >> 16u) * (src & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = tmp;
}

void TVPMulBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint tmp  = (dest & 0xffu) * (src & 0xffu) & 0xff00u;
	tmp |= ((dest & 0xff00u) >> 8u) * (src & 0xff00u) & 0xff0000u;
	tmp |= ((dest & 0xff0000u) >> 16u) * (src & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = tmp + (dest & 0xff000000u);
}

void TVPMulBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ~src;
	s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
	highp uint tmp  = (dest & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((dest & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((dest & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = tmp;
}

void TVPMulBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint s = ~src;
	s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
	highp uint tmp  = (dest & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((dest & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((dest & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = tmp + (dest & 0xff000000u);
}

void TVPColorDodgeBlend(inout highp uint dest, in highp uint src)
{
	highp uint tmp2 = ~src;
	highp uint tmp = (dest & 0xffu) * TVPRecipCalcTable256(tmp2 & 0xffu) >> 8u;
	highp uint tmp3 = (tmp | uint(int(~(tmp - 0x100u)) >> 31)) & 0xffu;
	tmp = ((dest & 0xff00u)>>8u) * TVPRecipCalcTable256((tmp2 & 0xff00u)>>8u);
	tmp3 |= (tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u;
	tmp = ((dest & 0xff0000u)>>16u) * TVPRecipCalcTable256((tmp2 & 0xff0000u)>>16u);
	tmp3 |= ((tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u ) << 8u;
	dest= tmp3;
}

void TVPColorDodgeBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint tmp2 = ~src;
	highp uint tmp = (dest & 0xffu) * TVPRecipCalcTable256(tmp2 & 0xffu) >> 8u;
	highp uint tmp3 = (tmp | uint(int(~(tmp - 0x100u)) >> 31)) & 0xffu;
	tmp = ((dest & 0xff00u)>>8u) * TVPRecipCalcTable256((tmp2 & 0xff00u)>>8u);
	tmp3 |= (tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u;
	tmp = ((dest & 0xff0000u)>>16u) * TVPRecipCalcTable256((tmp2 & 0xff0000u)>>16u);
	tmp3 |= ((tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u ) << 8u;
	dest= tmp3 + (dest & 0xff000000u);
}

void TVPColorDodgeBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint tmp2 = ~ (( ((src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (src&0xff00ffu) * opa >> 8u)&0xff00ffu) );
	highp uint tmp = (dest & 0xffu) * TVPRecipCalcTable256(tmp2 & 0xffu) >> 8u;
	highp uint tmp3 = (tmp | uint(int(~(tmp - 0x100u)) >> 31)) & 0xffu;
	tmp = ((dest & 0xff00u)>>8u) * TVPRecipCalcTable256((tmp2 & 0xff00u)>>8u);
	tmp3 |= (tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u;
	tmp = ((dest & 0xff0000u)>>16u) * TVPRecipCalcTable256((tmp2 & 0xff0000u)>>16u);
	tmp3 |= ((tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u ) << 8u;
	dest= tmp3;
}

void TVPColorDodgeBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint tmp2 = ~ (( ((src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (src&0xff00ffu) * opa >> 8u)&0xff00ffu) );
	highp uint tmp = (dest & 0xffu) * TVPRecipCalcTable256(tmp2 & 0xffu) >> 8u;
	highp uint tmp3 = (tmp | uint(int(~(tmp - 0x100u)) >> 31)) & 0xffu;
	tmp = ((dest & 0xff00u)>>8u) * TVPRecipCalcTable256((tmp2 & 0xff00u)>>8u);
	tmp3 |= (tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u;
	tmp = ((dest & 0xff0000u)>>16u) * TVPRecipCalcTable256((tmp2 & 0xff0000u)>>16u);
	tmp3 |= ((tmp | uint(int(~(tmp - 0x10000u)) >> 31)) & 0xff00u ) << 8u;
	dest= tmp3 + (dest & 0xff000000u);
}

void TVPDarkenBlend(inout highp uint dest, in highp uint src)
{
	highp uint m_src = ~src;
	highp uint tmp = ((m_src & dest) + (((m_src ^ dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest ^= (dest ^ src) & tmp;
}

void TVPDarkenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint m_src = ~src;
	highp uint tmp = ((m_src & dest) + (((m_src ^ dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest ^= ((dest ^ src) & tmp) & 0xffffffu;
}

void TVPDarkenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint m_src = ~src;
	highp uint tmp = ((m_src & dest) + (((m_src ^ dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	tmp = dest ^ ((dest ^ src) & tmp);
	highp uint d1 = dest & 0xff00ffu;
	d1 = (d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	m_src = dest & 0xff00u;
	tmp &= 0xff00u;
	dest = d1 + ((m_src + ((tmp - m_src) * opa >> 8u)) & 0xff00u);
}

void TVPDarkenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint m_src = ~src;
	highp uint tmp = ((m_src & dest) + (((m_src ^ dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	tmp = dest ^ (((dest ^ src) & tmp) & 0xffffffu);
	highp uint d1 = dest & 0xff00ffu;
	d1 = ((d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (dest & 0xff000000u); /* hda */
	m_src = dest & 0xff00u;
	tmp &= 0xff00u;
	dest = d1 + ((m_src + ((tmp - m_src) * opa >> 8u)) & 0xff00u);
}

void TVPLightenBlend(inout highp uint dest, in highp uint src)
{
	highp uint m_dest = ~dest;
	highp uint tmp = ((src & m_dest) + (((src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest ^= (dest ^ src) & tmp;
}

void TVPLightenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint m_dest = ~dest;
	highp uint tmp = ((src & m_dest) + (((src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	dest ^= ((dest ^ src) & tmp) & 0xffffffu;
}

void TVPLightenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint m_dest = ~dest;
	highp uint tmp = ((src & m_dest) + (((src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	tmp = dest ^ ((dest ^ src) & tmp);
	highp uint d1 = dest & 0xff00ffu;
	d1 = (d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	m_dest = dest & 0xff00u;
	tmp &= 0xff00u;
	dest = d1 + ((m_dest + ((tmp - m_dest) * opa >> 8u)) & 0xff00u);
}

void TVPLightenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint m_dest = ~dest;
	highp uint tmp = ((src & m_dest) + (((src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = (tmp << 1u) - (tmp >> 7u);
	tmp = dest ^ (((dest ^ src) & tmp) & 0xffffffu);
	highp uint d1 = dest & 0xff00ffu;
	d1 = ((d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (dest & 0xff000000u); /* hda */
	m_dest = dest & 0xff00u;
	tmp &= 0xff00u;
	dest = d1 + ((m_dest + ((tmp - m_dest) * opa >> 8u)) & 0xff00u);
}

void TVPScreenBlend(inout highp uint dest, in highp uint src)
{
	highp uint s = ~src;
	highp uint d = ~dest;
	highp uint tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = ~tmp;
}

void TVPScreenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint s = ~src;
	highp uint d = ~dest;
	highp uint tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = ~tmp ^ (d & 0xff000000u);
}

void TVPScreenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = ~dest;
	highp uint s = src;
	s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
	highp uint tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = tmp;
}

void TVPScreenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = ~dest;
	highp uint s = src;
	s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
		(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
	highp uint tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
	tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
	tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
	tmp >>= 8u;
	dest = ~tmp ^ (d & 0xff000000u);
}

highp uint TVPSaturatedAdd(in highp uint a, in highp uint b)
{
	highp uint tmp = (  ( a & b ) + ( ((a ^ b)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	return (a + b - tmp) | tmp;
}

highp uint TVPAddAlphaBlend_n_a(in highp uint dest, in highp uint src)
{
	highp uint sopa = (~src) >> 24u;
	return TVPSaturatedAdd((((dest & 0xff00ffu)*sopa >> 8u) & 0xff00ffu) + (((dest & 0xff00u)*sopa >> 8u) & 0xff00u), src);
}

highp uint TVPAddAlphaBlend_HDA_n_a(in highp uint dest, in highp uint src)
{
	return (dest & 0xff000000u) + (TVPAddAlphaBlend_n_a(dest, src) & 0xffffffu);
}

highp uint TVPAddAlphaBlend_n_a_o(in highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint ssrc = src;
	ssrc = (((ssrc & 0xff00ffu)*opa >> 8u) & 0xff00ffu) + (((ssrc >> 8u) & 0xff00ffu)*opa & 0xff00ff00u);
	return TVPAddAlphaBlend_n_a(dest, ssrc);
}

highp uint TVPAddAlphaBlend_HDA_n_a_o(in highp uint dest, in highp uint src, in lowp uint opa)
{
	return (dest & 0xff000000u) + (TVPAddAlphaBlend_n_a_o(dest, src, opa) & 0xffffffu);
}

void TVPAdditiveAlphaBlend(inout highp uint dest, in highp uint src)
{
	dest = TVPAddAlphaBlend_n_a(dest, src);
}

void TVPAdditiveAlphaBlend_HDA(inout highp uint dest, in highp uint src)
{
	dest = TVPAddAlphaBlend_HDA_n_a(dest, src);
}

void TVPAdditiveAlphaBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	dest = TVPAddAlphaBlend_n_a_o(dest, src, opa);
}

void TVPAdditiveAlphaBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	dest = TVPAddAlphaBlend_HDA_n_a_o(dest, src, opa);
}

highp uint TVPAddAlphaBlend_a_a(in highp uint dest, in highp uint src)
{
	/*
		Di = sat(Si, (1-Sa)*Di)
		Da = Sa + Da - SaDa
	*/

	highp uint dopa = dest >> 24u;
	highp uint sopa = src >> 24u;
	dopa = dopa + sopa - (dopa*sopa >> 8u);
	dopa -= (dopa >> 8u); /* adjust alpha */
	sopa ^= 0xffu;
	src &= 0xffffffu;
	return (dopa << 24u) + 
		TVPSaturatedAdd((((dest & 0xff00ffu)*sopa >> 8u) & 0xff00ffu) +
			(((dest & 0xff00u)*sopa >> 8u) & 0xff00u), src);
}

highp uint TVPAddAlphaBlend_a_a_o(in highp uint dest, in highp uint src, in lowp uint opa)
{
	src = (((src & 0xff00ffu)*opa >> 8u) & 0xff00ffu) + (((src >> 8u) & 0xff00ffu)*opa & 0xff00ff00u);
	return TVPAddAlphaBlend_a_a(dest, src);
}

void TVPAdditiveAlphaBlend_a(inout highp uint dest, in highp uint src)
{
	dest = TVPAddAlphaBlend_a_a(dest, src);
}

void TVPAdditiveAlphaBlend_ao(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	dest = TVPAddAlphaBlend_a_a_o(dest, src, opa);
}

highp uint TVPMulColor(in highp uint color, in highp uint fac)
{
	return (((((color & 0x00ff00u) * fac) & 0x00ff0000u) +
			(((color & 0xff00ffu) * fac) & 0xff00ff00u) ) >> 8u);
}

highp uint TVPAlphaAndColorToAdditiveAlpha(in lowp uint alpha, in highp uint color)
{
	return TVPMulColor(color, alpha) + (color & 0xff000000u);
}

highp uint TVPAlphaToAdditiveAlpha(in highp uint a)
{
	return TVPAlphaAndColorToAdditiveAlpha(a >> 24u, a);
}

highp uint TVPAddAlphaBlend_a_d(in highp uint dest, in highp uint src)
{
	return TVPAddAlphaBlend_a_a(dest, TVPAlphaToAdditiveAlpha(src));
}

highp uint TVPAddAlphaBlend_a_d_o(in highp uint dest, in highp uint src, in lowp uint opa)
{
	src = (src & 0xffffffu) + ((((src >> 24u) * opa) >> 8u) << 24u);
	return TVPAddAlphaBlend_a_d(dest, src);
}

void TVPAlphaBlend_a(inout highp uint dest, in highp uint src)
{
	dest = TVPAddAlphaBlend_a_d(dest, src);
}

void TVPAlphaBlend_ao(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	dest = TVPAddAlphaBlend_a_d_o(dest, src, opa);
}

void TVPConstAlphaBlend_a(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	opa <<= 24u;
	dest = TVPAddAlphaBlend_a_a(dest, (src & 0xffffffu) | opa);
}

void TVPPsOperation1_Begin(in highp uint src, in highp uint opa, out highp uint a)
{
	a = src>>24u;
}

void TVPPsOperation1_O_Begin(in highp uint src, in highp uint opa, out highp uint a)
{
	a = ((src>>24u)*opa)>>8;
}

void TVPPsOperation1_End(out highp uint dest, in highp uint s, in highp uint d)
{
	dest = s;
}

void TVPPsOperation1_HDA_End(out highp uint dest, in highp uint s, in highp uint d)
{
	dest = s|(d&0xff000000u);
}

void TVPPs_AlphaBlend(in highp uint d, inout highp uint s, in highp uint a)
{
	highp uint d1 = d&0x00ff00ffu;
	highp uint d2 = d&0x0000ff00u;
	s = ((((((s&0x00ff00ffu)-d1)*a)>>8u)+d1)&0x00ff00ffu)|((((((s&0x0000ff00u)-d2)*a)>>8u)+d2)&0x0000ff00u);
}

void TVPPs_FadeSrc(inout highp uint s, in highp uint a)
{
	s = ((((s&0x00ff00ffu)*a)>>8u)&0x00ff00ffu)|((((s&0x0000ff00u)*a)>>8u)&0x0000ff00u);
}

highp uint TVPPs_CalcTableSoftLight(in highp uint s, in highp uint d)
{
	return (s>=128u) ?
				( (uint(pow(float(d)/255.0, 128.0/float(s))*255.0) & 0xffu) ) :
				( (uint(pow(float(d)/255.0, (1.0-float(s)/255.0)/0.5)*255.0) & 0xffu) );
}

highp uint TVPPs_CalcTableDodge(in highp uint s, in highp uint d)
{
	return ((255u-s)<=d) ? (0xffu) : ((d*255u)/(255u-s));
}

highp uint TVPPs_CalcTableBurn(in highp uint s, in highp uint d)
{
	return (s<=(255u-d)) ? (0x00u) : (255u-((255u-d)*255u)/s);
}

highp uint TVPPs_CalcTableOverlay(in highp uint s, in highp uint d)
{
	return (d<128u) ? ((s*d*2u)/255u) : (((s+d)*2u)-((s*d*2u)/255u)-255u);
}

void TVPPs_OperationAlphaBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationAddBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	highp uint n = (((d&s)<<1)+((d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	s = (d+s-n)|n;
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationSubBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = ~s;
	highp uint n = (((~d&s)<<1u)+((~d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	s = (d|n)-(s|n);
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationMulBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = ( ((((d>>16u)&0xffu)*(s&0x00ff0000u))&0xff000000u) |
	      ((((d>>8u )&0xffu)*(s&0x0000ff00u))&0x00ff0000u) |
	      ((((d>>0u )&0xffu)*(s&0x000000ffu))           ) ) >> 8u;
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationScreenBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	/* c = ((s+d-(s*d)/255)-d)*a + d = (s-(s*d)/255)*a + d */
	highp uint sd1 = ( ((((d>>16u)&0xffu)*(s&0x00ff0000u))&0xff000000u) |
	        ((((d>>0u )&0xffu)*(s&0x000000ffu))           ) ) >> 8u;
	highp uint sd2 = ( ((((d>>8u )&0xffu)*(s&0x0000ff00u))&0x00ff0000u) ) >> 8u;
	s = ((((((s&0x00ff00ffu)-sd1)*a)>>8u)+(d&0x00ff00ffu))&0x00ff00ffu) |
	    ((((((s&0x0000ff00u)-sd2)*a)>>8u)+(d&0x0000ff00u))&0x0000ff00u);
}

void TVPPs_OperationOverlayBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = (TVPPs_CalcTableOverlay((s>>16u)&0xffu, (d>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableOverlay((s>>8u )&0xffu, (d>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableOverlay((s>>0u )&0xffu, (d>>0u )&0xffu)<<0u );
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationHardLightBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = (TVPPs_CalcTableOverlay((d>>16u)&0xffu, (s>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableOverlay((d>>8u )&0xffu, (s>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableOverlay((d>>0u )&0xffu, (s>>0u )&0xffu)<<0u );
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationSoftLightBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = (TVPPs_CalcTableSoftLight((s>>16u)&0xffu, (d>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableSoftLight((s>>8u )&0xffu, (d>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableSoftLight((s>>0u )&0xffu, (d>>0u )&0xffu)<<0u );
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationColorDodgeBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = (TVPPs_CalcTableDodge((s>>16u)&0xffu, (d>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableDodge((s>>8u )&0xffu, (d>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableDodge((s>>0u )&0xffu, (d>>0u )&0xffu)<<0u );
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationColorDodge5Blend(inout highp uint d, inout highp uint s, in highp uint a)
{
	TVPPs_FadeSrc(s, a);
	s = (TVPPs_CalcTableDodge((s>>16u)&0xffu, (d>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableDodge((s>>8u )&0xffu, (d>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableDodge((s>>0u )&0xffu, (d>>0u )&0xffu)<<0u );
}

void TVPPs_OperationColorBurnBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	s = (TVPPs_CalcTableBurn((s>>16u)&0xffu, (d>>16u)&0xffu)<<16u) |
	    (TVPPs_CalcTableBurn((s>>8u )&0xffu, (d>>8u )&0xffu)<<8u ) |
	    (TVPPs_CalcTableBurn((s>>0u )&0xffu, (d>>0u )&0xffu)<<0u );
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationLightenBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	highp uint n = 0u;
	n = (((~d&s)<<1u)+((~d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	/* n=mask (d<s:0xff, d>=s:0x00) */
	s = (s&n)|(d&~n);
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationDarkenBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	highp uint n = 0u;
	n = (((~d&s)<<1u)+((~d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	/* n=mask (d<s:0xff, d>=s:0x00) */
	s = (d&n)|(s&~n);
	TVPPs_AlphaBlend(d, s, a);
}

void TVPPs_OperationDiffBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	highp uint n = 0u;
	n = (((~d&s)<<1u)+((~d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	/* n=mask (d<s:0xff, d>=s:0x00) */
	s = ((s&n)-(d&n))|((d&~n)-(s&~n));
	TVPPs_AlphaBlend(d, s, a);     /* Alphablend result & dst */
}

void TVPPs_OperationDiff5Blend(inout highp uint d, inout highp uint s, in highp uint a)
{
	highp uint n = 0u;
	TVPPs_FadeSrc(s, a);        /* Fade src first */
	n = (((~d&s)<<1u)+((~d^s)&0x00fefefeu))&0x01010100u;
	n = ((n>>8u)+0x007f7f7fu)^0x007f7f7fu;
	/* n=mask (d<s:0xff, d>=s:0x00) */
	s = ((s&n)-(d&n))|((d&~n)-(s&~n));
}

void TVPPs_OperationExclusionBlend(inout highp uint d, inout highp uint s, in highp uint a)
{
	/* c = ((s+d-(s*d*2)/255)-d)*a + d = (s-(s*d*2)/255)*a + d */
	highp uint sd1 = ( ((((d>>16u)&0xffu)*((s&0x00ff0000u)>>7u))&0x01ff0000u) |
	        ((((d>>0u )&0xffu)*( s&0x000000ffu    ))>>7u        ) );
	highp uint sd2 = ( ((((d>>8u )&0xffu)*(s&0x0000ff00u))&0x00ff8000u) ) >> 7u;
	s = ((((((s&0x00ff00ffu)-sd1)*a)>>8u)+(d&0x00ff00ffu))&0x00ff00ffu) |
	    ((((((s&0x0000ff00u)-sd2)*a)>>8u)+(d&0x0000ff00u))&0x0000ff00u);
}

// Unfortunately, GLSL doesn't appear to support token concat properly… (this won't work)
#define TVPPS_FUNC(blendname) \
	void TVPPs ## blendname ## Blend(inout highp uint dest, in highp uint src) \
	{ \
		highp uint d = dest; \
		highp uint s = src; \
		highp uint a = 0u; \
		TVPPsOperation1_Begin(s, 255u, a); \
		TVPPs_Operation ## blendname ## Blend(d, s, a); \
		TVPPsOperation1_End(dest, s, d); \
	} \
	void TVPPs ## blendname ## Blend_o(inout highp uint dest, in highp uint src, in lowp uint opa) \
	{ \
		highp uint d = dest; \
		highp uint s = src; \
		highp uint a = 0u; \
		TVPPsOperation1_O_Begin(s, opa, a); \
		TVPPs_Operation ## blendname ## Blend(d, s, a); \
		TVPPsOperation1_End(dest, s, d); \
	} \
	void TVPPs ## blendname ## Blend_HDA(inout highp uint dest, in highp uint src) \
	{ \
		highp uint d = dest; \
		highp uint s = src; \
		highp uint a = 0u; \
		TVPPsOperation1_Begin(s, 255u, a); \
		TVPPs_Operation ## blendname ## Blend(d, s, a); \
		TVPPsOperation1_HDA_End(dest, s, d); \
	} \
	void TVPPs ## blendname ## Blend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa) \
	{ \
		highp uint d = dest; \
		highp uint s = src; \
		highp uint a = 0u; \
		TVPPsOperation1_O_Begin(s, opa, a); \
		TVPPs_Operation ## blendname ## Blend(d, s, a); \
		TVPPsOperation1_HDA_End(dest, s, d); \
	}

void TVPPsAlphaBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationAlphaBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsAlphaBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationAlphaBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsAlphaBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationAlphaBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsAlphaBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationAlphaBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsAddBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationAddBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsAddBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationAddBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsAddBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationAddBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsAddBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationAddBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsSubBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationSubBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsSubBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationSubBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsSubBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationSubBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsSubBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationSubBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsMulBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationMulBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsMulBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationMulBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsMulBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationMulBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsMulBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationMulBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsScreenBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationScreenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsScreenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationScreenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsScreenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationScreenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsScreenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationScreenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsOverlayBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationOverlayBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsOverlayBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationOverlayBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsOverlayBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationOverlayBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsOverlayBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationOverlayBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsHardLightBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationHardLightBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsHardLightBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationHardLightBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsHardLightBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationHardLightBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsHardLightBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationHardLightBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsSoftLightBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationSoftLightBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsSoftLightBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationSoftLightBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsSoftLightBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationSoftLightBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsSoftLightBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationSoftLightBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorDodgeBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorDodgeBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorDodgeBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorDodgeBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorDodgeBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorDodgeBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorDodgeBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorDodgeBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorDodge5Blend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorDodge5Blend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorDodge5Blend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorDodge5Blend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorDodge5Blend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorDodge5Blend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorDodge5Blend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorDodge5Blend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorBurnBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorBurnBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorBurnBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorBurnBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsColorBurnBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationColorBurnBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsColorBurnBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationColorBurnBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsLightenBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationLightenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsLightenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationLightenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsLightenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationLightenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsLightenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationLightenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDarkenBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDarkenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDarkenBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDarkenBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDarkenBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDarkenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDarkenBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDarkenBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDiffBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDiffBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDiffBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDiffBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDiffBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDiffBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDiffBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDiffBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDiff5Blend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDiff5Blend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDiff5Blend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDiff5Blend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsDiff5Blend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationDiff5Blend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsDiff5Blend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationDiff5Blend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsExclusionBlend(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationExclusionBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsExclusionBlend_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationExclusionBlend(d, s, a);
	TVPPsOperation1_End(dest, s, d);
}
void TVPPsExclusionBlend_HDA(inout highp uint dest, in highp uint src)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_Begin(s, 255u, a);
	TVPPs_OperationExclusionBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}
void TVPPsExclusionBlend_HDA_o(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	highp uint d = dest;
	highp uint s = src;
	highp uint a = 0u;
	TVPPsOperation1_O_Begin(s, opa, a);
	TVPPs_OperationExclusionBlend(d, s, a);
	TVPPsOperation1_HDA_End(dest, s, d);
}

#define TVP_BLEND_4(basename) \
	if(opa == 255u) \
	{ \
		if (hda == 0u) \
		{ \
			basename(dest, src); \
		} \
		else \
		{ \
			basename ## _HDA(dest, src); \
		} \
	} \
	else \
	{ \
		if (hda == 0u) \
		{ \
			basename ## _o(dest, src, opa); \
		} \
		else \
		{ \
			basename ## _HDA_o(dest, src, opa); \
		} \
	}

void TVPBltDispatch(inout highp uint dest, in highp uint src, in lowp uint opa)
{
	lowp uint method = uint(a_method);
	lowp uint hda = uint(a_hda);
	switch(method)
	{
	case bmCopy:
		// constant ratio alpha blendng
		if(opa == 255u && hda != 0u)
		{
			TVPCopyColor(dest, src);
		}
		else if(hda == 0u)
		{
			TVPConstAlphaBlend(dest, src, opa);
		}
		else
		{
			TVPConstAlphaBlend_HDA(dest, src, opa);
		}
		break;

	case bmCopyOnAlpha:
		// constant ratio alpha blending (assuming source is opaque)
		// with consideration of destination alpha
		if(opa == 255u)
		{
			TVPCopyOpaqueImage(dest, src);
		}
		else
		{
			TVPConstAlphaBlend_d(dest, src, opa);
		}
		break;


	case bmAlpha:
		// alpha blending, ignoring destination alpha
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPAlphaBlend(dest, src);
			}
			else
			{
				TVPAlphaBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPAlphaBlend_o(dest, src, opa);
			}
			else
			{
				TVPAlphaBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmAlphaOnAlpha:
		// alpha blending, with consideration of destination alpha
		if(opa == 255u)
		{
			TVPAlphaBlend_d(dest, src);
		}
		else
		{
			TVPAlphaBlend_do(dest, src, opa);
		}
		break;

	case bmAdd:
		// additive blending ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPAddBlend(dest, src);
			}
			else
			{
				TVPAddBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPAddBlend_o(dest, src, opa);
			}
			else
			{
				TVPAddBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmSub:
		// subtractive blending ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPSubBlend(dest, src);
			}
			else
			{
				TVPSubBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPSubBlend_o(dest, src, opa);
			}
			else
			{
				TVPSubBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmMul:
		// multiplicative blending ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPMulBlend(dest, src);
			}
			else
			{
				TVPMulBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPMulBlend_o(dest, src, opa);
			}
			else
			{
				TVPMulBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmDodge:
		// color dodge mode ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPColorDodgeBlend(dest, src);
			}
			else
			{
				TVPColorDodgeBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPColorDodgeBlend_o(dest, src, opa);
			}
			else
			{
				TVPColorDodgeBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmDarken:
		// darken mode ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPDarkenBlend(dest, src);
			}
			else
			{
				TVPDarkenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPDarkenBlend_o(dest, src, opa);
			}
			else
			{
				TVPDarkenBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmLighten:
		// lighten mode ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPLightenBlend(dest, src);
			}
			else
			{
				TVPLightenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPLightenBlend_o(dest, src, opa);
			}
			else
			{
				TVPLightenBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmScreen:
		// screen multiplicative mode ( this does not consider distination alpha )
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPScreenBlend(dest, src);
			}
			else
			{
				TVPScreenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPScreenBlend_o(dest, src, opa);
			}
			else
			{
				TVPScreenBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmAddAlpha:
		// Additive Alpha
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPAdditiveAlphaBlend(dest, src);
			}
			else
			{
				TVPAdditiveAlphaBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPAdditiveAlphaBlend_o(dest, src, opa);
			}
			else
			{
				TVPAdditiveAlphaBlend_HDA_o(dest, src, opa);
			}
		}
		break;


	case bmAddAlphaOnAddAlpha:
		// Additive Alpha on Additive Alpha
		if(opa == 255u)
		{
			TVPAdditiveAlphaBlend_a(dest, src);
		}
		else
		{
			TVPAdditiveAlphaBlend_ao(dest, src, opa);
		}
		break;


	case bmAddAlphaOnAlpha:
		// additive alpha on simple alpha
		// Not yet implemented
		break;

	case bmAlphaOnAddAlpha:
		// simple alpha on additive alpha
		if(opa == 255u)
		{
			TVPAlphaBlend_a(dest, src);
		}
		else
		{
			TVPAlphaBlend_ao(dest, src, opa);
		}
		break;

	case bmCopyOnAddAlpha:
		// constant ratio alpha blending (assuming source is opaque)
		// with consideration of destination additive alpha
		if(opa == 255u)
			TVPCopyOpaqueImage(dest, src);
		else
			TVPConstAlphaBlend_a(dest, src, opa);
		break;


	case bmPsNormal:
		// Photoshop compatible normal blend
		// (may take the same effect as bmAlpha)
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsAlphaBlend(dest, src);
			}
			else
			{
				TVPPsAlphaBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsAlphaBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsAlphaBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsAdditive:
		// Photoshop compatible additive blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsAddBlend(dest, src);
			}
			else
			{
				TVPPsAddBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsAddBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsAddBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsSubtractive:
		// Photoshop compatible subtractive blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsSubBlend(dest, src);
			}
			else
			{
				TVPPsSubBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsSubBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsSubBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsMultiplicative:
		// Photoshop compatible multiplicative blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsMulBlend(dest, src);
			}
			else
			{
				TVPPsMulBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsMulBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsMulBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsScreen:
		// Photoshop compatible screen blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsScreenBlend(dest, src);
			}
			else
			{
				TVPPsScreenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsScreenBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsScreenBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsOverlay:
		// Photoshop compatible overlay blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsOverlayBlend(dest, src);
			}
			else
			{
				TVPPsOverlayBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsOverlayBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsOverlayBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsHardLight:
		// Photoshop compatible hard light blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsHardLightBlend(dest, src);
			}
			else
			{
				TVPPsHardLightBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsHardLightBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsHardLightBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsSoftLight:
		// Photoshop compatible soft light blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsSoftLightBlend(dest, src);
			}
			else
			{
				TVPPsSoftLightBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsSoftLightBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsSoftLightBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsColorDodge:
		// Photoshop compatible color dodge blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsColorDodgeBlend(dest, src);
			}
			else
			{
				TVPPsColorDodgeBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsColorDodgeBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsColorDodgeBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsColorDodge5:
		// Photoshop 5.x compatible color dodge blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsColorDodge5Blend(dest, src);
			}
			else
			{
				TVPPsColorDodge5Blend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsColorDodge5Blend_o(dest, src, opa);
			}
			else
			{
				TVPPsColorDodge5Blend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsColorBurn:
		// Photoshop compatible color burn blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsColorBurnBlend(dest, src);
			}
			else
			{
				TVPPsColorBurnBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsColorBurnBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsColorBurnBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsLighten:
		// Photoshop compatible compare (lighten) blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsLightenBlend(dest, src);
			}
			else
			{
				TVPPsLightenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsLightenBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsLightenBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsDarken:
		// Photoshop compatible compare (darken) blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsDarkenBlend(dest, src);
			}
			else
			{
				TVPPsDarkenBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsDarkenBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsDarkenBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsDifference:
		// Photoshop compatible difference blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsDiffBlend(dest, src);
			}
			else
			{
				TVPPsDiffBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsDiffBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsDiffBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsDifference5:
		// Photoshop 5.x compatible difference blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsDiff5Blend(dest, src);
			}
			else
			{
				TVPPsDiff5Blend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsDiff5Blend_o(dest, src, opa);
			}
			else
			{
				TVPPsDiff5Blend_HDA_o(dest, src, opa);
			}
		}
		break;

	case bmPsExclusion:
		// Photoshop compatible exclusion blend
		if(opa == 255u)
		{
			if (hda == 0u)
			{
				TVPPsExclusionBlend(dest, src);
			}
			else
			{
				TVPPsExclusionBlend_HDA(dest, src);
			}
		}
		else
		{
			if (hda == 0u)
			{
				TVPPsExclusionBlend_o(dest, src, opa);
			}
			else
			{
				TVPPsExclusionBlend_HDA_o(dest, src, opa);
			}
		}
		break;

	default:
		break;
	}
}

void main()
{
	vec4 color0 = texture( s_tex0, v_texCoord );
	highp uint src = (uint(color0.b * 255.0) << 24u) | (uint(color0.g * 255.0) << 16u) | (uint(color0.r * 255.0) << 8u) | uint(color0.a * 255.0);
	vec4 color1 = texture( s_tex1, v_texCoord );
	highp uint dest = (uint(color1.b * 255.0) << 24u) | (uint(color1.g * 255.0) << 16u) | (uint(color1.r * 255.0) << 8u) | uint(color1.a * 255.0);
	TVPBltDispatch(dest, src, uint(a_opa));
	color1.b = float((dest >> 24u) & 255u) / 255.0;
	color1.g = float((dest >> 16u) & 255u) / 255.0;
	color1.r = float((dest >>  8u) & 255u) / 255.0;
	color1.a = float((dest >>  0u) & 255u) / 255.0;
	fragmentColor = color1;
}
