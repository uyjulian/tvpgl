/*
	this is a part of TVP (KIRIKIRI) software source.
	see other sources for license.
	(C)2001-2009 W.Dee <dee@kikyou.info> and contributors
*/

#ifndef __TVPGL_DETECT_CPU_H__
#define __TVPGL_DETECT_CPU_H__


#include "tjsTypes.h"

#if defined(_WIN32) && !defined(TVP_GL_IA32_FUNC_DECL)
#define TVP_GL_IA32_FUNC_DECL(rettype, funcname, arg)  rettype __cdecl funcname arg
#define TVP_GL_IA32_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype __cdecl funcname arg
#define TVP_GL_IA32_FUNC_PTR_DECL(rettype, funcname, arg) rettype __cdecl (*funcname) arg
#define TVP_GL_IA32_FUNC_PTR_EXTERN_DECL(rettype, funcname, arg) extern rettype __cdecl (*funcname) arg
#endif

#ifdef __cplusplus
 extern "C" {
#endif

/*[*/
//---------------------------------------------------------------------------
// CPU Types
//---------------------------------------------------------------------------
/*]*/
/*[*/
#define TVP_CPU_HAS_FPU      0x00010000
#define TVP_CPU_HAS_MMX      0x00020000
#define TVP_CPU_HAS_3DN      0x00040000
#define TVP_CPU_HAS_SSE      0x00080000
#define TVP_CPU_HAS_CMOV     0x00100000
#define TVP_CPU_HAS_E3DN     0x00200000
#define TVP_CPU_HAS_EMMX     0x00400000
#define TVP_CPU_HAS_SSE2     0x00800000
#define TVP_CPU_HAS_TSC      0x01000000
#define TVP_CPU_HAS_SSE3     0x02000000
#define TVP_CPU_HAS_SSSE3    0x04000000
#define TVP_CPU_HAS_SSE41    0x08000000
#define TVP_CPU_HAS_SSE42    0x10000000
#define TVP_CPU_HAS_SSE4a    0x20000000
#define TVP_CPU_HAS_AVX      0x40000000
#define TVP_CPU_HAS_AVX2     0x80000000
#define TVP_CPU_HAS_FMA3     0x00001000
#define TVP_CPU_HAS_AES      0x00002000
#define TVP_CPU_HAS_TSCP     0x00004000
#define TVP_CPU_HAS_RDRAND   0x00008000
#define TVP_CPU_HAS_RDSEED   0x00000100
#define TVP_CPU_FEATURE_MASK 0xffffff00

// note: EMMX is refered to as MMX2

#define TVP_CPU_IS_UNKNOWN   0x00000000
#define TVP_CPU_IS_INTEL     0x00000010
#define TVP_CPU_IS_AMD       0x00000020
#define TVP_CPU_IS_IDT       0x00000030
#define TVP_CPU_IS_CYRIX     0x00000040
#define TVP_CPU_IS_NEXGEN    0x00000050
#define TVP_CPU_IS_RISE      0x00000060
#define TVP_CPU_IS_UMC       0x00000070
#define TVP_CPU_IS_TRANSMETA 0x00000080
#define TVP_CPU_IS_NSC       0x00000090
#define TVP_CPU_IS_COMPAQ    0x000000a0
#define TVP_CPU_VENDOR_MASK  0x000000f0

#define TVP_CPU_FAMILY_ARM      0x00000001
#define TVP_CPU_FAMILY_X86      0x00000002
#define TVP_CPU_FAMILY_MIPS     0x00000003
#define TVP_CPU_FAMILY_ARM64    0x00000004
#define TVP_CPU_FAMILY_X86_64   0x00000005
#define TVP_CPU_FAMILY_MIPS64   0x00000006
#define TVP_CPU_FAMILY_MASK     0x0000000f

/*]*/
 extern tjs_uint32 TVPCPUFeatures;
 extern tjs_uint32 TVPCPUID1_EAX;
 extern tjs_uint32 TVPCPUID1_EBX;
 extern tjs_nchar TVPCPUVendor[16];
 extern tjs_nchar TVPCPUName[52];
TVP_GL_IA32_FUNC_EXTERN_DECL( tjs_uint32, TVPCheckCPU, ());
TVP_GL_IA32_FUNC_EXTERN_DECL( tjs_uint64, TVPGetTSC, ());


#ifdef __cplusplus
 }
#endif


#endif
