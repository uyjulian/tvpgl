; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; color filler/blender

%include		"nasm.nah"


globaldef		TVPFillARGB_mmx_pfraction_a
globaldef		TVPFillARGB_sse_pfraction_a
globaldef		TVPFillARGB_NC_sse_pfraction_a
globaldef		TVPFillColor_mmx_pfraction_a
globaldef		TVPConstColorAlphaBlend_mmx_pfraction_a
globaldef		TVPConstColorAlphaBlend_d_mmx_pfraction_a
globaldef		TVPConstColorAlphaBlend_a_mmx_pfraction_a
externdef		TVPOpacityOnOpacityTable
externdef		TVPNegativeMulTable

	segment_code
;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPFillARGB
;;void, TVPFillARGB_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value)

	function_align
TVPFillARGB_mmx_pfraction_a:						; fill destination
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	.pexit
	mov	edi,	[esp + 24]		; dest
	lea	ebx,	[edi + ecx * 4]		; limit
	mov	eax,	[esp + 32]		; value

.pfill:
	sub	ebx,	byte 15*4
	cmp	edi,	ebx

; fill fraction
.pfraction:
	add	ebx,	byte 15*4
	cmp	edi,	ebx
	jae	.pexit

.ploop2:
	mov	[edi],	eax
	add	edi,	byte 4
	cmp	edi,	ebx
	jb	.ploop2


.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_SSE] TVPFillARGB
;;void, TVPFillARGB_sse_pfraction_a, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value)

	function_align
TVPFillARGB_sse_pfraction_a:						; fill destination
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	.pexit
	mov	edi,	[esp + 24]		; dest
	lea	ebx,	[edi + ecx * 4]		; limit
	mov	eax,	[esp + 32]		; value

; fill 64-bytes ( 16-dwords ) at one loop unit
.pfill:
	sub	ebx,	byte 15*4
	cmp	edi,	ebx

; fill fraction
.pfraction:
	add	ebx,	byte 15*4
	cmp	edi,	ebx
	jae	.pexit

.ploop2:
	mov	[edi],	eax
	add	edi,	byte 4
	cmp	edi,	ebx
	jb	.ploop2


.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	ret

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_SSE] TVPFillARGB_NC
;;void, TVPFillARGB_NC_sse_pfraction_a, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value)

	function_align
TVPFillARGB_NC_sse_pfraction_a:						; fill destination (non-cached version)
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	.pexit
	mov	edi,	[esp + 24]		; dest
	lea	ebx,	[edi + ecx * 4]		; limit
	mov	eax,	[esp + 32]		; value

; fill 64-bytes ( 16-dwords ) at one loop unit
.pfill:
	sub	ebx,	byte 15*4
	cmp	edi,	ebx

; fill fraction
.pfraction:
	add	ebx,	byte 15*4
	cmp	edi,	ebx
	jae	.pexit

.ploop2:
	mov	[edi],	eax
	add	edi,	byte 4
	cmp	edi,	ebx
	jb	.ploop2


.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	ret

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPFillColor
;;void, TVPFillColor_mmx_pfraction_a, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color)

	function_align
TVPFillColor_mmx_pfraction_a:						; fill destination's color ( opacity will be still intact )
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	ecx,	[esp + 28]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 24]		; dest
	lea	ebx,	[edi + ecx * 4]		; limit
	mov	eax,	[esp + 32]		; color
	and	eax,	0ffffffh

; fill 64-bytes ( 16-dwords ) at once
.pfill:
	sub	ebx,	byte 15*4
	cmp	edi,	ebx

; fill fraction
.pfraction:
	add	ebx,	byte 15*4
	cmp	edi,	ebx
	jae	.pexit

.ploop2:
	mov	edx,	[edi]
	and	edx,	0ff000000h
	or	edx,	eax
	mov	[edi],	edx
	add	edi,	byte 4
	cmp	edi,	ebx
	jb	.ploop2

.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConstColorAlphaBlend
;;void, TVPConstColorAlphaBlend_mmx_pfraction_a,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)

; /* this function always holds desitination alpha channel */

	function_align
