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

#if 1
// FIXME: some channel is wrong on regular/HDA versions (not opacity version)
TVP_GL_IA32_BLEND_FUNC(Overlay, {
	tjs_uint16 k = s[j];
	k += d[j];
	k <<= 1u;
	tjs_uint16 l = s[j];
	l *= d[j];
	l >>= 7u;
	k -= l;
	k -= 0xff;
	if (0x80u > d[j])
	{
		k = l;
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
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v14 = _mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v5))),
								v9),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v15; // mm2

	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v15 = _mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v7))),
								v9),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v6, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v5))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsOverlayBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm0
	__m64         v13; // mm7
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v9);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v7))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u)), v14)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}
#endif

#if 1
// FIXME: some channel is wrong on regular/HDA versions (not opacity version)
TVP_GL_IA32_BLEND_FUNC(HardLight, {
	tjs_uint16 k = s[j];
	k += d[j];
	k <<= 1u;
	tjs_uint16 l = s[j];
	l *= d[j];
	l >>= 7u;
	k -= l;
	k -= 0xff;
	if (0x80u > s[j])
	{
		k = l;
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
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v12; // mm7
	__m64         v13; // mm0
	__m64         v14; // mm2

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(_mm_cvtsi32_si64(src[i]), _mm_setzero_si64());
		v12 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v13 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v14 = _mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(
									_m_pand(v12, v13),
									_m_pandn(v13, _m_psubw(_m_psubw(_m_psllwi(_m_paddw(v10, v9), 1u), v12), v5))),
								v9),
							v14),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v15; // mm2

	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v15 = _mm_set1_pi16((tjs_uint16)(((src[i] >> 0x19u) * opa) >> 8u));
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v7))),
								v9),
							v15),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	__m64         v5;  // mm3
	__m64         v6;  // mm2
	__m64         v7;  // mm0
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0

	v5 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v6  = _mm_cvtsi32_si64(src[i]);
		v7  = v6;
		v8  = _m_psrldi(v6, 0x19u);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v7, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v5))),
								v9),
							_m_punpckldq(_mm_set1_pi16((tjs_uint16)(src[i] >> 0x19u)), v8)),
						7u)),
				_mm_setzero_si64()));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPPsHardLightBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	__m64         v6;  // mm4
	__m64         v7;  // mm3
	__m64         v8;  // mm2
	__m64         v9;  // mm1
	__m64         v10; // mm0
	__m64         v11; // mm7
	__m64         v12; // mm6
	__m64         v13; // mm0
	__m64         v14; // mm2

	v6 = _mm_cvtsi32_si64(opa);
	v7 = _m_punpcklbw(_mm_cvtsi32_si64(0xFFFFFFFF), _mm_setzero_si64());
	for (tjs_int i = 0; i < len; i += 1)
	{
		v8  = _mm_cvtsi32_si64(src[i]);
		v9  = _m_punpcklbw(_mm_cvtsi32_si64(dest[i]), _mm_setzero_si64());
		v10 = _m_punpcklbw(v8, _mm_setzero_si64());
		v11 = _m_pcmpgtw(_m_punpcklbw(_mm_cvtsi32_si64(0x80808080), _mm_setzero_si64()), v10);
		v12 = _m_paddw(v10, v9);
		v13 = _m_psrlwi(_m_pmullw(v10, v9), 7u);
		v14 = _m_psrldi(_m_pmullw(_m_psrldi(v8, 0x19u), v6), 8u);
		dest[i] = _mm_cvtsi64_si32(
			_m_packuswb(
				_m_paddw(
					v9,
					_m_psrawi(
						_m_pmullw(
							_m_psubw(
								_m_por(_m_pand(v13, v11), _m_pandn(v11, _m_psubw(_m_psubw(_m_psllwi(v12, 1u), v13), v7))),
								v9),
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
