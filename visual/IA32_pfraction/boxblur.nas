; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; blur related functions

%ifndef	GEN_CODE

%include		"nasm.nah"

externdef		TVPDivTable
globaldef		TVPAddSubVertSum16_mmx_pfraction_a
globaldef		TVPAddSubVertSum16_emmx_pfraction_a
globaldef		TVPAddSubVertSum16_d_mmx_pfraction_a
globaldef		TVPAddSubVertSum16_d_emmx_pfraction_a
globaldef		TVPAddSubVertSum32_mmx_pfraction_a
globaldef		TVPAddSubVertSum32_emmx_pfraction_a
globaldef		TVPAddSubVertSum32_d_mmx_pfraction_a
globaldef		TVPAddSubVertSum32_d_emmx_pfraction_a
globaldef		TVPDoBoxBlurAvg16_mmx_pfraction_a
globaldef		TVPDoBoxBlurAvg16_emmx_pfraction_a
globaldef		TVPDoBoxBlurAvg16_sse_pfraction_a
globaldef		TVPDoBoxBlurAvg16_d_mmx_pfraction_a
globaldef		TVPDoBoxBlurAvg16_d_emmx_pfraction_a
globaldef		TVPDoBoxBlurAvg16_d_sse_pfraction_a
globaldef		TVPDoBoxBlurAvg32_mmx_pfraction_a
globaldef		TVPDoBoxBlurAvg32_emmx_pfraction_a
globaldef		TVPDoBoxBlurAvg32_d_mmx_pfraction_a
globaldef		TVPDoBoxBlurAvg32_d_emmx_pfraction_a


%define GEN_CODE

;--------------------------------------------------------------------
; MMX stuff
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum16
;;void, TVPAddSubVertSum16_mmx_pfraction_a, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum16_name TVPAddSubVertSum16_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum16_d
;;void, TVPAddSubVertSum16_d_mmx_pfraction_a, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum16_d_name TVPAddSubVertSum16_d_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum32
;;void, TVPAddSubVertSum32_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum32_name TVPAddSubVertSum32_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum32_d
;;void, TVPAddSubVertSum32_d_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum32_d_name TVPAddSubVertSum32_d_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16
;;void, TVPDoBoxBlurAvg16_mmx_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg16_name TVPDoBoxBlurAvg16_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16_d
;;void, TVPDoBoxBlurAvg16_d_mmx_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg16_d_name TVPDoBoxBlurAvg16_d_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg32
;;void, TVPDoBoxBlurAvg32_mmx_pfraction_a, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg32_name TVPDoBoxBlurAvg32_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg32_d
;;void, TVPDoBoxBlurAvg32_d_mmx_pfraction_a, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg32_d_name TVPDoBoxBlurAvg32_d_mmx_pfraction_a
;--------------------------------------------------------------------
	%include "boxblur.nas"
;--------------------------------------------------------------------

;--------------------------------------------------------------------
; EMMX stuff
;--------------------------------------------------------------------
%define USE_EMMX
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum16
;;void, TVPAddSubVertSum16_emmx_pfraction_a, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum16_name TVPAddSubVertSum16_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum16_d
;;void, TVPAddSubVertSum16_d_emmx_pfraction_a, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum16_d_name TVPAddSubVertSum16_d_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum32
;;void, TVPAddSubVertSum32_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum32_name TVPAddSubVertSum32_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAddSubVertSum32_d
;;void, TVPAddSubVertSum32_d_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
%define TVPAddSubVertSum32_d_name TVPAddSubVertSum32_d_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16
;;void, TVPDoBoxBlurAvg16_emmx_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg16_name TVPDoBoxBlurAvg16_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16_d
;;void, TVPDoBoxBlurAvg16_d_emmx_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg16_d_name TVPDoBoxBlurAvg16_d_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg32
;;void, TVPDoBoxBlurAvg32_emmx_pfraction_a, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg32_name TVPDoBoxBlurAvg32_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg32_d
;;void, TVPDoBoxBlurAvg32_d_emmx_pfraction_a, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
%define TVPDoBoxBlurAvg32_d_name TVPDoBoxBlurAvg32_d_emmx_pfraction_a
;--------------------------------------------------------------------
	%include "boxblur.nas"
