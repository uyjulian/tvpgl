/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#ifndef __TVPGL_IA32_BEHAVIOR_C_H__
#define __TVPGL_IA32_BEHAVIOR_C_H__


#include "tjsTypes.h"
#include "tvpgl.h"

#ifndef TVP_GL_IA32_FUNC_SUFFIX
#define TVP_GL_IA32_FUNC_SUFFIX _ia32_behavior_generic
#endif

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define TVP_GL_IA32_FUNCNAME(funcname) CAT(funcname, TVP_GL_IA32_FUNC_SUFFIX)

#if defined(_WIN32) && !defined(TVP_GL_IA32_FUNC_DECL)
#define TVP_GL_IA32_FUNC_DECL(rettype, funcname, arg)  rettype __cdecl TVP_GL_IA32_FUNCNAME(funcname) arg
#define TVP_GL_IA32_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype __cdecl TVP_GL_IA32_FUNCNAME(funcname) arg
#define TVP_GL_IA32_FUNC_PTR_DECL(rettype, funcname, arg) rettype __cdecl (*funcname) arg
#define TVP_GL_IA32_FUNC_PTR_EXTERN_DECL(rettype, funcname, arg) extern rettype __cdecl (*funcname) arg
#endif

#include "detect_cpu.h"

#define LOW_IND(x, part_type)  0

#define BYTEn(x, n)  (*((tjs_uint8 *)&(x) + n))

#define LOBYTE(x) BYTEn(x, LOW_IND(x, tjs_uint8))
#define BYTE1(x)  BYTEn(x, 1)

#ifdef __cplusplus
 extern "C" {
#endif

// alphablend, colorfill
extern unsigned char TVPOpacityOnOpacityTable[256 * 256];
extern unsigned char TVPNegativeMulTable[256 * 256];

// boxblur
extern unsigned char TVPDivTable[256 * 256];

// colormap
extern unsigned char TVPNegativeMulTable65[65 * 256];
extern unsigned char TVPOpacityOnOpacityTable65[65 * 256];

// tvpps_asm
extern unsigned char TVPPsTableSoftLight[256][256];
extern unsigned char TVPPsTableColorDodge[256][256];
extern unsigned char TVPPsTableColorBurn[256][256];

extern void TVPGL_IA32_behavior_c_Init();

/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_a_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
/*[*/
/*]*/
/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPLinTransConstAlphaBlend_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransAdditiveAlphaBlend_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransAdditiveAlphaBlend_o_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_d_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstAlphaBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstAlphaBlend_SD_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg32_c,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg32_d_c,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstColorAlphaBlend_d_c,   (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstColorAlphaBlend_a_c,   (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_d_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_a_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDarkenBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
/*[*/
/*]*/
/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchConstAlphaBlend_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchAlphaBlend_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchAdditiveAlphaBlend_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchConstAlphaBlend_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchAdditiveAlphaBlend_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchAdditiveAlphaBlend_o_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
/*[*/
/*]*/
/*[*/
/*]*/
/*[*/
/*]*/
/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_HDA_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_HDA_o_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInitUnivTransBlendTable_c,  (tjs_uint32 *table, tjs_int phase, tjs_int vague));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPUnivTransBlend_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPUnivTransBlend_switch_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv));

// TODO: verify that channels are correct
#define FOREACH_CHANNEL(body, len, src, dest) \
	{ \
		for (tjs_int i = 0; i < len; i += 1) \
		{ \
			__attribute__ ((unused)) const tjs_uint8 *s = (const tjs_uint8 *)&src[i]; \
			__attribute__ ((unused)) const tjs_uint8 *sr = s + 0; \
			__attribute__ ((unused)) const tjs_uint8 *sg = s + 1; \
			__attribute__ ((unused)) const tjs_uint8 *sb = s + 2; \
			__attribute__ ((unused)) const tjs_uint8 *sa = s + 3; \
			__attribute__ ((unused)) tjs_uint8 *d = (tjs_uint8 *)&dest[i]; \
			__attribute__ ((unused)) tjs_uint8 *dr = d + 0; \
			__attribute__ ((unused)) tjs_uint8 *dg = d + 1; \
			__attribute__ ((unused)) tjs_uint8 *db = d + 2; \
			__attribute__ ((unused)) tjs_uint8 *da = d + 3; \
			for (tjs_int j = 0; j < 4; j += 1) \
			{ \
				body \
			} \
		} \
	}

