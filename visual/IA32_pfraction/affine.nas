; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; stretching copy / linear interpolation / linear transformation copy

%include		"nasm.nah"

globaldef		TVPLinTransCopy_mmx_pfraction_a
globaldef		TVPLinTransConstAlphaBlend_mmx_pfraction_a
globaldef		TVPInterpLinTransCopy_mmx_pfraction_a
globaldef		TVPInterpLinTransConstAlphaBlend_mmx_pfraction_a
globaldef		TVPInterpLinTransAdditiveAlphaBlend_mmx_pfraction_a
globaldef		TVPInterpLinTransAdditiveAlphaBlend_o_mmx_pfraction_a

;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPLinTransCopy
;;void, TVPLinTransCopy_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)

	function_align
TVPLinTransCopy_mmx_pfraction_a:								; linear transforming copy
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
	movd	mm0,	[esp + 36]		; sx
	movd	mm1,	[esp + 40]		; sy
	movd	mm2,	[esp + 44]		; stepx
	movd	mm3,	[esp + 48]		; stepy
	psllq	mm1,	32		; mm1 <<= 32
	psllq	mm3,	32		; mm3 <<= 32
	por	mm0,	mm1		; mm0 = (sy << 32) + sx
	por	mm2,	mm3		; mm2 = (stepy << 32) + stepx
	movq	mm7,	mm0		; mm7 = mm0
	movq	mm4,	mm2		; mm4 = mm2
	paddd	mm4,	mm2		; mm4 *= 2
	push	ebp
	paddd	mm7,	mm2		; mm7 += mm2
	lea	ebp,	[edi+ecx*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:
	movq	mm5,	mm0		; mm5 = sx,sy
	psrad	mm5,	16		; mm5 >>= 16
	movd	ecx,	mm5		; ecx = (sx >> 16)
	psrlq	mm5,	32		; mm5 >>= 32
	movd	eax,	mm5		; eax = (sy >> 16)
	imul	eax,	[esp + 56]		; eax *= srcpitch
	add	eax,	esi		; eax += src
	mov	eax,	[eax + ecx*4]		; load
	mov	[edi],	eax		; store
	paddd	mm0,	mm2		; sx,sy += stepx,stepy
	add	edi,	byte 4

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPLinTransConstAlphaBlend
;;void, TVPLinTransConstAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)


	function_align
TVPLinTransConstAlphaBlend_mmx_pfraction_a:			; linear transforming copy with constant-ratio alpha blending
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
	movd	mm0,	[esp + 36]		; sx
	movd	mm1,	[esp + 40]		; sy
	movd	mm2,	[esp + 44]		; stepx
	movd	mm5,	[esp + 48]		; stepy
	movd	mm3,	[esp + 56]		; opa
	punpcklwd	mm3,	mm3
	punpcklwd	mm3,	mm3
	psllq	mm1,	32		; mm1 <<= 32
	psllq	mm5,	32		; mm5 <<= 32
	por	mm0,	mm1		; mm0 = (sy << 32) + sx
	por	mm2,	mm5		; mm2 = (stepy << 32) + stepx
	movq	mm7,	mm0		; mm7 = mm0
	movq	mm4,	mm2		; mm4 = mm2
	paddd	mm4,	mm2		; mm4 *= 2
	push	ebp
	paddd	mm7,	mm2		; mm7 += mm2
	movq	[esp - 16], mm2		; [esp-16] = tmp
	lea	ebp,	[edi+ecx*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

	movq	mm4, [esp - 16]		; [esp-16] = tmp
	pxor	mm6,	mm6

.ploop2:
	movq	mm5,	mm0		; mm5 = sx,sy
	psrad	mm5,	16		; mm5 >>= 16
	movd	ecx,	mm5		; ecx = (sx >> 16)
	psrlq	mm5,	32		; mm5 >>= 32
	movd	eax,	mm5		; eax = (sy >> 16)
	imul	eax,	[esp + 56]		; eax *= srcpitch
	add	eax,	esi		; eax += src

	movd	mm2,	[eax + ecx*4]		; load src to mm2
	movd	mm1,	[edi]		; load dst to mm1
	punpcklbw	mm2,	mm6		; unpack
	punpcklbw	mm1,	mm6		; unpack
	psubw	mm2,	mm1
	pmullw	mm2,	mm3
	psllw	mm1,	8
	paddw	mm1,	mm2
	psrlw	mm1,	8
	packuswb	mm1,	mm6
	movd	[edi],	mm1		; store to dest

	paddd	mm0,	mm4		; sx,sy += stepx,stepy
	add	edi,	byte 4

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpLinTransCopy
;;void, TVPInterpLinTransCopy_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)

	function_align
