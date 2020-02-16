/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2020-2020 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////

#ifndef __TVPGL_IA32_c_H__
#define __TVPGL_IA32_c_H__


#include "tjsTypes.h"
#include "tvpgl.h"
#include "detect_cpu.h"

#if defined(_WIN32) && !defined(TVP_GL_IA32_FUNC_DECL)
#define TVP_GL_IA32_FUNC_DECL(rettype, funcname, arg)  rettype __cdecl funcname arg
#define TVP_GL_IA32_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype __cdecl funcname arg
#define TVP_GL_IA32_FUNC_PTR_DECL(rettype, funcname, arg) rettype __cdecl (*funcname) arg
#define TVP_GL_IA32_FUNC_PTR_EXTERN_DECL(rettype, funcname, arg) extern rettype __cdecl (*funcname) arg
#endif

#define LAST_IND(x, part_type) (sizeof(x) / sizeof(part_type) - 1)
#define HIGH_IND(x, part_type) LAST_IND(x, part_type)
#define LOW_IND(x, part_type)  0

#define BYTEn(x, n)  (*((tjs_uint8 *)&(x) + n))
#define WORDn(x, n)  (*((tjs_uint16 *)&(x) + n))
#define DWORDn(x, n) (*((tjs_uint32 *)&(x) + n))

#define LOBYTE(x) BYTEn(x, LOW_IND(x, tjs_uint8))
#define HIBYTE(x) BYTEn(x, HIGH_IND(x, tjs_uint8))
#define BYTE1(x)  BYTEn(x, 1)
#define BYTE2(x)  BYTEn(x, 2)

#ifdef __cplusplus
 extern "C" {
#endif

// adjust_color
extern tjs_uint16 TVPRecipTable256_16[256];

// alphablend, colorfill
extern unsigned char TVPOpacityOnOpacityTable[256 * 256];
extern unsigned char TVPNegativeMulTable[256 * 256];

// boxblur
extern unsigned char TVPDivTable[256 * 256];

// colormap
extern unsigned char TVPNegativeMulTable65[65 * 256];
extern unsigned char TVPOpacityOnOpacityTable65[65 * 256];

// pixelformat
extern unsigned char TVPDitherTable_5_6[8][4][2][256];

// tlg6_golomb
#ifndef TVP_TLG6_GOLOMB_N_COUNT
#define TVP_TLG6_GOLOMB_N_COUNT 4
#endif
extern char TVPTLG6GolombBitLengthTable[TVP_TLG6_GOLOMB_N_COUNT * 2 * 128][TVP_TLG6_GOLOMB_N_COUNT];

// tvpps_asm
extern unsigned char TVPPsTableSoftLight[256][256];
extern unsigned char TVPPsTableColorDodge[256][256];
extern unsigned char TVPPsTableColorBurn[256][256];

extern void TVPGL_IA32_c_Init();

/*[*/
/*]*/
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdditiveAlphaBlend_a_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAdjustGamma_a_mmx_c,  (tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPLinTransCopy_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPLinTransConstAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransCopy_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransConstAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransAdditiveAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpLinTransAdditiveAlphaBlend_o_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAlphaBlend_d_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstAlphaBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstAlphaBlend_SD_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddSubVertSum16_mmx_c,  (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddSubVertSum16_d_mmx_c,  (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddSubVertSum32_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPAddSubVertSum32_d_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg16_mmx_c,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg16_d_mmx_c,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg32_mmx_c,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg32_d_mmx_c,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg16_sse_c,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDoBoxBlurAvg16_d_sse_c,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFillARGB_mmx_c,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 value));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFillARGB_sse_c,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 value));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFillARGB_NC_sse_c,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 value));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFillColor_mmx_c,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 color));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstColorAlphaBlend_mmx_c,   (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstColorAlphaBlend_d_mmx_c,   (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConstColorAlphaBlend_a_mmx_c,   (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_mmx_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_d_mmx_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPApplyColorMap65_a_mmx_c,  (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDarkenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDarkenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPLightenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPLightenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMakeAlphaFromKey_cmovcc_c,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 key));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPMulBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPConvert24BitTo32Bit_mmx_c,  (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPBLConvert24BitTo32Bit_mmx_c,  (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDither32BitTo16Bit565_mmx_c,  (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPDither32BitTo16Bit555_mmx_c,  (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPScreenBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchCopy_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchConstAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPStretchAdditiveAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFastLinearInterpV2_mmx_c,  (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src0, const tjs_uint32 *src1));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFastLinearInterpH2F_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPFastLinearInterpH2B_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchCopy_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchConstAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchAdditiveAlphaBlend_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInterpStretchAdditiveAlphaBlend_o_mmx_c,  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPSubBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPSubBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPSubBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPSubBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(tjs_int,  TVPTLG5DecompressSlide_c,  (tjs_uint8 *out, const tjs_uint8 *in, tjs_int insize, tjs_uint8 *text, tjs_int initialr));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG5ComposeColors3To4_mmx_c,  (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG5ComposeColors4To4_mmx_c,  (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeLine_mmx_c,  (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *input, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)
);
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeLine_sse_c,  (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *input, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir)
);
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeGolombValuesForFirst_c,  (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
);
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeGolombValues_c,  (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
);
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeGolombValuesForFirst_mmx_c,  (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
);
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPTLG6DecodeGolombValues_mmx_c,  (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool)
);
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAlphaBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsAddBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSubBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsMulBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsScreenBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsOverlayBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsHardLightBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsSoftLightBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodgeBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorDodge5Blend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsColorBurnBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsLightenBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDarkenBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiffBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsDiff5Blend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_HDA_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPPsExclusionBlend_HDA_o_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa));
/*[*/
/*]*/
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPInitUnivTransBlendTable_mmx_c_c_c,  (tjs_uint32 *table, tjs_int phase, tjs_int vague));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPUnivTransBlend_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len));
TVP_GL_IA32_FUNC_EXTERN_DECL(void,  TVPUnivTransBlend_switch_mmx_c,  (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv));

#ifdef __cplusplus
 }
#endif


#endif
