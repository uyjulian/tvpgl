; -*- Mode: asm; tab-width: 12; -*-
; this is a part of TVP (KIRIKIRI) software source.
; see other sources for license.
; (C)2001-2009 W.Dee <dee@kikyou.info> and contributors
; …頼むからテキストエディタさん、少なくともUTF-7として誤認しないでくださいお願いします

; addive alpha blend

%ifndef			GEN_CODE

%define GEN_CODE

%include		"nasm.nah"

globaldef		TVPAdditiveAlphaBlend_mmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_emmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_o_mmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_o_emmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_HDA_mmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_HDA_emmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_a_mmx_pfraction_a
globaldef		TVPAdditiveAlphaBlend_a_emmx_pfraction_a



;--------------------------------------------------------------------
	start_const_aligned
;--------------------------------------------------------------------
mask0000ffffffffffff	dd	0xffffffff, 0x0000ffff
mask00ffffff00ffffff	dd	0x00ffffff, 0x00ffffff
;--------------------------------------------------------------------
	end_const_aligned
;--------------------------------------------------------------------





;--------------------------------------------------------------------
; MMX stuff
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAdditiveAlphaBlend
;;void, TVPAdditiveAlphaBlend_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_name TVPAdditiveAlphaBlend_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAdditiveAlphaBlend_o
;;void, TVPAdditiveAlphaBlend_o_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPAdditiveAlphaBlend_o_name TVPAdditiveAlphaBlend_o_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAdditiveAlphaBlend_HDA
;;void, TVPAdditiveAlphaBlend_HDA_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_HDA_name TVPAdditiveAlphaBlend_HDA_mmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX] TVPAdditiveAlphaBlend_a
;;void, TVPAdditiveAlphaBlend_a_mmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_a_name TVPAdditiveAlphaBlend_a_mmx_pfraction_a
;--------------------------------------------------------------------
	%include "addalphablend.nas"
;--------------------------------------------------------------------




;--------------------------------------------------------------------
; EMMX stuff
;--------------------------------------------------------------------
%define			USE_EMMX
;--------------------------------------------------------------------
;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX && TVPCPUType & TVP_CPU_HAS_EMMX] TVPAdditiveAlphaBlend
;;void, TVPAdditiveAlphaBlend_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_name TVPAdditiveAlphaBlend_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX && TVPCPUType & TVP_CPU_HAS_EMMX] TVPAdditiveAlphaBlend_o
;;void, TVPAdditiveAlphaBlend_o_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa)
%define TVPAdditiveAlphaBlend_o_name TVPAdditiveAlphaBlend_o_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX && TVPCPUType & TVP_CPU_HAS_EMMX] TVPAdditiveAlphaBlend_HDA
;;void, TVPAdditiveAlphaBlend_HDA_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_HDA_name TVPAdditiveAlphaBlend_HDA_emmx_pfraction_a

;;[function_replace_by TVPCPUType & TVP_CPU_HAS_MMX && TVPCPUType & TVP_CPU_HAS_EMMX] TVPAdditiveAlphaBlend_a
;;void, TVPAdditiveAlphaBlend_a_emmx_pfraction_a, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len)
%define TVPAdditiveAlphaBlend_a_name TVPAdditiveAlphaBlend_a_emmx_pfraction_a
;--------------------------------------------------------------------
	%include "addalphablend.nas"
;--------------------------------------------------------------------



%else
;--------------------------------------------------------------------
	segment_code
;--------------------------------------------------------------------
	function_align
TVPAdditiveAlphaBlend_name:					; additive alpha blend
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
	sub	esi,	byte 8		; 2*4
	cmp	edi,	esi
	pxor	mm0,	mm0		; mm0 = 0

.pfraction:
	add	esi,	byte 8
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	mov	eax,	[ebp]		; src
	movd	mm4,	eax		; src
	shr	eax,	24		; eax = opa
	movd	mm2,	eax
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

	add	ebp,	byte 4
	add	edi,	byte 4

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

; di = di - di*a*opa + si*opa
;              ~~~~~Df ~~~~~~ Sf
;           ~~~~~~~~Ds
;      ~~~~~~~~~~~~~Dq
;
;


	function_align
TVPAdditiveAlphaBlend_o_name:	; additive alpha blend with opacity
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
	mov	eax,	[esp + 40]		; eax = opa (op)
	mov	ebx,	eax
	shr	ebx,	7
	add	eax,	ebx		; adjust
	movd	mm7,	eax
	punpcklwd	mm7,	mm7
	punpcklwd	mm7,	mm7		; mm7 = op00op00op00op00
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 8		; 2*4
	cmp	edi,	esi
	pxor	mm0,	mm0		; mm0 = 0

