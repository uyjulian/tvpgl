#include <windows.h>
#include "tjsCommHead.h"
#include "tvpgl.h"
#include "tvpgl_info.h"
#include "tvpgl_ia32_intf.h"
#include "tvpgl_ia32_intf_pfraction.h"
#include "tvpgl_ia32_intf_pfraction_c.h"
#include "detect_cpu.h"
#include <stdlib.h>
#include <stdio.h>
extern "C" tjs_uint32 TVPCPUType;
extern void TVPGL_C_Init();
extern void TVPGL_SSE2_Init();


static tjs_uint32 *testbuff = NULL;
static tjs_uint32 *testdata1 = NULL;
static tjs_uint32 *testdata2 = NULL;
static tjs_uint32 *testdest1 = NULL;
static tjs_uint32 *testdest2 = NULL;
static tjs_uint32 *testtable = NULL;
static tjs_uint8  *testrule = NULL;

#define TEST_COUNT 200
// #define TEST_TVPGL
#define BENCHMARK_TVPGL
// #define STRICT_TEST
// #define TEST_DIFFER_BASE
#define FORCE_ALPHA_TEST
#define TVPGetRoughTickCount32 timeGetTime

#define TVPGL_INIT_FUNCS_1 TVP_GL_FUNCNAME(TVPInitTVPGL)
#define TVPGL_INIT_FUNCS_2 TVP_GL_FUNCNAME(TVPInitTVPGL)
// #define TVPGL_INIT_FUNCS_2 TVPGL_IA32_Init

#define DEST_FUNC_APPENDIX(f) TVP_GL_FUNCNAME(f##_c)

static void InitTestData() {
	if(!testtable || !testrule || !testbuff) {
		if(testtable)
		{
			free(testtable);
			testtable = NULL;
		}
		testtable = (tjs_uint32*)malloc(256 * sizeof(tjs_uint32));
		for(int x = 0; x < 256; ++x) {
			testtable[x] = rand() & 0xFF;
		}
		if(testrule)
		{
			free(testrule);
			testrule = NULL;
		}
		testrule = (tjs_uint8*)malloc(256 * 256);
		for(int x = 0; x < 256 * 256; ++x) {
			testrule[x] = rand() & 0xFF;
		}
		if(testbuff)
		{
			free(testbuff);
			testbuff = NULL;
		}
		testbuff = (tjs_uint32*)malloc(256 * 256 * sizeof(tjs_uint32));
		if(testdest1)
		{
			free(testdest1);
			testdest1 = NULL;
		}
		testdest1 = (tjs_uint32*)malloc(256 * 256 * sizeof(tjs_uint32));
		if(testdest2)
		{
			free(testdest2);
			testdest2 = NULL;
		}
		testdest2 = (tjs_uint32*)malloc(256 * 256 * sizeof(tjs_uint32));
		if(testdata1)
		{
			free(testdata1);
			testdata1 = NULL;
		}
		testdata1 = (tjs_uint32*)malloc(256 * 256 * sizeof(tjs_uint32));
		if(testdata2)
		{
			free(testdata2);
			testdata2 = NULL;
		}
		testdata2 = (tjs_uint32*)malloc(256 * 256 * sizeof(tjs_uint32));
	}
	int obfu = 65531;
	for(int x = 0; x < 256; ++x) {
		for(int y = 0; y < 256; ++y) {
			typedef struct {
				unsigned char a;
				unsigned char r;
				unsigned char g;
				unsigned char b;
			} clr;
			clr *clr1 = (clr*)(testdata1 + 256 * y + x),
				*clr2 = (clr*)(testdata2 + 256 * y + x);
			clr1->a = 255 - x; clr2->a = 255 - y;
			clr1->r = x; clr2->r = y;
			clr1->g = y; clr2->g = 255 - x;
			clr1->b = 255 - y; clr2->b = x;
			if (y == 0) {
				clr1->a = obfu;
				obfu = obfu * 3 + 29;
				clr2->a = obfu;
				obfu = obfu * 3 + 29;
				clr1->r = obfu;
				obfu = obfu * 3 + 29;
				clr2->r = obfu;
				obfu = obfu * 3 + 29;
				clr1->g = obfu;
				obfu = obfu * 3 + 29;
				clr2->g = obfu;
				obfu = obfu * 3 + 29;
				clr1->b = obfu;
				obfu = obfu * 3 + 29;
				clr2->b = obfu;
				obfu = obfu * 3 + 29;
		}
	}
	}
	memcpy(testdest1, testdata2, 256 * 256 * sizeof(tjs_uint32));
	memcpy(testdest2, testdata2, 256 * 256 * sizeof(tjs_uint32));
}