TVPConstColorAlphaBlend_mmx_pfraction_a:				; constant ratio constant color alpha blender
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 32]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 28]		; dest
	mov	edx,	0ff000000h
	movd	mm6,	edx
	punpckldq	mm6,	mm6
	mov	eax,	[esp + 36]		; color
	mov	edx,	000ffffffh
	movd	mm7,	edx
	punpckldq	mm7,	mm7
	pxor	mm0,	mm0		; mm0 = 0
	movd	mm1,	eax
	punpcklbw	mm1,	mm0		; unpack color
	mov	ebx,	[esp + 40]		; opacity
	movd	mm2,	ebx
	punpcklwd	mm2,	mm2		; unpack
	punpcklwd	mm2,	mm2		; unpack
	not	ebx
	and	ebx,	0ffh
	pmullw	mm1,	mm2		; mm1 *= mm2
	movd	mm2,	ebx
	punpcklwd	mm2,	mm2		; unpack
	punpcklwd	mm2,	mm2		; unpack
	lea	ebp,	[edi + ecx*4]		; limit
	sub	ebp,	byte 8		; 2*4
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 8
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:	; fractions
	movd	mm3,	[edi]		; src
	movq	mm5,	mm3
	pand	mm5,	mm6		; mm5 = dest opa
	punpcklbw	mm3,	mm0		; unpack
	pmullw	mm3,	mm2		; mm3 *= mm2
	paddusw	mm3,	mm1		; mm3 += mm1
	psrlw	mm3,	8		; mm3 >>= 8
	packuswb	mm3,	mm0		; pack
	pand	mm3,	mm7		; mask
	por	mm3,	mm5		; bind dest opa
	movd	[edi],	mm3		; store
	add	edi,	byte 4

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConstColorAlphaBlend_d
;;void, TVPConstColorAlphaBlend_d_mmx_pfraction_a,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)

	function_align
TVPConstColorAlphaBlend_d_mmx_pfraction_a:	; constant ratio constant color alpha blender with destination alpha
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 32]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 28]		; dest
	mov	eax,	[esp + 36]		; color
	lea	ebp,	[edi + ecx*4]		; limit
	pxor	mm0,	mm0		; mm0 = 0
	and	eax,	0ffffffh		; mask
	movd	mm7,	eax
	punpcklbw	mm7,	mm0		; unpack color
	mov	esi,	[esp + 40]		; opacity
	shl	esi,	8		; esi <<= 8
	sub	ebp,	byte 4		; 1*4
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 4
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:	; fractions
	mov	edx,	[edi]		; dest
	mov	ebx,	edx		; dest
	and	ebx,	0ffffffh		; mask
	movd	mm1,	ebx		; dest
	punpcklbw	mm1,	mm0		; unpack
	movq	mm2,	mm7		; color
	shr	edx,	24		; edx >>= 24
	movzx	eax,	byte [TVPOpacityOnOpacityTable + esi + edx]		; blend opa table
	mov	cl,	[TVPNegativeMulTable + esi + edx]		; dest opa table
	shl	ecx,	24		; ecx <<= 24
	movd	mm4,	eax		; blend opacity
	punpcklwd	mm4,	mm4		; unpack
	punpcklwd	mm4,	mm4		; unpack
	psubw	mm2,	mm1		; mm2 -= mm1
	pmullw	mm2,	mm4		; mm2 *= mm4
	psllw	mm1,	8
	paddw	mm2,	mm1		; mm2 += mm1
	psrlw	mm2,	8
	packuswb	mm2,	mm0		; pack
	movd	eax,	mm2		; store
	or	eax,	ecx		; dest opacity
	mov	[edi],	eax		; store
	add	edi,	byte 4

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConstColorAlphaBlend_a
;;void, TVPConstColorAlphaBlend_a_mmx_pfraction_a,  (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa)

;		Di = Di - SaDi + Si
;		Da = Da - SaDa + Sa


	function_align
TVPConstColorAlphaBlend_a_mmx_pfraction_a:	; constant ratio constant color alpha blender with destination additive alpha
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 32]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 28]		; dest
	lea	ebp,	[edi + ecx*4]		; limit

	mov	eax,	[esp + 36]		; color    (Si)
	mov	esi,	[esp + 40]		; opacity  (Sa)

	mov	ebx,	esi
	shr	ebx,	7
	add	esi,	ebx		; adjust opacity

	pxor	mm0,	mm0		; mm0 = 0

	movd	mm7,	esi
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = 00 Sa 00 Sa 00 Sa 00 Sa

	shl	esi,	24

	movd	mm1,	esi
	punpcklbw	mm1,	mm0		; mm1 = 00 Sa 00 00 00 00 00 00

	and	eax,	0xffffff

	movd	mm6,	eax
	punpcklbw	mm6,	mm0		; mm6 = 00 00 00 Si 00 Si 00 Si
	pmullw	mm6,	mm7
	psrlw	mm6,	8
	por	mm6,	mm1		; mm6 = 00 Sa 00 Si 00 Si 00 Si

	sub	ebp,	byte 8		; 2*4
	cmp	edi,	ebp

.pfraction:
	add	ebp,	byte 8
	cmp	edi,	ebp
	jae	.pexit		; jump if edi >= ebp

.ploop2:	; fractions

	movd	mm1,	[edi]		; dest     (DaDiDiDi)
	punpcklbw	mm1,	mm0		; mm1 = 00 Da 00 Di 00 Di 00 Di
	movq	mm2,	mm1
	pmullw	mm2,	mm7
	psrlw	mm2,	8		; mm2 = 00 SaDa 00 SaDi 00 SaDi 00 SaDi
	psubw	mm1,	mm2
	paddw	mm1,	mm6
	packuswb	mm1,	mm0
	movd	[edi],	mm1		; store

	add	edi,	byte 4

	cmp	edi,	ebp
	jb	.ploop2		; jump if edi < ebp

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret

