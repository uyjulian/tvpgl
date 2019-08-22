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
#include "tjsTypes.h"
#include "tvpgl.h"
#include "tvpgl_ia32_intf.h"
#include "detect_cpu.h"

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

#define TVPGL_DEFINE(x, y)
#define TVPGL_OVERWRITE(x, y)
#define TVPGL_RESTORE(x, y)

#define TVPGL_EXPAND_MACRO(x) \
	x(TVPAlphaBlend, "void ::TVPAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAlphaBlend_HDA, "void ::TVPAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAlphaBlend_o, "void ::TVPAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAlphaBlend_HDA_o, "void ::TVPAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAlphaBlend_d, "void ::TVPAlphaBlend_d(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAlphaBlend_a, "void ::TVPAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAlphaBlend_do, "void ::TVPAlphaBlend_do(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAlphaBlend_ao, "void ::TVPAlphaBlend_ao(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAlphaColorMat, "void ::TVPAlphaColorMat(tjs_uint32 *,const tjs_uint32,tjs_int)") \
	x(TVPAdditiveAlphaBlend, "void ::TVPAdditiveAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAdditiveAlphaBlend_HDA, "void ::TVPAdditiveAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAdditiveAlphaBlend_o, "void ::TVPAdditiveAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAdditiveAlphaBlend_HDA_o, "void ::TVPAdditiveAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAdditiveAlphaBlend_a, "void ::TVPAdditiveAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAdditiveAlphaBlend_ao, "void ::TVPAdditiveAlphaBlend_ao(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConvertAdditiveAlphaToAlpha, "void ::TVPConvertAdditiveAlphaToAlpha(tjs_uint32 *,tjs_int)") \
	x(TVPConvertAlphaToAdditiveAlpha, "void ::TVPConvertAlphaToAdditiveAlpha(tjs_uint32 *,tjs_int)") \
	x(TVPStretchAlphaBlend, "void ::TVPStretchAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_HDA, "void ::TVPStretchAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_o, "void ::TVPStretchAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_HDA_o, "void ::TVPStretchAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_d, "void ::TVPStretchAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_a, "void ::TVPStretchAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_do, "void ::TVPStretchAlphaBlend_do(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAlphaBlend_ao, "void ::TVPStretchAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend, "void ::TVPStretchAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPInterpStretchAdditiveAlphaBlend, "void ::TVPInterpStretchAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend_HDA, "void ::TVPStretchAdditiveAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend_o, "void ::TVPStretchAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpStretchAdditiveAlphaBlend_o, "void ::TVPInterpStretchAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend_HDA_o, "void ::TVPStretchAdditiveAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend_a, "void ::TVPStretchAdditiveAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchAdditiveAlphaBlend_ao, "void ::TVPStretchAdditiveAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend, "void ::TVPLinTransAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_HDA, "void ::TVPLinTransAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_o, "void ::TVPLinTransAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_HDA_o, "void ::TVPLinTransAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_d, "void ::TVPLinTransAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_a, "void ::TVPLinTransAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_do, "void ::TVPLinTransAlphaBlend_do(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAlphaBlend_ao, "void ::TVPLinTransAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend, "void ::TVPLinTransAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpLinTransAdditiveAlphaBlend, "void ::TVPInterpLinTransAdditiveAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend_HDA, "void ::TVPLinTransAdditiveAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend_o, "void ::TVPLinTransAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpLinTransAdditiveAlphaBlend_o, "void ::TVPInterpLinTransAdditiveAlphaBlend_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend_HDA_o, "void ::TVPLinTransAdditiveAlphaBlend_HDA_o(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend_a, "void ::TVPLinTransAdditiveAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransAdditiveAlphaBlend_ao, "void ::TVPLinTransAdditiveAlphaBlend_ao(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPCopyOpaqueImage, "void ::TVPCopyOpaqueImage(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPConstAlphaBlend, "void ::TVPConstAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_HDA, "void ::TVPConstAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_d, "void ::TVPConstAlphaBlend_d(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_a, "void ::TVPConstAlphaBlend_a(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchCopyOpaqueImage, "void ::TVPStretchCopyOpaqueImage(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchConstAlphaBlend, "void ::TVPStretchConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpStretchConstAlphaBlend, "void ::TVPInterpStretchConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchConstAlphaBlend_HDA, "void ::TVPStretchConstAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchConstAlphaBlend_d, "void ::TVPStretchConstAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPStretchConstAlphaBlend_a, "void ::TVPStretchConstAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransCopyOpaqueImage, "void ::TVPLinTransCopyOpaqueImage(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransConstAlphaBlend, "void ::TVPLinTransConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpLinTransConstAlphaBlend, "void ::TVPInterpLinTransConstAlphaBlend(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransConstAlphaBlend_HDA, "void ::TVPLinTransConstAlphaBlend_HDA(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransConstAlphaBlend_d, "void ::TVPLinTransConstAlphaBlend_d(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransConstAlphaBlend_a, "void ::TVPLinTransConstAlphaBlend_a(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_SD, "void ::TVPConstAlphaBlend_SD(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_SD_a, "void ::TVPConstAlphaBlend_SD_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPConstAlphaBlend_SD_d, "void ::TVPConstAlphaBlend_SD_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPInitUnivTransBlendTable, "void ::TVPInitUnivTransBlendTable(tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPInitUnivTransBlendTable_d, "void ::TVPInitUnivTransBlendTable_d(tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPInitUnivTransBlendTable_a, "void ::TVPInitUnivTransBlendTable_a(tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPUnivTransBlend, "void ::TVPUnivTransBlend(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)") \
	x(TVPUnivTransBlend_switch, "void ::TVPUnivTransBlend_switch(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPUnivTransBlend_d, "void ::TVPUnivTransBlend_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)") \
	x(TVPUnivTransBlend_switch_d, "void ::TVPUnivTransBlend_switch_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPUnivTransBlend_a, "void ::TVPUnivTransBlend_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int)") \
	x(TVPUnivTransBlend_switch_a, "void ::TVPUnivTransBlend_switch_a(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,const tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPApplyColorMap, "void ::TVPApplyColorMap(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap_o, "void ::TVPApplyColorMap_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap65, "void ::TVPApplyColorMap65(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap65_o, "void ::TVPApplyColorMap65_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap_HDA, "void ::TVPApplyColorMap_HDA(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap_HDA_o, "void ::TVPApplyColorMap_HDA_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap65_HDA, "void ::TVPApplyColorMap65_HDA(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap65_HDA_o, "void ::TVPApplyColorMap65_HDA_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap_d, "void ::TVPApplyColorMap_d(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap65_d, "void ::TVPApplyColorMap65_d(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap_a, "void ::TVPApplyColorMap_a(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap65_a, "void ::TVPApplyColorMap65_a(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32)") \
	x(TVPApplyColorMap_do, "void ::TVPApplyColorMap_do(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap65_do, "void ::TVPApplyColorMap65_do(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap_ao, "void ::TVPApplyColorMap_ao(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPApplyColorMap65_ao, "void ::TVPApplyColorMap65_ao(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPConstColorAlphaBlend, "void ::TVPConstColorAlphaBlend(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPConstColorAlphaBlend_d, "void ::TVPConstColorAlphaBlend_d(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPConstColorAlphaBlend_a, "void ::TVPConstColorAlphaBlend_a(tjs_uint32 *,tjs_int,tjs_uint32,tjs_int)") \
	x(TVPRemoveConstOpacity, "void ::TVPRemoveConstOpacity(tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPRemoveOpacity, "void ::TVPRemoveOpacity(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPRemoveOpacity_o, "void ::TVPRemoveOpacity_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPRemoveOpacity65, "void ::TVPRemoveOpacity65(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPRemoveOpacity65_o, "void ::TVPRemoveOpacity65_o(tjs_uint32 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPAddBlend, "void ::TVPAddBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAddBlend_HDA, "void ::TVPAddBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAddBlend_o, "void ::TVPAddBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPAddBlend_HDA_o, "void ::TVPAddBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPSubBlend, "void ::TVPSubBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPSubBlend_HDA, "void ::TVPSubBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPSubBlend_o, "void ::TVPSubBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPSubBlend_HDA_o, "void ::TVPSubBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPMulBlend, "void ::TVPMulBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPMulBlend_HDA, "void ::TVPMulBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPMulBlend_o, "void ::TVPMulBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPMulBlend_HDA_o, "void ::TVPMulBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPColorDodgeBlend, "void ::TVPColorDodgeBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPColorDodgeBlend_HDA, "void ::TVPColorDodgeBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPColorDodgeBlend_o, "void ::TVPColorDodgeBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPColorDodgeBlend_HDA_o, "void ::TVPColorDodgeBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPDarkenBlend, "void ::TVPDarkenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPDarkenBlend_HDA, "void ::TVPDarkenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPDarkenBlend_o, "void ::TVPDarkenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPDarkenBlend_HDA_o, "void ::TVPDarkenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPLightenBlend, "void ::TVPLightenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPLightenBlend_HDA, "void ::TVPLightenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPLightenBlend_o, "void ::TVPLightenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPLightenBlend_HDA_o, "void ::TVPLightenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPScreenBlend, "void ::TVPScreenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPScreenBlend_HDA, "void ::TVPScreenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPScreenBlend_o, "void ::TVPScreenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPScreenBlend_HDA_o, "void ::TVPScreenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPStretchCopy, "void ::TVPStretchCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPInterpStretchCopy, "void ::TVPInterpStretchCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPFastLinearInterpH2F, "void ::TVPFastLinearInterpH2F(tjs_uint32 *,tjs_int,const tjs_uint32 *)") \
	x(TVPFastLinearInterpH2B, "void ::TVPFastLinearInterpH2B(tjs_uint32 *,tjs_int,const tjs_uint32 *)") \
	x(TVPFastLinearInterpV2, "void ::TVPFastLinearInterpV2(tjs_uint32 *,tjs_int,const tjs_uint32 *,const tjs_uint32 *)") \
	x(TVPStretchColorCopy, "void ::TVPStretchColorCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPLinTransCopy, "void ::TVPLinTransCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPInterpLinTransCopy, "void ::TVPInterpLinTransCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPLinTransColorCopy, "void ::TVPLinTransColorCopy(tjs_uint32 *,tjs_int,const tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPMakeAlphaFromKey, "void ::TVPMakeAlphaFromKey(tjs_uint32 *,tjs_int,tjs_uint32)") \
	x(TVPCopyMask, "void ::TVPCopyMask(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPCopyColor, "void ::TVPCopyColor(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPBindMaskToMain, "void ::TVPBindMaskToMain(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPFillARGB, "void ::TVPFillARGB(tjs_uint32 *,tjs_int,tjs_uint32)") \
	x(TVPFillARGB_NC, "void ::TVPFillARGB_NC(tjs_uint32 *,tjs_int,tjs_uint32)") \
	x(TVPFillColor, "void ::TVPFillColor(tjs_uint32 *,tjs_int,tjs_uint32)") \
	x(TVPFillMask, "void ::TVPFillMask(tjs_uint32 *,tjs_int,tjs_uint32)") \
	x(TVPAddSubVertSum16, "void ::TVPAddSubVertSum16(tjs_uint16 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAddSubVertSum16_d, "void ::TVPAddSubVertSum16_d(tjs_uint16 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAddSubVertSum32, "void ::TVPAddSubVertSum32(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPAddSubVertSum32_d, "void ::TVPAddSubVertSum32_d(tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPDoBoxBlurAvg16, "void ::TVPDoBoxBlurAvg16(tjs_uint32 *,tjs_uint16 *,const tjs_uint16 *,const tjs_uint16 *,tjs_int,tjs_int)") \
	x(TVPDoBoxBlurAvg16_d, "void ::TVPDoBoxBlurAvg16_d(tjs_uint32 *,tjs_uint16 *,const tjs_uint16 *,const tjs_uint16 *,tjs_int,tjs_int)") \
	x(TVPDoBoxBlurAvg32, "void ::TVPDoBoxBlurAvg32(tjs_uint32 *,tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPDoBoxBlurAvg32_d, "void ::TVPDoBoxBlurAvg32_d(tjs_uint32 *,tjs_uint32 *,const tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPSwapLine8, "void ::TVPSwapLine8(tjs_uint8 *,tjs_uint8 *,tjs_int)") \
	x(TVPSwapLine32, "void ::TVPSwapLine32(tjs_uint32 *,tjs_uint32 *,tjs_int)") \
	x(TVPReverse8, "void ::TVPReverse8(tjs_uint8 *,tjs_int)") \
	x(TVPReverse32, "void ::TVPReverse32(tjs_uint32 *,tjs_int)") \
	x(TVPDoGrayScale, "void ::TVPDoGrayScale(tjs_uint32 *,tjs_int)") \
	x(TVPRedBlueSwap, "void ::TVPRedBlueSwap(tjs_uint32 *,tjs_int)") \
	x(TVPRedBlueSwapCopy, "void ::TVPRedBlueSwapCopy(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPInitGammaAdjustTempData, "void ::TVPInitGammaAdjustTempData(tTVPGLGammaAdjustTempData *,const tTVPGLGammaAdjustData *)") \
	x(TVPUninitGammaAdjustTempData, "void ::TVPUninitGammaAdjustTempData(tTVPGLGammaAdjustTempData *)") \
	x(TVPAdjustGamma, "void ::TVPAdjustGamma(tjs_uint32 *,tjs_int,tTVPGLGammaAdjustTempData *)") \
	x(TVPAdjustGamma_a, "void ::TVPAdjustGamma_a(tjs_uint32 *,tjs_int,tTVPGLGammaAdjustTempData *)") \
	x(TVPChBlurMulCopy65, "void ::TVPChBlurMulCopy65(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPChBlurAddMulCopy65, "void ::TVPChBlurAddMulCopy65(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPChBlurCopy65, "void ::TVPChBlurCopy65(tjs_uint8 *,tjs_int,tjs_int,tjs_int,const tjs_uint8 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPChBlurMulCopy, "void ::TVPChBlurMulCopy(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPChBlurAddMulCopy, "void ::TVPChBlurAddMulCopy(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_int)") \
	x(TVPChBlurCopy, "void ::TVPChBlurCopy(tjs_uint8 *,tjs_int,tjs_int,tjs_int,const tjs_uint8 *,tjs_int,tjs_int,tjs_int,tjs_int,tjs_int)") \
	x(TVPBLExpand1BitTo8BitPal, "void ::TVPBLExpand1BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPBLExpand1BitTo8Bit, "void ::TVPBLExpand1BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)") \
	x(TVPBLExpand1BitTo32BitPal, "void ::TVPBLExpand1BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPBLExpand4BitTo8BitPal, "void ::TVPBLExpand4BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPBLExpand4BitTo8Bit, "void ::TVPBLExpand4BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)") \
	x(TVPBLExpand4BitTo32BitPal, "void ::TVPBLExpand4BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPBLExpand8BitTo8BitPal, "void ::TVPBLExpand8BitTo8BitPal(tjs_uint8 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPBLExpand8BitTo32BitPal, "void ::TVPBLExpand8BitTo32BitPal(tjs_uint32 *,const tjs_uint8 *,tjs_int,const tjs_uint32 *)") \
	x(TVPExpand8BitTo32BitGray, "void ::TVPExpand8BitTo32BitGray(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPBLConvert15BitTo8Bit, "void ::TVPBLConvert15BitTo8Bit(tjs_uint8 *,const tjs_uint16 *,tjs_int)") \
	x(TVPBLConvert15BitTo32Bit, "void ::TVPBLConvert15BitTo32Bit(tjs_uint32 *,const tjs_uint16 *,tjs_int)") \
	x(TVPBLConvert24BitTo8Bit, "void ::TVPBLConvert24BitTo8Bit(tjs_uint8 *,const tjs_uint8 *,tjs_int)") \
	x(TVPBLConvert24BitTo32Bit, "void ::TVPBLConvert24BitTo32Bit(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPConvert24BitTo32Bit, "void ::TVPConvert24BitTo32Bit(tjs_uint32 *,const tjs_uint8 *,tjs_int)") \
	x(TVPBLConvert32BitTo8Bit, "void ::TVPBLConvert32BitTo8Bit(tjs_uint8 *,const tjs_uint32 *,tjs_int)") \
	x(TVPBLConvert32BitTo32Bit_NoneAlpha, "void ::TVPBLConvert32BitTo32Bit_NoneAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPBLConvert32BitTo32Bit_MulAddAlpha, "void ::TVPBLConvert32BitTo32Bit_MulAddAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPBLConvert32BitTo32Bit_AddAlpha, "void ::TVPBLConvert32BitTo32Bit_AddAlpha(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPDither32BitTo16Bit565, "void ::TVPDither32BitTo16Bit565(tjs_uint16 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPDither32BitTo16Bit555, "void ::TVPDither32BitTo16Bit555(tjs_uint16 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPDither32BitTo8Bit, "void ::TVPDither32BitTo8Bit(tjs_uint8 *,const tjs_uint32 *,tjs_int,tjs_int,tjs_int)") \
	x(TVPTLG5ComposeColors3To4, "void ::TVPTLG5ComposeColors3To4(tjs_uint8 *,const tjs_uint8 *,tjs_uint8 * const *,tjs_int)") \
	x(TVPTLG5ComposeColors4To4, "void ::TVPTLG5ComposeColors4To4(tjs_uint8 *,const tjs_uint8 *,tjs_uint8 * const *,tjs_int)") \
	x(TVPTLG5DecompressSlide, "tjs_int ::TVPTLG5DecompressSlide(tjs_uint8 *,const tjs_uint8 *,tjs_int,tjs_uint8 *,tjs_int)") \
	x(TVPTLG6DecodeGolombValuesForFirst, "void ::TVPTLG6DecodeGolombValuesForFirst(tjs_int8 *,tjs_int,tjs_uint8 *)") \
	x(TVPTLG6DecodeGolombValues, "void ::TVPTLG6DecodeGolombValues(tjs_int8 *,tjs_int,tjs_uint8 *)") \
	x(TVPTLG6DecodeLineGeneric, "void ::TVPTLG6DecodeLineGeneric(tjs_uint32 *,tjs_uint32 *,tjs_int,tjs_int,tjs_int,tjs_uint8 *,tjs_int,tjs_uint32 *,tjs_uint32,tjs_int,tjs_int)") \
	x(TVPTLG6DecodeLine, "void ::TVPTLG6DecodeLine(tjs_uint32 *,tjs_uint32 *,tjs_int,tjs_int,tjs_uint8 *,tjs_int,tjs_uint32 *,tjs_uint32,tjs_int,tjs_int)") \
	x(TVPPsAlphaBlend, "void ::TVPPsAlphaBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsAlphaBlend_o, "void ::TVPPsAlphaBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsAlphaBlend_HDA, "void ::TVPPsAlphaBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsAlphaBlend_HDA_o, "void ::TVPPsAlphaBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsAddBlend, "void ::TVPPsAddBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsAddBlend_o, "void ::TVPPsAddBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsAddBlend_HDA, "void ::TVPPsAddBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsAddBlend_HDA_o, "void ::TVPPsAddBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsSubBlend, "void ::TVPPsSubBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsSubBlend_o, "void ::TVPPsSubBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsSubBlend_HDA, "void ::TVPPsSubBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsSubBlend_HDA_o, "void ::TVPPsSubBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsMulBlend, "void ::TVPPsMulBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsMulBlend_o, "void ::TVPPsMulBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsMulBlend_HDA, "void ::TVPPsMulBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsMulBlend_HDA_o, "void ::TVPPsMulBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsScreenBlend, "void ::TVPPsScreenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsScreenBlend_o, "void ::TVPPsScreenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsScreenBlend_HDA, "void ::TVPPsScreenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsScreenBlend_HDA_o, "void ::TVPPsScreenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsOverlayBlend, "void ::TVPPsOverlayBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsOverlayBlend_o, "void ::TVPPsOverlayBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsOverlayBlend_HDA, "void ::TVPPsOverlayBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsOverlayBlend_HDA_o, "void ::TVPPsOverlayBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsHardLightBlend, "void ::TVPPsHardLightBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsHardLightBlend_o, "void ::TVPPsHardLightBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsHardLightBlend_HDA, "void ::TVPPsHardLightBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsHardLightBlend_HDA_o, "void ::TVPPsHardLightBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsSoftLightBlend, "void ::TVPPsSoftLightBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsSoftLightBlend_o, "void ::TVPPsSoftLightBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsSoftLightBlend_HDA, "void ::TVPPsSoftLightBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsSoftLightBlend_HDA_o, "void ::TVPPsSoftLightBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorDodgeBlend, "void ::TVPPsColorDodgeBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorDodgeBlend_o, "void ::TVPPsColorDodgeBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorDodgeBlend_HDA, "void ::TVPPsColorDodgeBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorDodgeBlend_HDA_o, "void ::TVPPsColorDodgeBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorDodge5Blend, "void ::TVPPsColorDodge5Blend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorDodge5Blend_o, "void ::TVPPsColorDodge5Blend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorDodge5Blend_HDA, "void ::TVPPsColorDodge5Blend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorDodge5Blend_HDA_o, "void ::TVPPsColorDodge5Blend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorBurnBlend, "void ::TVPPsColorBurnBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorBurnBlend_o, "void ::TVPPsColorBurnBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsColorBurnBlend_HDA, "void ::TVPPsColorBurnBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsColorBurnBlend_HDA_o, "void ::TVPPsColorBurnBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsLightenBlend, "void ::TVPPsLightenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsLightenBlend_o, "void ::TVPPsLightenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsLightenBlend_HDA, "void ::TVPPsLightenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsLightenBlend_HDA_o, "void ::TVPPsLightenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDarkenBlend, "void ::TVPPsDarkenBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDarkenBlend_o, "void ::TVPPsDarkenBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDarkenBlend_HDA, "void ::TVPPsDarkenBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDarkenBlend_HDA_o, "void ::TVPPsDarkenBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDiffBlend, "void ::TVPPsDiffBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDiffBlend_o, "void ::TVPPsDiffBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDiffBlend_HDA, "void ::TVPPsDiffBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDiffBlend_HDA_o, "void ::TVPPsDiffBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDiff5Blend, "void ::TVPPsDiff5Blend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDiff5Blend_o, "void ::TVPPsDiff5Blend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsDiff5Blend_HDA, "void ::TVPPsDiff5Blend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsDiff5Blend_HDA_o, "void ::TVPPsDiff5Blend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsExclusionBlend, "void ::TVPPsExclusionBlend(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsExclusionBlend_o, "void ::TVPPsExclusionBlend_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)") \
	x(TVPPsExclusionBlend_HDA, "void ::TVPPsExclusionBlend_HDA(tjs_uint32 *,const tjs_uint32 *,tjs_int)") \
	x(TVPPsExclusionBlend_HDA_o, "void ::TVPPsExclusionBlend_HDA_o(tjs_uint32 *,const tjs_uint32 *,tjs_int,tjs_int)")

TVPGL_EXPAND_MACRO(TVPGL_DEFINE);

#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall

extern void TVPGL_C_Init();
extern void TVPGL_SSE2_Init();

EXPORT(HRESULT) V2Link(iTVPFunctionExporter *exporter) {
	TVPFunctionExporter = exporter;
	TVPCheckCPU();
	TVPInitTVPGL();
	TVPGL_C_Init();
	TVPGL_IA32_Init();
	TVPGL_SSE2_Init();
	TVPGL_EXPAND_MACRO(TVPGL_OVERWRITE);
	return S_OK;
}

EXPORT(HRESULT) V2Unlink() {
	TVPGL_EXPAND_MACRO(TVPGL_RESTORE);
	TVPUninitTVPGL();
	return S_OK;
}
