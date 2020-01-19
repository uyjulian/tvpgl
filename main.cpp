/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2019-2019 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <windows.h>
#include "tjsCommHead.h"
#include "tvpgl.h"
#include "tvpgl_info.h"
#if 0
#include "tvpgl_ia32_intf.h"
#include "detect_cpu.h"
#endif

//---------------------------------------------------------------------------
// iTVPFunctionExporter, exporting main module's functions
//---------------------------------------------------------------------------
struct iTVPFunctionExporter
{
	virtual bool TJS_INTF_METHOD QueryFunctions(const tjs_char **name, void **function,
		tjs_uint count) = 0;
	virtual bool TJS_INTF_METHOD QueryFunctionsByNarrowString(const char **name,
		void **function, tjs_uint count) = 0;
};
//---------------------------------------------------------------------------

static iTVPFunctionExporter * TVPFunctionExporter = NULL;

#define TVPGL_DEFINE(funcname, signature, rettype, arg, argnames) \
	rettype (__cdecl * funcname##_old) arg; \
	uintptr_t *funcname##_oldloc; \
	rettype __cdecl funcname##_thunk arg \
	{ \
		return funcname argnames ; \
	}

#define TVPGL_OVERWRITE(funcname, signature, rettype, arg, argnames) \
	{ \
		funcname##_oldloc = nullptr; \
		const char *name = signature; \
		void *funcptr; \
		if (TVPFunctionExporter->QueryFunctionsByNarrowString(&name, &funcptr, 1)) { \
			uint8_t *funcptrindex = reinterpret_cast<uint8_t*>(funcptr); \
			for (int i = 0; i < 128; i += 1) { \
				if (funcptrindex[i] == 0xFF) { \
					if (funcptrindex[i + 1] == 0x15) { \
						uintptr_t *callloc = reinterpret_cast<uintptr_t*>(funcptrindex + (i + 2)); \
						uintptr_t *ptrloc = reinterpret_cast<uintptr_t*>(*callloc); \
						funcname##_oldloc = ptrloc; \
						funcname##_old = reinterpret_cast<rettype (__cdecl *) arg>(*ptrloc); \
						*ptrloc = reinterpret_cast<uintptr_t>(funcname##_thunk); \
						break; \
					} \
				} \
				if (funcptrindex[i] == 0xC2) { \
					break; \
				} \
			} \
		} \
	}

#define TVPGL_RESTORE(funcname, signature, rettype, arg, argnames) \
	{ \
		if (funcname##_oldloc) { \
			*funcname##_oldloc = reinterpret_cast<uintptr_t>(funcname##_old); \
			funcname##_oldloc = nullptr; \
		} \
	}

TVPGL_EXPAND_MACRO(TVPGL_DEFINE);

#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall

#if 0
extern "C" tjs_uint32 TVPCPUType;
extern void TVPGL_C_Init();
extern void TVPGL_SSE2_Init();
#endif

EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter) {
	TVPFunctionExporter = exporter;
#if 0
	tjs_uint32 features = 0;
	TVPCheckCPU();
	features = (TVPCPUFeatures & TVP_CPU_FEATURE_MASK);
	TVPCPUType = TVPCPUFeatures;
	TVPCPUType &= ~ TVP_CPU_FEATURE_MASK;
	TVPCPUType |= features;
#endif
	TVP_GL_FUNCNAME(TVPInitTVPGL)();
#if 0
	TVPGL_C_Init();
	TVPGL_IA32_Init();
	TVPGL_SSE2_Init();
#endif
	TVPGL_EXPAND_MACRO(TVPGL_OVERWRITE);
	return S_OK;
}

EXPORT(HRESULT) V2Unlink() {
	TVPGL_EXPAND_MACRO(TVPGL_RESTORE);
	TVP_GL_FUNCNAME(TVPUninitTVPGL)();
	return S_OK;
}
