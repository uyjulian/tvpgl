; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; pixel additive blender

%include		"nasm.nah"


globaldef		TVPAddBlend_mmx_pfraction_a
globaldef		TVPAddBlend_HDA_mmx_pfraction_a
globaldef		TVPAddBlend_o_mmx_pfraction_a
globaldef		TVPAddBlend_HDA_o_mmx_pfraction_a


	segment_code
;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddBlend
;;void, TVPAddBlend_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
	function_align
TVPAddBlend_mmx_pfraction_a:			; pixel additive blender
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm0,	[edi]		; dest
	movd	mm1,	[ebp]		; src
	paddusb	mm0,	mm1		; dest += src
	movd	[edi],	mm0		; store
	add	edi,	byte 4
	add	ebp,	byte 4
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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddBlend_HDA
;;void, TVPAddBlend_HDA_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
	function_align
TVPAddBlend_HDA_mmx_pfraction_a:			; pixel additive blender (holding desitination alpha)
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edx,	000ffffffh
	movd	mm7,	edx
	punpckldq	mm7,	mm7		; mm7 = 0x00ffffff00ffffff
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm0,	[edi]		; dest
	movd	mm1,	[ebp]		; src
	pand	mm1,	mm7		; mask
	paddusb	mm0,	mm1		; dest += src
	movd	[edi],	mm0		; store
	add	edi,	byte 4
	add	ebp,	byte 4
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

; the routine below always holds the destination alpha

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddBlend_o
;;void, TVPAddBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAddBlend_HDA_o
;;void, TVPAddBlend_HDA_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
	function_align
TVPAddBlend_o_mmx_pfraction_a:			; pixel additive blender
TVPAddBlend_HDA_o_mmx_pfraction_a:		; pixel additive blender
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	movd	mm7,	[esp + 40]		; opa
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = 00oo00oo00oo00oo
	psrlq	mm7,	16		; mm7 = 000000oo00oo00oo
	pxor	mm0,	mm0		; mm0 = 0
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm1,	[edi]		; dest
	punpcklbw	mm1,	mm0		; mm1 = 00dd00dd00dd00dd
	movd	mm2,	[ebp]		; src
	punpcklbw	mm2,	mm0		; unpack mm2 = 00ss00ss00ss00ss
	pmullw	mm2,	mm7		; mm2 *= opa
	psrlw	mm2,	8		; mm2 >>=8
	paddw	mm1,	mm2		; mm1 += mm2
	packuswb	mm1,	mm0		; pack
	movd	[edi],	mm1		; store
	add	edi,	byte 4
	add	ebp,	byte 4
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