static void CheckTestData(const char *pszFuncName, bool ischeckalpha=true)
{
	for( int j = 0; j < 256 * 256; j++ ) {
		int a1 = (testdest1[j]>>24)&0xff;
		int r1 = (testdest1[j]>>16)&0xff;
		int g1 = (testdest1[j]>> 8)&0xff;
		int b1 = (testdest1[j]>> 0)&0xff;

		int a2 = (testdest2[j]>>24)&0xff;
		int r2 = (testdest2[j]>>16)&0xff;
		int g2 = (testdest2[j]>> 8)&0xff;
		int b2 = (testdest2[j]>> 0)&0xff;

		int adiff = abs( a1 - a2 );
		int rdiff = abs( r1 - r2 );
		int gdiff = abs( g1 - g2 );
		int bdiff = abs( b1 - b2 );
		// check color
#ifdef STRICT_TEST
		const int range = 0;
#else
		const int range = 3;
#endif
		if( (rdiff | gdiff | bdiff) ) {
			// Color difference detected.
			if( rdiff < range && gdiff < range && bdiff < range ) {	// warning error level because there is only one difference
			} else {
				fprintf(stderr, "test fail on function %s\n", pszFuncName);
				fprintf(stderr, "invalid color src : 0x%08x, 1 : 0x%08x, 2 : 0x%08x\n", testdata2[j], testdest1[j], testdest2[j] );
				fprintf(stderr, "1 0x%08x\n", testdest1[j] );
				fprintf(stderr, "2 0x%08x\n", testdest2[j] );
				int start = (j>>2)<<2;
				int limit = start + 4;
				fprintf(stderr, "src : " );
				for( int k = start; k < limit; k++ ) {
					fprintf(stderr, "0x%08x,", testdata2[k] );
				}
				fprintf(stderr, "\n1 : " );
				for( int k = start; k < limit; k++ ) {
					fprintf(stderr, "0x%08x,", testdest1[k] );
				}
				fprintf(stderr, "\n2 : " );
				for( int k = start; k < limit; k++ ) {
					fprintf(stderr, "0x%08x,", testdest2[k] );
				}
				fprintf(stderr, "\noffset : %d\n", j - start );
				break;
			}
		}
		if( ischeckalpha && adiff ) {
			// Alpha difference detected.
			if( adiff < range) {	// warning error level
			} else {
				fprintf(stderr, "test fail on function %s\n", pszFuncName);
				fprintf(stderr, "invalid alpha src : 0x%08x, 1 : 0x%08x, 2 : 0x%08x\n", testdata2[j], testdest1[j], testdest2[j] );
				fprintf(stderr, "1 0x%08x\n", testdest1[j] );
				fprintf(stderr, "2 0x%08x\n", testdest2[j] );
				break;
			}
		}
	}
}

static void CheckTestData_RGB(const char *pszFuncName)
{
#ifdef FORCE_ALPHA_TEST
	CheckTestData(pszFuncName, true);
#else
	CheckTestData(pszFuncName, false);
#endif
}

static bool IsDifferentFunction(void **fptr)
{
#ifdef TEST_DIFFER_BASE
	TVP_GL_FUNCNAME(TVPInitTVPGL)();
#else
	TVPGL_INIT_FUNCS_1();
#endif
	void *fptr1 = *fptr;
	TVPGL_INIT_FUNCS_2();
	void *fptr2 = *fptr;
	return (fptr1 != fptr2);
}

#if 0
static void testTLG6_chroma()
{
	tjs_uint8 tmpbuff[(32 + 256) * 4 * 2 + 16];
	tjs_uint8 *block_src_ref = (tjs_uint8*)((((intptr_t)tmpbuff) + 7) & ~7);
	tjs_uint8 *block_src = block_src_ref + 32 * 4;
	tjs_uint32 *testdest1 = (tjs_uint32*)(block_src + 32 * 4);
	tjs_uint32 *testdest2 = testdest1 + 256;
	tjs_uint8 *psrc[4] = {
		block_src,
		block_src + 1,
		block_src + 3,
		block_src + 2,
	};
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32 * 4; ++j) {
			block_src_ref[j] = 240 - i - j * 3;
			block_src[j] = 16 + i + j * 3;
		}
		for (tjs_uint8 ft = 0; ft < 32; ++ft) {
			TVPGL_INIT_FUNCS_1();
			TVPTLG6DecodeLineGeneric((tjs_uint32 *)block_src_ref, testdest1, 64, 0, 1, &ft, 0, (tjs_uint32 *)block_src, 0, 0, 0);
			TVPGL_INIT_FUNCS_2();
			TVPTLG6DecodeLineGeneric((tjs_uint32 *)block_src_ref, testdest2, 64, 0, 1, &ft, 0, (tjs_uint32 *)block_src, 0, 0, 0);
			if (memcmp(testdest1, testdest2, 8 * 4) != 0) {
				fprintf(stderr, "test fail on function TVPTLG6DecodeLineGeneric\n");
				exit(0);
			}
		}

		TVPGL_INIT_FUNCS_1();
		TVPTLG5ComposeColors3To4((tjs_uint8*)testdest1, block_src_ref, psrc, 67);
		TVPGL_INIT_FUNCS_2();
		TVPTLG5ComposeColors3To4((tjs_uint8*)testdest2, block_src_ref, psrc, 67);
		if (memcmp(testdest1, testdest2, 8 * 4) != 0) {
			fprintf(stderr, "test fail on function TVPTLG5ComposeColors3To4\n");
			exit(0);
		}
		TVPGL_INIT_FUNCS_1();
		TVPTLG5ComposeColors4To4((tjs_uint8*)testdest1, block_src_ref, psrc, 67);
		TVPGL_INIT_FUNCS_2();
		TVPTLG5ComposeColors4To4((tjs_uint8*)testdest2, block_src_ref, psrc, 67);
		if (memcmp(testdest1, testdest2, 8 * 4) != 0) {
			fprintf(stderr, "test fail on function TVPTLG5ComposeColors4To4\n");
			exit(0);
		}
	}
}
#endif

