; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; stretching copy / linear interpolation / linear transformation copy

%include		"nasm.nah"

globaldef		TVPStretchCopy_mmx_pfraction_a
globaldef		TVPStretchAlphaBlend_mmx_pfraction_a
globaldef		TVPStretchAdditiveAlphaBlend_mmx_pfraction_a
globaldef		TVPStretchConstAlphaBlend_mmx_pfraction_a
globaldef		TVPFastLinearInterpV2_mmx_pfraction_a
globaldef		TVPFastLinearInterpH2F_mmx_pfraction_a
globaldef		TVPFastLinearInterpH2B_mmx_pfraction_a
globaldef		TVPInterpStretchCopy_mmx_pfraction_a
globaldef		TVPInterpStretchConstAlphaBlend_mmx_pfraction_a
globaldef		TVPInterpStretchAdditiveAlphaBlend_mmx_pfraction_a
globaldef		TVPInterpStretchAdditiveAlphaBlend_o_mmx_pfraction_a

;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPStretchCopy
;;void, TVPStretchCopy_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)

	function_align
TVPStretchCopy_mmx_pfraction_a:					; stretch copy
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src
	mov	ebx,	[esp + 36]		; srcstart (srcp)
	mov	edx,	[esp + 40]		; srcstep
	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	sub	ebp,	byte 4*3
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*3
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:
	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16
	movd	mm0,	[eax*4 + esi]		; load to mm0
	movd	[edi],	mm0		; store to dest
	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPStretchConstAlphaBlend
;;void, TVPStretchConstAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa)

	function_align
TVPStretchConstAlphaBlend_mmx_pfraction_a:					; stretch copy with constant-ratio alpha blending
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	pxor	mm0,	mm0		; mm0 = 0
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src
	mov	ebx,	[esp + 36]		; srcstart (srcp)
	mov	edx,	[esp + 40]		; srcstep
	movd	mm7,	[esp + 44]		; opa
	push	ebp
	punpcklwd	mm7,	mm7
	lea	ebp,	[edi+ecx*4]		; limit
	punpcklwd	mm7,	mm7
	sub	ebp,	byte 4*3
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*3
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:
	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm2,	[eax*4 + esi]		; load src to mm2
	movd	mm1,	[edi]		; load dst to mm1
	punpcklbw	mm2,	mm0		; unpack
	punpcklbw	mm1,	mm0		; unpack
	psubw	mm2,	mm1
	pmullw	mm2,	mm7
	psllw	mm1,	8
	paddw	mm1,	mm2
	psrlw	mm1,	8
	packuswb	mm1,	mm0
	movd	[edi],	mm1		; store to dest

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPStretchAlphaBlend
;;void, TVPStretchAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)

	function_align
TVPStretchAlphaBlend_mmx_pfraction_a:					; stretch copy with pixel alpha blending
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	pxor	mm7,	mm7
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src
	mov	ebx,	[esp + 36]		; srcstart (srcp)
	mov	edx,	[esp + 40]		; srcstep
	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	sub	ebp,	byte 4*3
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*3
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:
	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm3,	[eax*4 + esi]
	movq	mm5,	mm3
	psrlq	mm5,	24
	movd	mm4,	[edi]
	punpcklbw	mm3,	mm7
	punpcklbw	mm4,	mm7
	punpcklwd	mm5,	mm5
	punpcklwd	mm5,	mm5
	psubw	mm3,	mm4
	pmullw	mm3,	mm5
	psllw	mm4,	8
	paddw	mm3,	mm4
	psrlw	mm3,	8
	packuswb	mm3,	mm7
	movd	[edi],	mm3

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPStretchAdditiveAlphaBlend
;;void, TVPStretchAdditiveAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)

	function_align
TVPStretchAdditiveAlphaBlend_mmx_pfraction_a:					; stretch copy with pixel additive alpha blending
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	pxor	mm0,	mm0
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src
	mov	ebx,	[esp + 36]		; srcstart (srcp)
	mov	edx,	[esp + 40]		; srcstep
	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	sub	ebp,	byte 4*3
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*3
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:
	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm4,	[eax*4 + esi]		; src
	movq	mm2,	mm4
	psrlq	mm2,	24
	movd	mm1,	[edi]		; dest
	punpcklwd	mm2,	mm2
	punpcklwd	mm2,	mm2
	punpcklbw	mm1,	mm0		; mm1 = 00dd00dd00dd00dd
	movq	mm3,	mm1
	pmullw	mm1,	mm2
	psrlw	mm1,	8
	psubw	mm3,	mm1
	packuswb	mm3,	mm0
	paddusb	mm3,	mm4		; add src
	movd	[edi],	mm3		; store

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPFastLinearInterpV2
;;void, TVPFastLinearInterpV2_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src0, const tjs_uint32 *src1)

	function_align