TVPInterpLinTransCopy_mmx_pfraction_a:			; bilinear affine copy
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	;		ebp	limit
	;		edi	dest
	;		ecx	sx
	;		edx	sy
	;		esi	srcpitch
	mov	eax,	[esp + 28]		; len
	cmp	eax,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	ecx,	[esp + 36]		; sx
	mov	edx,	[esp + 40]		; sy
	mov	esi,	[esp + 52]		; srcpitch

	pxor	mm0,	mm0		; mm0 = 0000 0000 0000 00000

	push	ebp

	;		now	[esp + 48]	is as	stepx
	;		now	[esp + 52]	is as	stepy
	;		now	[esp + 36]	is as	src

	lea	ebp,	[edi+eax*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:
	mov	eax,	edx		; eax = sy
	shr	eax,	16		; eax = sy>>16
	imul	eax,	esi		; eax = (sy>>16) * srcpitch
	mov	ebx,	ecx		; ebx = sx
	shr	ebx,	16		; ebx = (sx>>16)
	lea	eax,	[eax + ebx*4]	; eax = (sy>>16) * srcpitch + (sx>>16)
	add	eax,	[esp + 36]		; eax = (sy>>16) * srcpitch + (sx>>16) + src

	movd	mm1,	[eax]		; load: load s0p0
	movd	mm2,	[eax+4]		; load: load s0p1
	movd	mm3,	[eax+esi]		; load: load s1p0
	movd	mm4,	[eax+esi+4]		; load: load s1p1

	mov	eax,	ecx		; eax = sx
	mov	ebx,	edx		; ebx = sy
	and	eax,	0xffff		; eax = sx & 0xffff
	and	ebx,	0xffff		; ebx = sy & 0xffff
	shr	eax,	8		; eax = (sx & 0xffff) >> 8
	shr	ebx,	8		; ebx = (sy & 0xffff) >> 8

	movd	mm5,	eax		; mm5 = (sx & 0xffff) >> 8
	punpcklwd	mm5,	mm5		; unpack sx
	punpcklwd	mm5,	mm5		; unpack sx

	mov	eax,	ebx		; adjust sy
	shr	eax,	7		; adjust sy
	add	eax,	ebx		; adjust sy
	movd	mm7,	eax		; mm7 = (sy & 0xffff) >> 8
	punpcklwd	mm7,	mm7		; unpack sy
	punpcklwd	mm7,	mm7		; unpack sy

	punpcklbw	mm1,	mm0		; load: unpack s0p0
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklbw	mm4,	mm0		; load: unpack s1p1

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	pmullw	mm2,	mm5		; s0:
	psrlw	mm2,	8		; s0: mm2 = (s0p1 - s0p0) * bx
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0

	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm4,	mm5		; s1:
	psrlw	mm4,	8		; s1: mm4 = (s1p1 - s1p0) * bx
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1: mm3 = (S1 - S0) * by
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	packuswb	mm1,	mm0		; pack

	movd	[edi],	mm1		; store to dest


	add	ecx,	[esp + 48]		; sx += sxstep
	add	edx,	[esp + 52]		; sy += systep
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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpLinTransConstAlphaBlend
;;void, TVPInterpLinTransConstAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)

	function_align
