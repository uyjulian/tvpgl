/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#include "tjsTypes.h"

#include "tvpgl.h"
#include "tvpgl_ia32_intf_pfraction_c.h"

extern tjs_uint32 TVPCPUType;

void TVPGL_IA32_pfraction_c_Init()
{


if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAdditiveAlphaBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAdditiveAlphaBlend_a =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_a_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddBlend =  TVP_GL_IA32_FUNCNAME(TVPAddBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPAddBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddBlend_o =  TVP_GL_IA32_FUNCNAME(TVPAddBlend_HDA_o_c); // same as below
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPAddBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAdjustGamma_a =  TVP_GL_IA32_FUNCNAME(TVPAdjustGamma_a_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPLinTransCopy =  TVP_GL_IA32_FUNCNAME(TVPLinTransCopy_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPLinTransConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPLinTransConstAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpLinTransCopy =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransCopy_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpLinTransConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransConstAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpLinTransAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransAdditiveAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpLinTransAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransAdditiveAlphaBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAlphaBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAlphaBlend_d =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPConstAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConstAlphaBlend_SD =  TVP_GL_IA32_FUNCNAME(TVPConstAlphaBlend_SD_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddSubVertSum16 =  TVP_GL_IA32_FUNCNAME(TVPAddSubVertSum16_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddSubVertSum16_d =  TVP_GL_IA32_FUNCNAME(TVPAddSubVertSum16_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddSubVertSum32 =  TVP_GL_IA32_FUNCNAME(TVPAddSubVertSum32_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPAddSubVertSum32_d =  TVP_GL_IA32_FUNCNAME(TVPAddSubVertSum32_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg16 =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg16_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg16_d =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg16_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg32 =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg32_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg32_d =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg32_d_c);
if(TVPCPUType & TVP_CPU_HAS_SSE && TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg16 =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg16_sse_c);
if(TVPCPUType & TVP_CPU_HAS_SSE && TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDoBoxBlurAvg16_d =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg16_d_sse_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPFillARGB =  TVP_GL_IA32_FUNCNAME(TVPFillARGB_c);
if(TVPCPUType & TVP_CPU_HAS_SSE)
	 TVPFillARGB =  TVP_GL_IA32_FUNCNAME(TVPFillARGB_sse_c);
if(TVPCPUType & TVP_CPU_HAS_SSE)
	 TVPFillARGB_NC =  TVP_GL_IA32_FUNCNAME(TVPFillARGB_NC_sse_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPFillColor =  TVP_GL_IA32_FUNCNAME(TVPFillColor_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConstColorAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPConstColorAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConstColorAlphaBlend_d =  TVP_GL_IA32_FUNCNAME(TVPConstColorAlphaBlend_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConstColorAlphaBlend_a =  TVP_GL_IA32_FUNCNAME(TVPConstColorAlphaBlend_a_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPApplyColorMap65 =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPApplyColorMap65_d =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_d_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPApplyColorMap65_a =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_a_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDarkenBlend =  TVP_GL_IA32_FUNCNAME(TVPDarkenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDarkenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPDarkenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPLightenBlend =  TVP_GL_IA32_FUNCNAME(TVPLightenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPLightenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPLightenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_CMOV)
	 TVPMakeAlphaFromKey =  TVP_GL_IA32_FUNCNAME(TVPMakeAlphaFromKey_cmovcc_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPMulBlend =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPMulBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPMulBlend_o =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPMulBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPConvert24BitTo32Bit =  TVP_GL_IA32_FUNCNAME(TVPBLConvert24BitTo32Bit_c); // same as below
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPBLConvert24BitTo32Bit =  TVP_GL_IA32_FUNCNAME(TVPBLConvert24BitTo32Bit_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDither32BitTo16Bit565 =  TVP_GL_IA32_FUNCNAME(TVPDither32BitTo16Bit565_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPDither32BitTo16Bit555 =  TVP_GL_IA32_FUNCNAME(TVPDither32BitTo16Bit555_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPScreenBlend =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPScreenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPScreenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPScreenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPStretchCopy =  TVP_GL_IA32_FUNCNAME(TVPStretchCopy_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPStretchConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchConstAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPStretchAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPStretchAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchAdditiveAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPFastLinearInterpV2 =  TVP_GL_IA32_FUNCNAME(TVPFastLinearInterpV2_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPFastLinearInterpH2F =  TVP_GL_IA32_FUNCNAME(TVPFastLinearInterpH2F_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPFastLinearInterpH2B =  TVP_GL_IA32_FUNCNAME(TVPFastLinearInterpH2B_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpStretchCopy =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchCopy_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpStretchConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchConstAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpStretchAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchAdditiveAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInterpStretchAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchAdditiveAlphaBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPSubBlend =  TVP_GL_IA32_FUNCNAME(TVPSubBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPSubBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPSubBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPSubBlend_o =  TVP_GL_IA32_FUNCNAME(TVPSubBlend_HDA_o_c); // Same as above
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPSubBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPSubBlend_HDA_o_c);
 TVPTLG5DecompressSlide =  TVP_GL_IA32_FUNCNAME(TVPTLG5DecompressSlide_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG5ComposeColors3To4 =  TVP_GL_IA32_FUNCNAME(TVPTLG5ComposeColors3To4_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG5ComposeColors4To4 =  TVP_GL_IA32_FUNCNAME(TVPTLG5ComposeColors4To4_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG6DecodeLine
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeLine_c);
if(TVPCPUType & TVP_CPU_HAS_SSE && TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG6DecodeLine
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeLine_sse_c);
 TVPTLG6DecodeGolombValuesForFirst
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeGolombValuesForFirst_c);
 TVPTLG6DecodeGolombValues
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeGolombValues_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG6DecodeGolombValuesForFirst
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeGolombValuesForFirst_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPTLG6DecodeGolombValues
 =  TVP_GL_IA32_FUNCNAME(TVPTLG6DecodeGolombValues_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAlphaBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAlphaBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAddBlend =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAddBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAddBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsAddBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSubBlend =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSubBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSubBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSubBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsMulBlend =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsMulBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsMulBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsMulBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsScreenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsScreenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsScreenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsScreenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsOverlayBlend =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsOverlayBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsOverlayBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsOverlayBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsHardLightBlend =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsHardLightBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsHardLightBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsHardLightBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSoftLightBlend =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSoftLightBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSoftLightBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsSoftLightBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodgeBlend =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodgeBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodgeBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodgeBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodge5Blend =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodge5Blend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodge5Blend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorDodge5Blend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorBurnBlend =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorBurnBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorBurnBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsColorBurnBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsLightenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsLightenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsLightenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsLightenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDarkenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDarkenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDarkenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDarkenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiffBlend =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiffBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiffBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiffBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiff5Blend =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiff5Blend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiff5Blend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsDiff5Blend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsExclusionBlend =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsExclusionBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsExclusionBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_HDA_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPPsExclusionBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_HDA_o_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPInitUnivTransBlendTable =  TVP_GL_IA32_FUNCNAME(TVPInitUnivTransBlendTable_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPUnivTransBlend =  TVP_GL_IA32_FUNCNAME(TVPUnivTransBlend_c);
if(TVPCPUType & TVP_CPU_HAS_MMX)
	 TVPUnivTransBlend_switch =  TVP_GL_IA32_FUNCNAME(TVPUnivTransBlend_switch_c);

}


TVP_GL_IA32_FUNC_DECL(void, TVPInitUnivTransBlendTable_c, (tjs_uint32 *table, tjs_int phase, tjs_int vague))
{
	tjs_uint16 *t = (tjs_uint16*)table;
	tjs_int i;
	tjs_int phasemax = phase + vague;
	phase -= vague;
	for(i = 0; i<256; i++)
	{
		if(i<phase)  t[i] = 0<<7;
		else if(i>=phasemax) t[i] = 255<<7;
		else
		{
			int tmp = ( i - phase )*255 / vague;
			if(tmp<0) tmp = 0;
			if(tmp>255) tmp = 255;
			t[i] = tmp << 7;
		}
	}
}


