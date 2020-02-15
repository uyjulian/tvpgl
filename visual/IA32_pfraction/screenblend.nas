; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; pixel screen multiplicative blender

%include		"nasm.nah"


globaldef		TVPScreenBlend_mmx_pfraction_a
globaldef		TVPScreenBlend_HDA_mmx_pfraction_a
globaldef		TVPScreenBlend_o_mmx_pfraction_a
globaldef		TVPScreenBlend_HDA_o_mmx_pfraction_a


;--------------------------------------------------------------------

	segment_data

	align 16
		dd	0ffffffffh
		dd	0ffffffffh
		dd	0ffffffffh
		dd	0ffffffffh

	align 16
TVPScreenMulBlend_full_bit_aligned	dd	0ffffffffh
		dd	0ffffffffh

TVPScreenBlendHDA_alphamask			dd	0ff000000h
		dd	0ff000000h
TVPScreenBlendHDA_mulmask			dd	000ffffffh
		dd	000ffffffh
TVPScreenBlendHDA_mul_100bit		dd	000000000h
		dd	001000000h


	segment_code
;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPScreenBlend
;;void, TVPScreenBlend_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
	function_align
TVPScreenBlend_mmx_pfraction_a:			; pixel screen multiplicative blender
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	movq	mm6,	[TVPScreenMulBlend_full_bit_aligned] 	; load 0xffffffffffffffff
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 16		; 4*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 16
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm1,	[edi]		; dest
	pxor	mm1,	mm6		; not dest
	punpcklbw	mm1,	mm0		; 
	movd	mm3,	[ebp]		; src
	pxor	mm3,	mm6		; not src
	punpcklbw	mm3,	mm0		; 
	pmullw	mm1,	mm3		; multiply
	psrlw	mm1,	8		; shift
	packuswb	mm1,	mm0		; pack
	pxor	mm1,	mm6		; not result
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

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPScreenBlend_HDA
;;void, TVPScreenBlend_HDA_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)

	segment_code

	function_align
TVPScreenBlend_HDA_mmx_pfraction_a:			; pixel screen multiplicative blender (holding desitination alpha)
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0
	movq	mm6,	[TVPScreenBlendHDA_mul_100bit]       	; load 0x0100000000000000
	movq	mm7,	[TVPScreenBlendHDA_mulmask]          	; load 0x00ffffff00ffffff
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 16		; 4*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 16
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm1,	[edi]		; dest
	pxor	mm1,	mm7		; not dest
	punpcklbw	mm1,	mm0		; 
	movd	mm3,	[ebp]		; src
	pxor	mm3,	mm7		; not src
	punpcklbw	mm3,	mm0		; 
	por	mm3,	mm6		; 1 or-1
	pmullw	mm1,	mm3		; multiply
	psrlw	mm1,	8		; shift
	packuswb	mm1,	mm0		; pack
	pxor	mm1,	mm7		; not dest
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

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPScreenBlend_o
;;void, TVPScreenBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)

	segment_code

	function_align
TVPScreenBlend_o_mmx_pfraction_a:			; pixel screen multiplicative blender with opacity
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	movd	mm5,	[esp + 40]		; opa
	punpcklwd	mm5,	mm5
	punpcklwd	mm5,	mm5		; mm5 = 00oo00oo00oo00oo
	pxor	mm0,	mm0
	movq	mm6,	[TVPScreenMulBlend_full_bit_aligned]
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 16		; 4*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 16
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm1,	[edi]		; dest
	pxor	mm1,	mm6		; not dest
	punpcklbw	mm1,	mm0		; 
	movd	mm3,	[ebp]		; src
	punpcklbw	mm3,	mm0		; 
	pmullw	mm3,	mm5		; 1 opa multiply
	pxor	mm3,	mm6		; 1 not src
	psrlw	mm3,	8		; 1 opa shift
	pmullw	mm1,	mm3		; multiply
	psrlw	mm1,	8		; shift
	packuswb	mm1,	mm0		; pack
	pxor	mm1,	mm6		; not result
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

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPScreenBlend_HDA_o
;;void, TVPScreenBlend_HDA_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)

	segment_code

	function_align
TVPScreenBlend_HDA_o_mmx_pfraction_a:			; pixel screen multiplicative blender with opacity (HDA)
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	movd	mm5,	[esp + 40]		; opa
	punpcklwd	mm5,	mm5
	punpcklwd	mm5,	mm5		; mm5 = 00oo00oo00oo00oo
	pxor	mm0,	mm0
	lea	edx,	[TVPScreenMulBlend_full_bit_aligned] 	; load 0xffffffffffffffff
	and	edx,	0fffffff0h
	movq	mm6,	[TVPScreenBlendHDA_alphamask]    	; load 0xff000000ff000000
	movq	mm7,	[TVPScreenBlendHDA_mulmask]      	; load 0x00ffffff00ffffff
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 16		; 4*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 16
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm1,	[edi]		; dest
	pxor	mm1,	mm7		; not dest
	punpcklbw	mm1,	mm0		; 
	movd	mm3,	[ebp]		; src
	punpcklbw	mm3,	mm0		; 
	pmullw	mm3,	mm5		; opa multiply
	pxor	mm3,	[edx]		; 1 not src
	psrlw	mm3,	8		; opa shift
	pand	mm3,	mm6		; mask
	por	mm3,	mm7		; 
	pmullw	mm1,	mm3		; multiply
	psrlw	mm1,	8		; shift
	movd	mm3,	[edi]		; dest
	packuswb	mm1,	mm0		; pack
	pand	mm3,	mm6		; mask
	pxor	mm1,	mm7		; not result
	pand	mm1,	mm7		; drop result alpha
	por	mm1,	mm3		; restore dest alpha
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