;--------------------------------------------------------------------

%define GEN_SSE


;--------------------------------------------------------------------
	start_const_aligned
;--------------------------------------------------------------------
c_0000ffffffffffff		dd	0xffffffff, 0x0000ffff
c_ffff000000000000		dd	0x00000000, 0xffff0000
c_0100000000000000		dd	0x00000000, 0x01000000
;--------------------------------------------------------------------
	end_const_aligned
;--------------------------------------------------------------------


%else


;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
	function_align
TVPAddSubVertSum16_name:			; vertical addition of pixel valus, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 40]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	mov	edi,	[esp + 28]		; dest
	mov	eax,	[esp + 32]		; addline
	mov	ebx,	[esp + 36]		; subline
	shl	ecx,	3
	lea	esi,	[edi + ecx]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit			; jump if edi >= esi

.ploop2:	; fractions
	movq	mm1,	[edi]		; A|R|G|B
	movd	mm3,	[eax]		; 00|00|00|00|A+|R+|G+|B+
	movd	mm4,	[ebx]		; 00|00|00|00|A-|R-|G-|B-

	punpcklbw	mm3,	mm0		; 00|A+|00|R+|00|G+|00|B+
	punpcklbw	mm4,	mm0		; 00|A-|00|R-|00|G-|00|B-

	paddd	mm1,	mm3
	psubd	mm1,	mm4

	movq	[edi],	mm1

	add	edi,	byte 8
	add	eax,	byte 4
	add	ebx,	byte 4

	cmp	edi,	esi
	jb	short .ploop2		; jump if edi < esi

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------
	function_align
TVPAddSubVertSum16_d_name:			; vertical addition of pixel valus with alpha, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 40]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	mov	edi,	[esp + 28]		; dest
	mov	eax,	[esp + 32]		; addline
	mov	ebx,	[esp + 36]		; subline
	shl	ecx,	3
	lea	esi,	[edi + ecx]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit			; jump if edi >= esi

.ploop2:	; fractions
	movq	mm1,	[edi]		; A|R|G|B
	movd	mm3,	[eax]		; 00|00|00|00|A+|R+|G+|B+
	movd	mm4,	[ebx]		; 00|00|00|00|A-|R-|G-|B-

	punpcklbw	mm3,	mm0		; 00|A+|00|R+|00|G+|00|B+
	punpcklbw	mm4,	mm0		; 00|A-|00|R-|00|G-|00|B-

	movq	mm5,	mm3		; addalpha:
	movq	mm6,	mm4		; addalpha:
	psrlw	mm5,	7		; addalpha:
	psrlw	mm6,	7		; addalpha:
	paddw	mm5,	mm3		; addalpha: adjust alpha
	paddw	mm6,	mm4		; addalpha: adjust alpha
	psrlq	mm5,	48		; addalpha: 0000|0000|0000|A+
	psrlq	mm6,	48		; addalpha: 0000|0000|0000|A-
	punpcklwd	mm5,	mm5		; addalpha: 0000|0000|A+|A+
	punpcklwd	mm6,	mm6		; addalpha: 0000|0000|A-|A-
	punpcklwd	mm5,	mm5		; addalpha: A+|A+|A+|A+
	punpcklwd	mm6,	mm6		; addalpha: A-|A-|A-|A-
	pand	mm5,	[c_0000ffffffffffff]	; addalpha:
	pand	mm6,	[c_0000ffffffffffff]	; addalpha:
	por	mm5,	[c_0100000000000000]	; addalpha: 0100|A+|A+|A+
	por	mm6,	[c_0100000000000000]	; addalpha: 0100|A-|A-|A-
	pmullw	mm3,	mm5		; addalpha:
	pmullw	mm4,	mm6		; addalpha:
	psrlw	mm3,	8		; addalpha:
	psrlw	mm4,	8		; addalpha:

	paddd	mm1,	mm3
	psubd	mm1,	mm4

	movq	[edi],	mm1

	add	edi,	byte 8
	add	eax,	byte 4
	add	ebx,	byte 4

	cmp	edi,	esi
	jb	short .ploop2		; jump if edi < esi

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

	function_align