TVPInterpLinTransConstAlphaBlend_mmx_pfraction_a:		; bilinear affine copy with opacity
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx

	mov	eax,	[esp + 28]		; len
	cmp	eax,	byte 0
	jle	near	.pexit2

	mov	ecx,	[esp + 56]		; opa (0..255)
	mov	ebx,	ecx
	shr	ebx,	7
	add	ecx,	ebx		; adjust opa
	movd	mm6,	ecx
	punpcklwd	mm6,	mm6
	punpcklwd	mm6,	mm6		; mm6 = opa

	mov	edi,	[esp + 24]		; dest
	mov	ecx,	[esp + 36]		; sx
	mov	edx,	[esp + 40]		; sy
	mov	esi,	[esp + 52]		; srcpitch

	pxor	mm0,	mm0		; mm0 = 0000 0000 0000 00000

	push	ebp

	lea	ebp,	[edi+eax*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:
	mov	eax,	edx		; addr: eax = sy
	shr	eax,	16		; addr: eax = sy>>16
	imul	eax,	esi		; addr: eax = (sy>>16) * srcpitch
	mov	ebx,	ecx		; addr: ebx = sx
	shr	ebx,	16		; addr: ebx = (sx>>16)
	lea	eax,	[eax + ebx*4]	; addr: eax = (sy>>16) * srcpitch + (sx>>16)
	add	eax,	[esp + 36]		; addr: eax = (sy>>16) * srcpitch + (sx>>16) + src

	movd	mm1,	[eax]		; load: load s0p0
	movd	mm2,	[eax+4]		; load: load s0p1
	movd	mm3,	[eax+esi]		; load: load s1p0
	movd	mm4,	[eax+esi+4]		; load: load s1p1

	mov	eax,	ecx		; sx_ratio: eax = sx
	and	eax,	0xffff		; sx_ratio: eax = sx & 0xffff
	shr	eax,	8		; sx_ratio: eax = (sx & 0xffff) >> 8
	movd	mm5,	eax		; sx_ratio: mm5 = (sx & 0xffff) >> 8
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx

	mov	ebx,	edx		; sy_ratio: ebx = sy
	and	ebx,	0xffff		; sy_ratio: ebx = sy & 0xffff
	shr	ebx,	8		; sy_ratio: ebx = (sy & 0xffff) >> 8
	mov	eax,	ebx		; sy_ratio: adjust sy
	shr	eax,	7		; sy_ratio: adjust sy
	add	eax,	ebx		; sy_ratio: adjust sy
	movd	mm7,	eax		; sy_ratio: mm7 = (sy & 0xffff) >> 8
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy

	punpcklbw	mm1,	mm0		; load: unpack s0p0
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklbw	mm4,	mm0		; load: unpack s1p1

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	pmullw	mm2,	mm5		; s0:
	psrlw	mm2,	8		; s0: mm2 = (s0p1 - s0p0) * bx
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0

	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm4,	mm5		; s1:
	psrlw	mm4,	8		; s1: mm4 = (s1p1 - s1p0) * bx
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1: mm3 = (S1 - S0) * by
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	movd	mm2,	[edi]		; blend dest: load original destination
	punpcklbw	mm2,	mm0		; blend dest: unpack orgdst
	psubw	mm1,	mm2		; blend dest: mm1 = orgdst - dest
	pmullw	mm1,	mm6		; blend dest:
	psrlw	mm1,	8		; blend dest:
	paddb	mm2,	mm1		; blend dest: mm2 = final dst

	packuswb	mm2,	mm0		; store: pack
	movd	[edi],	mm2		; store: store to dest

	add	ecx,	[esp + 48]		; step: sx += sxstep
	add	edx,	[esp + 52]		; step: sy += systep
	add	edi,	byte 4		; step: dest ++

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpLinTransAdditiveAlphaBlend
;;void, TVPInterpLinTransAdditiveAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)

	function_align
