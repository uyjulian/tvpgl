/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

TVP_GL_IA32_FUNC_DECL(tjs_int, TVPTLG5DecompressSlide_c, (tjs_uint8 *out, const tjs_uint8 *in, tjs_int insize, tjs_uint8 *text, tjs_int initialr))
{
	tjs_int          result; // eax
	const tjs_uint8 *v6;     // ebx
	tjs_uint8 *      v7;     // edi
	unsigned int     v9;     // ecx
	char             v10;    // cf
	tjs_uint8        v11;    // dl
	int              v12;    // eax
	int              v13;    // edx
	int              v14;    // ecx
	const tjs_uint8 *v15;    // ebx
	tjs_uint8        v16;    // bl
	int              v17;    // eax
	const tjs_uint8 *v18;    // [esp-8h] [ebp-20h]
	unsigned int     v19;    // [esp-4h] [ebp-1Ch]

	result = initialr;
	v6     = in;
	v7     = out;
	if (in < &in[insize])
	{
		while (1)
		{
			v9 = *v6++ | 0x100;
			while (1)
			{
				v10 = v9 & 1;
				v9 >>= 1;
				if (!v9)
					break;
				if (v10)
				{
					v19 = v9;
					v13 = *(tjs_uint16 *)v6 & 0xFFF;
					v14 = ((unsigned int)*(tjs_uint16 *)v6 >> 12) + 3;
					v15 = v6 + 2;
					if (v14 == 18)
						v14 = *v15++ + 18;
					v18 = v15;
					do
					{
						v16           = text[v13];
						v17           = result + 1;
						text[v17 - 1] = v16;
						v13           = ((tjs_uint16)v13 + 1) & 0xFFF;
						result        = v17 & 0xFFF;
						--v14;
						*v7 = v16;
						++v7;
					} while (v14);
					v6 = v18;
					v9 = v19;
					if (v18 >= &in[insize])
						return result;
				}
				else
				{
					v11 = *v6;
					v12 = result + 1;
					++v6;
					text[v12 - 1] = v11;
					result        = v12 & 0xFFF;
					*v7           = v11;
					++v7;
					if (v6 >= &in[insize])
						return result;
				}
			}
		}
	}
	return result;
}

TVP_GL_IA32_FUNC_DECL(void, TVPTLG5ComposeColors3To4_c, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 *const *buf, tjs_int width))
{
	unsigned int v4;  // edx
	__m64        v5;  // mm5
	__m64        v6;  // mm6
	__m64        v7;  // mm7
	__m64        v8;  // mm4
	__m64        v10; // mm0
	tjs_int      i;   // ecx

	v5  = _mm_cvtsi32_si64((unsigned int)*buf);
	v6  = _mm_cvtsi32_si64(*((tjs_uint32 *)buf + 1));
	v7  = _mm_cvtsi32_si64(*((tjs_uint32 *)buf + 2));
	v8  = _mm_setzero_si64();
	v10 = _mm_set1_pi32(0xFF000000);
	for (i = 0;
		 i < width;
		 *(tjs_uint32 *)&outp[4 * i - 4] = _mm_cvtsi64_si32(_m_por(_m_paddb(v8, _mm_cvtsi32_si64(*(tjs_uint32 *)&upper[4 * i - 4])), v10)))
	{
		LOBYTE(v4) = *(tjs_uint8 *)(_mm_cvtsi64_si32(v6) + i);
		BYTE1(v4)  = v4 + *(tjs_uint8 *)(_mm_cvtsi64_si32(v7) + i);
		v4 <<= 8;
		LOBYTE(v4) = BYTE1(v4) + *(tjs_uint8 *)(_mm_cvtsi64_si32(v5) + i++);
		v8         = _m_paddb(v8, _mm_cvtsi32_si64(v4));
	}
	_m_empty();
}

TVP_GL_IA32_FUNC_DECL(void, TVPTLG5ComposeColors4To4_c, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 *const *buf, tjs_int width))
{
	unsigned int v4;  // edx
	__m64        v5;  // mm5
	__m64        v6;  // mm6
	__m64        v7;  // mm7
	__m64        v8;  // mm2
	__m64        v9;  // mm4
	tjs_int      i;   // ecx
	unsigned int v11; // edx

	v5 = _mm_cvtsi32_si64((unsigned int)*buf);
	v6 = _mm_cvtsi32_si64(*((tjs_uint32 *)buf + 1));
	v7 = _mm_cvtsi32_si64(*((tjs_uint32 *)buf + 2));
	v8 = _mm_cvtsi32_si64(*((tjs_uint32 *)buf + 3));
	v9 = _mm_setzero_si64();
	for (i = 0;
		 i < width;
		 *(tjs_uint32 *)&outp[4 * i - 4] = _mm_cvtsi64_si32(_m_paddb(v9, _mm_cvtsi32_si64(*(tjs_uint32 *)&upper[4 * i - 4]))))
	{
		LOBYTE(v4)  = *(tjs_uint8 *)(_mm_cvtsi64_si32(v8) + i);
		v11         = v4 << 16;
		LOBYTE(v11) = *(tjs_uint8 *)(_mm_cvtsi64_si32(v6) + i);
		BYTE1(v11)  = v11 + *(tjs_uint8 *)(_mm_cvtsi64_si32(v7) + i);
		v4          = v11 << 8;
		LOBYTE(v4)  = BYTE1(v4) + *(tjs_uint8 *)(_mm_cvtsi64_si32(v5) + i++);
		v9          = _m_paddb(v9, _mm_cvtsi32_si64(v4));
	}
	_m_empty();
}