TVPAddSubVertSum32_name:			; vertical addition of pixel valus, 32bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 40]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	mov	edi,	[esp + 28]		; dest
	mov	eax,	[esp + 32]		; addline
	mov	ebx,	[esp + 36]		; subline
	shl	ecx,	4
	lea	esi,	[edi + ecx]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit			; jump if edi >= esi

.ploop2:	; fractions
	movq	mm1,	[edi]		; G|B
	movq	mm2,	[edi+8]		; A|R
	movd	mm3,	[eax]		; 00|00|00|00|A+|R+|G+|B+
	movd	mm4,	[ebx]		; 00|00|00|00|A-|R-|G-|B-

	punpcklbw	mm3,	mm0		; 00|A+|00|R+|00|G+|00|B+
	punpcklbw	mm4,	mm0		; 00|A-|00|R-|00|G-|00|B-

	movq	mm5,	mm3
	movq	mm6,	mm4

	punpcklwd	mm3,	mm0		; 00|00|00|G+|00|00|00|B+
	punpcklwd	mm4,	mm0		; 00|00|00|G-|00|00|00|B-

	paddd	mm1,	mm3
	psubd	mm1,	mm4

	punpckhwd	mm5,	mm0		; 00|00|00|A+|00|00|00|R+
	punpckhwd	mm6,	mm0		; 00|00|00|A-|00|00|00|R-

	paddd	mm2,	mm5
	psubd	mm2,	mm6

	movq	[edi],	mm1
	movq	[edi+8],	mm2

	add	edi,	byte 16
	add	eax,	byte 4
	add	ebx,	byte 4
	cmp	edi,	esi

	jb	short .ploop2		; jump if edi < esi

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------
	function_align
TVPAddSubVertSum32_d_name:			; vertical addition of pixel valus with alpha, 32bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 40]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	mov	edi,	[esp + 28]		; dest
	mov	eax,	[esp + 32]		; addline
	mov	ebx,	[esp + 36]		; subline
	shl	ecx,	4
	lea	esi,	[edi + ecx]		; limit

.pfraction:
	cmp	edi,	esi
	jae	near .pexit			; jump if edi >= esi

.ploop2:	; fractions
	movq	mm1,	[edi]		; load: G|B
	movq	mm2,	[edi+8]		; load: A|R
	movd	mm3,	[eax]		; load: 00|00|00|00|A+|R+|G+|B+
	movd	mm4,	[ebx]		; load: 00|00|00|00|A-|R-|G-|B-
	punpcklbw	mm3,	mm0		; load: 00|A+|00|R+|00|G+|00|B+
	punpcklbw	mm4,	mm0		; load: 00|A-|00|R-|00|G-|00|B-

	movq	mm5,	mm3		; addalpha:
	movq	mm6,	mm4		; addalpha:
	psrlw	mm5,	7		; addalpha:
	psrlw	mm6,	7		; addalpha:
	paddw	mm5,	mm3		; addalpha: adjust alpha
	paddw	mm6,	mm4		; addalpha: adjust alpha
	psrlq	mm5,	48		; addalpha: 0000|0000|0000|A+
	psrlq	mm6,	48		; addalpha: 0000|0000|0000|A-
	punpcklwd	mm5,	mm5		; addalpha: 0000|0000|A+|A+
	punpcklwd	mm6,	mm6		; addalpha: 0000|0000|A-|A-
	punpcklwd	mm5,	mm5		; addalpha: A+|A+|A+|A+
	punpcklwd	mm6,	mm6		; addalpha: A-|A-|A-|A-
	pand	mm5,	[c_0000ffffffffffff]	; addalpha:
	pand	mm6,	[c_0000ffffffffffff]	; addalpha:
	por	mm5,	[c_0100000000000000]	; addalpha: 0100|A+|A+|A+
	por	mm6,	[c_0100000000000000]	; addalpha: 0100|A-|A-|A-
	pmullw	mm3,	mm5		; addalpha:
	pmullw	mm4,	mm6		; addalpha:
	psrlw	mm3,	8		; addalpha:
	psrlw	mm4,	8		; addalpha:

	movq	mm5,	mm3		; sum:
	movq	mm6,	mm4		; sum:
	punpcklwd	mm3,	mm0		; sum: 00|00|00|G+|00|00|00|B+
	punpcklwd	mm4,	mm0		; sum: 00|00|00|G-|00|00|00|B-
	paddd	mm1,	mm3		; sum:
	psubd	mm1,	mm4		; sum:
	punpckhwd	mm5,	mm0		; sum: 00|00|00|A+|00|00|00|R+
	punpckhwd	mm6,	mm0		; sum: 00|00|00|A-|00|00|00|R-
	paddd	mm2,	mm5		; sum:
	psubd	mm2,	mm6		; sum:

	movq	[edi],	mm1		; store:
	movq	[edi+8],	mm2		; store:

	add	edi,	byte 16
	add	eax,	byte 4
	add	ebx,	byte 4

	cmp	edi,	esi
	jb	near .ploop2		; jump if edi < esi

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg16_name:			; do blur using box-blur algorithm, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp

	mov	eax,	[esp + 44]		; n
	cmp	eax,	3
	IF	ge
	  cmp	eax,	128
	  IF	l
	    ; if n>=3 && n < 128
	    jmp near  .TVPDoBoxBlurAvg15		; do 15bit precition routine
	  ENDIF
	ENDIF

	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2

	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	pxor	mm0,	mm0		; mm0 = 0

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	shl	eax,	16
	mov	esi,	eax		; esi = rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebx,	[esp + 36]		; addptr
	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	[esp-8],	eax		; [esp-8] = limit