.pfraction:
	add	esi,	byte 8
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	movd	mm4,	[ebp]		; src                  (ss)

	movd	mm2,	[edi]		; dest                 (dd)
	punpcklbw	mm4,	mm0		; mm4 = 00ss00ss00ss00ss
	punpcklbw	mm2,	mm0		; mm2 = 00dd00dd00dd00dd
	pmullw	mm4,	mm7		; mm4 = 00Sf00Sf00Sf00Sf
	movq	mm3,	mm2

	psrlw	mm4,	8

	movq	mm1,	mm4
	psrlq	mm1,	48
	punpcklwd	mm1,	mm1
	punpcklwd	mm1,	mm1		; mm1 = Df00Df00Df00Df00

	pmullw	mm2,	mm1		; mm2 = 00Ds00Ds00Ds00Ds
	psrlw	mm2,	8
	psubw	mm3,	mm2		; mm1 = 00Dq00Dq00Dq00Dq

	paddw	mm3,	mm4
	packuswb	mm3,	mm0

	movd	[edi],	mm3		; store

	add	ebp,	byte 4
	add	edi,	byte 4

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
TVPAdditiveAlphaBlend_HDA_name:		; additive alpha blend holding destination alpha
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
	sub	esi,	byte 4		; 1*4
	cmp	edi,	esi
	pxor	mm0,	mm0		; mm0 = 0

.pfraction:
	add	esi,	byte 4
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions
	mov	eax,	[ebp]		; src
	movd	mm4,	eax		; src
	shr	eax,	24		; eax = opa
	movd	mm2,	eax
	movd	mm1,	[edi]		; dest
	punpcklwd	mm2,	mm2
	pand	mm4,	[mask00ffffff00ffffff]
	punpcklwd	mm2,	mm2
	punpcklbw	mm1,	mm0		; mm1 = 00dd00dd00dd00dd
	movq	mm3,	mm1
	pmullw	mm1,	mm2
	psrlw	mm1,	8
	pand	mm1,	[mask0000ffffffffffff]
	psubw	mm3,	mm1
	packuswb	mm3,	mm0
	paddusb	mm3,	mm4		; add src
	movd	[edi],	mm3		; store

	add	ebp,	byte 4
	add	edi,	byte 4

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
TVPAdditiveAlphaBlend_a_name:					; additive alpha blend on additive alpha
	push	edi
	push	esi
	push	ebx
	push	ecx
	push	edx
	push	ebp
	mov	ecx,	[esp + 36]		; len
	cmp	ecx,	byte 0
	jle	near .pexit
	pxor	mm0,	mm0		; mm0 = 0
	mov	eax,	0xffffff
	movd	mm7,	eax		; mm7 = 0xffffff
	punpckldq	mm7,	mm7		; mm7 = 0x00ffffff00ffffff; for mask
	mov	edi,	[esp + 28]		; dest
	mov	ebp,	[esp + 32]		; src
	lea	esi,	[edi + ecx*4]		; limit
	sub	esi,	byte 8		; 2*4
	cmp	edi,	esi

.pfraction:
	add	esi,	byte 8
	cmp	edi,	esi
	jae	.pexit		; jump if edi >= esi

.ploop2:	; fractions

	movd	mm3,	[ebp]		; src      (SaSiSiSi)
	movq	mm4,	mm3
	psrlq	mm4,	24		; mm4 = Sa
	punpcklbw	mm3,	mm0		; mm3 = 00 Sa 00 Si 00 Si 00 Si
	punpcklwd	mm4,	mm4
	movd	mm1,	[edi]		; dest     (DaDiDiDi)
	punpcklwd	mm4,	mm4		; mm4 = 00 Sa 00 Sa 00 Sa 00 Sa
	punpcklbw	mm1,	mm0		; mm1 = 00 Da 00 Di 00 Di 00 Di
	movq	mm2,	mm1
	pmullw	mm2,	mm4
	psrlw	mm2,	8		; mm2 = 00 SaDa 00 SaDi 00 SaDi 00 SaDi
	psubw	mm1,	mm2
	paddw	mm1,	mm3
	packuswb	mm1,	mm0
	movd	[edi],	mm1		; store

	add	ebp,	byte 4
	add	edi,	byte 4

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
%endif
