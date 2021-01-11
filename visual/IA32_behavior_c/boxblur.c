/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tvpgl_ia32_intf_behavior_c.h"

TVP_GL_IA32_FUNC_DECL(void, TVPDoBoxBlurAvg32_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len))
{
	tjs_uint32 half_n = n >> 1;

	tjs_uint32 v8 = 0x100000000ll / n;
	tjs_uint32 sum_tmp[4];
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum_tmp[i] = sum[i];
	}
	for (tjs_int i = 0; i < len; i += 1)
	{
		dest[i] = 0 +
		    (((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[3] + half_n)) >> 32) << 24) +
		    (((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[2] + half_n)) >> 32) << 16) +
		    (((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[1] + half_n)) >> 32) <<  8) +
		    (((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[0] + half_n)) >> 32) <<  0);

		for (tjs_int j = 0; j < 4; j += 1)
		{
			sum_tmp[j] += add[(i * 4) + j];
			sum_tmp[j] -= sub[(i * 4) + j];
		}
	}
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum[i] = sum_tmp[i];
	}
}

TVP_GL_IA32_FUNC_DECL(void, TVPDoBoxBlurAvg32_d_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 *add, const tjs_uint32 *sub, tjs_int n, tjs_int len))
{
	tjs_uint32 half_n = n >> 1;

	tjs_uint32 v8 = 0x100000000ll / n;
	tjs_uint32 sum_tmp[4];
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum_tmp[i] = sum[i];
	}
	for (tjs_int i = 0; i < len; i += 1)
	{
		tjs_int32 v15 = (sum_tmp[3] + half_n);
		tjs_int32 v16 = (tjs_uint64)(v8 * (tjs_uint64)v15) >> 32 << 8;
		dest[i] = 0 +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[2] + half_n)) >> 32)] << 16) +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[1] + half_n)) >> 32)] <<  8) +
		    (TVPDivTable[v16 + ((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)(sum_tmp[0] + half_n)) >> 32)] <<  0) +
		                      (((tjs_uint64)((tjs_uint64)v8 * (tjs_uint64)(tjs_int32)                  v15) >> 32)  << 24);
		for (tjs_int j = 0; j < 4; j += 1)
		{
			sum_tmp[j] += add[(i * 4) + j];
			sum_tmp[j] -= sub[(i * 4) + j];
		}
	}
	for (tjs_int i = 0; i < 4; i += 1)
	{
		sum[i] = sum_tmp[i];
	}
}
