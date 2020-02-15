; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; pixel format conversion

%include		"nasm.nah"


globaldef		TVPConvert24BitTo32Bit_mmx_a
globaldef		TVPBLConvert24BitTo32Bit_mmx_a
globaldef		TVPDither32BitTo16Bit565_mmx_a
globaldef		TVPDither32BitTo16Bit555_mmx_a
externdef		TVPDitherTable_5_6

	segment_code
;--------------------------------------------------------------------

; these two are actually the same

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPConvert24BitTo32Bit
;;void, TVPConvert24BitTo32Bit_mmx_a, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPBLConvert24BitTo32Bit
;;void, TVPBLConvert24BitTo32Bit_mmx_a, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len)


	function_align
TVPConvert24BitTo32Bit_mmx_a:					; 24bpp RGB -> 32bpp ARGB
TVPBLConvert24BitTo32Bit_mmx_a:					; 24bpp RGB -> 32bpp ARGB
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	esi,	[esp + 32]		; len
	cmp	esi,	byte 0
	jle	near .pexit
	mov	edi,	[esp + 24]		; dest
	mov	ecx,	[esp + 28]		; buf
	lea	ebx,	[edi + esi *4]		; ebx = limit

	sub	ebx,	byte 28		; 28 = 7*4
	cmp	edi,	ebx

.pfraction:
	add	ebx,	byte 28
	cmp	edi,	ebx
	jae	.pexit		; jump if edi >= ebx

.ploop2:
	movzx	eax,	byte [ecx]		; b
	movzx	edx,	byte [ecx+1]		; g
	shl	edx,	8
	add	eax,	edx
	movzx	edx,	byte [ecx+2]		; r
	shl	edx,	16
	add	eax,	edx
	or	eax,	0ff000000h		; opaque
	mov	[edi],	eax		; store

	add	edi,	byte 4
	add	ecx, 	byte 3
	cmp	edi,	ebx
	jb	.ploop2		; jump if edi < ebx

.pexit:
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


;--------------------------------------------------------------------

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDither32BitTo16Bit565
;;void, TVPDither32BitTo16Bit565_mmx_a, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)

	function_align
TVPDither32BitTo16Bit565_mmx_a:					; 32bpp (A)RGB -> 16bpp with ordered dithering
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
	lea	esi,	[ebp + ecx *4]		; esi = limit
	mov	eax,	[esp + 44]		; yofs
	and	eax,	3
	shl	eax,	11		; eax *= 4*2*256
	lea	edx,	[TVPDitherTable_5_6+eax] 	; line
	mov	ecx,	[esp + 40]		; xofs
	and	ecx,	3
	shl	ecx,	9		; ecx *= 2*256

	sub	esi,	byte 12		; 12 = 3*4
	cmp	ebp,	esi

.pfraction:
	add	esi,	byte 12
	cmp	ebp,	esi
	jae	.pexit		; jump if ebp >= esi

.ploop2:
	add	edx,	ecx
	movzx	eax,	byte [ebp+2]		; r
	movzx	ebx,	byte [eax+edx]		; 5bit dither table
	shl	ebx,	11
	movzx	eax,	byte [ebp+1]		; g
	movzx	eax,	byte [eax+edx+256]		; 6bit dither table
	shl	eax,	5
	or	ebx,	eax
	movzx	eax,	byte [ebp]		; b
	movzx	eax,	byte [eax+edx]		; 5bit dither table
	or	ebx,	eax
	mov	[edi],	bx		; store
	add	ebp,	byte 4
	add	edi,	byte 2
	sub	edx,	ecx
	add	ecx,	0200h		; ecx += 0x200
	and	ecx,	0600h		; ecx &= 0x600

	cmp	ebp,	esi
	jb	.ploop2		; jump if ebp < esi

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPDither32BitTo16Bit555
;;void, TVPDither32BitTo16Bit555_mmx_a, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs)

	function_align
TVPDither32BitTo16Bit555_mmx_a:					; 32bpp (A)RGB -> 16bpp with ordered dithering
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
	lea	esi,	[ebp + ecx *4]		; esi = limit
	mov	eax,	[esp + 44]		; yofs
	and	eax,	3
	shl	eax,	11		; eax *= 4*2*256
	lea	edx,	[TVPDitherTable_5_6+eax] 	; line
	mov	ecx,	[esp + 40]		; xofs
	and	ecx,	3
	shl	ecx,	9		; ecx *= 2*256

	sub	esi,	byte 12		; 12 = 3*4
	cmp	ebp,	esi

.pfraction:
	add	esi,	byte 12
	cmp	ebp,	esi
	jae	.pexit		; jump if ebp >= esi

.ploop2:
	add	edx,	ecx
	movzx	eax,	byte [ebp+2]		; r
	movzx	ebx,	byte [eax+edx]		; 5bit dither table
	shl	ebx,	10
	movzx	eax,	byte [ebp+1]		; g
	movzx	eax,	byte [eax+edx]		; 5bit dither table
	shl	eax,	5
	or	ebx,	eax
	movzx	eax,	byte [ebp]		; b
	movzx	eax,	byte [eax+edx]		; 5bit dither table
	or	ebx,	eax
	mov	[edi],	bx		; store
	add	ebp,	byte 4
	add	edi,	byte 2
	sub	edx,	ecx
	add	ecx,	0200h		; ecx += 0x200
	and	ecx,	0600h		; ecx &= 0x600

	cmp	ebp,	esi
	jb	.ploop2		; jump if ebp < esi

.pexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret


