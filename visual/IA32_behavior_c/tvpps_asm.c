/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <immintrin.h>
#include <mmintrin.h>
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

TVP_GL_IA32_BLEND_FUNC(Alpha, {
	tjs_uint16 k = s[j];
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Add, {
	tjs_uint16 k = s[j];
	k += d[j];
	if (k > 0xff)
	{
		k = 0xff;
	}
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Sub, {
	tjs_uint16 k = s[j];
	k ^= 0xff;
	k = d[j] - k;
	if (k > 0xff)
	{
		k = 0;
	}
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Mul, {
	tjs_uint16 k = s[j];
	k *= d[j];
	k >>= 8u;
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Screen, {
	tjs_uint16 k = s[j];
	k *= d[j];
	k >>= 8u;
	k = s[j] - k;
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

#if 0
// FIXME: some channel is wrong on regular/HDA versions (not opacity version)
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
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});
#else
TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v12 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v13 = _m_pcmpgtw(_mm_set1_pi16(0x0080), d);
		__m64 v14 = _mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(s, d), 1u), v12), _mm_set1_pi16(0x00FF)))),
								d),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v12 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v13 = _m_pcmpgtw(_mm_set1_pi16(0x0080), d);
		__m64 v15 = _mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(s, d), 1u), v12), _mm_set1_pi16(0x00FF)))),
								d),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 v8  = _m_psrldi(_mm_cvtsi32_si64(src[i]), 0x19u);
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v12 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v13 = _m_pcmpgtw(_mm_set1_pi16(0x0080), d);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(s, d), 1u), v12), _mm_set1_pi16(0x00FF)))),
								d),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v12 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v13 = _m_pcmpgtw(_mm_set1_pi16(0x0080), d);
		__m64 v14 = _m_psrldi(_m_pmullw(_m_psrldi(_mm_cvtsi32_si64(src[i]), 0x19u), _mm_cvtsi32_si64(opa)), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(s, d), 1u), v12), _mm_set1_pi16(0x00FF)))),
								d),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u)), v14)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
#endif

#if 0
// FIXME: some channel is wrong on regular/HDA versions (not opacity version)
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
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});
#else
TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v12 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v13 = _m_pcmpgtw(_mm_set1_pi16(0x0080), s);
		__m64 v14 = _mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(s, d), 1u), v12), _mm_set1_pi16(0x00FF)))),
								d),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v11 = _m_pcmpgtw(_mm_set1_pi16(0x0080), s);
		__m64 v12 = _m_paddw(s, d);
		__m64 v13 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v15 = _mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), _mm_set1_pi16(0x00FF)))),
								d),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 v8  = _m_psrldi(_mm_cvtsi32_si64(src[i]), 0x19u);
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v11 = _m_pcmpgtw(_mm_set1_pi16(0x0080), s);
		__m64 v12 = _m_paddw(s, d);
		__m64 v13 = _m_psrlwi(_m_pmullw(s, d), 7u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), _mm_set1_pi16(0x00FF)))),
								d),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	for (tjs_int i = 0; i < len; i += 1)
	{
		__m64 d = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		__m64 s = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		__m64 v11 = _m_pcmpgtw(_mm_set1_pi16(0x0080), s);
		__m64 v12 = _m_paddw(s, d);
		__m64 v13 = _m_psrlwi(_m_pmullw(s, d), 7u);
		__m64 v14 = _m_psrldi(_m_pmullw(_m_psrldi(_mm_cvtsi32_si64(src[i]), 0x19u), _mm_cvtsi32_si64(opa)), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					d,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), _mm_set1_pi16(0x00FF)))),
								d),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u)), v14)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
#endif

#define CALC_USING_TABLE(x, s, d) (                 \
	(((s >> 24) & 0xff) << 24) |                    \
	(x[(s >> 16) & 0xff][(d >> 16) & 0xff] << 16) | \
	(x[(s >> 8) & 0xff][(d >> 8) & 0xff] << 8) |    \
	(x[(s >> 0) & 0xff][(d >> 0) & 0xff] << 0))

#define CALC_USING_TABLE_HDA(x, s, d) (             \
	(((d >> 24) & 0xff) << 24) |                    \
	(x[(s >> 16) & 0xff][(d >> 16) & 0xff] << 16) | \
	(x[(s >> 8) & 0xff][(d >> 8) & 0xff] << 8) |    \
	(x[(s >> 0) & 0xff][(d >> 0) & 0xff] << 0))

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
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
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
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Lighten, {
	tjs_uint16 k = d[j];
	k -= s[j];
	if (k > 0xff)
	{
		k = 0;
	}
	k += s[j];
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});

TVP_GL_IA32_BLEND_FUNC(Darken, {
	tjs_uint16 k = s[j];
	k -= d[j];
	if (k > 0xff)
	{
		k = 0;
	}
	k = s[j] - k;
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
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
	k -= d[j];
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
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
	k = s[j] - k;
	k *= sevenbit;
	k >>= 7;
	k += d[j];
	d[j] = k;
});