.pfraction:
	cmp	edi,	[esp-8]
	jae	.pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN

	movq	mm2,	mm1
	punpcklwd	mm1,	mm0		; mm1 = 0000|Gs+HalfN|0000|Bs+HalfN
	punpckhwd	mm2,	mm0		; mm1 = 0000|As+HalfN|0000|Rs+HalfN

	movd	eax,	mm1
	mul	esi
	mov	ecx,	edx

	psrlq	mm1,	32
	movd	eax,	mm1
	mul	esi
	shl	edx,	8
	add	ecx,	edx

	movd	eax,	mm2
	mul	esi
	shl	edx,	16
	add	ecx,	edx

	psrlq	mm2,	32
	movd	eax,	mm2
	mul	esi
	shl	edx,	24
	add	ecx,	edx

	mov	[edi],	ecx

	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	ebx,	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	[esp-8]
	jb	short .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

.TVPDoBoxBlurAvg15:				; 15bit precision routine
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .epexit2

	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	movd	mm6,	eax
	punpcklwd	mm6,	mm6		; mm6 = 0000|0000|rcp|rcp
	punpcklwd	mm6,	mm6		; mm6 = rcp|rcp|rcp|rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebx,	[esp + 36]		; addptr
	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	esi,	eax		; esi = limit

.epfraction:
	cmp	edi,	esi
	jae	.epexit			; jump if edi >= limit

.eploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN
	pmulhw	mm1,	mm6		; mm1 = Aavg|Ravg|Gavg|Bavg
	packuswb	mm1,	mm1		; pack mm1
	movd	[edi],	mm1

	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	ebx,	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	esi
	jb	short .eploop2		; jump if edi < limit

.epexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.epexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg16_d_name:			; do blur using box-blur algorithm with alpha, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp

	mov	eax,	[esp + 44]		; n
	cmp	eax,	3
	IF	ge
	  cmp	eax,	128
	  IF	l
	    ; if n>=3 && n < 128
	    jmp near  .TVPDoBoxBlurAvg15_d		; do 15bit precition routine
	  ENDIF
	ENDIF

	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2

	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	shl	eax,	16
	mov	esi,	eax		; esi = rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	[esp-8],	eax		; [esp-8] = limit

