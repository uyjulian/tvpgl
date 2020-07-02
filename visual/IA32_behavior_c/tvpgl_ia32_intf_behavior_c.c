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
#include "tvpgl_ia32_intf_behavior_c.h"

extern tjs_uint32 TVPCPUType;

void TVPGL_IA32_behavior_c_Init()
{

	TVPAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_c);

	TVPAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_o_c);

	TVPAdditiveAlphaBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_HDA_c);

	TVPAdditiveAlphaBlend_a =  TVP_GL_IA32_FUNCNAME(TVPAdditiveAlphaBlend_a_c);

#if 1
	TVPLinTransConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPLinTransConstAlphaBlend_c);

	TVPInterpLinTransAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransAdditiveAlphaBlend_c);

	TVPInterpLinTransAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPInterpLinTransAdditiveAlphaBlend_o_c);
#endif

	TVPAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_c);

	TVPAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_o_c);

	TVPAlphaBlend_d =  TVP_GL_IA32_FUNCNAME(TVPAlphaBlend_d_c);

	TVPConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPConstAlphaBlend_c);

	TVPConstAlphaBlend_SD =  TVP_GL_IA32_FUNCNAME(TVPConstAlphaBlend_SD_c);

#if 1
	TVPDoBoxBlurAvg32 =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg32_c);

	TVPDoBoxBlurAvg32_d =  TVP_GL_IA32_FUNCNAME(TVPDoBoxBlurAvg32_d_c);
#endif

	TVPConstColorAlphaBlend_d =  TVP_GL_IA32_FUNCNAME(TVPConstColorAlphaBlend_d_c);

	TVPConstColorAlphaBlend_a =  TVP_GL_IA32_FUNCNAME(TVPConstColorAlphaBlend_a_c);

#if 1
	TVPApplyColorMap65 =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_c);

	TVPApplyColorMap65_d =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_d_c);

	TVPApplyColorMap65_a =  TVP_GL_IA32_FUNCNAME(TVPApplyColorMap65_a_c);
#endif

#if 1
	TVPDarkenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPDarkenBlend_HDA_c);
#endif

	TVPMulBlend =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_c);

	TVPMulBlend_o =  TVP_GL_IA32_FUNCNAME(TVPMulBlend_o_c);

	TVPScreenBlend =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_c);

	TVPScreenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_o_c);

	TVPScreenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPScreenBlend_HDA_o_c);

#if 1
	TVPStretchConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchConstAlphaBlend_c);

	TVPStretchAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchAlphaBlend_c);

	TVPStretchAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPStretchAdditiveAlphaBlend_c);

	TVPInterpStretchConstAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchConstAlphaBlend_c);

	TVPInterpStretchAdditiveAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchAdditiveAlphaBlend_c);

	TVPInterpStretchAdditiveAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPInterpStretchAdditiveAlphaBlend_o_c);
#endif

	TVPPsAlphaBlend =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_c);

	TVPPsAlphaBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_o_c);

	TVPPsAlphaBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_HDA_c);

	TVPPsAlphaBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsAlphaBlend_HDA_o_c);

	TVPPsAddBlend =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_c);

	TVPPsAddBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_o_c);

	TVPPsAddBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_HDA_c);

	TVPPsAddBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsAddBlend_HDA_o_c);

	TVPPsSubBlend =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_c);

	TVPPsSubBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_o_c);

	TVPPsSubBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_HDA_c);

	TVPPsSubBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsSubBlend_HDA_o_c);

	TVPPsMulBlend =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_c);

	TVPPsMulBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_o_c);

	TVPPsMulBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_HDA_c);

	TVPPsMulBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsMulBlend_HDA_o_c);

	TVPPsScreenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_c);

	TVPPsScreenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_o_c);

	TVPPsScreenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_HDA_c);

	TVPPsScreenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsScreenBlend_HDA_o_c);

	TVPPsOverlayBlend =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_c);

	TVPPsOverlayBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_o_c);

	TVPPsOverlayBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_HDA_c);

	TVPPsOverlayBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsOverlayBlend_HDA_o_c);

	TVPPsHardLightBlend =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_c);

	TVPPsHardLightBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_o_c);

	TVPPsHardLightBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_HDA_c);

	TVPPsHardLightBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsHardLightBlend_HDA_o_c);

	TVPPsSoftLightBlend =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_c);

	TVPPsSoftLightBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_o_c);

	TVPPsSoftLightBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_HDA_c);

	TVPPsSoftLightBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsSoftLightBlend_HDA_o_c);

	TVPPsColorDodgeBlend =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_c);

	TVPPsColorDodgeBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_o_c);

	TVPPsColorDodgeBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_HDA_c);

	TVPPsColorDodgeBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodgeBlend_HDA_o_c);

	TVPPsColorDodge5Blend =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_c);

	TVPPsColorDodge5Blend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_o_c);

	TVPPsColorDodge5Blend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_HDA_c);

	TVPPsColorDodge5Blend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorDodge5Blend_HDA_o_c);

	TVPPsColorBurnBlend =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_c);

	TVPPsColorBurnBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_o_c);

	TVPPsColorBurnBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_HDA_c);

	TVPPsColorBurnBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsColorBurnBlend_HDA_o_c);

	TVPPsLightenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_c);

	TVPPsLightenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_o_c);

	TVPPsLightenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_HDA_c);

	TVPPsLightenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsLightenBlend_HDA_o_c);

	TVPPsDarkenBlend =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_c);

	TVPPsDarkenBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_o_c);

	TVPPsDarkenBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_HDA_c);

	TVPPsDarkenBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDarkenBlend_HDA_o_c);

	TVPPsDiffBlend =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_c);

	TVPPsDiffBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_o_c);

	TVPPsDiffBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_HDA_c);

	TVPPsDiffBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiffBlend_HDA_o_c);

	TVPPsDiff5Blend =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_c);

	TVPPsDiff5Blend_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_o_c);

	TVPPsDiff5Blend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_HDA_c);

	TVPPsDiff5Blend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsDiff5Blend_HDA_o_c);

	TVPPsExclusionBlend =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_c);

	TVPPsExclusionBlend_o =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_o_c);

	TVPPsExclusionBlend_HDA =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_HDA_c);

	TVPPsExclusionBlend_HDA_o =  TVP_GL_IA32_FUNCNAME(TVPPsExclusionBlend_HDA_o_c);

	TVPInitUnivTransBlendTable =  TVP_GL_IA32_FUNCNAME(TVPInitUnivTransBlendTable_c);

	TVPUnivTransBlend =  TVP_GL_IA32_FUNCNAME(TVPUnivTransBlend_c);

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
			tjs_int32 tmp = ( i - phase )*255 / vague;
			if(tmp<0) tmp = 0;
			if(tmp>255) tmp = 255;
			t[i] = tmp << 7;
		}
	}
}


