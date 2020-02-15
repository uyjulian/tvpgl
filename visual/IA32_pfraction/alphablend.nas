; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; pixel alpha blender

%ifndef GEN_CODE

%include		"nasm.nah"

globaldef		TVPAlphaBlend_mmx_pfraction_a
globaldef		TVPAlphaBlend_emmx_pfraction_a
globaldef		TVPAlphaBlend_o_mmx_pfraction_a
globaldef		TVPAlphaBlend_o_emmx_pfraction_a
globaldef		TVPAlphaBlend_HDA_mmx_pfraction_a
globaldef		TVPAlphaBlend_HDA_emmx_pfraction_a
globaldef		TVPAlphaBlend_d_mmx_pfraction_a
globaldef		TVPAlphaBlend_d_emmx_pfraction_a
globaldef		TVPConstAlphaBlend_mmx_pfraction_a
globaldef		TVPConstAlphaBlend_emmx_pfraction_a
globaldef		TVPConstAlphaBlend_SD_mmx_pfraction_a
globaldef		TVPConstAlphaBlend_SD_emmx_pfraction_a
externdef		TVPOpacityOnOpacityTable
externdef		TVPNegativeMulTable


%define GEN_CODE

;--------------------------------------------------------------------
; MMX stuff
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend
;;void, TVPAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_name TVPAlphaBlend_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_o
;;void, TVPAlphaBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPAlphaBlend_o_name TVPAlphaBlend_o_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_HDA
;;void, TVPAlphaBlend_HDA_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_HDA_name TVPAlphaBlend_HDA_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_d
;;void, TVPAlphaBlend_d_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_d_name TVPAlphaBlend_d_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConstAlphaBlend
;;void, TVPConstAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPConstAlphaBlend_name TVPConstAlphaBlend_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConstAlphaBlend_SD
;;void, TVPConstAlphaBlend_SD_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)
%define TVPConstAlphaBlend_SD_name TVPConstAlphaBlend_SD_mmx_pfraction_a
;--------------------------------------------------------------------
	%include "alphablend.nas"
;--------------------------------------------------------------------


;--------------------------------------------------------------------
; EMMX stuff
;--------------------------------------------------------------------
%define USE_EMMX
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend
;;void, TVPAlphaBlend_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_name TVPAlphaBlend_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_o
;;void, TVPAlphaBlend_o_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPAlphaBlend_o_name TVPAlphaBlend_o_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_HDA
;;void, TVPAlphaBlend_HDA_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_HDA_name TVPAlphaBlend_HDA_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPAlphaBlend_d
;;void, TVPAlphaBlend_d_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAlphaBlend_d_name TVPAlphaBlend_d_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPConstAlphaBlend
;;void, TVPConstAlphaBlend_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPConstAlphaBlend_name TVPConstAlphaBlend_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPConstAlphaBlend_SD
;;void, TVPConstAlphaBlend_SD_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa)
%define TVPConstAlphaBlend_SD_name TVPConstAlphaBlend_SD_emmx_pfraction_a
;--------------------------------------------------------------------
	%include "alphablend.nas"
;--------------------------------------------------------------------


%else

;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
	function_align
TVPAlphaBlend_name:
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
	pxor	mm0,	mm0		; mm0 = 0
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	mov	eax,	[ebp]		; src
	cmp	eax,	0ff000000h
	IF	ae
		; totally opaque
		mov	[edi],	eax	; intact copy
	ELSE
		; not totally opaque
		movd	mm2,	eax
		shr	eax,	24		; eax >>= 24
		movd	mm1,	[edi]		; dest
		movd	mm4,	eax
		punpcklbw	mm2,	mm0		; unpack
		punpcklwd	mm4,	mm4		; unpack
		punpcklbw	mm1,	mm0		; unpack
		punpcklwd	mm4,	mm4		; unpack
		psubw	mm2,	mm1		; mm2 -= mm1
		pmullw	mm2,	mm4		; mm2 *= mm4
		psrlw	mm2,	8		; mm2 >>= 8
		paddb	mm1,	mm2		; mm1 += mm2
		packuswb	mm1,	mm0		; pack
		movd	[edi],	mm1		; store
	ENDIF
	add	edi,	byte 4
	add	ebp,	byte 4
	cmp	edi,	esi

	jb	.ploop2		; jump if edi < esi

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
TVPAlphaBlend_o_name:
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
	movd	mm6,	[esp + 40]		; opa
	punpckldq	mm6,	mm6		; mm6 |= (mm6 << 32)
	pxor	mm0,	mm0		; mm0 = 0
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

	mov	ebx,	[esp + 40]		; opa

.ploop2:	; fractions
	mov	eax,	[ebp]		; src
	movd	mm2,	eax
	mul	ebx		; edx:eax = eax * ebx
	movd	mm1,	[edi]		; dest
	movd	mm4,	edx
	punpcklbw	mm2,	mm0		; unpack
	punpcklwd	mm4,	mm4		; unpack
	punpcklbw	mm1,	mm0		; unpack
	punpcklwd	mm4,	mm4		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm1,	mm2		; mm1 += mm2
	psrlw	mm1,	8
	packuswb	mm1,	mm0		; pack
	movd	[edi],	mm1		; store
	add	edi,	byte 4
	add	ebp,	byte 4
	cmp	edi,	esi

	jb	.ploop2		; jump if edi < esi

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
TVPAlphaBlend_HDA_name:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 32]		; len
	dec	ecx
	js	.pexit
	shl	ecx,	2
	mov	edi,	[esp + 24]		; dest
	mov	esi,	[esp + 28]		; src
	pxor	mm0,	mm0		; mm0 = 0

	loop_align
