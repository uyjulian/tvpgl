/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <mmintrin.h>
#include <stdbool.h>
#include "tvpgl_ia32_intf_pfraction_c.h"

static tjs_uint32 bit_mask_tbl[33] =
	{
		(1u << 0) - 1,
		(1u << 1) - 1,
		(1u << 2) - 1,
		(1u << 3) - 1,
		(1u << 4) - 1,
		(1u << 5) - 1,
		(1u << 6) - 1,
		(1u << 7) - 1,
		(1u << 8) - 1,
		(1u << 9) - 1,
		(1u << 10) - 1,
		(1u << 11) - 1,
		(1u << 12) - 1,
		(1u << 13) - 1,
		(1u << 14) - 1,
		(1u << 15) - 1,
		(1u << 16) - 1,
		(1u << 17) - 1,
		(1u << 18) - 1,
		(1u << 19) - 1,
		(1u << 20) - 1,
		(1u << 21) - 1,
		(1u << 22) - 1,
		(1u << 23) - 1,
		(1u << 24) - 1,
		(1u << 25) - 1,
		(1u << 26) - 1,
		(1u << 27) - 1,
		(1u << 28) - 1,
		(1u << 29) - 1,
		(1u << 30) - 1,
		(1u << 31) - 1,
		(1u << 32) - 1};

static tjs_uint32 bit_mask_neg_tbl[33] =
	{
		~((1u << 0) - 1),
		~((1u << 1) - 1),
		~((1u << 2) - 1),
		~((1u << 3) - 1),
		~((1u << 4) - 1),
		~((1u << 5) - 1),
		~((1u << 6) - 1),
		~((1u << 7) - 1),
		~((1u << 8) - 1),
		~((1u << 9) - 1),
		~((1u << 10) - 1),
		~((1u << 11) - 1),
		~((1u << 12) - 1),
		~((1u << 13) - 1),
		~((1u << 14) - 1),
		~((1u << 15) - 1),
		~((1u << 16) - 1),
		~((1u << 17) - 1),
		~((1u << 18) - 1),
		~((1u << 19) - 1),
		~((1u << 20) - 1),
		~((1u << 21) - 1),
		~((1u << 22) - 1),
		~((1u << 23) - 1),
		~((1u << 24) - 1),
		~((1u << 25) - 1),
		~((1u << 26) - 1),
		~((1u << 27) - 1),
		~((1u << 28) - 1),
		~((1u << 29) - 1),
		~((1u << 30) - 1),
		~((1u << 31) - 1),
		~((1u << 32) - 1)};