#define FOREACH_CHANNEL_PIXELBODY(body_for_channel, body_for_pixel, len, src, dest) \
	{ \
		for (tjs_int i = 0; i < len; i += 1) \
		{ \
			__attribute__ ((unused)) const tjs_uint8 *s = (const tjs_uint8 *)&src[i]; \
			__attribute__ ((unused)) const tjs_uint8 *sr = s + 0; \
			__attribute__ ((unused)) const tjs_uint8 *sg = s + 1; \
			__attribute__ ((unused)) const tjs_uint8 *sb = s + 2; \
			__attribute__ ((unused)) const tjs_uint8 *sa = s + 3; \
			__attribute__ ((unused)) tjs_uint8 *d = (tjs_uint8 *)&dest[i]; \
			__attribute__ ((unused)) tjs_uint8 *dr = d + 0; \
			__attribute__ ((unused)) tjs_uint8 *dg = d + 1; \
			__attribute__ ((unused)) tjs_uint8 *db = d + 2; \
			__attribute__ ((unused)) tjs_uint8 *da = d + 3; \
			{ \
				body_for_pixel \
			} \
			for (tjs_int j = 0; j < 4; j += 1) \
			{ \
				body_for_channel \
			} \
		} \
	}

#define FOREACH_CHANNEL_2SRC(body, len, src1, src2, dest) \
	{ \
		for (tjs_int i = 0; i < len; i += 1) \
		{ \
			__attribute__ ((unused)) const tjs_uint8 *s1 = (const tjs_uint8 *)&src1[i]; \
			__attribute__ ((unused)) const tjs_uint8 *s1r = s1 + 0; \
			__attribute__ ((unused)) const tjs_uint8 *s1g = s1 + 1; \
			__attribute__ ((unused)) const tjs_uint8 *s1b = s1 + 2; \
			__attribute__ ((unused)) const tjs_uint8 *s1a = s1 + 3; \
			__attribute__ ((unused)) const tjs_uint8 *s2 = (const tjs_uint8 *)&src2[i]; \
			__attribute__ ((unused)) const tjs_uint8 *s2r = s2 + 0; \
			__attribute__ ((unused)) const tjs_uint8 *s2g = s2 + 1; \
			__attribute__ ((unused)) const tjs_uint8 *s2b = s2 + 2; \
			__attribute__ ((unused)) const tjs_uint8 *s2a = s2 + 3; \
			__attribute__ ((unused)) tjs_uint8 *d = (tjs_uint8 *)&dest[i]; \
			__attribute__ ((unused)) tjs_uint8 *dr = d + 0; \
			__attribute__ ((unused)) tjs_uint8 *dg = d + 1; \
			__attribute__ ((unused)) tjs_uint8 *db = d + 2; \
			__attribute__ ((unused)) tjs_uint8 *da = d + 3; \
			for (tjs_int j = 0; j < 4; j += 1) \
			{ \
				body \
			} \
		} \
	}

#define FOREACH_CHANNEL_COLOR(body, len, color, dest) \
	{ \
		for (tjs_int i = 0; i < len; i += 1) \
		{ \
			__attribute__ ((unused)) const tjs_uint8 *c = (const tjs_uint8 *)&color; \
			__attribute__ ((unused)) const tjs_uint8 *cr = c + 0; \
			__attribute__ ((unused)) const tjs_uint8 *cg = c + 1; \
			__attribute__ ((unused)) const tjs_uint8 *cb = c + 2; \
			__attribute__ ((unused)) const tjs_uint8 *ca = c + 3; \
			__attribute__ ((unused)) tjs_uint8 *d = (tjs_uint8 *)&dest[i]; \
			__attribute__ ((unused)) tjs_uint8 *dr = d + 0; \
			__attribute__ ((unused)) tjs_uint8 *dg = d + 1; \
			__attribute__ ((unused)) tjs_uint8 *db = d + 2; \
			__attribute__ ((unused)) tjs_uint8 *da = d + 3; \
			for (tjs_int j = 0; j < 4; j += 1) \
			{ \
				body \
			} \
		} \
	}

#ifdef __cplusplus
 }
#endif


#endif