.pfraction:
	cmp	edi,	[esp-8]
	jae	.pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN

	movq	mm2,	mm1
	punpcklwd	mm1,	mm0		; mm1 = 0000|Gs+HalfN|0000|Bs+HalfN
	punpckhwd	mm2,	mm0		; mm1 = 0000|As+HalfN|0000|Rs+HalfN

	movq	mm3,	mm2
	psrlq	mm3,	32
	movd	eax,	mm3
	mul	esi
	mov	ebx,	edx
	shl	edx,	24
	shl	ebx,	8
	add	ebx,	TVPDivTable		; ebx = TVPDivTable[alpha][]
	mov	ecx,	edx

	movd	eax,	mm1
	mul	esi
	movzx	edx,	byte[ebx+edx]	; look up the table
	add	ecx,	edx

	psrlq	mm1,	32
	movd	eax,	mm1
	mul	esi
	movzx	edx,	byte[ebx+edx]	; look up the table
	shl	edx,	8
	add	ecx,	edx

	movd	eax,	mm2
	mul	esi
	movzx	edx,	byte[ebx+edx]	; look up the table
	shl	edx,	16
	add	ecx,	edx

	mov	[edi],	ecx

	mov	ebx,	[esp + 36]		; addptr
	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	dword[esp + 36],	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	[esp-8]
	jb	short .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

.TVPDoBoxBlurAvg15_d:				; do 15bit precision
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .epexit2

	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	movd	mm6,	eax
	punpcklwd	mm6,	mm6		; mm6 = 0000|0000|rcp|rcp
	punpcklwd	mm6,	mm6		; mm6 = rcp|rcp|rcp|rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebx,	[esp + 36]		; addptr
	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	esi,	eax		; esi = limit

.epfraction:
	cmp	edi,	esi
	jae	.epexit			; jump if edi >= limit

.eploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN
	pmulhw	mm1,	mm6		; mm1 = Aavg|Ravg|Gavg|Bavg
	packuswb	mm1,	mm0

	movd	ecx,	mm1
	shr	ecx,	16
	and	ecx,	0xff00
	add	ecx,	TVPDivTable		; ecx = TVPDivTable[Alpha][]
	movd	edx,	mm1
	and	edx,	0xff000000

	movd	eax,	mm1
	and	eax,	0xff		; eax = 00|00|00|B
	movzx	eax,byte	[ecx+eax]		; look up the table
	add	edx,	eax		; edx = alpha|00|00|[B]

	movd	eax,	mm1
	shr	eax,	8
	and	eax,	0xff		; eax = Gavg
	movzx	eax,byte	[ecx+eax]		; look up the table
	shl	eax,	8		; eax = 00|00|[G]|00
	add	edx,	eax		; edx = alpha|00|[G]|[B]

	movd	eax,	mm1
	shr	eax,	16
	and	eax,	0xff		; eax = Ravg
	movzx	eax,byte	[ecx+eax]		; look up the table
	shl	eax,	16		; eax = 00|[R]|00|00
	add	edx,	eax		; edx = alpha|[R]|[G]|[B]

	mov	[edi],	edx

	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	ebx,	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	esi
	jb	short .eploop2		; jump if edi < limit

.epexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.epexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg32_name:			; do blur using box-blur algorithm, 32bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2
	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpckldq	mm7,	mm7		; mm7 = half_n|half_n

	mov	ebx,	[esp + 44]		; n
	xor	eax,	eax
	mov	edx,	1
	idiv	ebx			; eax = (1<<32) / n
	mov	esi,	eax		; store to esi

	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; sum

	movq	mm1,	[ebp]		; mm1 = Gs|Bs
	movq	mm2,	[ebp+8]		; mm2 = As|Rs

	mov	ebx,	[esp + 36]		; addptr
	lea	eax,	[edi + ecx*4]	; limit
	mov	ebp,	[esp + 40]		; subptr
	mov	[esp-8],	eax		; esp-8 = limit

