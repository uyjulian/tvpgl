/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2019-2019 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include <windows.h>
#include <string>
#include <stdio.h>
#include "tjsTypes.h"
#include "tvpgl.h"
#include "tvpgl_ia32_intf.h"
#if 0
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

#define TVPGL_DEFINE(funcname, signature, rettype, arg) \
	rettype (__cdecl * funcname##_old) arg; \
	uintptr_t *funcname##_oldloc;

#define TVPGL_OVERWRITE(funcname, signature, rettype, arg) \
	{ \
		funcname##_oldloc = nullptr; \
		const char *name = signature; \
		void *funcptr; \
		if (TVPFunctionExporter->QueryFunctionsByNarrowString(&name, &funcptr, 1)) { \
			uint8_t *funcptrindex = reinterpret_cast<uint8_t*>(funcptr); \
			for (int i = 0; i < 64; i += 1) { \
				if (funcptrindex[i] == 0xFF) { \
					if (funcptrindex[i + 1] == 0x15) { \
						uintptr_t *callloc = reinterpret_cast<uintptr_t*>(funcptrindex + (i + 2)); \
						uintptr_t *ptrloc = reinterpret_cast<uintptr_t*>(*callloc); \
						funcname##_oldloc = ptrloc; \
						funcname##_old = reinterpret_cast<rettype (__cdecl *) arg>(*ptrloc); \
						*ptrloc = reinterpret_cast<uintptr_t>(funcname); \
						break; \
					} \
				} \
			} \
		} \
	}

#define TVPGL_RESTORE(funcname, signature, rettype, arg) \
	{ \
		if (funcname##_oldloc) { \
			*funcname##_oldloc = reinterpret_cast<uintptr_t>(funcname##_old); \
			funcname##_oldloc = nullptr; \
		} \
	}

#define TVPGL_EXPAND_MACRO(x) \
	x(TVPAlphaBlend, "void ::TVPAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAlphaBlend_HDA, "void ::TVPAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAlphaBlend_o, "void ::TVPAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAlphaBlend_HDA_o, "void ::TVPAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAlphaBlend_d, "void ::TVPAlphaBlend_d(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAlphaBlend_a, "void ::TVPAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAlphaBlend_do, "void ::TVPAlphaBlend_do(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAlphaBlend_ao, "void ::TVPAlphaBlend_ao(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAlphaColorMat, "void ::TVPAlphaColorMat(tjs_uint32 *,const tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 color, tjs_int len)) \
	x(TVPAdditiveAlphaBlend, "void ::TVPAdditiveAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAdditiveAlphaBlend_HDA, "void ::TVPAdditiveAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAdditiveAlphaBlend_o, "void ::TVPAdditiveAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAdditiveAlphaBlend_HDA_o, "void ::TVPAdditiveAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAdditiveAlphaBlend_a, "void ::TVPAdditiveAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAdditiveAlphaBlend_ao, "void ::TVPAdditiveAlphaBlend_ao(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPConvertAdditiveAlphaToAlpha, "void ::TVPConvertAdditiveAlphaToAlpha(tjs_uint32 *,tjs_int)", void, (tjs_uint32 *buf, tjs_int len)) \
	x(TVPConvertAlphaToAdditiveAlpha, "void ::TVPConvertAlphaToAdditiveAlpha(tjs_uint32 *,tjs_int)", void, (tjs_uint32 *buf, tjs_int len)) \
	x(TVPStretchAlphaBlend, "void ::TVPStretchAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAlphaBlend_HDA, "void ::TVPStretchAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAlphaBlend_o, "void ::TVPStretchAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAlphaBlend_HDA_o, "void ::TVPStretchAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAlphaBlend_d, "void ::TVPStretchAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAlphaBlend_a, "void ::TVPStretchAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAlphaBlend_do, "void ::TVPStretchAlphaBlend_do(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAlphaBlend_ao, "void ::TVPStretchAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAdditiveAlphaBlend, "void ::TVPStretchAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPInterpStretchAdditiveAlphaBlend, "void ::TVPInterpStretchAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAdditiveAlphaBlend_HDA, "void ::TVPStretchAdditiveAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAdditiveAlphaBlend_o, "void ::TVPStretchAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPInterpStretchAdditiveAlphaBlend_o, "void ::TVPInterpStretchAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAdditiveAlphaBlend_HDA_o, "void ::TVPStretchAdditiveAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchAdditiveAlphaBlend_a, "void ::TVPStretchAdditiveAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchAdditiveAlphaBlend_ao, "void ::TVPStretchAdditiveAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPLinTransAlphaBlend, "void ::TVPLinTransAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAlphaBlend_HDA, "void ::TVPLinTransAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAlphaBlend_o, "void ::TVPLinTransAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAlphaBlend_HDA_o, "void ::TVPLinTransAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAlphaBlend_d, "void ::TVPLinTransAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAlphaBlend_a, "void ::TVPLinTransAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAlphaBlend_do, "void ::TVPLinTransAlphaBlend_do(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAlphaBlend_ao, "void ::TVPLinTransAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAdditiveAlphaBlend, "void ::TVPLinTransAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPInterpLinTransAdditiveAlphaBlend, "void ::TVPInterpLinTransAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAdditiveAlphaBlend_HDA, "void ::TVPLinTransAdditiveAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAdditiveAlphaBlend_o, "void ::TVPLinTransAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPInterpLinTransAdditiveAlphaBlend_o, "void ::TVPInterpLinTransAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAdditiveAlphaBlend_HDA_o, "void ::TVPLinTransAdditiveAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransAdditiveAlphaBlend_a, "void ::TVPLinTransAdditiveAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransAdditiveAlphaBlend_ao, "void ::TVPLinTransAdditiveAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPCopyOpaqueImage, "void ::TVPCopyOpaqueImage(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPConstAlphaBlend, "void ::TVPConstAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPConstAlphaBlend_HDA, "void ::TVPConstAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPConstAlphaBlend_d, "void ::TVPConstAlphaBlend_d(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPConstAlphaBlend_a, "void ::TVPConstAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPStretchCopyOpaqueImage, "void ::TVPStretchCopyOpaqueImage(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPStretchConstAlphaBlend, "void ::TVPStretchConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPInterpStretchConstAlphaBlend, "void ::TVPInterpStretchConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchConstAlphaBlend_HDA, "void ::TVPStretchConstAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchConstAlphaBlend_d, "void ::TVPStretchConstAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPStretchConstAlphaBlend_a, "void ::TVPStretchConstAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)) \
	x(TVPLinTransCopyOpaqueImage, "void ::TVPLinTransCopyOpaqueImage(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransConstAlphaBlend, "void ::TVPLinTransConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPInterpLinTransConstAlphaBlend, "void ::TVPInterpLinTransConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransConstAlphaBlend_HDA, "void ::TVPLinTransConstAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransConstAlphaBlend_d, "void ::TVPLinTransConstAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPLinTransConstAlphaBlend_a, "void ::TVPLinTransConstAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)) \
	x(TVPConstAlphaBlend_SD, "void ::TVPConstAlphaBlend_SD(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)) \
	x(TVPConstAlphaBlend_SD_a, "void ::TVPConstAlphaBlend_SD_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)) \
	x(TVPConstAlphaBlend_SD_d, "void ::TVPConstAlphaBlend_SD_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)) \
	x(TVPInitUnivTransBlendTable, "void ::TVPInitUnivTransBlendTable(tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *table, tjs_int phase, tjs_int vague)) \
	x(TVPInitUnivTransBlendTable_d, "void ::TVPInitUnivTransBlendTable_d(tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *table, tjs_int phase, tjs_int vague)) \
	x(TVPInitUnivTransBlendTable_a, "void ::TVPInitUnivTransBlendTable_a(tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *table, tjs_int phase, tjs_int vague)) \
	x(TVPUnivTransBlend, "void ::TVPUnivTransBlend(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len)) \
	x(TVPUnivTransBlend_switch, "void ::TVPUnivTransBlend_switch(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv)) \
	x(TVPUnivTransBlend_d, "void ::TVPUnivTransBlend_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len)) \
	x(TVPUnivTransBlend_switch_d, "void ::TVPUnivTransBlend_switch_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv)) \
	x(TVPUnivTransBlend_a, "void ::TVPUnivTransBlend_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len)) \
	x(TVPUnivTransBlend_switch_a, "void ::TVPUnivTransBlend_switch_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv)) \
	x(TVPApplyColorMap, "void ::TVPApplyColorMap(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap_o, "void ::TVPApplyColorMap_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap65, "void ::TVPApplyColorMap65(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap65_o, "void ::TVPApplyColorMap65_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap_HDA, "void ::TVPApplyColorMap_HDA(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap_HDA_o, "void ::TVPApplyColorMap_HDA_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap65_HDA, "void ::TVPApplyColorMap65_HDA(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap65_HDA_o, "void ::TVPApplyColorMap65_HDA_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap_d, "void ::TVPApplyColorMap_d(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap65_d, "void ::TVPApplyColorMap65_d(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap_a, "void ::TVPApplyColorMap_a(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap65_a, "void ::TVPApplyColorMap65_a(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)) \
	x(TVPApplyColorMap_do, "void ::TVPApplyColorMap_do(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap65_do, "void ::TVPApplyColorMap65_do(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap_ao, "void ::TVPApplyColorMap_ao(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPApplyColorMap65_ao, "void ::TVPApplyColorMap65_ao(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPConstColorAlphaBlend, "void ::TVPConstColorAlphaBlend(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPConstColorAlphaBlend_d, "void ::TVPConstColorAlphaBlend_d(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPConstColorAlphaBlend_a, "void ::TVPConstColorAlphaBlend_a(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)) \
	x(TVPRemoveConstOpacity, "void ::TVPRemoveConstOpacity(tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int len, tjs_int strength)) \
	x(TVPRemoveOpacity, "void ::TVPRemoveOpacity(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len)) \
	x(TVPRemoveOpacity_o, "void ::TVPRemoveOpacity_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength)) \
	x(TVPRemoveOpacity65, "void ::TVPRemoveOpacity65(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len)) \
	x(TVPRemoveOpacity65_o, "void ::TVPRemoveOpacity65_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength)) \
	x(TVPAddBlend, "void ::TVPAddBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAddBlend_HDA, "void ::TVPAddBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPAddBlend_o, "void ::TVPAddBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPAddBlend_HDA_o, "void ::TVPAddBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPSubBlend, "void ::TVPSubBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPSubBlend_HDA, "void ::TVPSubBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPSubBlend_o, "void ::TVPSubBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPSubBlend_HDA_o, "void ::TVPSubBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPMulBlend, "void ::TVPMulBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPMulBlend_HDA, "void ::TVPMulBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPMulBlend_o, "void ::TVPMulBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPMulBlend_HDA_o, "void ::TVPMulBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPColorDodgeBlend, "void ::TVPColorDodgeBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPColorDodgeBlend_HDA, "void ::TVPColorDodgeBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPColorDodgeBlend_o, "void ::TVPColorDodgeBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPColorDodgeBlend_HDA_o, "void ::TVPColorDodgeBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPDarkenBlend, "void ::TVPDarkenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPDarkenBlend_HDA, "void ::TVPDarkenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPDarkenBlend_o, "void ::TVPDarkenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPDarkenBlend_HDA_o, "void ::TVPDarkenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPLightenBlend, "void ::TVPLightenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPLightenBlend_HDA, "void ::TVPLightenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPLightenBlend_o, "void ::TVPLightenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPLightenBlend_HDA_o, "void ::TVPLightenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPScreenBlend, "void ::TVPScreenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPScreenBlend_HDA, "void ::TVPScreenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPScreenBlend_o, "void ::TVPScreenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPScreenBlend_HDA_o, "void ::TVPScreenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPStretchCopy, "void ::TVPStretchCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPInterpStretchCopy, "void ::TVPInterpStretchCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPFastLinearInterpH2F, "void ::TVPFastLinearInterpH2F(tjs_uint32 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)) \
	x(TVPFastLinearInterpH2B, "void ::TVPFastLinearInterpH2B(tjs_uint32 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)) \
	x(TVPFastLinearInterpV2, "void ::TVPFastLinearInterpV2(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src0, const tjs_uint32 *src1)) \
	x(TVPStretchColorCopy, "void ::TVPStretchColorCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)) \
	x(TVPLinTransCopy, "void ::TVPLinTransCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPInterpLinTransCopy, "void ::TVPInterpLinTransCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPLinTransColorCopy, "void ::TVPLinTransColorCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)) \
	x(TVPMakeAlphaFromKey, "void ::TVPMakeAlphaFromKey(tjs_uint32 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 key)) \
	x(TVPCopyMask, "void ::TVPCopyMask(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPCopyColor, "void ::TVPCopyColor(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPBindMaskToMain, "void ::TVPBindMaskToMain(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *main, const tjs_uint8 *mask, tjs_int len)) \
	x(TVPFillARGB, "void ::TVPFillARGB(tjs_uint32 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value)) \
	x(TVPFillARGB_NC, "void ::TVPFillARGB_NC(tjs_uint32 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value)) \
	x(TVPFillColor, "void ::TVPFillColor(tjs_uint32 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color)) \
	x(TVPFillMask, "void ::TVPFillMask(tjs_uint32 *,tjs_int,tjs_uint32)", void, (tjs_uint32 *dest, tjs_int len, tjs_uint32 mask)) \
	x(TVPAddSubVertSum16, "void ::TVPAddSubVertSum16(tjs_uint16 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)) \
	x(TVPAddSubVertSum16_d, "void ::TVPAddSubVertSum16_d(tjs_uint16 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)) \
	x(TVPAddSubVertSum32, "void ::TVPAddSubVertSum32(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)) \
	x(TVPAddSubVertSum32_d, "void ::TVPAddSubVertSum32_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)) \
	x(TVPDoBoxBlurAvg16, "void ::TVPDoBoxBlurAvg16(tjs_uint32 *,tjs_uint16 *,const tjs_uint16 *,const tjs_uint16 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)) \
	x(TVPDoBoxBlurAvg16_d, "void ::TVPDoBoxBlurAvg16_d(tjs_uint32 *,tjs_uint16 *,const tjs_uint16 *,const tjs_uint16 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)) \
	x(TVPDoBoxBlurAvg32, "void ::TVPDoBoxBlurAvg32(tjs_uint32 *,tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)) \
	x(TVPDoBoxBlurAvg32_d, "void ::TVPDoBoxBlurAvg32_d(tjs_uint32 *,tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)) \
	x(TVPSwapLine8, "void ::TVPSwapLine8(tjs_uint8 *,tjs_uint8 *,tjs_int)", void, (tjs_uint8 *line1, tjs_uint8 *line2, tjs_int len)) \
	x(TVPSwapLine32, "void ::TVPSwapLine32(tjs_uint32 *,tjs_uint32 *,tjs_int)", void, (tjs_uint32 *line1, tjs_uint32 *line2, tjs_int len)) \
	x(TVPReverse8, "void ::TVPReverse8(tjs_uint8 *,tjs_int)", void, (tjs_uint8 *pixels, tjs_int len)) \
	x(TVPReverse32, "void ::TVPReverse32(tjs_uint32 *,tjs_int)", void, (tjs_uint32 *pixels, tjs_int len)) \
	x(TVPDoGrayScale, "void ::TVPDoGrayScale(tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, tjs_int len)) \
	x(TVPRedBlueSwap, "void ::TVPRedBlueSwap(tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, tjs_int len)) \
	x(TVPRedBlueSwapCopy, "void ::TVPRedBlueSwapCopy(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPInitGammaAdjustTempData, "void ::TVPInitGammaAdjustTempData(tTVPGLGammaAdjustTempData *,const tTVPGLGammaAdjustData *)", void, (tTVPGLGammaAdjustTempData *temp, const tTVPGLGammaAdjustData *data)) \
	x(TVPUninitGammaAdjustTempData, "void ::TVPUninitGammaAdjustTempData(tTVPGLGammaAdjustTempData *)", void, (tTVPGLGammaAdjustTempData *temp)) \
	x(TVPAdjustGamma, "void ::TVPAdjustGamma(tjs_uint32 *,tjs_int,tTVPGLGammaAdjustTempData *)", void, (tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp)) \
	x(TVPAdjustGamma_a, "void ::TVPAdjustGamma_a(tjs_uint32 *,tjs_int,tTVPGLGammaAdjustTempData *)", void, (tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp)) \
	x(TVPChBlurMulCopy65, "void ::TVPChBlurMulCopy65(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level)) \
	x(TVPChBlurAddMulCopy65, "void ::TVPChBlurAddMulCopy65(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level)) \
	x(TVPChBlurCopy65, "void ::TVPChBlurCopy65(tjs_uint8 *,tjs_int,tjs_int,tjs_int,const tjs_uint8 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint8 *dest, tjs_int destpitch, tjs_int destwidth, tjs_int destheight, const tjs_uint8 * src, tjs_int srcpitch, tjs_int srcwidth, tjs_int srcheight, tjs_int blurwidth, tjs_int blurlevel)) \
	x(TVPChBlurMulCopy, "void ::TVPChBlurMulCopy(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level) ) \
	x(TVPChBlurAddMulCopy, "void ::TVPChBlurAddMulCopy(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level) ) \
	x(TVPChBlurCopy, "void ::TVPChBlurCopy(tjs_uint8 *,tjs_int,tjs_int,tjs_int,const tjs_uint8 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)", void, (tjs_uint8 *dest, tjs_int destpitch, tjs_int destwidth, tjs_int destheight, const tjs_uint8 * src, tjs_int srcpitch, tjs_int srcwidth, tjs_int srcheight, tjs_int blurwidth, tjs_int blurlevel) ) \
	x(TVPBLExpand1BitTo8BitPal, "void ::TVPBLExpand1BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPBLExpand1BitTo8Bit, "void ::TVPBLExpand1BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPBLExpand1BitTo32BitPal, "void ::TVPBLExpand1BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPBLExpand4BitTo8BitPal, "void ::TVPBLExpand4BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPBLExpand4BitTo8Bit, "void ::TVPBLExpand4BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPBLExpand4BitTo32BitPal, "void ::TVPBLExpand4BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPBLExpand8BitTo8BitPal, "void ::TVPBLExpand8BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPBLExpand8BitTo32BitPal, "void ::TVPBLExpand8BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal)) \
	x(TVPExpand8BitTo32BitGray, "void ::TVPExpand8BitTo32BitGray(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPBLConvert15BitTo8Bit, "void ::TVPBLConvert15BitTo8Bit(tjs_uint8 *,const tjs_uint16 *,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint16 *buf, tjs_int len)) \
	x(TVPBLConvert15BitTo32Bit, "void ::TVPBLConvert15BitTo32Bit(tjs_uint32 *,const tjs_uint16 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint16 *buf, tjs_int len)) \
	x(TVPBLConvert24BitTo8Bit, "void ::TVPBLConvert24BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPBLConvert24BitTo32Bit, "void ::TVPBLConvert24BitTo32Bit(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPConvert24BitTo32Bit, "void ::TVPConvert24BitTo32Bit(tjs_uint32 *,const tjs_uint8 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)) \
	x(TVPBLConvert32BitTo8Bit, "void ::TVPBLConvert32BitTo8Bit(tjs_uint8 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint32 *buf, tjs_int len)) \
	x(TVPBLConvert32BitTo32Bit_NoneAlpha, "void ::TVPBLConvert32BitTo32Bit_NoneAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len)) \
	x(TVPBLConvert32BitTo32Bit_MulAddAlpha, "void ::TVPBLConvert32BitTo32Bit_MulAddAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len)) \
	x(TVPBLConvert32BitTo32Bit_AddAlpha, "void ::TVPBLConvert32BitTo32Bit_AddAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len)) \
	x(TVPDither32BitTo16Bit565, "void ::TVPDither32BitTo16Bit565(tjs_uint16 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)) \
	x(TVPDither32BitTo16Bit555, "void ::TVPDither32BitTo16Bit555(tjs_uint16 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)) \
	x(TVPDither32BitTo8Bit, "void ::TVPDither32BitTo8Bit(tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)", void, (tjs_uint8 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)) \
	x(TVPTLG5ComposeColors3To4, "void ::TVPTLG5ComposeColors3To4(tjs_uint8 *,const tjs_uint8 *,tjs_uint8 * const *,tjs_int)", void, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width)) \
	x(TVPTLG5ComposeColors4To4, "void ::TVPTLG5ComposeColors4To4(tjs_uint8 *,const tjs_uint8 *,tjs_uint8 * const *,tjs_int)", void, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const* buf, tjs_int width)) \
	x(TVPTLG5DecompressSlide, "tjs_int ::TVPTLG5DecompressSlide(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_uint8 *,tjs_int)", tjs_int, (tjs_uint8 *out, const tjs_uint8 *in, tjs_int insize, tjs_uint8 *text, tjs_int initialr)) \
	x(TVPTLG6DecodeGolombValuesForFirst, "void ::TVPTLG6DecodeGolombValuesForFirst(tjs_int8 *,tjs_int,tjs_uint8 *)", void, (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)) \
	x(TVPTLG6DecodeGolombValues, "void ::TVPTLG6DecodeGolombValues(tjs_int8 *,tjs_int,tjs_uint8 *)", void, (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)) \
	x(TVPTLG6DecodeLineGeneric, "void ::TVPTLG6DecodeLineGeneric(tjs_uint32 *,tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_uint8 *,tjs_int,tjs_uint32 *,tjs_uint32,tjs_int,tjs_int)", void, (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int start_block, tjs_int block_limit, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *in, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)) \
	x(TVPTLG6DecodeLine, "void ::TVPTLG6DecodeLine(tjs_uint32 *,tjs_uint32 *,tjs_int,tjs_int,tjs_uint8 *,tjs_int,tjs_uint32 *,tjs_uint32,tjs_int,tjs_int)", void, (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *in, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)) \
	x(TVPPsAlphaBlend, "void ::TVPPsAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsAlphaBlend_o, "void ::TVPPsAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsAlphaBlend_HDA, "void ::TVPPsAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsAlphaBlend_HDA_o, "void ::TVPPsAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsAddBlend, "void ::TVPPsAddBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsAddBlend_o, "void ::TVPPsAddBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsAddBlend_HDA, "void ::TVPPsAddBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsAddBlend_HDA_o, "void ::TVPPsAddBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsSubBlend, "void ::TVPPsSubBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsSubBlend_o, "void ::TVPPsSubBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsSubBlend_HDA, "void ::TVPPsSubBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsSubBlend_HDA_o, "void ::TVPPsSubBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsMulBlend, "void ::TVPPsMulBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsMulBlend_o, "void ::TVPPsMulBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsMulBlend_HDA, "void ::TVPPsMulBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsMulBlend_HDA_o, "void ::TVPPsMulBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsScreenBlend, "void ::TVPPsScreenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsScreenBlend_o, "void ::TVPPsScreenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsScreenBlend_HDA, "void ::TVPPsScreenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsScreenBlend_HDA_o, "void ::TVPPsScreenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsOverlayBlend, "void ::TVPPsOverlayBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsOverlayBlend_o, "void ::TVPPsOverlayBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsOverlayBlend_HDA, "void ::TVPPsOverlayBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsOverlayBlend_HDA_o, "void ::TVPPsOverlayBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsHardLightBlend, "void ::TVPPsHardLightBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsHardLightBlend_o, "void ::TVPPsHardLightBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsHardLightBlend_HDA, "void ::TVPPsHardLightBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsHardLightBlend_HDA_o, "void ::TVPPsHardLightBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsSoftLightBlend, "void ::TVPPsSoftLightBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsSoftLightBlend_o, "void ::TVPPsSoftLightBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsSoftLightBlend_HDA, "void ::TVPPsSoftLightBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsSoftLightBlend_HDA_o, "void ::TVPPsSoftLightBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorDodgeBlend, "void ::TVPPsColorDodgeBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorDodgeBlend_o, "void ::TVPPsColorDodgeBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorDodgeBlend_HDA, "void ::TVPPsColorDodgeBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorDodgeBlend_HDA_o, "void ::TVPPsColorDodgeBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorDodge5Blend, "void ::TVPPsColorDodge5Blend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorDodge5Blend_o, "void ::TVPPsColorDodge5Blend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorDodge5Blend_HDA, "void ::TVPPsColorDodge5Blend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorDodge5Blend_HDA_o, "void ::TVPPsColorDodge5Blend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorBurnBlend, "void ::TVPPsColorBurnBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorBurnBlend_o, "void ::TVPPsColorBurnBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsColorBurnBlend_HDA, "void ::TVPPsColorBurnBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsColorBurnBlend_HDA_o, "void ::TVPPsColorBurnBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsLightenBlend, "void ::TVPPsLightenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsLightenBlend_o, "void ::TVPPsLightenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsLightenBlend_HDA, "void ::TVPPsLightenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsLightenBlend_HDA_o, "void ::TVPPsLightenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDarkenBlend, "void ::TVPPsDarkenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDarkenBlend_o, "void ::TVPPsDarkenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDarkenBlend_HDA, "void ::TVPPsDarkenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDarkenBlend_HDA_o, "void ::TVPPsDarkenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDiffBlend, "void ::TVPPsDiffBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDiffBlend_o, "void ::TVPPsDiffBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDiffBlend_HDA, "void ::TVPPsDiffBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDiffBlend_HDA_o, "void ::TVPPsDiffBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDiff5Blend, "void ::TVPPsDiff5Blend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDiff5Blend_o, "void ::TVPPsDiff5Blend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsDiff5Blend_HDA, "void ::TVPPsDiff5Blend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsDiff5Blend_HDA_o, "void ::TVPPsDiff5Blend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsExclusionBlend, "void ::TVPPsExclusionBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsExclusionBlend_o, "void ::TVPPsExclusionBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)) \
	x(TVPPsExclusionBlend_HDA, "void ::TVPPsExclusionBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)) \
	x(TVPPsExclusionBlend_HDA_o, "void ::TVPPsExclusionBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)", void, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))

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
	TVPInitTVPGL();
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
	TVPUninitTVPGL();
	return S_OK;
}