TVPFastLinearInterpV2_mmx_pfraction_a:					; interpolation between two sources (2x)
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src0
	mov	edx,	[esp + 36]		; src1
	push	ebp
	pxor	mm0,	mm0
	lea	ebp,	[ecx*4]		; limit
	xor	ecx,	ecx
	sub	ebp,	byte 4*3
	cmp	ecx,	ebp

.pfraction:
	add	ebp,	byte 4*3
	cmp	ecx,	ebp
	jge	.pexit		; jump if ecx >= ebp

.ploop2:
	movd	mm1,	[esi + ecx]		; src0
	movd	mm2,	[edx + ecx]		; src1
	punpcklbw	mm1,	mm0		; unpack
	punpcklbw	mm2,	mm0		; unpack
	paddw	mm1,	mm2		; mm1 += mm2
	psrlw	mm1,	1		; mm1 >>= 1
	packuswb	mm1,	mm0		; pack
	movd	[edi + ecx],	mm1		; store
	add	ecx,	byte 4

	cmp	ecx,	ebp
	jl	.ploop2		; jump if ecx < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPFastLinearInterpH2F
;;void, TVPFastLinearInterpH2F_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)

	function_align
TVPFastLinearInterpH2F_mmx_pfraction_a:					; horizontal 2x linear interpolation forward
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	ebx,	[esp + 32]		; src
	push	ebp
	pxor	mm0,	mm0
	lea	ebp,	[ecx*4]		; limit
	xor	ecx,	ecx
	sub	ebp,	byte 4*4
	cmp	ecx,	ebp

.pfraction:
	add	ebp,	byte 4*4
	sub	ebp,	byte 4*2
	cmp	ecx,	ebp

.pfraction2:
	add	ebp,	byte 4*2
	cmp	ecx,	ebp
	jge	.pexit		; jump if ecx >= ebp

.ploop3:
	movd	mm1,	[ebx]		; load
	movd	[edi + ecx],	mm1		; store
	add	ecx,	byte 4
	cmp	ecx,	ebp
	jl	.ploop3		; jump if ecx < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPFastLinearInterpH2B
;;void, TVPFastLinearInterpH2B_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src)

	function_align
TVPFastLinearInterpH2B_mmx_pfraction_a:					; horizontal 2x linear interpolation backward
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	ebx,	[esp + 32]		; src
	push	ebp
	pxor	mm0,	mm0
	lea	ebp,	[ecx*4]		; limit
	xor	ecx,	ecx
	sub	ebp,	4*4
	cmp	ecx,	ebp

.pfraction:
	add	ebp,	byte 4*4
	sub	ebp,	byte 4*2
	cmp	ecx,	ebp

.pfraction2:
	add	ebp,	byte 4*2
	cmp	ecx,	ebp
	jge	.pexit		; jump if ecx >= ebp

.ploop3:
	movd	mm1,	[ebx]		; load
	movd	[edi + ecx],	mm1		; store
	add	ecx,	byte 4
	cmp	ecx,	ebp
	jl	.ploop3		; jump if ecx < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret



;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
; memo
;          s0p0   s0p1
;          s1p0   s1p1
;
;	dst = (s0p0 * (1-bx) + s0p1 * bx ) * (1-by) +
;	      (s1p0 * (1-bx) + s1p1 * bx ) * by

;	dst = (s0p0 - s0p0 * bx + s0p1 * bx ) * (1-by) +
;	      (s1p0 - s1p0 * bx + s1p1 * bx ) * by

;	dst = (s0p0 + s0p1 * bx - s0p0 * bx ) * (1-by) +
;	      (s1p0 + s1p1 * bx - s1p0 * bx ) * by

;	dst = (s0p0 + (s0p1 - s0p0) * bx ) * (1-by) +
;	      (s1p0 + (s1p1 - s1p0) * bx ) * by

;	dst = S0 * (1-by) +
;	      S1 * by

;	dst = S0 - S0 * by + S1 * by

;	dst = S0 + (S1 - S0) * by



;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpStretchCopy
;;void, TVPInterpStretchCopy_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)

	function_align