#ifdef TEST_TVPGL

#define TEST_TVPGL_BLEND_FUNC_2(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, 256 * 256);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, testdata1, 256 * 256);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_BLEND_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, 256 * 256, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\\
	origf(testdest2, testdata1, 256 * 256, __VA_ARGS__);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_STRECH_FUNC_2(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 16 * 256, testdata1, 0, 1 << 16);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 16 * 256, testdata1, 0, 1 << 16);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_STRECH_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 16 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 16 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_LINTRANS_FUNC_2(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 8 * 256, testdata1, 0, 0, 1<<16, 1<<16, 64);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 8 * 256, testdata1, 0, 0, 1<<16, 1<<16, 64);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_LINTRANS_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 8 * 256, testdata1, 0, 0, 1<<16, 1<<16, 64, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 8 * 256, testdata1, 0, 0, 1<<16, 1<<16, 64, __VA_ARGS__);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_UNIVTRANS_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	CheckTestData_RGB(#origf);\
	}
#define TEST_TVPGL_CUSTOM_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, __VA_ARGS__);\
	CheckTestData(#origf);\
	}
#define TEST_TVPGL_CUSTOM_FUNC_RGB(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, __VA_ARGS__);\
	CheckTestData_RGB(#origf);\
	}
#define TEST_TVPGL_CUSTOM_FUNC_TYPE(origf, DT, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf((DT)testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf((DT)testdest2, __VA_ARGS__);\
	CheckTestData(#origf);\
	}
#elif defined(BENCHMARK_TVPGL)

static tjs_uint32 lastTick1, lastTick2;
static tjs_int tick1, tick2;

// TODO: test TLG5/TLG6 decoding
#if 0
#include "tjsCommHead.h"
#include "GraphicsLoaderIntf.h"
#include "UtilStreams.h"
#include "LayerBitmapIntf.h"
#include "LayerBitmapImpl.h"
#define TVP_clNone				((tjs_uint32)(0x1fffffff))
static void logTLG6_chroma() {
	if (!TEST_COUNT) return;
	tjs_uint8 tmpbuff[(32 + 256) * 4 * 2 + 16];
	tjs_uint8 *block_src_ref = (tjs_uint8*)((((intptr_t)tmpbuff) + 7) & ~7);
	tjs_uint8 *block_src = block_src_ref + 32 * 4;
	tjs_uint32 *testdest1 = (tjs_uint32*)(block_src + 32 * 4);
	tjs_uint32 *testdest2 = testdest1 + 256;
	tjs_uint8 *psrc[4] = {
		block_src,
		block_src + 1,
		block_src + 3,
		block_src + 2,
	};
	tick1 = 0; tick2 = 0;
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32 * 4; ++j) {
			block_src_ref[j] = 240 - i - j * 3;
			block_src[j] = 16 + i + j * 3;
		}
		TVPGL_INIT_FUNCS_1();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 4; ++n)
		for (tjs_uint8 ft = 0; ft < 32; ++ft) {
			TVPTLG6DecodeLineGeneric((tjs_uint32 *)block_src_ref, testdest2, 64, 0, 1, &ft, 0, (tjs_uint32 *)block_src, 0, 0, 0);
		}
		tick1 += TVPGetRoughTickCount32() - lastTick1;
		TVPGL_INIT_FUNCS_2();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 4; ++n)
		for (tjs_uint8 ft = 0; ft < 32; ++ft) {
			TVPTLG6DecodeLineGeneric((tjs_uint32 *)block_src_ref, testdest2, 64, 0, 1, &ft, 0, (tjs_uint32 *)block_src, 0, 0, 0);
		}
		tick2 += TVPGetRoughTickCount32() - lastTick1;
	}
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", "TVPTLG6DecodeLineGeneric", tick1, tick2, (float)tick2 / tick1 * 100);

	tick1 = 0; tick2 = 0;
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32 * 4; ++j) {
			block_src_ref[j] = 240 - i - j * 3;
			block_src[j] = 16 + i + j * 3;
		}

		TVPGL_INIT_FUNCS_1();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 16; ++n) TVPTLG5ComposeColors3To4((tjs_uint8*)testdest2, block_src_ref, psrc, 67);
		tick1 += TVPGetRoughTickCount32() - lastTick1;
		TVPGL_INIT_FUNCS_2();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 16; ++n) TVPTLG5ComposeColors3To4((tjs_uint8*)testdest1, block_src_ref, psrc, 67);
		tick2 += TVPGetRoughTickCount32() - lastTick1;
	}
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", "TVPTLG5ComposeColors3To4", tick1, tick2, (float)tick2 / tick1 * 100);

	tick1 = 0; tick2 = 0;
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32 * 4; ++j) {
			block_src_ref[j] = 240 - i - j * 3;
			block_src[j] = 16 + i + j * 3;
		}

		TVPGL_INIT_FUNCS_1();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 16; ++n) TVPTLG5ComposeColors4To4((tjs_uint8*)testdest2, block_src_ref, psrc, 67);
		tick1 += TVPGetRoughTickCount32() - lastTick1;
		TVPGL_INIT_FUNCS_2();
		lastTick1 = TVPGetRoughTickCount32();
		for (int n = 0; n < TEST_COUNT * 16; ++n) TVPTLG5ComposeColors4To4((tjs_uint8*)testdest1, block_src_ref, psrc, 67);
		tick2 += TVPGetRoughTickCount32() - lastTick1;
	}
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", "TVPTLG5ComposeColors4To4", tick1, tick2, (float)tick2 / tick1 * 100);