TVPInterpLinTransAdditiveAlphaBlend_mmx_pfraction_a:		; bilinear affine additive alpha blend
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx

	mov	eax,	[esp + 28]		; len
	cmp	eax,	byte 0
	jle	near	.pexit2
	mov	edi,	[esp + 24]		; dest
	mov	ecx,	[esp + 36]		; sx
	mov	edx,	[esp + 40]		; sy
	mov	esi,	[esp + 52]		; srcpitch

	pxor	mm0,	mm0		; mm0 = 0000 0000 0000 00000

	push	ebp

	lea	ebp,	[edi+eax*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:
	mov	eax,	edx		; addr: eax = sy
	shr	eax,	16		; addr: eax = sy>>16
	imul	eax,	esi		; addr: eax = (sy>>16) * srcpitch
	mov	ebx,	ecx		; addr: ebx = sx
	shr	ebx,	16		; addr: ebx = (sx>>16)
	lea	eax,	[eax + ebx*4]	; addr: eax = (sy>>16) * srcpitch + (sx>>16)
	add	eax,	[esp + 36]		; addr: eax = (sy>>16) * srcpitch + (sx>>16) + src

	movd	mm1,	[eax]		; load: load s0p0
	movd	mm2,	[eax+4]		; load: load s0p1
	movd	mm3,	[eax+esi]		; load: load s1p0
	movd	mm4,	[eax+esi+4]		; load: load s1p1

	mov	eax,	ecx		; sx_ratio: eax = sx
	mov	ebx,	edx		; sy_ratio: ebx = sy
	and	eax,	0xffff		; sx_ratio: eax = sx & 0xffff
	and	ebx,	0xffff		; sy_ratio: ebx = sy & 0xffff
	shr	eax,	8		; sx_ratio: eax = (sx & 0xffff) >> 8
	shr	ebx,	8		; sy_ratio: ebx = (sy & 0xffff) >> 8

	movd	mm5,	eax		; sx_ratio: mm5 = (sx & 0xffff) >> 8
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx

	mov	eax,	ebx		; sy_ratio: adjust sy
	shr	eax,	7		; sy_ratio: adjust sy
	add	eax,	ebx		; sy_ratio: adjust sy
	movd	mm7,	eax		; sy_ratio: mm7 = (sy & 0xffff) >> 8
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy

	punpcklbw	mm1,	mm0		; load: unpack s0p0
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklbw	mm4,	mm0		; load: unpack s1p1

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	pmullw	mm2,	mm5		; s0:
	psrlw	mm2,	8		; s0: mm2 = (s0p1 - s0p0) * bx
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0

	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm4,	mm5		; s1:
	psrlw	mm4,	8		; s1: mm4 = (s1p1 - s1p0) * bx
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1: mm3 = (S1 - S0) * by
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

	packuswb	mm1,	mm0		; store: pack

	movd	[edi],	mm1		; store: store to dest

	add	ecx,	[esp + 48]		; step: sx += sxstep
	add	edx,	[esp + 52]		; step: sy += systep
	add	edi,	byte 4		; step: dest ++

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPInterpLinTransAdditiveAlphaBlend_o
;;void, TVPInterpLinTransAdditiveAlphaBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src,  tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa)

	function_align
TVPInterpLinTransAdditiveAlphaBlend_o_mmx_pfraction_a:		; bilinear affine additive alpha blend with opacity
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx

	mov	eax,	[esp + 28]		; len
	cmp	eax,	byte 0
	jle	near	.pexit2

	mov	ecx,	[esp + 56]		; opa (0..255)
	mov	ebx,	ecx
	shr	ebx,	7
	add	ecx,	ebx		; adjust opa
	movd	mm6,	ecx
	punpcklwd	mm6,	mm6
	punpcklwd	mm6,	mm6		; mm6 = opa

	mov	edi,	[esp + 24]		; dest
	mov	ecx,	[esp + 36]		; sx
	mov	edx,	[esp + 40]		; sy
	mov	esi,	[esp + 52]		; srcpitch

	pxor	mm0,	mm0		; mm0 = 0000 0000 0000 00000

	push	ebp

	lea	ebp,	[edi+eax*4]		; limit