TVPInterpStretchCopy_mmx_pfraction_a:			; bilinear stretch copy
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2

	mov	eax,	[esp + 40]		; blend_y (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust by
	movd	mm7,	eax
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = bx

	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src1
	mov	ebx,	[esp + 44]		; srcstart (srcp)
	mov	edx,	[esp + 48]		; srcstep
	mov	eax,	[esp + 36]		; src2

	pxor	mm0,	mm0		; mm0 = 0

	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	mov	ecx,	eax		; src2
	sub	ebp,	byte 4*1
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*1
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:

	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm1,	[eax*4+esi]		; load: load s0p0
	movd	mm2,	[eax*4+esi+4]	; load: load s0p1
	movd	mm3,	[eax*4+ecx]		; load: load s1p0
	movd	mm4,	[eax*4+ecx+4]	; load: load s1p1

	mov	eax,	ebx		; bx:
	shr	eax,	8		; bx:
	punpcklbw	mm1,	mm0		; load: unpack s0p0
	and	eax,	0xff		; bx: eax = bx
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	movd	mm5,	eax		; bx:
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklwd	mm5,	mm5		; bx:
	punpcklbw	mm4,	mm0		; load: unpack s1p1
	punpcklwd	mm5,	mm5		; bx: mm5 = bx

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm2,	mm5		; s0:
	pmullw	mm4,	mm5		; s1:
	psrlw	mm2,	8		; s0: mm2 = (s0p1 - s0p0) * bx
	psrlw	mm4,	8		; s1: mm4 = (s1p1 - s1p0) * bx
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1: mm3 = (S1 - S0) * by
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	packuswb	mm1,	mm0		; pack

	movd	[edi],	mm1		; store to dest

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2			; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpStretchConstAlphaBlend
;;void, TVPInterpStretchConstAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)

	function_align