#if 0
	FILE *fp = fopen("lena.tlg", "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		size_t n = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		tTVPMemoryStream memio; memio.SetSize(n);
		fread(memio.GetInternalBuffer(), 1, n, fp);
		fclose(fp);
		static tTVPBitmap *testbmp = nullptr;
		TVPTLG5ComposeColors3To4 = DEST_FUNC_APPENDIX(TVPTLG5ComposeColors3To4);
		TVPTLG5ComposeColors4To4 = DEST_FUNC_APPENDIX(TVPTLG5ComposeColors4To4);
		TVPTLG5DecompressSlide = TVPTLG5DecompressSlide_c;
		lastTick1 = TVPGetRoughTickCount32();
		for (int i = 0; i < 32; ++i) {
			memio.SetPosition(0);
			TVPLoadTLG(nullptr, nullptr, [](void *callbackdata, tjs_uint w, tjs_uint h, tTVPGraphicPixelFormat fmt)->int {
				if (!testbmp) testbmp = new tTVPBitmap(w, h, 32);
				return testbmp->GetPitch();
			}, [](void *callbackdata, tjs_int y)->void* {
				return testbmp->GetScanLine(y);
			}, [](void *callbackdata, const ttstr & name, const ttstr & value){}, &memio, TVP_clNone, glmNormal);
		}
		tick1 = TVPGetRoughTickCount32() - lastTick1;
		delete testbmp; testbmp = nullptr;
		TVPTLG5DecompressSlide = DEST_FUNC_APPENDIX(TVPTLG5DecompressSlide);
		lastTick1 = TVPGetRoughTickCount32();
		for (int i = 0; i < 32; ++i) {
			memio.SetPosition(0);
			TVPLoadTLG(nullptr, nullptr, [](void *callbackdata, tjs_uint w, tjs_uint h, tTVPGraphicPixelFormat fmt)->int {
				if (!testbmp) testbmp = new tTVPBitmap(w, h, 32);
				return testbmp->GetPitch();
			}, [](void *callbackdata, tjs_int y)->void* {
				return testbmp->GetScanLine(y);
			}, [](void *callbackdata, const ttstr & name, const ttstr & value){}, &memio, TVP_clNone, glmNormal);
		}
		tick2 = TVPGetRoughTickCount32() - lastTick1;
		delete testbmp; testbmp = nullptr;
		fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", "TVPLoadTLG5", tick1, tick2, (float)tick2 / tick1 * 100);
	}
#endif
}
#endif

#define TEST_TVPGL_BLEND_FUNC_2(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, 256 * 256);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, testdata1, 256 * 256);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest1, testdata1, 256 * 256); \
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest2, testdata1, 256 * 256); \
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}

#define TEST_TVPGL_BLEND_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, 256 * 256, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, testdata1, 256 * 256, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest1, testdata1, 256 * 256, __VA_ARGS__); \
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest2, testdata1, 256 * 256, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}

