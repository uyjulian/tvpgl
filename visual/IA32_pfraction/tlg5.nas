; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors
; …頼むからテキストエディタさん、少なくともUTF-7として誤認しないでくださいお願いします

; TLG5 lossless graphic decompressor

%include		"nasm.nah"

globaldef		TVPTLG5DecompressSlide_pfraction_a
globaldef		TVPTLG5ComposeColors3To4_mmx_pfraction_a
globaldef		TVPTLG5ComposeColors4To4_mmx_pfraction_a

	segment_code
;--------------------------------------------------------------------

;;[function_replace_by 1] TVPTLG5DecompressSlide
;;tjs_int, TVPTLG5DecompressSlide_pfraction_a, (tjs_uint8 *out, const tjs_uint8 *in, tjs_int insize, tjs_uint8 *text, tjs_int initialr)

	function_align
TVPTLG5DecompressSlide_pfraction_a:					; modified LZSS decompressor
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	mov	eax,	[esp + 40]		; eax as R
	mov	ebx,	[esp + 28]		; ebx as in
	mov	edi,	[esp + 24]		; edi as out
	mov	esi,	[esp + 36]		; esi as text
	mov	edx,	[esp + 32]

	push	ebp

	lea	ebp,	[ebx + edx]		; ebp as limit

	cmp	ebx,	ebp		; edx == 0 ?
	jae	near .sdpexit		; then exit

	loop_align
	.sdgetmore:
	movzx	ecx,	byte [ebx]		; load new flags ; ecx as flags
	or	ecx,	0100h		; set flag sentinel
	inc	ebx		; in++

	.sdploop:
	shr	ecx,	1		; flags >>= 1
	jz	.sdgetmore		; go to sdgetmore if ecx == 0
	jc	.sdpdiclook		; go to .sdpdiclook if last LSB flags

	.sdpbyte:
		; byte store
	mov	dl,	[ebx]		; load literal input
	inc	eax		; R++
	inc	ebx		; in++
	mov	[esi+eax-1],dl		; store to text[R-1]
	inc	edi		; out++
	and	eax,	0fffh		; R &= 0xfff
	cmp	ebx,	ebp		; in < limit ?
	mov	[edi-1],	dl		; store to the output

	jb	.sdploop		; then go to .sdploop
	jmp	.sdpexit		; else exit

	.sdpdiclook:
		; dictionary lookup
	push	ecx

	movzx	edx,	word [ebx]		; load 2bytes from input
	mov	ecx,	edx
	shr	ecx,	12		; ecx is now length
	and	edx,	0fffh		; edx is now pos

	add	ecx,	byte 3		; length += 3
	add	ebx,	byte 2		; in+=2

	cmp	ecx,	byte 18		; length != 18 ?
	jne	.sdpdics1		; then go to .sdpdics1

		; additional length
	push	edx

	movzx	edx,	byte [ebx]		; retrieve one byte
	add	ecx,	edx		; add it to the length
	inc	ebx		; in++

	pop	edx

	.sdpdics1:
	push	ebx

	.sdpdicloop:
		; lookup loop
	mov	bl,	[esi+edx]		; restore from dictionary
	inc	eax		; R++
	inc	edx		; pos++
	mov	[esi+eax-1],bl		; store to text[R-1]
	and	edx,	0fffh		; pos &= 0xfff
	inc	edi		; out++
	and	eax,	0fffh		; R &= 0xfff
	dec	ecx		; length--
	mov	[edi-1],	bl		; store to the output

	jnz	.sdpdicloop		; go to .sdpdicloop if length!=0

	pop	ebx
	pop	ecx

	cmp	ebx,	ebp		; in < limit ?
	jb	near .sdploop		; then go to .sdploop
	jmp	.sdpexit		; else exit


	.sdpexit:
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	ret

;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPTLG5ComposeColors3To4
;;void, TVPTLG5ComposeColors3To4_mmx_pfraction_a, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width)

	function_align
TVPTLG5ComposeColors3To4_mmx_pfraction_a:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx

	mov	edi,	[esp + 24]		; outp
	mov	esi,	[esp + 28]		; upper
	mov	ebx,	[esp + 36]		; width

	mov	eax,	[esp + 32]		; buf
	movd	mm5,	[eax + 0]
	movd	mm6,	[eax + 4]
	movd	mm7,	[eax + 8]
	pxor	mm4,	mm4
	mov	eax,	0ff000000h		; opacity mask
	movd	mm0,	eax
	punpckldq	mm0,	mm0

	push	ebp
	mov	ebp,	ebx
	xor	ecx,	ecx

	.pfraction:

	cmp	ecx,	ebp
	jge	.pexit

	.ploop2:

	movd	eax,	mm7
	mov	dh,	[eax + ecx]
	movd	eax,	mm6
	mov	dl,	[eax + ecx]
	add	dh,	dl
	movd	eax,	mm5
	shl	edx,	8
	mov	dl,	[eax + ecx]
	add	dl,	dh
	movd	mm3,	edx
	inc	ecx
	paddb	mm4,	mm3
	movq	mm3,	mm4
	movd	mm1,	[esi + ecx*4 - 4]
	paddb	mm3,	mm1
	cmp	ecx,	ebp
	por	mm3,	mm0
	movd	[edi + ecx*4 - 4], mm3

	jl	.ploop2

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

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPTLG5ComposeColors4To4
;;void, TVPTLG5ComposeColors4To4_mmx_pfraction_a, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width)

	function_align
TVPTLG5ComposeColors4To4_mmx_pfraction_a:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx

	mov	edi,	[esp + 24]		; outp
	mov	esi,	[esp + 28]		; upper
	mov	ebx,	[esp + 36]		; width

	mov	eax,	[esp + 32]		; buf
	movd	mm5,	[eax + 0]
	movd	mm6,	[eax + 4]
	movd	mm7,	[eax + 8]
	movd	mm2,	[eax + 12]
	pxor	mm4,	mm4

	push	ebp
	mov	ebp,	ebx
	xor	ecx,	ecx

	.pfraction4:

	cmp	ecx,	ebp
	jge	.pexit4

	.ploop2_4:
	movd	eax,	mm2
	mov	dl,	[eax + ecx]
	shl	edx,	16
	movd	eax,	mm7
	mov	dh,	[eax + ecx]
	movd	eax,	mm6
	mov	dl,	[eax + ecx]
	add	dh,	dl
	movd	eax,	mm5
	shl	edx,	8
	mov	dl,	[eax + ecx]
	add	dl,	dh
	movd	mm3,	edx
	inc	ecx
	paddb	mm4,	mm3
	movq	mm3,	mm4
	movd	mm1,	[esi + ecx*4 - 4]
	paddb	mm3,	mm1
	movd	[edi + ecx*4 - 4], mm3

	cmp	ecx,	ebp
	jl	.ploop2_4

	.pexit4:
	pop	ebp

	pop	edx
	pop	ecx
	pop	ebx
	pop	esi
	pop	edi
	emms
	ret
