; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors

; constant color alpha mapping

%ifndef		GEN_CODE

%include		"nasm.nah"


globaldef		TVPApplyColorMap65_mmx_pfraction_a
globaldef		TVPApplyColorMap65_emmx_pfraction_a
globaldef		TVPApplyColorMap65_d_mmx_pfraction_a
globaldef		TVPApplyColorMap65_d_emmx_pfraction_a
globaldef		TVPApplyColorMap65_a_mmx_pfraction_a
globaldef		TVPApplyColorMap65_a_emmx_pfraction_a
externdef		TVPNegativeMulTable65
externdef		TVPOpacityOnOpacityTable65

%define		GEN_CODE


;--------------------------------------------------------------------
; MMX stuff
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65
;;void, TVPApplyColorMap65_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_name TVPApplyColorMap65_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65_d
;;void, TVPApplyColorMap65_d_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_d_name TVPApplyColorMap65_d_mmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65_a
;;void, TVPApplyColorMap65_a_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_a_name TVPApplyColorMap65_a_mmx_pfraction_a
;--------------------------------------------------------------------
	%include "colormap.nas"
;--------------------------------------------------------------------

;--------------------------------------------------------------------
; EMMX stuff
;--------------------------------------------------------------------
%define USE_EMMX
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65
;;void, TVPApplyColorMap65_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_name TVPApplyColorMap65_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65_d
;;void, TVPApplyColorMap65_d_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_d_name TVPApplyColorMap65_d_emmx_pfraction_a
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_EMMX && TVPCPUType & TVP_CPU_HAS_MMX] TVPApplyColorMap65_a
;;void, TVPApplyColorMap65_a_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color)
%define TVPApplyColorMap65_a_name TVPApplyColorMap65_a_emmx_pfraction_a
;--------------------------------------------------------------------
	%include "colormap.nas"
;--------------------------------------------------------------------


%else

;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
	function_align
TVPApplyColorMap65_name:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit		; jump if ecx <= 0 
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	pxor	mm0,	mm0		; mm0 = 0
	mov	esi,	[esp + 40]		; color
	movd	mm7,	esi		; color
	lea	edx,	[edi + ecx*4]		; limit
	sub	edx,	byte 4		; 1*4
	punpcklbw	mm7,	mm0		; unpack
	cmp	edi,	edx

.pfraction:
	add	edx,	byte 4
	cmp	edi,	edx
	jae	.pexit		; jump if edi >= edx

.ploop2:	; fractions

	movzx	eax,	byte [ebp]		; opacity
	movd	mm4,	eax
	punpcklwd	mm4,	mm4		; unpack
	punpcklwd	mm4,	mm4		; unpack
	movd	mm1,	[edi]		; dest
	punpcklbw	mm1,	mm0		; unpack
	movq	mm2,	mm7
	psubw	mm2,	mm1
	pmullw	mm2,	mm4		; mm2 *= mm4
	psraw	mm2,	6		; mm2 >>= 6
	paddw	mm1,	mm2		; mm1 += mm2
	packuswb	mm1,	mm0		; pack
	movd	[edi], mm1		; store
	add	edi,	byte 4
	inc	ebp

	cmp	edi,	edx
	jb	.ploop2		; jump if edi < edx

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
TVPApplyColorMap65_d_name:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit		; jump if ecx <= 0
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx *4]		; limit
	pxor	mm0,	mm0		; mm0 = 0
	mov	eax,	[esp + 40]		; color
	and	eax,	000ffffffh		; transparent
	or	dword [esp+40],	0ff000000h		; opaque
	movd	mm7,	eax
	punpcklbw	mm7,	mm0		; unpack
	sub	esi,	byte 4		; 1*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 4
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fraction
	movzx	eax,	byte [ebp]		; opacity
	mov	ebx,	[edi]		; dest
	movq	mm2,	mm7		; color
	movd	mm1,	ebx		; dest
	psllq	mm1,	40
	psrlq	mm1,	40		; mask
	shl	eax,	8
	shr	ebx,	24
	add	eax,	ebx
	punpcklbw	mm1,	mm0		; unpack
	mov	bl,	byte [TVPNegativeMulTable65 + eax]		; dest opa table
	movzx	eax,	byte [TVPOpacityOnOpacityTable65 + eax]		; blend opa table
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
	movd	eax,	mm1
	or	eax,	ebx
	mov	[edi],	eax		; store
	add	ebp,	byte 1
	add	edi,	byte 4

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
TVPApplyColorMap65_a_name:
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit		; jump if ecx <= 0
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx *4]		; limit
	pxor	mm0,	mm0		; mm0 = 0
	mov	eax,	[esp + 40]		; color   (co)
	and	eax,	000ffffffh		; transparent
	movd	mm7,	eax
	mov	eax,	0x100
	movd	mm1,	eax
	psllq	mm1,	48
	punpcklbw	mm7,	mm0
	por	mm7,	mm1		; mm7 = 01 00 00 co 00 co 00 co
	sub	esi,	byte 8		; 2*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 8
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fraction
	movzx	eax,	byte [ebp]		; opacity   (Sa)
	movd	mm3,	eax
	punpcklwd	mm3,	mm3
	punpcklwd	mm3,	mm3		; mm3 = 00 Sa 00 Sa 00 Sa 00 Sa
	movq	mm4,	mm3
	pmullw	mm4,	mm7
	psrlw	mm4,	6		; mm4 = 00 Sa 00 Si 00 Si 00 Si
	movd	mm1,	[edi]		; dest     (DaDiDiDi)
	punpcklbw	mm1,	mm0		; mm1 = 00 Da 00 Di 00 Di 00 Di
	movq	mm2,	mm1
	pmullw	mm2,	mm3
	psrlw	mm2,	6		; mm2 = 00 SaDa 00 SaDi 00 SaDi 00 SaDi
	psubw	mm1,	mm2
	paddw	mm1,	mm4
	packuswb	mm1,	mm0
	movd	[edi],	mm1		; store

	add	ebp,	byte 1
	add	edi,	byte 4

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


%endif