.pfraction:
	cmp	edi,	[esp-8]
	jae	.pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm3,	mm1		; mm3 = Gs|Bs
	movq	mm4,	mm2		; mm4 = As|Rs

	paddd	mm3,	mm7		; mm3 = Gs+HalfN|Bs+HalfN
	paddd	mm4,	mm7		; mm4 = As+HalfN|Rs+HalfN

	movd	eax,	mm3		; eax = Bs+HalfN
	imul	esi			; edx = (Bs+HalfN) / n
	mov	ecx,	edx

	psrlq	mm3,	32
	movd	eax,	mm3		; eax = Gs+HalfN
	imul	esi			; edx = (Gs+HalfN) / n
	shl	edx,	8
	add	ecx,	edx

	movd	eax,	mm4		; eax = Rs+HalfN
	imul	esi			; edx = (Rs+HalfN) / n
	shl	edx,	16
	add	ecx,	edx

	psrlq	mm4,	32
	movd	eax,	mm4		; eax = As+HalfN
	imul	esi			; edx = (As+HalfN) / n
	shl	edx,	24
	add	ecx,	edx

	mov	[edi],	ecx		; store result

	paddd	mm1,	[ebx]
	paddd	mm2,	[ebx+8]

	psubd	mm1,	[ebp]
	psubd	mm2,	[ebp+8]

	add	ebx,	byte 16
	add	ebp,	byte 16

	add	edi,	byte 4
	cmp	edi,	[esp-8]

	jb	short .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum

	movq	[ebp],	mm1		; store sum
	movq	[ebp+8],	mm2		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg32_d_name:			; do blur using box-blur algorithm with alpha, 32bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2
	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpckldq	mm7,	mm7		; mm7 = half_n|half_n

	mov	ebx,	[esp + 44]		; n
	xor	eax,	eax
	mov	edx,	1
	idiv	ebx			; eax = (1<<32) / n
	mov	esi,	eax		; store to esi

	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; sum

	movq	mm1,	[ebp]		; mm1 = Gs|Bs
	movq	mm2,	[ebp+8]		; mm2 = As|Rs

	lea	eax,	[edi + ecx*4]	; limit
	mov	ebp,	[esp + 40]		; subptr
	mov	[esp-8],	eax		; esp-8 = limit

.pfraction:
	cmp	edi,	[esp-8]
	jae	near .pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm3,	mm1		; load: mm3 = Gs|Bs
	movq	mm4,	mm2		; load: mm4 = As|Rs
	paddd	mm3,	mm7		; halfn: mm3 = Gs+HalfN|Bs+HalfN
	paddd	mm4,	mm7		; halfn: mm4 = As+HalfN|Rs+HalfN

	movq	mm5,	mm4		; a:
	psrlq	mm5,	32		; a:
	movd	eax,	mm5		; a: eax = As+HalfN
	imul	esi			; a: edx = (As+HalfN) / n
	mov	ebx,	edx		; a:
	shl	ebx,	8		; a:
	shl	edx,	24		; a:
	mov	ecx,	edx		; a:

	movd	eax,	mm3		; b: eax = Bs+HalfN
	imul	esi			; b: edx = (Bs+HalfN) / n
	movzx	edx,byte	[TVPDivTable+ebx+edx]	; b: lookup the table
	add	ecx,	edx		; b:

	psrlq	mm3,	32		; g:
	movd	eax,	mm3		; g: eax = Gs+HalfN
	imul	esi			; g: edx = (Gs+HalfN) / n
	movzx	edx,byte	[TVPDivTable+ebx+edx]	; g: lookup the table
	shl	edx,	8		; g:
	add	ecx,	edx		; g:

	movd	eax,	mm4		; r: eax = Rs+HalfN
	imul	esi			; r: edx = (Rs+HalfN) / n
	movzx	edx,byte	[TVPDivTable+ebx+edx]	; r: lookup the table
	shl	edx,	16		; r:
	add	ecx,	edx		; r:

	mov	[edi],	ecx		; store: store result

	psubd	mm1,	[ebp]		; sub:
	psubd	mm2,	[ebp+8]		; sub:

	mov	ebx,	[esp + 36]		; add: load addptr
	paddd	mm1,	[ebx]		; add:
	paddd	mm2,	[ebx+8]		; add:

	add	dword[esp + 36],	byte 16
	add	ebp,	byte 16
	add	edi,	byte 4

	cmp	edi,	[esp-8]
	jb	near .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum

	movq	[ebp],	mm1		; store sum
	movq	[ebp+8],	mm2		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------
%endif


%ifdef GEN_SSE