#define TEST_TVPGL_STRECH_FUNC_2(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 127 * 256, testdata1, 0, 1 << 16);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 127 * 256, testdata1, 0, 1 << 16);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest1, 127 * 256, testdata1, 0, 1 << 16); \
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest2, 127 * 256, testdata1, 0, 1 << 16);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_STRECH_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 127 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 127 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest1, 127 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for (int i = 0; i < TEST_COUNT; ++i) origf(testdest2, 127 * 256, testdata1, 0, 1 << 16, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_STRECH_FUNC_0(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 127 * 256, testdata1, 0, 1 << 16);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 127 * 256, testdata1, 0, 1 << 16);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, 127 * 256, testdata1, 0, 1 << 16);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, 127 * 256, testdata1, 0, 1 << 16);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_LINTRANS_FUNC_2(origf) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 127 * 256, testdata1, 0, 0, 1 << 16, 0, 256); \
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 127 * 256, testdata1, 0, 0, 1 << 16, 0, 256); \
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, 127 * 256, testdata1, 0, 0, 1 << 16, 0, 256);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, 127 * 256, testdata1, 0, 0, 1 << 16, 0, 256);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_LINTRANS_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, 127 * 256, testdata1, 0, 0, 1<<16, 0, 256, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, 127 * 256, testdata1, 0, 0, 1<<16, 0, 256, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, 127 * 256, testdata1, 0, 0, 1<<16, 0, 256, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, 127 * 256, testdata1, 0, 0, 1<<16, 0, 256, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_UNIVTRANS_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, testdata1, testdata2, testrule, testtable, 256 * 256, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_CUSTOM_FUNC(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_CUSTOM_FUNC_RGB(origf, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf(testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf(testdest2, __VA_ARGS__);\
	CheckTestData_RGB(#origf); if (TEST_COUNT) {\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32();\
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest1, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for(int i = 0; i < TEST_COUNT; ++i) origf(testdest2, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#define TEST_TVPGL_CUSTOM_FUNC_TYPE(origf, DT, ...) \
	if(IsDifferentFunction((void **)&origf)){\
	InitTestData();\
	TVPGL_INIT_FUNCS_1();\
	origf((DT)testdest1, __VA_ARGS__);\
	TVPGL_INIT_FUNCS_2();\
	origf((DT)testdest2, __VA_ARGS__);\
	CheckTestData(#origf); if (TEST_COUNT) {\
	InitTestData(); \
	TVPGL_INIT_FUNCS_1();\
	lastTick1 = TVPGetRoughTickCount32(); \
	for (int i = 0; i < TEST_COUNT; ++i) origf((DT)testdest1, __VA_ARGS__);\
	tick1 = TVPGetRoughTickCount32() - lastTick1;\
	TVPGL_INIT_FUNCS_2();\
	lastTick2 = TVPGetRoughTickCount32(); \
	for (int i = 0; i < TEST_COUNT; ++i) origf((DT)testdest2, __VA_ARGS__);\
	tick2 = TVPGetRoughTickCount32() - lastTick2; \
	fprintf(stderr, "%s: %d ms, Optimized: %d ms(%g%%)\n", #origf, tick1, tick2, (float)tick2 / tick1 * 100); \
	}\
	}
#else
#define TEST_TVPGL_BLEND_FUNC_2(origf, ...) origf = f;
#define TEST_TVPGL_BLEND_FUNC(origf, ...) origf = f;
#define TEST_TVPGL_STRECH_FUNC_2(origf, ...) origf = f;
#define TEST_TVPGL_STRECH_FUNC(origf, ...) origf = f;
#define TEST_TVPGL_LINTRANS_FUNC_2(origf, ...) origf = f;
#define TEST_TVPGL_LINTRANS_FUNC(origf, ...) origf = f;
#define TEST_TVPGL_UNIVTRANS_FUNC(origf, ...) origf = f;
#define TEST_TVPGL_CUSTOM_FUNC(origf, ...) origf = f;
#define TEST_TVPGL_CUSTOM_FUNC_RGB(origf, ...) origf = f;
#define TEST_TVPGL_CUSTOM_FUNC_TYPE(origf, ...) origf = f;
#endif

int wmain(int argc, wchar_t** argv)
{
	tjs_uint32 features = 0;
	TVPCheckCPU();
	features = (TVPCPUFeatures & TVP_CPU_FEATURE_MASK);
	TVPCPUType = TVPCPUFeatures;
	TVPCPUType &= ~ TVP_CPU_FEATURE_MASK;
	TVPCPUType |= features;
	TVP_GL_FUNCNAME(TVPInitTVPGL)();
	TVPGL_C_Init();
	TVPGL_IA32_Init();
	TVPGL_IA32_pfraction_Init();
	TVPGL_SSE2_Init();

	{
#if 0
		testTLG6_chroma();
		logTLG6_chroma();
#endif

		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAlphaBlend, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_HDA, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAlphaBlend_o, testdata1, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_HDA_o, testdata1, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_d, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_a, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_do, testdata1, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaBlend_ao, testdata1, 256 * 256, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPAlphaColorMat, 0x98765432, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAdditiveAlphaBlend, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPAdditiveAlphaBlend_HDA, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAdditiveAlphaBlend_o, testdata1, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPAdditiveAlphaBlend_HDA_o, testdata1, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAdditiveAlphaBlend_a, testdata1, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPAdditiveAlphaBlend_ao, testdata1, 256 * 256, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPConvertAdditiveAlphaToAlpha, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPConvertAlphaToAdditiveAlpha, 256 * 256);

		TEST_TVPGL_CUSTOM_FUNC(TVPStretchAlphaBlend, 16 * 256, testdata1, 0, 1 << 16);
		TEST_TVPGL_CUSTOM_FUNC(TVPStretchAlphaBlend_o, 16 * 256, testdata1, 0, 1 << 16, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPStretchAlphaBlend_HDA, 16 * 256, testdata1, 0, 1 << 16);
		TEST_TVPGL_CUSTOM_FUNC(TVPStretchAlphaBlend_HDA_o, 16 * 256, testdata1, 0, 1 << 16, 100);
		TEST_TVPGL_STRECH_FUNC_2(TVPStretchAlphaBlend_d);
		TEST_TVPGL_STRECH_FUNC_2(TVPStretchAlphaBlend_a);
		TEST_TVPGL_STRECH_FUNC(TVPStretchAlphaBlend_do, 100);
		TEST_TVPGL_STRECH_FUNC(TVPStretchAlphaBlend_ao, 100);

		TEST_TVPGL_STRECH_FUNC_2(TVPStretchAdditiveAlphaBlend);
		TEST_TVPGL_STRECH_FUNC(TVPStretchAdditiveAlphaBlend_o, 100);
		TEST_TVPGL_STRECH_FUNC_2(TVPStretchAdditiveAlphaBlend_HDA);
		TEST_TVPGL_STRECH_FUNC(TVPStretchAdditiveAlphaBlend_HDA_o, 100);
		TEST_TVPGL_STRECH_FUNC_2(TVPStretchAdditiveAlphaBlend_a);
		TEST_TVPGL_STRECH_FUNC(TVPStretchAdditiveAlphaBlend_ao, 100);

		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAlphaBlend);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAlphaBlend_o, 100);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAlphaBlend_HDA);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAlphaBlend_HDA_o, 100);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAlphaBlend_d);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAlphaBlend_a);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAlphaBlend_do, 100);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAlphaBlend_ao, 100);

		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAdditiveAlphaBlend);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAdditiveAlphaBlend_o, 100);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAdditiveAlphaBlend_HDA);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAdditiveAlphaBlend_HDA_o, 100);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransAdditiveAlphaBlend_a);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransAdditiveAlphaBlend_ao, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPInterpStretchCopy, 127 * 256, testdata1, testdata2, 127, 0, 1 << 16);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPInterpLinTransCopy);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPInterpStretchAdditiveAlphaBlend, 16 * 256, testdata1, testdata2, 127, 0, 1 << 16);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPInterpStretchAdditiveAlphaBlend_o, 16 * 256, testdata1, testdata2, 127, 0, 1 << 16, 100);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPInterpLinTransAdditiveAlphaBlend, 8 * 256, testdata1, 0, 0, 1 << 16, 1 << 16, 64);
		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPInterpLinTransAdditiveAlphaBlend_o, 8 * 256, testdata1, 0, 0, 1 << 16, 1 << 16, 64, 100);

		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPInterpStretchConstAlphaBlend, 16 * 256, testdata1, testdata2, 127, 0, 1 << 16, 100);
		TEST_TVPGL_LINTRANS_FUNC(TVPInterpLinTransConstAlphaBlend, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPCopyOpaqueImage);
		TEST_TVPGL_CUSTOM_FUNC(TVPStretchCopy, 127 * 256, testdata1, 0, 1 << 16);
		TEST_TVPGL_CUSTOM_FUNC(TVPStretchCopyOpaqueImage, 127 * 256, testdata1, 0, 1 << 16);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransCopy);
		TEST_TVPGL_LINTRANS_FUNC_2(TVPLinTransCopyOpaqueImage);
		TEST_TVPGL_BLEND_FUNC(TVPConstAlphaBlend, 100);
		TEST_TVPGL_BLEND_FUNC(TVPConstAlphaBlend_HDA, 100);
		TEST_TVPGL_BLEND_FUNC(TVPConstAlphaBlend_d, 100);
		TEST_TVPGL_BLEND_FUNC(TVPConstAlphaBlend_a, 100);

		TEST_TVPGL_STRECH_FUNC(TVPStretchConstAlphaBlend, 100);
		TEST_TVPGL_STRECH_FUNC(TVPStretchConstAlphaBlend_HDA, 100);
		TEST_TVPGL_STRECH_FUNC(TVPStretchConstAlphaBlend_d, 100);
		TEST_TVPGL_STRECH_FUNC(TVPStretchConstAlphaBlend_a, 100);

		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransConstAlphaBlend, 100);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransConstAlphaBlend_HDA, 100);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransConstAlphaBlend_d, 100);
		TEST_TVPGL_LINTRANS_FUNC(TVPLinTransConstAlphaBlend_a, 100);

		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPConstAlphaBlend_SD, testdata1, testdata2, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPConstAlphaBlend_SD_a, testdata1, testdata2, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPConstAlphaBlend_SD_d, testdata1, testdata2, 256 * 256, 100);

		TEST_TVPGL_CUSTOM_FUNC_RGB(TVPUnivTransBlend, testdata1, testdata2, testrule, testtable, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPUnivTransBlend_d, testdata1, testdata2, testrule, testtable, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPUnivTransBlend_a, testdata1, testdata2, testrule, testtable, 256 * 256);
		TEST_TVPGL_UNIVTRANS_FUNC(TVPUnivTransBlend_switch, 240, 32);
		TEST_TVPGL_UNIVTRANS_FUNC(TVPUnivTransBlend_switch_d, 240, 32);
		TEST_TVPGL_UNIVTRANS_FUNC(TVPUnivTransBlend_switch_a, 240, 32);

		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap, testrule, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_HDA, testrule, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_d, testrule, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_a, testrule, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_o, testrule, 256 * 256, 0x55d20688, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_HDA_o, testrule, 256 * 256, 0x55d20688, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_do, testrule, 256 * 256, 0x55d20688, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPApplyColorMap_ao, testrule, 256 * 256, 0x55d20688, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPConstColorAlphaBlend, 256 * 256, 0x55d20688, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPConstColorAlphaBlend_d, 256 * 256, 0x55d20688, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPConstColorAlphaBlend_a, 256 * 256, 0x55d20688, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPRemoveConstOpacity, 256 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPRemoveOpacity, testrule, 255 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPRemoveOpacity_o, testrule, 255 * 256, 100);
		TEST_TVPGL_CUSTOM_FUNC(TVPRemoveOpacity65, testrule, 255 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPRemoveOpacity65_o, testrule, 255 * 256, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPAddBlend);
		TEST_TVPGL_BLEND_FUNC_2(TVPAddBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPAddBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC(TVPAddBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPSubBlend);
		TEST_TVPGL_BLEND_FUNC_2(TVPSubBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPSubBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC(TVPSubBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPMulBlend_HDA);
		TEST_TVPGL_BLEND_FUNC_2(TVPMulBlend);
		TEST_TVPGL_BLEND_FUNC(TVPMulBlend_HDA_o, 100);
		TEST_TVPGL_BLEND_FUNC(TVPMulBlend_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPColorDodgeBlend);
		TEST_TVPGL_BLEND_FUNC(TVPColorDodgeBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPColorDodgeBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPColorDodgeBlend_HDA_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPDarkenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPDarkenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPDarkenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPDarkenBlend_HDA_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPLightenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPLightenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPLightenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPLightenBlend_HDA_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPScreenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPScreenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPScreenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPScreenBlend_HDA_o, 100);

		TEST_TVPGL_CUSTOM_FUNC(TVPFastLinearInterpV2, 256 * 256, testdata1, testdata2);

		TEST_TVPGL_BLEND_FUNC_2(TVPCopyMask);
		TEST_TVPGL_BLEND_FUNC_2(TVPCopyColor);
		TEST_TVPGL_CUSTOM_FUNC(TVPBindMaskToMain, testrule, 256 * 256);

		TEST_TVPGL_CUSTOM_FUNC(TVPFillARGB, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPFillARGB_NC, 256 * 256, 0x55d20688);

		TEST_TVPGL_CUSTOM_FUNC(TVPFillColor, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC(TVPFillMask, 256 * 256, 0x55d20688);
		TEST_TVPGL_CUSTOM_FUNC_TYPE(TVPAddSubVertSum16, tjs_uint16*, testdata1, testdata2, 128 * 256);
		TEST_TVPGL_CUSTOM_FUNC_TYPE(TVPAddSubVertSum16_d, tjs_uint16*, testdata1, testdata2, 128 * 256);
		TEST_TVPGL_CUSTOM_FUNC_TYPE(TVPAddSubVertSum32, tjs_uint32*, testdata1, testdata2, 64 * 256);
		TEST_TVPGL_CUSTOM_FUNC_TYPE(TVPAddSubVertSum32_d, tjs_uint32*, testdata1, testdata2, 64 * 256);

		TEST_TVPGL_CUSTOM_FUNC(TVPDoGrayScale, 256 * 256);

		TEST_TVPGL_CUSTOM_FUNC(TVPExpand8BitTo32BitGray, testrule, 256 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPBLConvert15BitTo32Bit, (const tjs_uint16*)testrule, 128 * 256);
		TEST_TVPGL_CUSTOM_FUNC(TVPConvert24BitTo32Bit, testrule, 256 * 256 / 3);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsAlphaBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsAlphaBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsAlphaBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsAlphaBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsAddBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsAddBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsAddBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsAddBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsSubBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsSubBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsSubBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsSubBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsMulBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsMulBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsMulBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsMulBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsScreenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsScreenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsScreenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsScreenBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsOverlayBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsOverlayBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsOverlayBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsOverlayBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsHardLightBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsHardLightBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsHardLightBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsHardLightBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsSoftLightBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsSoftLightBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsSoftLightBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsSoftLightBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorDodgeBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorDodgeBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorDodgeBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorDodgeBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorDodge5Blend);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorDodge5Blend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorDodge5Blend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorDodge5Blend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorBurnBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorBurnBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsColorBurnBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsColorBurnBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsLightenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsLightenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsLightenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsLightenBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsDarkenBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsDarkenBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsDarkenBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsDarkenBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsDiffBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsDiffBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsDiffBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsDiffBlend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsDiff5Blend);
		TEST_TVPGL_BLEND_FUNC(TVPPsDiff5Blend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsDiff5Blend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsDiff5Blend_HDA_o, 100);

		TEST_TVPGL_BLEND_FUNC_2(TVPPsExclusionBlend);
		TEST_TVPGL_BLEND_FUNC(TVPPsExclusionBlend_o, 100);
		TEST_TVPGL_BLEND_FUNC_2(TVPPsExclusionBlend_HDA);
		TEST_TVPGL_BLEND_FUNC(TVPPsExclusionBlend_HDA_o, 100);

		// TODO: test the following
		// TVPAdjustGamma
		// TVPAdjustGamma_a
		// TVPApplyColorMap65
		// TVPApplyColorMap65_HDA
		// TVPApplyColorMap65_HDA_o
		// TVPApplyColorMap65_a
		// TVPApplyColorMap65_ao
		// TVPApplyColorMap65_d
		// TVPApplyColorMap65_do
		// TVPApplyColorMap65_o
		// TVPBLConvert15BitTo8Bit
		// TVPBLConvert24BitTo32Bit
		// TVPBLConvert24BitTo8Bit
		// TVPBLConvert32BitTo32Bit_AddAlpha
		// TVPBLConvert32BitTo32Bit_MulAddAlpha
		// TVPBLConvert32BitTo32Bit_NoneAlpha
		// TVPBLConvert32BitTo8Bit
		// TVPBLExpand1BitTo32BitPal
		// TVPBLExpand1BitTo8Bit
		// TVPBLExpand1BitTo8BitPal
		// TVPBLExpand4BitTo32BitPal
		// TVPBLExpand4BitTo8Bit
		// TVPBLExpand4BitTo8BitPal
		// TVPBLExpand8BitTo32BitPal
		// TVPBLExpand8BitTo8BitPal
		// TVPChBlurAddMulCopy
		// TVPChBlurAddMulCopy65
		// TVPChBlurCopy
		// TVPChBlurCopy65
		// TVPChBlurMulCopy
		// TVPChBlurMulCopy65
		// TVPDither32BitTo16Bit555
		// TVPDither32BitTo16Bit565
		// TVPDither32BitTo8Bit
		// TVPDoBoxBlurAvg16
		// TVPDoBoxBlurAvg16_d
		// TVPDoBoxBlurAvg32
		// TVPDoBoxBlurAvg32_d
		// TVPFastLinearInterpH2B
		// TVPFastLinearInterpH2F
		// TVPInitGammaAdjustTempData
		// TVPInitUnivTransBlendTable
		// TVPInitUnivTransBlendTable_a
		// TVPInitUnivTransBlendTable_d
		// TVPLinTransColorCopy
		// TVPMakeAlphaFromKey
		// TVPRedBlueSwap
		// TVPRedBlueSwapCopy
		// TVPReverse32
		// TVPReverse8
		// TVPStretchColorCopy
		// TVPSwapLine32
		// TVPSwapLine8
		// TVPTLG5ComposeColors3To4
		// TVPTLG5ComposeColors4To4
		// TVPTLG5DecompressSlide
		// TVPTLG6DecodeGolombValues
		// TVPTLG6DecodeGolombValuesForFirst
		// TVPTLG6DecodeLine
		// TVPTLG6DecodeLineGeneric
		// TVPUninitGammaAdjustTempData
	}
	if(testtable)
	{
		free(testtable);
		testtable = NULL;
	}
	if(testrule)
	{
		free(testrule);
		testrule = NULL;
	}
	if(testbuff)
	{
		free(testbuff);
		testbuff = NULL;
	}
	if(testdest1)
	{
		free(testdest1);
		testdest1 = NULL;
	}
	if(testdest2)
	{
		free(testdest2);
		testdest2 = NULL;
	}
	if(testdata1)
	{
		free(testdata1);
		testdata1 = NULL;
	}
	if(testdata2)
	{
		free(testdata2);
		testdata2 = NULL;
	}
	return 0;
}