TVPInterpStretchConstAlphaBlend_mmx_pfraction_a:		; bilinear stretch copy with constant-alpha blending
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2

	mov	eax,	[esp + 40]		; blend_y (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust by
	movd	mm7,	eax
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = bx

	mov	eax,	[esp + 52]		; opa (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust opa
	movd	mm6,	eax
	punpcklwd	mm6,	mm6
	punpcklwd	mm6,	mm6		; mm6 = opa

	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src1
	mov	ebx,	[esp + 44]		; srcstart (srcp)
	mov	edx,	[esp + 48]		; srcstep
	mov	eax,	[esp + 36]		; src2

	pxor	mm0,	mm0		; mm0 = 0

	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	mov	ecx,	eax		; src2
	sub	ebp,	byte 4*1
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*1
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:

	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm1,	[eax*4+esi]		; load: load s0p0
	movd	mm2,	[eax*4+esi+4]	; load: load s0p1
	movd	mm3,	[eax*4+ecx]		; load: load s1p0
	movd	mm4,	[eax*4+ecx+4]	; load: load s1p1

	mov	eax,	ebx		; bx:
	shr	eax,	8		; bx:
	punpcklbw	mm1,	mm0		; load: unpack s0p0
	and	eax,	0xff		; bx: eax = bx
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	movd	mm5,	eax		; bx:
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklwd	mm5,	mm5		; bx:
	punpcklbw	mm4,	mm0		; load: unpack s1p1
	punpcklwd	mm5,	mm5		; bx: mm5 = bx

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm2,	mm5		; s0:
	pmullw	mm4,	mm5		; s1:
	psrlw	mm2,	8		; s0:
	psrlw	mm4,	8		; s1:
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1:
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	movd	mm2,	[edi]		; blend dest: load original destination
	punpcklbw	mm2,	mm0		; blend dest: unpack orgdst
	psubw	mm1,	mm2		; blend dest: mm1 = orgdst - dest
	pmullw	mm1,	mm6		; blend dest:
	psrlw	mm1,	8		; blend dest:
	paddb	mm2,	mm1		; blend dest: mm2 = final dst

	packuswb	mm2,	mm0		; pack

	movd	[edi],	mm2		; store to dest

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2			; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpStretchAdditiveAlphaBlend
;;void, TVPInterpStretchAdditiveAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep)

	function_align
TVPInterpStretchAdditiveAlphaBlend_mmx_pfraction_a:		; bilinear stretching additive alpha blend
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2

	mov	eax,	[esp + 40]		; blend_y (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust by
	movd	mm7,	eax
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = bx

	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src1
	mov	ebx,	[esp + 44]		; srcstart (srcp)
	mov	edx,	[esp + 48]		; srcstep
	mov	eax,	[esp + 36]		; src2

	pxor	mm0,	mm0		; mm0 = 0

	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	mov	ecx,	eax		; src2
	sub	ebp,	byte 4*1
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*1
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:

	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm1,	[eax*4+esi]		; load: load s0p0
	movd	mm2,	[eax*4+esi+4]	; load: load s0p1
	movd	mm3,	[eax*4+ecx]		; load: load s1p0
	movd	mm4,	[eax*4+ecx+4]	; load: load s1p1

	mov	eax,	ebx		; bx:
	shr	eax,	8		; bx:
	punpcklbw	mm1,	mm0		; load: unpack s0p0
	and	eax,	0xff		; bx: eax = bx
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	movd	mm5,	eax		; bx:
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklwd	mm5,	mm5		; bx:
	punpcklbw	mm4,	mm0		; load: unpack s1p1
	punpcklwd	mm5,	mm5		; bx: mm5 = bx

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm2,	mm5		; s0:
	pmullw	mm4,	mm5		; s1:
	psrlw	mm2,	8		; s0:
	psrlw	mm4,	8		; s1:
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1:
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	movd	mm2,	[edi]		; addalphablend: load destination
	movq	mm3,	mm1		; addalphablend:
	psrlq	mm3,	48		; addalphablend:
	punpcklbw	mm2,	mm0		; addalphablend:
	punpcklwd	mm3,	mm3		; addalphablend:
	punpcklwd	mm3,	mm3		; addalphablend: mm3 = dst_opa
	movq	mm4,	mm2		; addalphablend:
	pmullw	mm2,	mm3		; addalphablend:
	psrlw	mm2,	8		; addalphablend:
	psubw	mm4,	mm2		; addalphablend:
	paddw	mm1,	mm4		; addalphablend:

	packuswb	mm1,	mm0		; pack

	movd	[edi],	mm1		; store to dest

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2			; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpStretchAdditiveAlphaBlend_o
;;void, TVPInterpStretchAdditiveAlphaBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa)

	function_align
TVPInterpStretchAdditiveAlphaBlend_o_mmx_pfraction_a:		; bilinear stretching additive alpha blend with opacity
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near	.pexit2

	mov	eax,	[esp + 40]		; blend_y (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust by
	movd	mm7,	eax
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = bx

	mov	eax,	[esp + 52]		; opa (0..255)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust opa
	movd	mm6,	eax
	punpcklwd	mm6,	mm6
	punpcklwd	mm6,	mm6		; mm6 = opa

	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 32]		; src1
	mov	ebx,	[esp + 44]		; srcstart (srcp)
	mov	edx,	[esp + 48]		; srcstep
	mov	eax,	[esp + 36]		; src2

	pxor	mm0,	mm0		; mm0 = 0

	push	ebp
	lea	ebp,	[edi+ecx*4]		; limit
	mov	ecx,	eax		; src2
	sub	ebp,	byte 4*1
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4*1
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:

	mov	eax,	ebx		; tmp = srcp
	shr	eax,	16		; tmp >>= 16

	movd	mm1,	[eax*4+esi]		; load: load s0p0
	movd	mm2,	[eax*4+esi+4]	; load: load s0p1
	movd	mm3,	[eax*4+ecx]		; load: load s1p0
	movd	mm4,	[eax*4+ecx+4]	; load: load s1p1

	mov	eax,	ebx		; bx:
	shr	eax,	8		; bx:
	punpcklbw	mm1,	mm0		; load: unpack s0p0
	and	eax,	0xff		; bx: eax = bx
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	movd	mm5,	eax		; bx:
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklwd	mm5,	mm5		; bx:
	punpcklbw	mm4,	mm0		; load: unpack s1p1
	punpcklwd	mm5,	mm5		; bx: mm5 = bx

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm2,	mm5		; s0:
	pmullw	mm4,	mm5		; s1:
	psrlw	mm2,	8		; s0:
	psrlw	mm4,	8		; s1:
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1:
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	pmullw	mm1,	mm6		; addalphablend:
	psrlw	mm1,	8		; dst *= opa

	movd	mm2,	[edi]		; addalphablend: load destination
	movq	mm3,	mm1		; addalphablend:
	psrlq	mm3,	48		; addalphablend:
	punpcklbw	mm2,	mm0		; addalphablend:
	punpcklwd	mm3,	mm3		; addalphablend:
	punpcklwd	mm3,	mm3		; addalphablend: mm3 = dst_opa
	movq	mm4,	mm2		; addalphablend:
	pmullw	mm2,	mm3		; addalphablend:
	psrlw	mm2,	8		; addalphablend:
	psubw	mm4,	mm2		; addalphablend:
	paddw	mm1,	mm4		; addalphablend:

	packuswb	mm1,	mm0		; pack

	movd	[edi],	mm1		; store to dest

	add	ebx,	edx		; srcp += srcstep
	add	edi,	byte 4		; dest ++

	cmp	edi,	ebp
	jb	.ploop2			; jump if edi < ebp

.pexit:
	pop	ebp
.pexit2:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