;--------------------------------------------------------------------
; SSE stuff
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_SSE && TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16
;;void, TVPDoBoxBlurAvg16_sse_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg16_sse_pfraction_a:			; do blur using box-blur algorithm, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2

	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	movd	mm6,	eax
	punpcklwd	mm6,	mm6		; mm6 = 0000|0000|rcp|rcp
	punpcklwd	mm6,	mm6		; mm6 = rcp|rcp|rcp|rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebx,	[esp + 36]		; addptr
	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	esi,	eax		; esi = limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN
	pmulhuw	mm1,	mm6		; mm1 = Aavg|Ravg|Gavg|Bavg   (pmulhuw = SSE inst.)
	packuswb	mm1,	mm1		; pack mm1
	movd	[edi],	mm1

	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	ebx,	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	esi
	jb	short .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret
;--------------------------------------------------------------------

;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_SSE && TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPDoBoxBlurAvg16_d
;;void, TVPDoBoxBlurAvg16_d_sse_pfraction_a, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
;--------------------------------------------------------------------

	function_align
TVPDoBoxBlurAvg16_d_sse_pfraction_a:			; do blur using box-blur algorithm with alpha, 16bit precision
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 48]		; len
	cmp	ecx,	byte 0
	jle	near .pexit2

	pxor	mm0,	mm0		; mm0 = 0

	mov	eax,	[esp + 44]		; n
	shr	eax,	1		; eax = n / 2
	movd	mm7,	eax
	punpcklwd	mm7,	mm7		; mm7 = 0000|0000|half_n|half_n
	punpcklwd	mm7,	mm7		; mm7 = half_n|half_n|half_n|half_n

	mov	ebx,	[esp + 44]		; n
	mov	eax,	(1<<16)
	xor	edx,	edx
	idiv	ebx			; eax = rcp = (1<<16) / n
	movd	mm6,	eax
	punpcklwd	mm6,	mm6		; mm6 = 0000|0000|rcp|rcp
	punpcklwd	mm6,	mm6		; mm6 = rcp|rcp|rcp|rcp

	mov	edi,	[esp + 28]		; dest

	mov	ebp,	[esp + 32]		; sum
	movq	mm5,	[ebp]		; mm5 = As|Rs|Gs|Bs

	mov	ebx,	[esp + 36]		; addptr
	mov	ebp,	[esp + 40]		; subptr

	lea	eax,	[edi + ecx*4]	; limit
	mov	esi,	eax		; esi = limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit			; jump if edi >= limit

.ploop2:	; fractions
	movq	mm1,	mm5		; mm1 = As|Rs|Gs|Bs
	paddw	mm1,	mm7		; mm1 = As+HalfN|Rs+HalfN|Gs+HalfN|Bs+HalfN
	pmulhuw	mm1,	mm6		; mm1 = Aavg|Ravg|Gavg|Bavg   (pmulhuw = SSE inst.)
	packuswb	mm1,	mm0

	movd	ecx,	mm1
	shr	ecx,	16
	and	ecx,	0xff00
	add	ecx,	TVPDivTable		; ecx = TVPDivTable[Alpha][]
	movd	edx,	mm1
	and	edx,	0xff000000

	movd	eax,	mm1
	and	eax,	0xff		; eax = 00|00|00|B
	movzx	eax,byte	[ecx+eax]		; look up the table
	add	edx,	eax		; edx = alpha|00|00|[B]

	movd	eax,	mm1
	shr	eax,	8
	and	eax,	0xff		; eax = Gavg
	movzx	eax,byte	[ecx+eax]		; look up the table
	shl	eax,	8		; eax = 00|00|[G]|00
	add	edx,	eax		; edx = alpha|00|[G]|[B]

	movd	eax,	mm1
	shr	eax,	16
	and	eax,	0xff		; eax = Ravg
	movzx	eax,byte	[ecx+eax]		; look up the table
	shl	eax,	16		; eax = 00|[R]|00|00
	add	edx,	eax		; edx = alpha|[R]|[G]|[B]

	mov	[edi],	edx

	paddw	mm5,	[ebx]		; sum += *addptr
	psubw	mm5,	[ebp]		; sum -= *subptr

	add	ebx,	byte 8
	add	ebp,	byte 8
	add	edi,	byte 4

	cmp	edi,	esi
	jb	short .ploop2		; jump if edi < limit

.pexit:
	mov	ebp,	[esp + 32]		; sum
	movq	[ebp],	mm5		; store sum

.pexit2:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret
;--------------------------------------------------------------------


%endif