.ploop:
	mov	eax,	[esi + ecx]		; src
%ifdef	USE_EMMX
	prefetcht0	[esi + ecx - 8]
%endif
	mov	edx,	eax
	shr	eax,	24		; eax >>= 24
	movd	mm4,	eax
	movd	mm2,	edx
	punpcklwd	mm4,	mm4		; unpack
	mov	eax,	[edi + ecx]		; dest
	movd	mm1,	eax
	punpcklbw	mm2,	mm0		; unpack
	punpcklbw	mm1,	mm0		; unpack
	punpcklwd	mm4,	mm4		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	and	eax,	0ff000000h		; mask alpha
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm1,	mm2		; mm1 += mm2
	psrlw	mm1,	8
	packuswb	mm1,	mm0		; pack
	movd	edx,	mm1
	and	edx,	0ffffffh		; mask color
	or	edx,	eax		; store destination
	mov	[edi + ecx], edx		; store

	sub	ecx,	byte 4
	jns	.ploop

.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------
	function_align
TVPAlphaBlend_d_name:
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
	mov	eax,	0ffffffh
	movd	mm7,	eax		; mm7 = 0ffffffh
	pxor	mm0,	mm0		; mm0 = 0
	lea	esi,	[edi + ecx*4]		; limit

.pfraction:
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	mov	eax,	[ebp]		; src
	mov	ebx,	[edi]		; dest
	movd	mm2,	eax		; src
	movd	mm1,	ebx		; dest
	pand	mm2,	mm7		; mask
	pand	mm1,	mm7		; mask
	shr	eax,	16
	shr	ebx,	24
	and	eax,	0ff00h
	add	eax,	ebx
	punpcklbw	mm1,	mm0		; unpack
	movzx	ebx,	byte [TVPNegativeMulTable + eax]		; dest opa table
	movzx	eax,	byte [TVPOpacityOnOpacityTable + eax]		; blend opa table
	punpcklbw	mm2,	mm0		; unpack
	shl	ebx,	24
	movd	mm4,	eax
	punpcklwd	mm4,	mm4		; unpack
	punpcklwd	mm4,	mm4		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm1,	mm2		; mm1 += mm2
	psrlw	mm1,	8
	packuswb	mm1,	mm0		; pack
	movd	eax,	mm1		; store
	or	eax,	ebx
	mov	[edi],	eax
	add	edi,	byte 4
	add	ebp,	byte 4

	cmp	edi,	esi
	jb	near .ploop2

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
TVPConstAlphaBlend_name:
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
	mov	esi,	[esp + 32]		; src
	movd	mm4,	[esp + 40]		; opa
	xor	ebx,	ebx		; counter
	pxor	mm0,	mm0		; mm0 = 0
	lea	ebp,	[ecx*4]		; limit
	punpcklwd	mm4,	mm4		; unpack
	punpcklwd	mm4,	mm4		; unpack
	movq	mm3,	mm4

.pfraction:
	cmp	ebx,	ebp
	jge	.pexit		; jump if ebx >= ebp

.ploop2:	; fractions
	movd	mm2,	[esi+ebx]		; src
	movd	mm1,	[edi+ebx]		; dest
	punpcklbw	mm2,	mm0		; unpack
	punpcklbw	mm1,	mm0		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	add	ebx,	byte 4
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm1,	mm2		; mm1 += mm2
	cmp	ebx,	ebp
	psrlw	mm1,	8
	packuswb	mm1,	mm0		; pack
	movd	[edi-4+ebx], mm1		; store

	jl	.ploop2		; jump if ebx < ebp

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
TVPConstAlphaBlend_SD_name:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 40]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 28]		; dest
	mov	esi,	[esp + 36]		; src2
	mov	eax,	[esp + 32]		; src1
	movd	mm4,	[esp + 44]		; opa
	pxor	mm0,	mm0		; mm0 = 0
	xor	ebx,	ebx		; counter
	lea	ebp,	[ecx*4]		; limit
	punpcklwd	mm4,	mm4		; unpack
	punpcklwd	mm4,	mm4		; unpack

.pfraction:
	cmp	ebx,	ebp
	jge	.pexit		; jump if ebx >= ebp

.ploop2:	; fractions
	movd	mm2,	[esi+ebx]		; src1
	movd	mm1,	[eax+ebx]		; src2
	punpcklbw	mm2,	mm0		; unpack
	punpcklbw	mm1,	mm0		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm1,	mm2		; mm1 += mm2
	psrlw	mm1,	8
	add	ebx,	byte 4
	packuswb	mm1,	mm0		; pack
	cmp	ebx,	ebp
	movd	[edi-4+ebx], mm1		; store

	jl	.ploop2		; jump if ebx < ebp

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
%endif