void __cdecl TVPTLG6DecodeGolombValuesForFirst_pfraction_c(tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
{
	tjs_uint8 *  v3;  // esi
	int          v4;  // ecx
	int          v5;  // edi
	int          v6;  // eax
	int          v7;  // edx
	int          v8;  // eax
	unsigned int v9;  // ebx
	int          v10; // ecx
	tjs_int8 *   v11; // ebx
	int          v12; // edi
	int          v13; // edi
	int          v14; // eax
	int          v15; // edx
	int          v16; // eax
	unsigned int v17; // ebx
	int          v18; // ecx
	tjs_int8 *   v19; // edx
	int          v20; // edx
	int          v22; // eax
	int          v23; // edi
	unsigned int v24; // ecx
	int          v25; // eax
	int          v26; // ecx
	unsigned int v27; // eax
	int          v28; // edx
	int          v29; // eax
	bool         v30; // sf
	tjs_int8 *   v31; // [esp-8h] [ebp-1Ch]
	int          v32; // [esp-4h] [ebp-18h]
	tjs_int8 *   v33; // [esp+4h] [ebp-10h]
	signed int   v34; // [esp+8h] [ebp-Ch]
	unsigned int v35; // [esp+Ch] [ebp-8h]

	v33 = &pixelbuf[4 * pixel_count];
	v34 = 3;
	v35 = 0;
	v3  = bit_pool;
	v4  = 1;
	if (*bit_pool & 1)
		goto _TVPTLG6DecodeGolombValuesForFirst_pfraction_a_non_zero_part;
	do
	{
		v5         = 1;
		v6         = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
		v7         = v6;
		v8         = v6 - v4;
		LOBYTE(v4) = v7 + 1;
		v9         = v7 + 1;
		if (v8)
		{
			v3 += v9 >> 3;
			v10 = v4 & 7;
			v5  = bit_mask_tbl[v8] + (bit_mask_tbl[v8] & (*(tjs_uint32 *)v3 >> v10)) + 1;
			v4  = v8 + v10;
			v9  = v4;
		}
		v3 += v9 >> 3;
		v4 &= 7u;
		v11 = &pixelbuf[4 * v5];
		v12 = 4 * v5 - 4;
		do
		{
			*(tjs_uint32 *)&pixelbuf[v12] = 0;
			v12 -= 4;
		} while (v12 >= 0);
		pixelbuf = v11;
		if (v33 == v11)
			break;
	_TVPTLG6DecodeGolombValuesForFirst_pfraction_a_non_zero_part:
		v13        = 1;
		v14        = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
		v15        = v14;
		v16        = v14 - v4;
		LOBYTE(v4) = v15 + 1;
		v17        = v15 + 1;
		if (v16)
		{
			v3 += v17 >> 3;
			v18 = v4 & 7;
			v13 = bit_mask_tbl[v16] + (bit_mask_tbl[v16] & (*(tjs_uint32 *)v3 >> v18)) + 1;
			v4  = v16 + v18;
			v17 = v4;
		}
		v3 += v17 >> 3;
		v4 &= 7u;
		v19 = pixelbuf;
		v31 = v19;
		do
		{
			v32 = v13;
			v20 = (int)(v19 + 4);
			v22 = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
			if ((*(tjs_uint32 *)v3 & (-1 << v4)))
			{
				v3 += 4;
				v23 = *v3;
				v24 = 8;
			}
			else
			{
				v23 = v22 - v4;
				v24 = v22 + 1;
			}
			v25 = (tjs_uint8)TVPTLG6GolombBitLengthTable[v35][v34];
			if (TVPTLG6GolombBitLengthTable[v35][v34])
			{
				v3 += v24 >> 3;
				v26 = v24 & 7;
				v23 <<= v25;
				v25 = bit_mask_tbl[v25] & (*(tjs_uint32 *)v3 >> v26);
				v24 = v26 + (tjs_uint8)TVPTLG6GolombBitLengthTable[v35][v34];
			}
			v27 = v23 + v25;
			v3 += v24 >> 3;
			v28 = (v27 & 1) - 1;
			v27 >>= 1;
			v35 += v27;
			v4  = v24 & 7;
			v29 = (v28 ^ v27) + v28 + 1;
			v30 = v34-- - 1 < 0;
			v19 = v31;
			if (v30)
			{
				v35 >>= 1;
				v34 = 3;
			}
			v13                  = v32 - 1;
			*((tjs_uint32 *)v31) = (tjs_uint8)v29;
			v31                  = (tjs_int8 *)v20;
		} while (v32 != 1);
		pixelbuf = v31;
	} while (v33 != v31);
}

void __cdecl TVPTLG6DecodeGolombValues_pfraction_c(tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
{
	tjs_uint8 *  v3;  // esi
	int          v4;  // ecx
	int          v5;  // edi
	int          v6;  // eax
	int          v7;  // edx
	int          v8;  // eax
	unsigned int v9;  // ebx
	int          v10; // ecx
	int          v11; // edi
	int          v12; // eax
	int          v13; // edx
	int          v14; // eax
	unsigned int v15; // ebx
	int          v16; // ecx
	tjs_int8 *   v17; // edx
	int          v18; // edx
	int          v20; // eax
	int          v21; // edi
	unsigned int v22; // ecx
	int          v23; // eax
	int          v24; // ecx
	unsigned int v25; // eax
	int          v26; // edx
	int          v27; // eax
	bool         v28; // sf
	tjs_int8 *   v29; // [esp-8h] [ebp-1Ch]
	int          v30; // [esp-4h] [ebp-18h]
	tjs_int8 *   v31; // [esp+4h] [ebp-10h]
	signed int   v32; // [esp+8h] [ebp-Ch]
	unsigned int v33; // [esp+Ch] [ebp-8h]

	v31 = &pixelbuf[4 * pixel_count];
	v32 = 3;
	v33 = 0;
	v3  = bit_pool;
	v4  = 1;
	if (*bit_pool & 1)
		goto _TVPTLG6DecodeGolombValues_pfraction_a_non_zero_part;
	do
	{
		v5         = 1;
		v6         = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
		v7         = v6;
		v8         = v6 - v4;
		LOBYTE(v4) = v7 + 1;
		v9         = v7 + 1;
		if (v8)
		{
			v3 += v9 >> 3;
			v10 = v4 & 7;
			v5  = bit_mask_tbl[v8] + (bit_mask_tbl[v8] & (*(tjs_uint32 *)v3 >> v10)) + 1;
			v4  = v8 + v10;
			v9  = v4;
		}
		v3 += v9 >> 3;
		v4 &= 7u;
		pixelbuf += 4 * v5;
		if (v31 == pixelbuf)
			break;
	_TVPTLG6DecodeGolombValues_pfraction_a_non_zero_part:
		v11        = 1;
		v12        = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
		v13        = v12;
		v14        = v12 - v4;
		LOBYTE(v4) = v13 + 1;
		v15        = v13 + 1;
		if (v14)
		{
			v3 += v15 >> 3;
			v16 = v4 & 7;
			v11 = bit_mask_tbl[v14] + (bit_mask_tbl[v14] & (*(tjs_uint32 *)v3 >> v16)) + 1;
			v4  = v14 + v16;
			v15 = v4;
		}
		v3 += v15 >> 3;
		v4 &= 7u;
		v17 = pixelbuf;
		do
		{
			v30 = v11;
			v18 = (int)(v17 + 4);
			v20 = __builtin_ctz(*(tjs_uint32 *)v3 & (-1 << v4));
			if ((*(tjs_uint32 *)v3 & (-1 << v4)))
			{
				v3 += 4;
				v21 = *v3;
				v22 = 8;
			}
			else
			{
				v21 = v20 - v4;
				v22 = v20 + 1;
			}
			v29 = (tjs_int8 *)v18;
			v23 = (tjs_uint8)TVPTLG6GolombBitLengthTable[v33][v32];
			if (TVPTLG6GolombBitLengthTable[v33][v32])
			{
				v3 += v22 >> 3;
				v24 = v22 & 7;
				v21 <<= v23;
				v23 = bit_mask_tbl[v23] & (*(tjs_uint32 *)v3 >> v24);
				v22 = v24 + (tjs_uint8)TVPTLG6GolombBitLengthTable[v33][v32];
			}
			v25 = v21 + v23;
			v3 += v22 >> 3;
			v26 = (v25 & 1) - 1;
			v25 >>= 1;
			v33 += v25;
			v4  = v22 & 7;
			v27 = (v26 ^ v25) + v26 + 1;
			v28 = v32-- - 1 < 0;
			v17 = v29;
			if (v28)
			{
				v33 >>= 1;
				v32 = 3;
			}
			v11        = v30 - 1;
			*(v29 - 4) = v27;
		} while (v30 != 1);
		pixelbuf = v29;
	} while (v31 != v29);
}

void __cdecl TVPTLG6DecodeGolombValuesForFirst_mmx_pfraction_c(tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
{
	__m64        v3;  // mm0
	tjs_uint8 *  v4;  // esi
	int          v5;  // ecx
	int          v6;  // edi
	int          v7;  // eax
	int          v8;  // edx
	int          v9;  // eax
	unsigned int v10; // ebx
	int          v11; // ecx
	int          v12; // eax
	unsigned int v13; // ebx
	int          v14; // edi
	int          v15; // edi
	int          v16; // eax
	int          v17; // edx
	int          v18; // eax
	unsigned int v19; // ebx
	int          v20; // ecx
	int          v21; // edx
	int          v22; // edx
	int          v24; // eax
	int          v25; // edi
	unsigned int v26; // ecx
	int          v27; // eax
	int          v28; // ecx
	unsigned int v29; // eax
	int          v30; // edx
	int          v31; // eax
	bool         v32; // sf
	unsigned int v33; // [esp-8h] [ebp-1Ch]
	int          v34; // [esp-4h] [ebp-18h]
	tjs_int8 *   v35; // [esp+4h] [ebp-10h]
	signed int   v36; // [esp+8h] [ebp-Ch]
	unsigned int v37; // [esp+Ch] [ebp-8h]

	v3  = _mm_cvtsi32_si64((unsigned int)pixelbuf);
	v35 = &pixelbuf[4 * pixel_count];
	v36 = 3;
	v37 = 0;
	v4  = bit_pool;
	v5  = 1;
	if (*bit_pool & 1)
		goto _TVPTLG6DecodeGolombValuesForFirst_mmx_pfraction_a_non_zero_part;
	do
	{
		v6         = 1;
		v7         = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
		v8         = v7;
		v9         = v7 - v5;
		LOBYTE(v5) = v8 + 1;
		v10        = v8 + 1;
		if (v9)
		{
			v4 += v10 >> 3;
			v11 = v5 & 7;
			v6  = bit_mask_tbl[v9] + (bit_mask_tbl[v9] & (*(tjs_uint32 *)v4 >> v11)) + 1;
			v5  = v9 + v11;
			v10 = v5;
		}
		v4 += v10 >> 3;
		v5 &= 7u;
		v12 = _mm_cvtsi64_si32(v3);
		v13 = v12 + 4 * v6;
		v14 = 4 * v6 - 4;
		do
		{
			*(tjs_uint32 *)(v12 + v14) = 0;
			v14 -= 4;
		} while (v14 >= 0);
		v3 = _mm_cvtsi32_si64(v13);
		if (v35 == (tjs_int8 *)v13)
			break;
	_TVPTLG6DecodeGolombValuesForFirst_mmx_pfraction_a_non_zero_part:
		v15        = 1;
		v16        = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
		v17        = v16;
		v18        = v16 - v5;
		LOBYTE(v5) = v17 + 1;
		v19        = v17 + 1;
		if (v18)
		{
			v4 += v19 >> 3;
			v20 = v5 & 7;
			v15 = bit_mask_tbl[v18] + (bit_mask_tbl[v18] & (*(tjs_uint32 *)v4 >> v20)) + 1;
			v5  = v18 + v20;
			v19 = v5;
		}
		v4 += v19 >> 3;
		v5 &= 7u;
		v21 = _mm_cvtsi64_si32(v3);
		do
		{
			v34 = v15;
			v22 = v21 + 4;
			v24 = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
			if ((*(tjs_uint32 *)v4 & (-1 << v5)))
			{
				v4 += 4;
				v25 = *v4;
				v26 = 8;
			}
			else
			{
				v25 = v24 - v5;
				v26 = v24 + 1;
			}
			v33 = v22;
			v27 = (tjs_uint8)TVPTLG6GolombBitLengthTable[v37][v36];
			if (TVPTLG6GolombBitLengthTable[v37][v36])
			{
				v4 += v26 >> 3;
				v28 = v26 & 7;
				v25 <<= v27;
				v27 = bit_mask_tbl[v27] & (*(tjs_uint32 *)v4 >> v28);
				v26 = v28 + (tjs_uint8)TVPTLG6GolombBitLengthTable[v37][v36];
			}
			v29 = v25 + v27;
			v4 += v26 >> 3;
			v30 = (v29 & 1) - 1;
			v29 >>= 1;
			v37 += v29;
			v5  = v26 & 7;
			v31 = (v30 ^ v29) + v30 + 1;
			v32 = v36-- - 1 < 0;
			v21 = v33;
			if (v32)
			{
				v37 >>= 1;
				v36 = 3;
			}
			v15                      = v34 - 1;
			*(tjs_uint32 *)(v33 - 4) = (tjs_uint8)v31;
		} while (v34 != 1);
		v3 = _mm_cvtsi32_si64(v33);
	} while (v35 != (tjs_int8 *)v33);
	_m_empty();
}

void __cdecl TVPTLG6DecodeGolombValues_mmx_pfraction_c(tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
{
	__m64        v3;  // mm0
	tjs_uint8 *  v4;  // esi
	int          v5;  // ecx
	int          v6;  // edi
	int          v7;  // eax
	int          v8;  // edx
	int          v9;  // eax
	unsigned int v10; // ebx
	int          v11; // ecx
	unsigned int v12; // ebx
	int          v13; // edi
	int          v14; // eax
	int          v15; // edx
	int          v16; // eax
	unsigned int v17; // ebx
	int          v18; // ecx
	int          v19; // edx
	int          v20; // edx
	int          v22; // eax
	int          v23; // edi
	unsigned int v24; // ecx
	int          v25; // eax
	int          v26; // ecx
	unsigned int v27; // eax
	int          v28; // edx
	int          v29; // eax
	bool         v30; // sf
	unsigned int v31; // [esp-8h] [ebp-1Ch]
	int          v32; // [esp-4h] [ebp-18h]
	tjs_int8 *   v33; // [esp+4h] [ebp-10h]
	signed int   v34; // [esp+8h] [ebp-Ch]
	unsigned int v35; // [esp+Ch] [ebp-8h]

	v3  = _mm_cvtsi32_si64((unsigned int)pixelbuf);
	v33 = &pixelbuf[4 * pixel_count];
	v34 = 3;
	v35 = 0;
	v4  = bit_pool;
	v5  = 1;
	if (*bit_pool & 1)
		goto _TVPTLG6DecodeGolombValues_mmx_pfraction_a_non_zero_part;
	do
	{
		v6         = 1;
		v7         = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
		v8         = v7;
		v9         = v7 - v5;
		LOBYTE(v5) = v8 + 1;
		v10        = v8 + 1;
		if (v9)
		{
			v4 += v10 >> 3;
			v11 = v5 & 7;
			v6  = bit_mask_tbl[v9] + (bit_mask_tbl[v9] & (*(tjs_uint32 *)v4 >> v11)) + 1;
			v5  = v9 + v11;
			v10 = v5;
		}
		v4 += v10 >> 3;
		v5 &= 7u;
		v12 = _mm_cvtsi64_si32(v3) + 4 * v6;
		v3  = _mm_cvtsi32_si64(v12);
		if (v33 == (tjs_int8 *)v12)
			break;
	_TVPTLG6DecodeGolombValues_mmx_pfraction_a_non_zero_part:
		v13        = 1;
		v14        = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
		v15        = v14;
		v16        = v14 - v5;
		LOBYTE(v5) = v15 + 1;
		v17        = v15 + 1;
		if (v16)
		{
			v4 += v17 >> 3;
			v18 = v5 & 7;
			v13 = bit_mask_tbl[v16] + (bit_mask_tbl[v16] & (*(tjs_uint32 *)v4 >> v18)) + 1;
			v5  = v16 + v18;
			v17 = v5;
		}
		v4 += v17 >> 3;
		v5 &= 7u;
		v19 = _mm_cvtsi64_si32(v3);
		do
		{
			v32 = v13;
			v20 = v19 + 4;
			v22 = __builtin_ctz(*(tjs_uint32 *)v4 & (-1 << v5));
			if ((*(tjs_uint32 *)v4 & (-1 << v5)))
			{
				v4 += 4;
				v23 = *v4;
				v24 = 8;
			}
			else
			{
				v23 = v22 - v5;
				v24 = v22 + 1;
			}
			v31 = v20;
			v25 = (tjs_uint8)TVPTLG6GolombBitLengthTable[v35][v34];
			if (TVPTLG6GolombBitLengthTable[v35][v34])
			{
				v4 += v24 >> 3;
				v26 = v24 & 7;
				v23 <<= v25;
				v25 = bit_mask_tbl[v25] & (*(tjs_uint32 *)v4 >> v26);
				v24 = v26 + (tjs_uint8)TVPTLG6GolombBitLengthTable[v35][v34];
			}
			v27 = v23 + v25;
			v4 += v24 >> 3;
			v28 = (v27 & 1) - 1;
			v27 >>= 1;
			v35 += v27;
			v5  = v24 & 7;
			v29 = (v28 ^ v27) + v28 + 1;
			v30 = v34-- - 1 < 0;
			v19 = v31;
			if (v30)
			{
				v35 >>= 1;
				v34 = 3;
			}
			v13                     = v32 - 1;
			*(tjs_uint8 *)(v31 - 4) = v29;
		} while (v32 != 1);
		v3 = _mm_cvtsi32_si64(v31);
	} while (v33 != (tjs_int8 *)v31);
	_m_empty();
}