.pfraction:
	cmp	edi,	ebp
	jae	near .pexit			; jump if edi >= ebp

.ploop2:
	mov	eax,	edx		; addr: eax = sy
	shr	eax,	16		; addr: eax = sy>>16
	imul	eax,	esi		; addr: eax = (sy>>16) * srcpitch
	mov	ebx,	ecx		; addr: ebx = sx
	shr	ebx,	16		; addr: ebx = (sx>>16)
	lea	eax,	[eax + ebx*4]	; addr: eax = (sy>>16) * srcpitch + (sx>>16)
	add	eax,	[esp + 36]		; addr: eax = (sy>>16) * srcpitch + (sx>>16) + src

	movd	mm1,	[eax]		; load: load s0p0
	movd	mm2,	[eax+4]		; load: load s0p1
	movd	mm3,	[eax+esi]		; load: load s1p0
	movd	mm4,	[eax+esi+4]		; load: load s1p1

	mov	eax,	ecx		; sx_ratio: eax = sx
	mov	ebx,	edx		; sy_ratio: ebx = sy
	and	eax,	0xffff		; sx_ratio: eax = sx & 0xffff
	and	ebx,	0xffff		; sy_ratio: ebx = sy & 0xffff
	shr	eax,	8		; sx_ratio: eax = (sx & 0xffff) >> 8
	shr	ebx,	8		; sy_ratio: ebx = (sy & 0xffff) >> 8

	movd	mm5,	eax		; sx_ratio: mm5 = (sx & 0xffff) >> 8
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx
	punpcklwd	mm5,	mm5		; sx_ratio: unpack sx

	mov	eax,	ebx		; sy_ratio: adjust sy
	shr	eax,	7		; sy_ratio: adjust sy
	add	eax,	ebx		; sy_ratio: adjust sy
	movd	mm7,	eax		; sy_ratio: mm7 = (sy & 0xffff) >> 8
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy
	punpcklwd	mm7,	mm7		; sy_ratio: unpack sy

	punpcklbw	mm1,	mm0		; load: unpack s0p0
	punpcklbw	mm2,	mm0		; load: unpack s0p1
	punpcklbw	mm3,	mm0		; load: unpack s1p0
	punpcklbw	mm4,	mm0		; load: unpack s1p1

	psubw	mm2,	mm1		; s0: mm2 = s0p1 - s0p0
	pmullw	mm2,	mm5		; s0:
	psrlw	mm2,	8		; s0: mm2 = (s0p1 - s0p0) * bx
	paddb	mm1,	mm2		; s0: mm1 = s0p0 + (s0p1 - s0p0) * bx = S0

	psubw	mm4,	mm3		; s1: mm4 = s1p1 - s1p0
	pmullw	mm4,	mm5		; s1:
	psrlw	mm4,	8		; s1: mm4 = (s1p1 - s1p0) * bx
	paddb	mm3,	mm4		; s1: mm3 = s1p0 + (s1p1 - s1p0) * bx = S1

	psubw	mm3,	mm1		; s0/s1: mm3 = S1 - S0
	pmullw	mm3,	mm7		; s0/s1:
	psrlw	mm3,	8		; s0/s1: mm3 = (S1 - S0) * by
	paddb	mm1,	mm3		; s0/s1: mm1 = S0 + (S1 - S0) * by = dst

	pmullw	mm1,	mm6		; addalphablend:
	psrlw	mm1,	8		; addalphablend: dst *= opa

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

	packuswb	mm1,	mm0		; store: pack

	movd	[edi],	mm1		; store: store to dest

	add	ecx,	[esp + 48]		; step: sx += sxstep
	add	edx,	[esp + 52]		; step: sy += systep
	add	edi,	byte 4		; step: dest ++

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
