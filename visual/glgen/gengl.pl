#!perl
# tvpgl.* generator



$gpl = <<EOF;

	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2009 W.Dee <dee\@kikyou.info> and contributors

	See details of license at "license.txt"

EOF

$rev = "0.1";

$should_unroll = 0;
$skip_template_funcs = 0;

;#-----------------------------------------------------------------
;# file content retrieving
;#-----------------------------------------------------------------
sub get_file_content
{
	local($name, @all);
	$name = $_[0];
	open IFH, $name;
	@all = <IFH>;
	return join('', @all);
}

;#-----------------------------------------------------------------
;# exported function gathering
;#-----------------------------------------------------------------


sub format_function
{
	local($a, $b);
	$a = $_[0];
	$a =~ s/\n//g;
	push (@function_list, $a);
}

sub get_function_list
{
	@function_list = ();
	local($content, $ret);
	$content = &get_file_content($_[0]);
	$ret = '';
	$content =~ s/\/\*export\*\/(.*?)\{/&format_function($1)/seg;
}

;#-----------------------------------------------------------------
;# loop unrolling
;#-----------------------------------------------------------------

sub loop_unroll_c
{
	local($content, $times, $unroll_times, $i);
	$content = $_[0];
	$times = $_[1];
	$unroll_times = $_[2];

	if($should_unroll == 1)
	{
		print FC <<EOF;
	  if($times > 0)
	  {
		int lu_n = ($times + ($unroll_times-1)) / $unroll_times;
		switch($times % $unroll_times)
		{
		case 0: do { $content;
EOF
		for($i = $unroll_times-1; $i>=1; $i--)
		{
			print FC <<EOF;
		case $i: $content;
EOF
		}
		print FC <<EOF;
		   } while(-- lu_n);
		}
	  }
EOF
	}
	else
	{
		print FC <<EOF;
		for(int lu_n = 0; lu_n < $times; lu_n++)
		{
			$content
		}
EOF
	}

}

;# type 2 for more simple case
;# backward operation
sub loop_unroll_c_2_backward
{
	local($content, $times, $unroll_times, $i, $con);
	$content = $_[0];
	$times = $_[1];
	$unroll_times = $_[2];
	if($should_unroll == 1)
	{
		print FC <<EOF;
		$times --;

		while($times >= ($unroll_times -1))
		{
EOF

		for($i = 0; $i<$unroll_times; $i++)
		{
			$con = $content;
			$con =~ s/\{ofs\}/($times-$i)/g;
			print FC $con;
		}

	  	print FC <<EOF;
			$times -= $unroll_times;
		}

		while($times >= 0)
		{
EOF

		$con = $content;
		$con =~ s/\{ofs\}/$times/g;
		print FC $con;

	  	print FC <<EOF;
			$times --;
		}
EOF
	}
	else
	{
	  	print FC <<EOF;
	  	for(int ___index = $times; ___index >= 0; ___index--)
		{
EOF

		$con = $content;
		$con =~ s/\{ofs\}/___index/g;
		print FC $con;

	  	print FC <<EOF;
		}
EOF
	}
}

;# type 2 forward
sub loop_unroll_c_2
{
	local($content, $times, $unroll_times, $i, $con);
	$content = $_[0];
	$times = $_[1];
	$unroll_times = $_[2];

	if($should_unroll == 1)
	{
		print FC <<EOF;
		{
			int ___index = 0;
			$times -= ($unroll_times-1);

			while(___index < $times)
			{
EOF

		for($i = 0; $i<$unroll_times; $i++)
		{
			$con = $content;
			$con =~ s/\{ofs\}/(___index+$i)/g;
			print FC $con;
		}

	  	print FC <<EOF;
				___index += $unroll_times;
			}

			$times += ($unroll_times-1);

			while(___index < $times)
			{
EOF

			$con = $content;
			$con =~ s/\{ofs\}/___index/g;
			print FC $con;

	  		print FC <<EOF;
				___index ++;
			}
		}
EOF
	}
	else
	{
		print FC <<EOF;
		{
			for(int ___index = 0; ___index < $times; ___index++)
			{
EOF

			$con = $content;
			$con =~ s/\{ofs\}/___index/g;
			print FC $con;

	  		print FC <<EOF;
			}
		}
EOF
	}
}


;# twice-interleaved type of type 2
sub loop_unroll_c_int_2
{
	local($content, $times, $content2, $int_times, $unroll_times, $i, $con, $con2, @cont, @cont2);
	$content = $_[0];
	$content2 = $_[1];
	$times = $_[2];
	$unroll_times = $_[3];

	if($should_unroll == 1)
	{
		print FC <<EOF;
		{
			int ___index = 0;
			$times -= ($unroll_times-1);

			while(___index < $times)
			{
EOF

		for($i = 0; $i<$unroll_times/2; $i++)
		{
			local($j);
			$con = $content;
			$con =~ s/\{ofs\}/(___index+($i*2))/g;
			$con2 = $content2;
			$con2 =~ s/\{ofs\}/(___index+($i*2+1))/g;
			$con =~ s/\n//g;
			$con2 =~ s/\n//g;
			@cont = split(/;;/, $con);
			@cont2 = split(/;;/, $con2);

			for($j = 0; $j <= $#cont; $j++)
			{
				print FC $cont[$j];
				print FC ";\n";
				print FC $cont2[$j];
				print FC ";\n";
			}
		}

	  	print FC <<EOF;
				___index += $unroll_times;
			}

			$times += ($unroll_times-1);

			while(___index < $times)
			{
EOF

		$con = $content;
		$con =~ s/\{ofs\}/___index/g;
		print FC $con;

	  	print FC <<EOF;
				___index ++;
			}
		}
EOF
	}
	else
	{
		print FC <<EOF;
		{
			for(int ___index = 0; ___index < $times; ___index++)
			{
EOF

		$con = $content;
		$con =~ s/\{ofs\}/___index/g;
		print FC $con;

	  	print FC <<EOF;
			}
		}
EOF
	}
}



$serial = 0;

sub _label_expand
{
	local($num);
	$num = $_[0];
	return "ll_". ($num + $serial);
}

sub label_expand
{
	local($content);
	$content = $_[0];
	$serial += ($content =~ s/\{(\d+)\}/&_label_expand($1)/eg);
	return $content;
}

sub _adds_expand
{
	local($reg, $num, $mul);
	$reg = $_[0];
	$num = $_[1];
	$mul = $_[2];
	$num *= $mul;
	if($num == -1)
	{
		return "dec $reg";
	}
	elsif($num == 1)
	{
		return "inc $reg";
	}
	else
	{
		return "add $reg, $num";
	}
}

sub adds_expand
{
	local($content, $mul);
	$content = $_[0];
	$mul = $_[1];
	$content =~ s/\{(.+?),(\d+)\}/&_adds_expand($1, $2, $mul)/eg;
	return $content;
}



;#-----------------------------------------------------------------
;# write the header
;#-----------------------------------------------------------------


open FC , ">tvpgl.c";
open FH , ">tvpgl.h";



;#-----------------------------------------------------------------
print FC <<EOF;
/*
$gpl
*/

/* core C routines for graphics operations */
/* this file is always generated by gengl.pl rev. $rev */

/* #include "tjsCommHead.h" */
#include <memory.h>
#include <math.h>
#include "tjsTypes.h"
#include "tvpgl.h"

#if __BORLANDC__
	#pragma warn -par /* prevent unsed parameter warning */
	#pragma warn -aus /* prevent unsed variable warning */
#endif

EOF
;#-----------------------------------------------------------------
print FH <<EOF;
/*
$gpl
*/
/* core C routines for graphics operations */
/* this file is always generated by gengl.pl rev. $rev */
#ifndef _TVPGL_H_
#define _TVPGL_H_

/*
	key to blending suffix:
	d : destination has alpha
	a : destination has additive-alpha
	o : blend with opacity
*/


/*[*/
#ifdef __cplusplus
 extern "C" {
#endif
/*]*/

/*[*/
#pragma pack(push, 4)
typedef struct
{
	/* structure used for adjustment of gamma levels */

	float RGamma; /* R gamma   ( 0.10 -- 1.00 -- 9.99) */
	tjs_int RFloor;   /* output floor value  ( 0 -- 255 ) */
	tjs_int RCeil;    /* output ceil value ( 0 -- 255 ) */
	float GGamma; /* G */
	tjs_int GFloor;
	tjs_int GCeil;
	float BGamma; /* B */
	tjs_int BFloor;
	tjs_int BCeil;
} tTVPGLGammaAdjustData;
#pragma pack(pop)
/*]*/

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#ifndef TVP_GL_FUNC_SUFFIX
#define TVP_GL_FUNC_SUFFIX _generic
#endif

#ifndef TVP_GL_FUNC_DEFINE_FUNC_PTR
#define TVP_GL_FUNC_DEFINE_FUNC_PTR 1
#endif

#ifndef TVP_GL_FUNC_DEFINE_DATA_GEN
#define TVP_GL_FUNC_DEFINE_DATA_GEN 1
#endif

#if TVP_GL_FUNC_DEFINE_DATA_GEN
#define TVP_GL_DATA_INIT
#else
#define TVP_GL_DATA_INIT extern
#endif

/* add here compiler specific inline directives */
#if defined( __BORLANDC__ ) || ( _MSC_VER )
	#define TVP_INLINE_FUNC __inline
#elif defined(__GNUC__)
	#define TVP_INLINE_FUNC inline
#else
	#define TVP_INLINE_FUNC
#endif

#define TVP_GL_FUNCNAME(funcname) CAT(funcname, TVP_GL_FUNC_SUFFIX)

#ifdef _WIN32
#define TVP_GL_FUNC_DECL(rettype, funcname, arg)  rettype __cdecl TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype __cdecl TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_STATIC_DECL(rettype, funcname, arg)  static rettype __cdecl TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_INLINE_DECL(rettype, funcname, arg)  static rettype TVP_INLINE_FUNC __cdecl TVP_GL_FUNCNAME(funcname) arg
#if TVP_GL_FUNC_DEFINE_FUNC_PTR
#define TVP_GL_FUNC_PTR_DECL(rettype, funcname, arg) rettype (__cdecl * funcname) arg
#else
#define TVP_GL_FUNC_PTR_DECL(rettype, funcname, arg)
#endif
#define TVP_GL_FUNC_PTR_EXTERN_DECL_(rettype, funcname, arg) extern rettype (__cdecl * funcname) arg
#define TVP_GL_FUNC_PTR_EXTERN_DECL TVP_GL_FUNC_PTR_EXTERN_DECL_
#elif defined(__GNUC__)
#define TVP_GL_FUNC_DECL(rettype, funcname, arg)  rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_STATIC_DECL(rettype, funcname, arg)  static rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_INLINE_DECL(rettype, funcname, arg)  static rettype TVP_INLINE_FUNC TVP_GL_FUNCNAME(funcname) arg
#if TVP_GL_FUNC_DEFINE_FUNC_PTR
#define TVP_GL_FUNC_PTR_DECL(rettype, funcname, arg) rettype (*funcname) arg
#else
#define TVP_GL_FUNC_PTR_DECL(rettype, funcname, arg)
#endif
#define TVP_GL_FUNC_PTR_EXTERN_DECL_(rettype, funcname, arg) extern rettype (*funcname) arg
#define TVP_GL_FUNC_PTR_EXTERN_DECL TVP_GL_FUNC_PTR_EXTERN_DECL_
#endif

extern unsigned char TVPDivTable[256*256];
extern unsigned char TVP252DitherPalette[3][256];

#define TVP_TLG6_H_BLOCK_SIZE 8
#define TVP_TLG6_W_BLOCK_SIZE 8

/* put platform dependent declaration here */

EOF
;#-----------------------------------------------------------------


;#-----------------------------------------------------------------
;# tables used by various routines
;#-----------------------------------------------------------------

print FC &get_file_content('maketab.c');


;#-----------------------------------------------------------------
;# common functions
;#-----------------------------------------------------------------

;# some common inline functions.
;# mainly for additive-alpha blending mode, by historical reason.

$cnt =  <<EOF;

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPSaturatedAdd, (tjs_uint32 a, tjs_uint32 b))
{
	/* Add each byte of packed 8bit values in two 32bit uint32, with saturation. */
	tjs_uint32 tmp = (  ( a & b ) + ( ((a ^ b)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
	tmp = (tmp<<1u) - (tmp>>7u);
	return (a + b - tmp) | tmp;
}

/*
	TVPAddAlphaBlend_dest_src[_o]
	dest/src    :    a(additive-alpha)  d(alpha)  n(none alpha)
	_o          :    with opacity
*/

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_n_a, (tjs_uint32 dest, tjs_uint32 src))
{
	tjs_uint32 sopa = (~src) >> 24u;
	return TVP_GL_FUNCNAME(TVPSaturatedAdd)((((dest & 0xff00ffu)*sopa >> 8u) & 0xff00ffu) + 
		(((dest & 0xff00u)*sopa >> 8u) & 0xff00u), src);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_HDA_n_a, (tjs_uint32 dest, tjs_uint32 src))
{
	return (dest & 0xff000000u) + (TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest, src) & 0xffffffu);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_n_a_o, (tjs_uint32 dest, tjs_uint32 src, tjs_int opa))
{
	src = (((src & 0xff00ffu)*opa >> 8u) & 0xff00ffu) + (((src >> 8u) & 0xff00ffu)*opa & 0xff00ff00u);
	return TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest, src);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_HDA_n_a_o, (tjs_uint32 dest, tjs_uint32 src, tjs_int opa))
{
	return (dest & 0xff000000u) + (TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest, src, opa) & 0xffffffu);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_a_a, (tjs_uint32 dest, tjs_uint32 src))
{
	/*
		Di = sat(Si, (1-Sa)*Di)
		Da = Sa + Da - SaDa
	*/

	tjs_uint32 dopa = dest >> 24u;
	tjs_uint32 sopa = src >> 24u;
	dopa = dopa + sopa - (dopa*sopa >> 8u);
	dopa -= (dopa >> 8u); /* adjust alpha */
	sopa ^= 0xffu;
	src &= 0xffffffu;
	return (dopa << 24u) + 
		TVP_GL_FUNCNAME(TVPSaturatedAdd)((((dest & 0xff00ffu)*sopa >> 8u) & 0xff00ffu) +
			(((dest & 0xff00u)*sopa >> 8u) & 0xff00u), src);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_a_ca, (tjs_uint32 dest, tjs_uint32 sopa, tjs_uint32 sopa_inv, tjs_uint32 src))
{
	/*
		Di = sat(Si, (1-Sa)*Di)
		Da = Sa + Da - SaDa
	*/

	tjs_uint32 dopa = dest >> 24u;
	dopa = dopa + sopa - (dopa*sopa >> 8u);
	dopa -= (dopa >> 8u); /* adjust alpha */
	return (dopa << 24u) + 
		TVP_GL_FUNCNAME(TVPSaturatedAdd)((((dest & 0xff00ffu)*sopa_inv >> 8u) & 0xff00ffu) +
			(((dest & 0xff00u)*sopa_inv >> 8u) & 0xff00u), src);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_a_a_o, (tjs_uint32 dest, tjs_uint32 src, tjs_int opa))
{
	src = (((src & 0xff00ffu)*opa >> 8u) & 0xff00ffu) + (((src >> 8u) & 0xff00ffu)*opa & 0xff00ff00u);
	return TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(dest, src);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPMulColor, (tjs_uint32 color, tjs_uint32 fac))
{
	return (((((color & 0x00ff00u) * fac) & 0x00ff0000u) +
			(((color & 0xff00ffu) * fac) & 0xff00ff00u) ) >> 8u);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAlphaAndColorToAdditiveAlpha, (tjs_uint32 alpha, tjs_uint32 color))
{
	return TVP_GL_FUNCNAME(TVPMulColor)(color, alpha) + (color & 0xff000000u);

}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAlphaToAdditiveAlpha, (tjs_uint32 a))
{
	return TVP_GL_FUNCNAME(TVPAlphaAndColorToAdditiveAlpha)(a >> 24u, a);
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_a_d, (tjs_uint32 dest, tjs_uint32 src))
{
	return TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(dest, TVP_GL_FUNCNAME(TVPAlphaToAdditiveAlpha)(src));
}

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPAddAlphaBlend_a_d_o, (tjs_uint32 dest, tjs_uint32 src, tjs_int opa))
{
	src = (src & 0xffffffu) + ((((src >> 24u) * opa) >> 8u) << 24u);
	return TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d)(dest, src);
}

/* TVPAddAlphaBlend_d_a is not yet implemented because the expression may loose precision. */


/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, TVPBlendARGB, (tjs_uint32 b, tjs_uint32 a, tjs_int ratio))
{
	/* returns a * ratio + b * (1 - ratio) */
	tjs_uint32 b2;
	tjs_uint32 t;
	b2 = b & 0x00ff00ffu;
	t = (b2 + (((a & 0x00ff00ffu) - b2) * ratio >> 8u)) & 0x00ff00ffu;
	b2 = (b & 0xff00ff00u) >> 8u;
	return t + 
		(((b2 + (( ((a & 0xff00ff00u) >> 8u) - b2) * ratio >> 8u)) << 8u)& 0xff00ff00u);
}




EOF
print FH $cnt;


;#-----------------------------------------------------------------
;# pixel alpha blending
;#-----------------------------------------------------------------

;# simple pixel alpha blending ( destination alpha alpha/additive-alpha/off )
;# alpha blending with opacity ( destination alpha alpha/additive-alpha/off )

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	tjs_uint32 d1, s, d, sopa;
EOF


$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u); /* hda */
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF


$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_d_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF


$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	addr = ((s >> 16u) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
	//tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d)(dest[{ofs}], src[{ofs}]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_do_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF


$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	addr = (( (s>>24u)*opa) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaBlend_ao_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d_o)(dest[{ofs}], src[{ofs}], opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# alpha blend matting
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAlphaColorMat_c, (tjs_uint32 *dest, const tjs_uint32 color, tjs_int len))
{
	tjs_uint32 d1, s, d, sopa;
EOF


$content = <<EOF;
{
	s = *dest;
	d = color;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + 0xff000000u;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# pixel additive alpha blending
;#-----------------------------------------------------------------

;# pixel additive alpha blending ( destination alpha is alpha/additive-alpha/off )
;# additive alpha blending with opacity ( destination alpha alpha/additive-alpha/off )


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[{ofs}], src[{ofs}]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a)(dest[{ofs}], src[{ofs}]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[{ofs}], src[{ofs}], opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a_o)(dest[{ofs}], src[{ofs}], opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_d_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF


$content = <<EOF;
	dest[{ofs}] = 0u;
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(dest[{ofs}], src[{ofs}]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_do_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF


$content = <<EOF;
	dest[{ofs}] = 0u;
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdditiveAlphaBlend_ao_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
EOF


$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a_o)(dest[{ofs}], src[{ofs}], opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# conversion between additive-alpha and simple alpha
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConvertAdditiveAlphaToAlpha_c, (tjs_uint32 *buf, tjs_int len))
{/*MAY LOOSE ADDITIVE STUFF*/
	tjs_uint32 tmp;
	const tjs_uint8 * t;
EOF


$content = <<EOF;
{
	tmp = *buf;
	t = ((tmp >> 16u) & 0xff00u) + TVPDivTable;
	*buf = (tmp & 0xff000000u) +
		(t[(tmp >> 16u) & 0xffu] << 16u) +
		(t[(tmp >>  8u) & 0xffu] <<  8u) +
		(t[ tmp        & 0xffu]      );
	buf++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPConvertAlphaToAdditiveAlpha_c, (tjs_uint32 *buf, tjs_int len))
{
EOF


$content = <<EOF;
	buf[{ofs}] = TVP_GL_FUNCNAME(TVPAlphaToAdditiveAlpha)(buf[{ofs}]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# stretching pixel alpha blending
;#-----------------------------------------------------------------

;# stretching simple pixel alpha blending ( destination alpha on/off )
;# stretching alpha blending with opacity ( destination alpha on/off )


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	tjs_uint32 d1, s, d, sopa;
EOF


$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u); /* hda */
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_HDA_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	addr = ((s >> 16u) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d)(*dest, src[srcstart >> 16u]);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_do_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	addr = (( (s>>24u)*opa) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAlphaBlend_ao_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d_o)(*dest, src[srcstart >> 16u], opa);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF



;#-----------------------------------------------------------------
;# stretching additive alpha blending
;#-----------------------------------------------------------------

;# stretching pixel additive alpha blending ( destination alpha alpha/additive-alpha/off )
;# stretching alpha blending with opacity ( destination alpha alpha/additive-alpha/off )


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(*dest, src[srcstart >> 16u]);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
	/* stretching additive alpha blend with bilinear interpolation */
	tjs_int blend_x;
	tjs_int sp;

	blend_y += blend_y >> 7u; /* adjust blend ratio */

EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y));
			srcstart += srcstep;

			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[1u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y));
			srcstart += srcstep;

			dest += 2u;
			destlen -= 2;
		}

		destlen += 1;
EOF
}

print FC <<EOF;

	while(destlen > 0)
	{
		blend_x = (srcstart & 0xffffu) >> 8u;
		sp = srcstart >> 16u;
		dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
				blend_y));
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a)(*dest, src[srcstart >> 16u]);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(*dest, src[srcstart >> 16u], opa);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpStretchAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	/* stretching additive alpha blend with bilinear interpolation */
	tjs_int blend_x;
	tjs_int sp;

	blend_y += blend_y >> 7u; /* adjust blend ratio */

EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y), opa);
			srcstart += srcstep;

			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[1u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y), opa);
			srcstart += srcstep;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		blend_x = (srcstart & 0xffffu) >> 8u;
		sp = srcstart >> 16u;
		dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
				blend_y), opa);
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_HDA_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a_o)(*dest, src[srcstart >> 16u], opa);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF

$content = <<EOF;
{
	srcstart += srcstep;
	*dest = 0u;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(*dest, src[srcstart >> 16u]);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_do_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF

$content = <<EOF;
{
	srcstart += srcstep;
	*dest = 0u;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchAdditiveAlphaBlend_ao_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a_o)(*dest, src[srcstart >> 16u], opa);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF



;#-----------------------------------------------------------------
;# linear transforming pixel alpha blending
;#-----------------------------------------------------------------

;# linear transforming simple pixel alpha blending ( destination alpha alpha/additive-alpha/off )
;# linear transforming alpha blending with opacity ( destination alpha alpha/additive-alpha/off )

;# 'linear transformation' does not means that does linear interpolation.

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	sopa = s >> 24u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u); /* hda */
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_HDA_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	sopa = ((s >> 24u) * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	addr = ((s >> 16u) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_do_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d, sopa, addr, destalpha;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	addr = (( (s>>24u)*opa) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 + ((d + ((s - d) * sopa >> 8u)) & 0xff00u) + destalpha;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAlphaBlend_ao_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_d_o)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)), opa);
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF




;#-----------------------------------------------------------------
;# linear transforming pixel additive alpha blending
;#-----------------------------------------------------------------

;# linear transforming pixel additive alpha blending ( destination alpha alpha/additive-alpha/off )
;# linear transforming additive alpha blending with opacity ( destination alpha alpha/additive-alpha/off )

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	/* bilinear interpolation version */
	/* note that srcpitch unit is in byte */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			int blend_x, blend_y;
			const tjs_uint32 *p0, *p1;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y));
			sx += stepx, sy += stepy;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[1u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y));
			sx += stepx, sy += stepy;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		int blend_x, blend_y;
		const tjs_uint32 *p0, *p1;

		blend_x = (sx & 0xffffu) >> 8u;
		blend_x += blend_x >> 7u;
		blend_y = (sy & 0xffffu) >> 8u;
		blend_y += blend_y >> 7u;
		p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
		p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
		dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
				blend_y));
		sx += stepx, sy += stepy;

		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/* HDA : hold destination alpha */

/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)), opa);
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpLinTransAdditiveAlphaBlend_o_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	/* bilinear interpolation version */
	/* note that srcpitch unit is in byte */
	opa += opa >> 7u;
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			int blend_x, blend_y;
			const tjs_uint32 *p0, *p1;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y), opa);
			sx += stepx, sy += stepy;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[1u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y), opa);
			sx += stepx, sy += stepy;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		int blend_x, blend_y;
		const tjs_uint32 *p0, *p1;

		blend_x = (sx & 0xffffu) >> 8u;
		blend_x += blend_x >> 7u;
		blend_y = (sy & 0xffffu) >> 8u;
		blend_y += blend_y >> 7u;
		p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
		p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
		dest[0u] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_n_a_o)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
				blend_y), opa);
		sx += stepx, sy += stepy;

		dest ++;
		destlen --;
	}
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_HDA_o_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_HDA_n_a_o)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)), opa);
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF

$content = <<EOF;
{
	*dest = 0u;
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_do_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
EOF

$content = <<EOF;
{
	*dest = 0u;
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransAdditiveAlphaBlend_ao_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a_o)(*dest, 
		*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)), opa);
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF






;#-----------------------------------------------------------------
;# constant ratio alpha blending
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPCopyOpaqueImage_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 t1, t2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 t1;
EOF
}

$content = <<EOF;
	t1 = src[{ofs}];;
	t1 |= 0xff000000u;;
	dest[{ofs}] = t1;;
EOF

$content2 = <<EOF;
	t2 = src[{ofs}];;
	t2 |= 0xff000000u;;
	dest[{ofs}] = t2;;
EOF
	
&loop_unroll_c_int_2($content, $content2, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_d_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	tjs_uint32 d1, s, d, addr;
	tjs_int alpha;
	opa <<= 8u;
EOF

$content = <<EOF;
{
	s = *src;
	src++;
	d = *dest;
	addr = opa + (d>>24u);
	alpha = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * alpha >> 8u)) & 0xff00ffu) +
		(TVPNegativeMulTable[addr]<<24u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * alpha >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
	opa <<= 24u;
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(dest[{ofs}], (src[{ofs}] & 0xffffffu) | opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# constant ratio alpha blending with stretching
;#-----------------------------------------------------------------

;#  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep)

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchCopyOpaqueImage_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = 0xff000000u | src[srcstart >> 16u];
			srcstart += srcstep;
			dest[1u] = 0xff000000u | src[srcstart >> 16u];
			srcstart += srcstep;
			dest[2u] = 0xff000000u | src[srcstart >> 16u];
			srcstart += srcstep;
			dest[3u] = 0xff000000u | src[srcstart >> 16u];
			srcstart += srcstep;
			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = 0xff000000u | src[srcstart >> 16u];
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = src[srcstart>>16u];
	srcstart += srcstep;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpStretchConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	/* stretch copy with bilinear interpolation */
	tjs_int blend_x;
	tjs_int sp;

	blend_y += blend_y >> 7u; /* adjust blend ratio */
	opa += opa > 7u; /* adjust opa */

EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y), opa);
			srcstart += srcstep;

			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[1u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y), opa);
			srcstart += srcstep;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		blend_x = (srcstart & 0xffffu) >> 8u;
		sp = srcstart >> 16u;
		dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
				blend_y), opa);
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF
;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchConstAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchConstAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	tjs_uint32 d1, s, d, addr;
	tjs_int alpha;
	if(opa > 128u) opa ++; /* adjust for error */
EOF

$content = <<EOF;
{
	s = src[srcstart >> 16u];
	srcstart += srcstep;
	d = *dest;
	addr = (( (s>>24u)*opa) & 0xff00u) + (d>>24u);
	alpha = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * alpha >> 8u)) & 0xff00ffu) +
		(TVPNegativeMulTable[addr]<<24u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * alpha >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchConstAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep, tjs_int opa))
{
	opa <<= 24u;
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(*dest, (src[srcstart >> 16u] & 0xffffffu) | opa);
	srcstart += srcstep;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF





;#-----------------------------------------------------------------
;# constant ratio alpha blending with linear transformation
;#-----------------------------------------------------------------

;#  (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch)

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransCopyOpaqueImage_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = 0xff000000u | *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx;
			sy += stepy;
			dest[1u] = 0xff000000u | *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx;
			sy += stepy;
			dest[2u] = 0xff000000u | *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx;
			sy += stepy;
			dest[3u] = 0xff000000u | *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx;
			sy += stepy;
			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = 0xff000000u | *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
		sx += stepx;
		sy += stepy;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = (d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpLinTransConstAlphaBlend_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	/* bilinear interpolation version */
	/* note that srcpitch unit is in byte */
	opa += opa >> 7u; /* adjust opacity */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			int blend_x, blend_y;
			const tjs_uint32 *p0, *p1;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y), opa);
			sx += stepx, sy += stepy;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[1u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[1u], TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y), opa);
			sx += stepx, sy += stepy;

			dest += 2u;
			destlen -= 2;
		}

		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		int blend_x, blend_y;
		const tjs_uint32 *p0, *p1;

		blend_x = (sx & 0xffffu) >> 8u;
		blend_x += blend_x >> 7u;
		blend_y = (sy & 0xffffu) >> 8u;
		blend_y += blend_y >> 7u;
		p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
		p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
		dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(dest[0u], TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
				blend_y), opa);
		sx += stepx, sy += stepy;

		dest ++;
		destlen --;
	}
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransConstAlphaBlend_HDA_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransConstAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	tjs_uint32 d1, s, d, addr;
	tjs_int alpha;
	opa <<= 8u;
EOF

$content = <<EOF;
{
	s = *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
	sx += stepx;
	sy += stepy;
	d = *dest;
	addr = opa + (d>>24u);
	alpha = TVPOpacityOnOpacityTable[addr];
	d1 = d & 0xff00ffu;
	d1 = ((d1 + (((s & 0xff00ffu) - d1) * alpha >> 8u)) & 0xff00ffu) +
		(TVPNegativeMulTable[addr]<<24u);
	d &= 0xff00u;
	s &= 0xff00u;
	*dest = d1 | ((d + ((s - d) * alpha >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransConstAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch, tjs_int opa))
{
	opa <<= 24u;
EOF

$content = <<EOF;
{
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_a)(*dest, 
		((*( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u))) & 0xffffffu) | opa);
	sx += stepx;
	sy += stepy;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF







;#-----------------------------------------------------------------
;# constant ratio alpha blending ( separated destination )
;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_SD_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa))
{
	tjs_uint32 s1_, s1, s2;
EOF

$content = <<EOF;
{
	s1 = *src1;
	s2 = *src2;
	s1_ = s1 & 0xff00ffu;
	s1_ = (s1_ + (((s2 & 0xff00ffu) - s1_) * opa >> 8u)) & 0xff00ffu;
	src1++;
	src2++;
	s2 &= 0xff00u;
	s1 &= 0xff00u;
	*dest = s1_ | ((s1 + ((s2 - s1) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_SD_a_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa))
{
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPBlendARGB)(src1[{ofs}], src2[{ofs}], opa);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------



if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstAlphaBlend_SD_d_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int len, tjs_int opa))
{/* alpha vs alpha, destination has alpha */
	tjs_uint32 s1_, s2, s1, addr;
	tjs_uint32 a1, a2;
	tjs_int alpha;
	tjs_int iopa;
	if(opa > 127u) opa ++; /* adjust for error */
	iopa = 256u - opa;
	/* blending function for 'alpha-per-pixel enabled alpha blending' is complex. */
EOF

$content = <<EOF;
{
	s1 = *src1;
	s2 = *src2;
	a1 = s1 >> 24u;
	a2 = s2 >> 24u;
	addr = (a2*opa & 0xff00u) + (a1*iopa >> 8u);
	alpha = TVPOpacityOnOpacityTable[addr];
	s1_ = s1 & 0xff00ffu;
	s1_ = ((s1_ + (((s2 & 0xff00ffu) - s1_) * alpha >> 8u)) & 0xff00ffu);
	src1++;
	src2++;
	s1 &= 0xff00u;
	s2 &= 0xff00u;
	s1_ |= (a1 + ((a2 - a1)*opa >> 8u)) << 24u;
	*dest = s1_ | ((s1 + ((s2 - s1) * alpha >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}



;#-----------------------------------------------------------------
;# blending function for universal transition
;#-----------------------------------------------------------------

;# Note: blending incompatible alpha type (such as additive-alpha vs alpha)
;#       is not yet implemented.


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInitUnivTransBlendTable_c, (tjs_uint32 *table, tjs_int phase, tjs_int vague))
{
	tjs_int i;
	tjs_int phasemax = phase;
	phase -= vague;
	for(i = 0u; i<256u; i++)
	{
		if(i<phase) table[i] = 255u;
		else if(i>=phasemax) table[i] = 0u;
		else
		{
			int tmp = (255u-(( i - phase )*255u / vague));
			if(tmp<0u) tmp = 0u;
			if(tmp>255u) tmp = 255u;
			table[i] = tmp;
		}
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInitUnivTransBlendTable_d_c, (tjs_uint32 *table, tjs_int phase, tjs_int vague))
{
	/* alias to TVPInitUnivTransBlendTable_c */
	TVP_GL_FUNCNAME(TVPInitUnivTransBlendTable_c)(table, phase, vague);
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInitUnivTransBlendTable_a_c, (tjs_uint32 *table, tjs_int phase, tjs_int vague))
{
	/* alias to TVPInitUnivTransBlendTable_c */
	TVP_GL_FUNCNAME(TVPInitUnivTransBlendTable_c)(table, phase, vague);
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len))
{
	tjs_uint32 s1_, s1, s2;
	tjs_int opa;
EOF

$content = <<EOF;
{
	opa = table[*rule];
	rule++;
	s1 = *src1;
	src1++;
	s2 = *src2;
	src2++;
	s1_ = s1 & 0xff00ffu;
	s1_ = (s1_ + (((s2 & 0xff00ffu) - s1_) * opa >> 8u)) & 0xff00ffu;
	s2 &= 0xff00u;
	s1 &= 0xff00u;
	*dest = s1_ | ((s1 + ((s2 - s1) * opa >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_switch_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
	tjs_uint32 s1_, s1, s2;
	tjs_int opa;
EOF

$content = <<EOF;
{
	opa = *rule;
	if(opa >= src1lv)
	{
		*dest = *src1;
		rule++; src1++; src2++; dest++;
	}
	else if(opa < src2lv)
	{
		*dest = *src2;
		rule++; src1++; src2++; dest++;
	}
	else
	{
		opa = table[opa];
		rule++;
		s1 = *src1;
		src1++;
		s2 = *src2;
		src2++;
		s1_ = s1 & 0xff00ffu;
		s1_ = (s1_ + (((s2 & 0xff00ffu) - s1_) * opa >> 8u)) & 0xff00ffu;
		s2 &= 0xff00u;
		s1 &= 0xff00u;
		*dest = s1_ | ((s1 + ((s2 - s1) * opa >> 8u)) & 0xff00u);
		dest++;
	}
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_d_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len))
{
	tjs_uint32 s1_, s2, s1, addr;
	tjs_uint32 a1, a2;
	tjs_int alpha;
	tjs_int opa;
EOF

$content = <<EOF;
{
	opa = table[*rule];
	rule++;
	s1 = *src1;
	s2 = *src2;
	a1 = s1 >> 24u;
	a2 = s2 >> 24u;
	addr = (a2*opa & 0xff00u) + (a1*(256u-opa) >> 8u);
	alpha = TVPOpacityOnOpacityTable[addr];
	s1_ = s1 & 0xff00ffu;
	s1_ = ((s1_ + (((s2 & 0xff00ffu) - s1_) * alpha >> 8u)) & 0xff00ffu);
	src1++;
	src2++;
	s1 &= 0xff00u;
	s2 &= 0xff00u;
	s1_ |= (a1 + ((a2 - a1)*opa >> 8u)) << 24u;
	*dest = s1_ | ((s1 + ((s2 - s1) * alpha >> 8u)) & 0xff00u);
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_switch_d_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
	tjs_uint32 s1_, s2, s1, addr;
	tjs_uint32 a1, a2;
	tjs_int alpha;
	tjs_int opa;
EOF

$content = <<EOF;
{
	opa = *rule;
	if(opa >= src1lv)
	{
		*dest = *src1;
		rule++; src1++; src2++; dest++;
	}
	else if(opa < src2lv)
	{
		*dest = *src2;
		rule++; src1++; src2++; dest++;
	}
	else
	{
		opa = table[opa];
		rule++;
		s1 = *src1;
		s2 = *src2;
		a1 = s1 >> 24u;
		a2 = s2 >> 24u;
		addr = (a2*opa & 0xff00u) + (a1*(256u-opa) >> 8u);
		alpha = TVPOpacityOnOpacityTable[addr];
		s1_ = s1 & 0xff00ffu;
		s1_ = ((s1_ + (((s2 & 0xff00ffu) - s1_) * alpha >> 8u)) & 0xff00ffu) +
			(TVPNegativeMulTable[addr]<<24u);
		src1++;
		src2++;
		s1 &= 0xff00u;
		s2 &= 0xff00u;
		*dest = s1_ | ((s1 + ((s2 - s1) * alpha >> 8u)) & 0xff00u);
		dest++;
	}
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_a_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len))
{
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPBlendARGB)(src1[{ofs}], src2[{ofs}], table[rule[{ofs}]]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPUnivTransBlend_switch_a_c, (tjs_uint32 *dest, const tjs_uint32 *src1, const tjs_uint32 *src2, const tjs_uint8 *rule, const tjs_uint32 *table, tjs_int len, tjs_int src1lv, tjs_int src2lv))
{
	tjs_int opa;
EOF

$content = <<EOF;
	opa = rule[{ofs}];
	if(opa >= src1lv)
		dest[{ofs}] = src1[{ofs}];
	else if(opa < src2lv)
		dest[{ofs}] = src2[{ofs}];
	else
		dest[{ofs}] = TVP_GL_FUNCNAME(TVPBlendARGB)(src1[{ofs}], src2[{ofs}], table[opa]);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF



;#-----------------------------------------------------------------
;# alpha color mapping
;#-----------------------------------------------------------------


sub alpha_color_map
{
	local($bit, $namesuffix);
	$bit = $_[0];
	$namesuffix = $_[1];

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	tjs_uint32 d1, d, sopa;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	sopa = *src;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + ((c1 - d1) * sopa >> $bit)) & 0xff00ffu);
	d &= 0xff00u;
	*dest = d1 | ((d + ((color - d) * sopa >> $bit)) & 0x00ff00u);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 d1, d, sopa;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	sopa = (*src * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + ((c1 - d1) * sopa >> $bit)) & 0xff00ffu);
	d &= 0x00ff00u;
	*dest = d1 | ((d + ((color - d) * sopa >> $bit)) & 0x00ff00u);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF




}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map(8, '');
&alpha_color_map(6, '65');
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

sub alpha_color_map_hda
{
	local($bit, $namesuffix);
	$bit = $_[0];
	$namesuffix = $_[1];

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_HDA_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	tjs_uint32 d1, d, sopa;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	sopa = *src;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + ((c1 - d1) * sopa >> $bit)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0xff00u;
	*dest = d1 | ((d + ((color - d) * sopa >> $bit)) & 0x00ff00u);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_HDA_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 d1, d, sopa;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	sopa = (*src * opa) >> 8u;
	d1 = d & 0xff00ffu;
	d1 = ((d1 + ((c1 - d1) * sopa >> $bit)) & 0xff00ffu) + (d & 0xff000000u);
	d &= 0x00ff00u;
	*dest = d1 | ((d + ((color - d) * sopa >> $bit)) & 0x00ff00u);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF




}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map_hda(8, '');
&alpha_color_map_hda(6, '65');
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

sub alpha_color_map_d
{
	local($namesuffix);
	$namesuffix = $_[0];

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_d_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	tjs_uint32 d1, d, sopa, addr, destalpha;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	addr = (*src<<8u) + (d>>24u);
	destalpha = TVPNegativeMulTable${namesuffix}[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable${namesuffix}[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + ((c1 - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0x00ff00u;
	*dest = d1 + ((d + ((color - d) * sopa >> 8u)) & 0x00ff00u) + destalpha;
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map_d('');
&alpha_color_map_d('65');
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------


sub alpha_color_map_a
{
	local($namesuffix, $shift);
	$namesuffix = $_[0];
	$shift = $_[1];

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_a_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color))
{
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	tjs_int s_tmp = *src;
	tjs_uint32 tmp =
		((s_tmp * (c1    & 0xff00ffu) >> $shift) & 0xff00ffu) + 
		((s_tmp * (color & 0x00ff00u) >> $shift) & 0x00ff00u);
	s_tmp <<= (8u - $shift);
	s_tmp -= (s_tmp >> 8u); /* adjust alpha */
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_ca)(*dest, s_tmp, s_tmp ^ 0xffu, tmp);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map_a('', 8);
&alpha_color_map_a('65', 6);
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

sub alpha_color_map_do
{
	local($namesuffix);
	$namesuffix = $_[0];

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_do_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 d1, d, sopa, addr, destalpha;
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	addr = ((*src * opa) & 0xff00u) + (d>>24u);
	destalpha = TVPNegativeMulTable${namesuffix}[addr]<<24u;
	sopa = TVPOpacityOnOpacityTable${namesuffix}[addr];
	d1 = d & 0xff00ffu;
	d1 = (d1 + ((c1 - d1) * sopa >> 8u)) & 0xff00ffu;
	d &= 0x00ff00u;
	*dest = d1 + ((d + ((color - d) * sopa >> 8u)) & 0x00ff00u) + destalpha;
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

}


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map_do('');
&alpha_color_map_do('65');
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


sub alpha_color_map_ao
{
	local($namesuffix, $shift);
	$namesuffix = $_[0];
	$shift = $_[1];

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPApplyColorMap${namesuffix}_ao_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 c1 = color & 0xff00ffu;
	color = color & 0x00ff00u;
EOF

$content = <<EOF;
{
	tjs_int s_tmp = (*src * opa) >> 8u;
	tjs_uint32 tmp =
		((s_tmp * (c1    & 0xff00ffu) >> $shift) & 0xff00ffu) + 
		((s_tmp * (color & 0x00ff00u) >> $shift) & 0x00ff00u);
	s_tmp <<= (8u - $shift);
	s_tmp -= (s_tmp >> 8u); /* adjust alpha */
	*dest = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_ca)(*dest, s_tmp, s_tmp ^ 0xffu, tmp);
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
&alpha_color_map_ao('', 8);
&alpha_color_map_ao('65', 6);
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# constant ratio constant color alpha blending
;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstColorAlphaBlend_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	/* this function always holds desitination alpha channel */
	tjs_uint32 s1, d;
	s1 = (color & 0xff00ffu)*opa ;
	color = (color & 0xff00u)*opa ;
	opa = 255u - opa;
EOF

$content = <<EOF;
{
	d = *dest;
	*dest = (d & 0xff000000u) + ((((d & 0xff00ffu) * opa + s1) >> 8u) & 0xff00ffu) +
		((((d&0xff00u) * opa + color) >> 8u) & 0xff00u);
	dest ++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstColorAlphaBlend_d_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 d1, s1, d, dopa;
	tjs_int alpha;
	s1 = color & 0xff00ffu;
	color = color & 0xff00u;
EOF

$content = <<EOF;
{
	d = *dest;
	dopa = d>>24u;
	alpha = TVPOpacityOnOpacityTable[dopa + (opa<<8u)];
	d1 = d & 0xff00ffu;
	d1 = ((d1 + ((s1 - d1) * alpha >> 8u)) & 0xff00ffu) |
		((255u-((255u-dopa)*(255u-opa)>>8u)) << 24u);
	d &= 0xff00u;
	*dest = d1 | ((d + ((color - d) * alpha >> 8u)) & 0xff00u);
	dest ++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConstColorAlphaBlend_a_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color, tjs_int opa))
{
	tjs_uint32 src = TVP_GL_FUNCNAME(TVPMulColor)(color & 0xffffffu, opa);
	tjs_uint32 opa_inv = opa ^ 0xffu;
EOF

$content = <<EOF;
	dest[{ofs}] = TVP_GL_FUNCNAME(TVPAddAlphaBlend_a_ca)(dest[{ofs}], opa, opa_inv, src);
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# opacity removal
;#-----------------------------------------------------------------

;# ??? where are these used in ?


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRemoveConstOpacity_c, (tjs_uint32 *dest, tjs_int len, tjs_int strength))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 d, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 d;
EOF
}

print FC <<EOF;

	strength = 255u - strength;

EOF

$content = <<EOF;
	d = dest[{ofs}];;
	dest[{ofs}] = (d & 0xffffffu) + ( (((d>>24u)*strength) << 16u) & 0xff000000u);;
EOF

$content2 = <<EOF;
	d2 = dest[{ofs}];;
	dest[{ofs}] = (d2 & 0xffffffu) + ( (((d2>>24u)*strength) << 16u) & 0xff000000u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRemoveOpacity_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 d, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 d;
EOF
}

$content = <<EOF;
	d = dest[{ofs}];;
	dest[{ofs}] = (d & 0xffffffu) + ( (((d>>24u) * (255u-src[{ofs}])) << 16u) & 0xff000000u);;
EOF
$content2 = <<EOF;
	d2 = dest[{ofs}];;
	dest[{ofs}] = (d2 & 0xffffffu) + ( (((d2>>24u) * (255u-src[{ofs}])) << 16u) & 0xff000000u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRemoveOpacity_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 d, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 d;
EOF
}

print FC <<EOF;

	if(strength > 127u) strength ++; /* adjust for error */
EOF

$content = <<EOF;
	d = dest[{ofs}];;
	dest[{ofs}] = (d & 0xffffffu) + ( (((d>>24u) * (65535u-src[{ofs}]*strength )) << 8u) & 0xff000000u);;
EOF
$content2 = <<EOF;
	d2 = dest[{ofs}];;
	dest[{ofs}] = (d2 & 0xffffffu) + ( (((d2>>24u) * (65535u-src[{ofs}]*strength )) << 8u) & 0xff000000u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRemoveOpacity65_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 d, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 d;
EOF
}

$content = <<EOF;
	d = dest[{ofs}];;
	dest[{ofs}] = (d & 0xffffffu) + ( (((d>>24u) * (64u-src[{ofs}])) << 18u) & 0xff000000u);;
EOF
$content2 = <<EOF;
	d2 = dest[{ofs}];;
	dest[{ofs}] = (d2 & 0xffffffu) + ( (((d2>>24u) * (64u-src[{ofs}])) << 18u) & 0xff000000u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRemoveOpacity65_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 d, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 d;
EOF
}

print FC <<EOF;

	if(strength > 127u) strength ++; /* adjust for error */
EOF

$content = <<EOF;
	d = dest[{ofs}];;
	dest[{ofs}] = (d & 0xffffffu) + ( (((d>>24u) * (16384u-src[{ofs}]*strength )) << 10u) & 0xff000000u);;
EOF

$content2 = <<EOF;
	d2 = dest[{ofs}];;
	dest[{ofs}] = (d2 & 0xffffffu) + ( (((d2>>24u) * (16384u-src[{ofs}]*strength )) << 10u) & 0xff000000u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPRemoveAdditiveConstOpacity_c, (tjs_uint32 *dest, tjs_int len, tjs_int strength))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPRemoveAdditiveOpacity_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPRemoveAdditiveOpacity_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPRemoveAdditiveOpacity65_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*not export*/
TVP_GL_FUNC_DECL(void, TVPRemoveAdditiveOpacity65_o_c, (tjs_uint32 *dest, const tjs_uint8 *src, tjs_int len, tjs_int strength))
{/*YET NOT IMPLEMENTED*//*MAY LOOSE ADDITIVE STUFF*/
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}



;#-----------------------------------------------------------------
;# pixel addition with saturation
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
tmp = (  ( *src & *dest ) + ( ((*src^*dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp<<1u) - (tmp>>7u);
*dest= (*src + *dest - tmp) | tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
tmp = (  ( *src & *dest ) + ( ((*src^*dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp<<1u) - (tmp>>7u);
*dest= (((*src + *dest - tmp) | tmp) & 0xffffffu) | (*dest & 0xff000000u) ;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ( ((*src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (*src&0xff00ffu) * opa >> 8u)&0xff00ffu);
tmp = (  ( s & *dest ) + ( ((s^*dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
src++;
tmp = (tmp<<1u) - (tmp>>7u);
*dest= (s + *dest - tmp) | tmp;
dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ( ((*src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (*src&0xff00ffu) * opa >> 8u)&0xff00ffu);
tmp = (  ( s & *dest ) + ( ((s^*dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
src++;
tmp = (tmp<<1u) - (tmp>>7u);
*dest= (((s + *dest - tmp) | tmp) & 0xffffffu) + (*dest & 0xff000000u) ;
dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# pixel subtract with saturation
;#-----------------------------------------------------------------

;# thanks Mr. Sugi

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSubBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
tmp = (  ( *src & *dest ) + ( ((*src ^ *dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest = (*src + *dest - tmp) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSubBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, s;
EOF

$content = <<EOF;
{
s = *src | 0xff000000u;
tmp = (  ( s & *dest ) + ( ((s ^ *dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest = (s + *dest - tmp) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSubBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ~*src;
s = ~ (( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu) );
tmp = (  ( s & *dest ) + ( ((s ^ *dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest = (s + *dest - tmp) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSubBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s/*, d*/;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ~*src;
s = 0xff000000u | ~ (( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu) );
tmp = (  ( s & *dest ) + ( ((s ^ *dest)>>1u) & 0x7f7f7f7fu)  ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest = (s + *dest - tmp) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# pixel multiplactive blend
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPMulBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
tmp  = (*dest & 0xffu) * (*src & 0xffu) & 0xff00u;
tmp |= ((*dest & 0xff00u) >> 8u) * (*src & 0xff00u) & 0xff0000u;
tmp |= ((*dest & 0xff0000u) >> 16u) * (*src & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPMulBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
tmp  = (*dest & 0xffu) * (*src & 0xffu) & 0xff00u;
tmp |= ((*dest & 0xff00u) >> 8u) * (*src & 0xff00u) & 0xff0000u;
tmp |= ((*dest & 0xff0000u) >> 16u) * (*src & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = tmp + (*dest & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPMulBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ~*src;
s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
tmp  = (*dest & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((*dest & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((*dest & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPMulBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
s = ~*src;
s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
tmp  = (*dest & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((*dest & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((*dest & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = tmp + (*dest & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# "color dodge" blend
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPColorDodgeBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, tmp2, tmp3;
EOF

$content = <<EOF;
{
tmp2 = ~*src;
tmp = (*dest & 0xffu) * TVPRecipTable256[tmp2 & 0xffu] >> 8u;
tmp3 = (tmp | ((tjs_int32)~(tmp - 0x100u) >> 31u)) & 0xffu;
tmp = ((*dest & 0xff00u)>>8u) * TVPRecipTable256[(tmp2 & 0xff00u)>>8u];
tmp3 |= (tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u;
tmp = ((*dest & 0xff0000u)>>16u) * TVPRecipTable256[(tmp2 & 0xff0000u)>>16u];
tmp3 |= ((tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u ) << 8u;
*dest= tmp3;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPColorDodgeBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, tmp2, tmp3;
EOF

$content = <<EOF;
{
tmp2 = ~*src;
tmp = (*dest & 0xffu) * TVPRecipTable256[tmp2 & 0xffu] >> 8u;
tmp3 = (tmp | ((tjs_int32)~(tmp - 0x100u) >> 31u)) & 0xffu;
tmp = ((*dest & 0xff00u)>>8u) * TVPRecipTable256[(tmp2 & 0xff00u)>>8u];
tmp3 |= (tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u;
tmp = ((*dest & 0xff0000u)>>16u) * TVPRecipTable256[(tmp2 & 0xff0000u)>>16u];
tmp3 |= ((tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u ) << 8u;
*dest= tmp3 + (*dest & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPColorDodgeBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, tmp2, tmp3;
EOF

$content = <<EOF;
{
tmp2 = ~ (( ((*src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (*src&0xff00ffu) * opa >> 8u)&0xff00ffu) );
tmp = (*dest & 0xffu) * TVPRecipTable256[tmp2 & 0xffu] >> 8u;
tmp3 = (tmp | ((tjs_int32)~(tmp - 0x100u) >> 31u)) & 0xffu;
tmp = ((*dest & 0xff00u)>>8u) * TVPRecipTable256[(tmp2 & 0xff00u)>>8u];
tmp3 |= (tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u;
tmp = ((*dest & 0xff0000u)>>16u) * TVPRecipTable256[(tmp2 & 0xff0000u)>>16u];
tmp3 |= ((tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u ) << 8u;
*dest= tmp3;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPColorDodgeBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, tmp2, tmp3;
EOF

$content = <<EOF;
{
tmp2 = ~ (( ((*src&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (*src&0xff00ffu) * opa >> 8u)&0xff00ffu) );
tmp = (*dest & 0xffu) * TVPRecipTable256[tmp2 & 0xffu] >> 8u;
tmp3 = (tmp | ((tjs_int32)~(tmp - 0x100u) >> 31u)) & 0xffu;
tmp = ((*dest & 0xff00u)>>8u) * TVPRecipTable256[(tmp2 & 0xff00u)>>8u];
tmp3 |= (tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u;
tmp = ((*dest & 0xff0000u)>>16u) * TVPRecipTable256[(tmp2 & 0xff0000u)>>16u];
tmp3 |= ((tmp | ((tjs_int32)~(tmp - 0x10000u) >> 31u)) & 0xff00u ) << 8u;
*dest= tmp3 + (*dest & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# darken blend
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDarkenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, m_src;
EOF

$content = <<EOF;
{
m_src = ~*src;
tmp = ((m_src & *dest) + (((m_src ^ *dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest ^= (*dest ^ *src) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDarkenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, m_src;
EOF

$content = <<EOF;
{
m_src = ~*src;
tmp = ((m_src & *dest) + (((m_src ^ *dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest ^= ((*dest ^ *src) & tmp) & 0xffffffu;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDarkenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, m_src, d1;
EOF

$content = <<EOF;
{
m_src = ~*src;
tmp = ((m_src & *dest) + (((m_src ^ *dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
tmp = *dest ^ ((*dest ^ *src) & tmp);
d1 = *dest & 0xff00ffu;
d1 = (d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
m_src = *dest & 0xff00u;
tmp &= 0xff00u;
*dest = d1 + ((m_src + ((tmp - m_src) * opa >> 8u)) & 0xff00u);
dest++;
src++;

}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDarkenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, m_src, d1;
EOF

$content = <<EOF;
{
m_src = ~*src;
tmp = ((m_src & *dest) + (((m_src ^ *dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
tmp = *dest ^ (((*dest ^ *src) & tmp) & 0xffffffu);
d1 = *dest & 0xff00ffu;
d1 = ((d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (*dest & 0xff000000u); /* hda */
m_src = *dest & 0xff00u;
tmp &= 0xff00u;
*dest = d1 + ((m_src + ((tmp - m_src) * opa >> 8u)) & 0xff00u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# lighten blend
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLightenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, m_dest;
EOF

$content = <<EOF;
{
m_dest = ~*dest;
tmp = ((*src & m_dest) + (((*src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest ^= (*dest ^ *src) & tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLightenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, m_dest;
EOF

$content = <<EOF;
{
m_dest = ~*dest;
tmp = ((*src & m_dest) + (((*src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
*dest ^= ((*dest ^ *src) & tmp) & 0xffffffu;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLightenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, m_dest, d1;
EOF

$content = <<EOF;
{
m_dest = ~*dest;
tmp = ((*src & m_dest) + (((*src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
tmp = *dest ^ ((*dest ^ *src) & tmp);
d1 = *dest & 0xff00ffu;
d1 = (d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu;
m_dest = *dest & 0xff00u;
tmp &= 0xff00u;
*dest = d1 + ((m_dest + ((tmp - m_dest) * opa >> 8u)) & 0xff00u);
dest++;
src++;

}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLightenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 tmp, m_dest, d1;
EOF

$content = <<EOF;
{
m_dest = ~*dest;
tmp = ((*src & m_dest) + (((*src ^ m_dest) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
tmp = (tmp << 1u) - (tmp >> 7u);
tmp = *dest ^ (((*dest ^ *src) & tmp) & 0xffffffu);
d1 = *dest & 0xff00ffu;
d1 = ((d1 + (((tmp & 0xff00ffu) - d1) * opa >> 8u)) & 0xff00ffu) + (*dest & 0xff000000u); /* hda */
m_dest = *dest & 0xff00u;
tmp &= 0xff00u;
*dest = d1 + ((m_dest + ((tmp - m_dest) * opa >> 8u)) & 0xff00u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# pixel screen multiplactive blend
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPScreenBlend_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, s, d;
EOF

$content = <<EOF;
{
s = ~*src;
d = ~*dest;
tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = ~tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPScreenBlend_HDA_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
register tjs_uint32 tmp, s, d;
EOF

$content = <<EOF;
{
s = ~*src;
d = ~*dest;
tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = ~tmp ^ (d & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPScreenBlend_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s, d;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
d = ~*dest;
s = *src;
s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = tmp;
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPScreenBlend_HDA_o_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))
{
register tjs_uint32 s, d;
register tjs_uint32 tmp;
EOF

$content = <<EOF;
{
d = ~*dest;
s = *src;
s = ~( ( ((s&0x00ff00u)  * opa >> 8u)&0x00ff00u) +
	(( (s&0xff00ffu) * opa >> 8u)&0xff00ffu));
tmp  = (d & 0xffu) * (s & 0xffu) & 0xff00u;
tmp |= ((d & 0xff00u) >> 8u) * (s & 0xff00u) & 0xff0000u;
tmp |= ((d & 0xff0000u) >> 16u) * (s & 0xff0000u) & 0xff000000u;
tmp >>= 8u;
*dest = ~tmp ^ (d & 0xff000000u);
dest++;
src++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# stretch copy
;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = src[srcstart >> 16u];
			srcstart += srcstep;
			dest[1u] = src[srcstart >> 16u];
			srcstart += srcstep;
			dest[2u] = src[srcstart >> 16u];
			srcstart += srcstep;
			dest[3u] = src[srcstart >> 16u];
			srcstart += srcstep;
			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = src[srcstart >> 16u];
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpStretchCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src1, const tjs_uint32 *src2, tjs_int blend_y, tjs_int srcstart, tjs_int srcstep))
{
	/* stretch copy with bilinear interpolation */
	tjs_int blend_x;
	tjs_int sp;

	blend_y += blend_y >> 7u; /* adjust blend ratio */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y);
			srcstart += srcstep;

			blend_x = (srcstart & 0xffffu) >> 8u;
			sp = srcstart >> 16u;
			dest[1u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
					blend_y);
			srcstart += srcstep;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		blend_x = (srcstart & 0xffffu) >> 8u;
		sp = srcstart >> 16u;
		dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(src1[sp], src1[sp+1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(src2[sp], src2[sp+1u], blend_x),
				blend_y);
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
#define AVG_PACKED(x, y) (((x) & (y)) + ((((x) ^ (y)) & 0xfefefefeu) >> 1u))

/*export*/
TVP_GL_FUNC_DECL(void, TVPFastLinearInterpH2F_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src))
{
	/* horizontal 2x fast linear interpolation; forward */
	destlen -= 2;
	while(destlen > 0)
	{
		dest[0u] = src[0u];
		dest[1u] = AVG_PACKED(src[0u], src[1u]);
		dest += 2u;
		src ++;
		destlen -= 2;
	}

	destlen += 2;

	while(destlen > 0)
	{
		dest[0u] = src[0u];
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFastLinearInterpH2B_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src))
{
	/* horizontal 2x fast linear interpolation; backward */
	destlen -= 2;
	while(destlen > 0)
	{
		dest[0u] = src[0u];
		dest[1u] = AVG_PACKED(src[0u], src[-1u]);
		dest += 2u;
		src --;
		destlen -= 2;
	}

	destlen += 2;

	while(destlen > 0)
	{
		dest[0u] = src[0u];
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFastLinearInterpV2_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src0, const tjs_uint32 *src1))
{
	/* vertical 2x fast linear interpolation */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = AVG_PACKED(src0[0u], src1[0u]);
			dest[1u] = AVG_PACKED(src0[1u], src1[1u]);
			dest[2u] = AVG_PACKED(src0[2u], src1[2u]);
			dest[3u] = AVG_PACKED(src0[3u], src1[3u]);
			dest += 4u;
			src0 += 4u;
			src1 += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = AVG_PACKED(src0[0u], src1[0u]);
		dest ++;
		src0 ++;
		src1 ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPStretchColorCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int srcstart, tjs_int srcstep))
{
	/* this performs only color(main) copy */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = (dest[0u] & 0xff000000u) + (src[srcstart >> 16u] & 0xffffffu);
			srcstart += srcstep;
			dest[1u] = (dest[1u] & 0xff000000u) + (src[srcstart >> 16u] & 0xffffffu);
			srcstart += srcstep;
			dest[2u] = (dest[2u] & 0xff000000u) + (src[srcstart >> 16u] & 0xffffffu);
			srcstart += srcstep;
			dest[3u] = (dest[3u] & 0xff000000u) + (src[srcstart >> 16u] & 0xffffffu);
			srcstart += srcstep;
			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = (dest[0u] & 0xff0000u) + (src[srcstart >> 16u] & 0xffffffu);
		srcstart += srcstep;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------
;# linear transforming copy
;#-----------------------------------------------------------------



print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	/* note that srcpitch unit is in byte */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = *( (const tjs_uint32*)((tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx, sy += stepy;
			dest[1u] = *( (const tjs_uint32*)((tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx, sy += stepy;
			dest[2u] = *( (const tjs_uint32*)((tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx, sy += stepy;
			dest[3u] = *( (const tjs_uint32*)((tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
			sx += stepx, sy += stepy;

			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = *( (const tjs_uint32*)((tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u));
		sx += stepx, sy += stepy;
		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPInterpLinTransCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	/* bilinear interpolation version */
	/* note that srcpitch unit is in byte */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 1;
		while(destlen > 0)
		{
			int blend_x, blend_y;
			const tjs_uint32 *p0, *p1;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y);
			sx += stepx, sy += stepy;

			blend_x = (sx & 0xffffu) >> 8u;
			blend_x += blend_x >> 7u;
			blend_y = (sy & 0xffffu) >> 8u;
			blend_y += blend_y >> 7u;
			p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
			p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
			dest[1u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
				TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
				TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
					blend_y);
			sx += stepx, sy += stepy;

			dest += 2u;
			destlen -= 2;
		}
		destlen += 1;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		int blend_x, blend_y;
		const tjs_uint32 *p0, *p1;

		blend_x = (sx & 0xffffu) >> 8u;
		blend_x += blend_x >> 7u;
		blend_y = (sy & 0xffffu) >> 8u;
		blend_y += blend_y >> 7u;
		p0 = (const tjs_uint32*)((const tjs_uint8*)src + ((sy>>16u)  )*srcpitch) + (sx>>16u);
		p1 = (const tjs_uint32*)((const tjs_uint8*)p0 + srcpitch);
		dest[0u] = TVP_GL_FUNCNAME(TVPBlendARGB)(
			TVP_GL_FUNCNAME(TVPBlendARGB)(p0[0u], p0[1u], blend_x),
			TVP_GL_FUNCNAME(TVPBlendARGB)(p1[0u], p1[1u], blend_x),
				blend_y);
		sx += stepx, sy += stepy;

		dest ++;
		destlen --;
	}
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPLinTransColorCopy_c, (tjs_uint32 *dest, tjs_int destlen, const tjs_uint32 *src, tjs_int sx, tjs_int sy, tjs_int stepx, tjs_int stepy, tjs_int srcpitch))
{
	/* note that srcpitch unit is in byte */
EOF

if($should_unroll == 1)
{
	print FC <<EOF;
		destlen -= 3;
		while(destlen > 0)
		{
			dest[0u] = (dest[0u] & 0xff000000u) + (0x00ffffffu & *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
			sx += stepx, sy += stepy;
			dest[1u] = (dest[1u] & 0xff000000u) + (0x00ffffffu & *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
			sx += stepx, sy += stepy;
			dest[2u] = (dest[2u] & 0xff000000u) + (0x00ffffffu & *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
			sx += stepx, sy += stepy;
			dest[3u] = (dest[3u] & 0xff000000u) + (0x00ffffffu & *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
			sx += stepx, sy += stepy;

			dest += 4u;
			destlen -= 4;
		}
		destlen += 3;
EOF
}

print FC <<EOF;
	while(destlen > 0)
	{
		dest[0u] = (dest[0u] & 0xff000000u) + (0x00ffffffu & *( (const tjs_uint32*)((const tjs_uint8*)src + (sy>>16u)*srcpitch) + (sx>>16u)));
		sx += stepx, sy += stepy;
		dest ++;
		destlen --;
	}
}

EOF



;#-----------------------------------------------------------------
;# make alpha from the color key
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPMakeAlphaFromKey_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 key))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 a;
EOF
}

$content = <<EOF;
	a = dest[{ofs}] & 0xffffffu;;
	if(a != key) a |= 0xff000000u;;
	dest[{ofs}] = a;;
EOF

$content2 = <<EOF;
	b = dest[{ofs}] & 0xffffffu;;
	if(b != key) b |= 0xff000000u;;
	dest[{ofs}] = b;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}



;#-----------------------------------------------------------------
;# copy the mask only
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPCopyMask_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF

$content = <<EOF;
		{
			dest[{ofs}] = (dest[{ofs}] & 0xffffffu) +
				(src[{ofs}] & 0xff000000u);
		}
EOF

print FC <<EOF;
	if(dest < src)
	{
		/* backward */
EOF

&loop_unroll_c_2_backward($content, 'len', 8);

print FC <<EOF;
	}
	else
	{
		/* forward */
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
	}
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# copy the color (main) only
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPCopyColor_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF

$content = <<EOF;
{
	dest[{ofs}] = (dest[{ofs}] & 0xff000000u) +
		(src[{ofs}] & 0x00ffffffu);
}
EOF

print FC <<EOF;
	if(dest < src)
	{
		/* backward */
EOF

&loop_unroll_c_2_backward($content, 'len', 8);

print FC <<EOF;
	}
	else
	{
		/* forward */
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
	}
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# bind mask image to main image 
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBindMaskToMain_c, (tjs_uint32 *main, const tjs_uint8 *mask, tjs_int len))
{
EOF

$content = <<EOF;
{
	main[{ofs}] = (main[{ofs}] & 0xffffffu) + (mask[{ofs}] << 24u);
}
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


;#-----------------------------------------------------------------
;# fill ARGB
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFillARGB_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value))
{
EOF

$content = <<EOF;
{
	dest[{ofs}] = value;
}
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFillARGB_NC_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 value))
{
	/* non-cached version of TVPFillARGB */
	/* this routine written in C has no difference from TVPFillARGB. */ 
EOF

$content = <<EOF;
{
	dest[{ofs}] = value;
}
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# fill color
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFillColor_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 color))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 t1, t2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 t1;
EOF
}

print FC <<EOF;

	color &= 0xffffffu;
EOF

$content = <<EOF;
	t1 = dest[{ofs}];;
	t1 &= 0xff000000u;;
	t1 += color;;
	dest[{ofs}] = t1;;
EOF

$content2 = <<EOF;
	t2 = dest[{ofs}];;
	t2 &= 0xff000000u;;
	t2 += color;;
	dest[{ofs}] = t2;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# fill mask
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPFillMask_c, (tjs_uint32 *dest, tjs_int len, tjs_uint32 mask))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 t1, t2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 t1;
EOF
}

print FC <<EOF;
	mask <<= 24u;
EOF

$content = <<EOF;
	t1 = dest[{ofs}];;
	t1 &= 0x00ffffffu;;
	t1 += mask;;
	dest[{ofs}] = t1;;
EOF

$content2 = <<EOF;
	t2 = dest[{ofs}];;
	t2 &= 0x00ffffffu;;
	t2 += mask;;
	dest[{ofs}] = t2;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 8);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# functions for blur operation
;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 add, sub;
	add = addline[{ofs}];
	sub = subline[{ofs}];
	dest[{ofs}*4u+0u] += ((add    ) & 0xffu) - ((sub    ) & 0xffu);
	dest[{ofs}*4u+1u] += ((add>>8u ) & 0xffu) - ((sub>>8u ) & 0xffu);
	dest[{ofs}*4u+2u] += ((add>>16u) & 0xffu) - ((sub>>16u) & 0xffu);
	dest[{ofs}*4u+3u] += ((add>>24u)       ) - ((sub>>24u)       );
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_d_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 add, sub;
	tjs_int add_a, sub_a;
	add = addline[{ofs}];
	sub = subline[{ofs}];
	dest[{ofs}*4u+3u] += (add_a = (add>>24u)       ) - (sub_a = (sub>>24u)       );
	add_a += add_a >> 7u;
	sub_a += sub_a >> 7u;
	dest[{ofs}*4u+0u] += (((add    ) & 0xffu) * add_a >> 8u) - (((sub    ) & 0xffu) * sub_a >> 8u);
	dest[{ofs}*4u+1u] += (((add>>8u ) & 0xffu) * add_a >> 8u) - (((sub>>8u ) & 0xffu) * sub_a >> 8u);
	dest[{ofs}*4u+2u] += (((add>>16u) & 0xffu) * add_a >> 8u) - (((sub>>16u) & 0xffu) * sub_a >> 8u);
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 add, sub;
	add = addline[{ofs}];
	sub = subline[{ofs}];
	dest[{ofs}*4u+0u] += ((add    ) & 0xffu) - ((sub    ) & 0xffu);
	dest[{ofs}*4u+1u] += ((add>>8u ) & 0xffu) - ((sub>>8u ) & 0xffu);
	dest[{ofs}*4u+2u] += ((add>>16u) & 0xffu) - ((sub>>16u) & 0xffu);
	dest[{ofs}*4u+3u] += ((add>>24u)       ) - ((sub>>24u)       );
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_d_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 add, sub;
	tjs_int add_a, sub_a;
	add = addline[{ofs}];
	sub = subline[{ofs}];
	dest[{ofs}*4u+3u] += (add_a = (add>>24u)       ) - (sub_a = (sub>>24u)       );
	add_a += add_a >> 7u;
	sub_a += sub_a >> 7u;
	dest[{ofs}*4u+0u] += (((add    ) & 0xffu) * add_a >> 8u) - (((sub    ) & 0xffu) * sub_a >> 8u);
	dest[{ofs}*4u+1u] += (((add>>8u ) & 0xffu) * add_a >> 8u) - (((sub>>8u ) & 0xffu) * sub_a >> 8u);
	dest[{ofs}*4u+2u] += (((add>>16u) & 0xffu) * add_a >> 8u) - (((sub>>16u) & 0xffu) * sub_a >> 8u);
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	tjs_int rcp = (1u<<16u) / n;
	tjs_int half_n = n >> 1u;
EOF

$content = <<EOF;
{
	dest[{ofs}] =
		(((sum[0u] + half_n) * rcp >> 16u)       )+
		(((sum[1u] + half_n) * rcp >> 16u) << 8u  )+
		(((sum[2u] + half_n) * rcp >> 16u) << 16u )+
		(((sum[3u] + half_n) * rcp >> 16u) << 24u );

	sum[0u] += add[{ofs}*4u+0u] - sub[{ofs}*4u+0u];
	sum[1u] += add[{ofs}*4u+1u] - sub[{ofs}*4u+1u];
	sum[2u] += add[{ofs}*4u+2u] - sub[{ofs}*4u+2u];
	sum[3u] += add[{ofs}*4u+3u] - sub[{ofs}*4u+3u];
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_d_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	tjs_int rcp = (1u<<16u) / n;
	tjs_int half_n = n >> 1u;
EOF

$content = <<EOF;
{
	tjs_int a = ((sum[3u] + half_n) * rcp >> 16u);
	tjs_uint8 * t = TVPDivTable + (a << 8u);
	dest[{ofs}] =
		(t[(sum[0u] + half_n) * rcp >> 16u]       )+
		(t[(sum[1u] + half_n) * rcp >> 16u] << 8u  )+
		(t[(sum[2u] + half_n) * rcp >> 16u] << 16u )+
		(a << 24u );

	sum[0u] += add[{ofs}*4u+0u] - sub[{ofs}*4u+0u];
	sum[1u] += add[{ofs}*4u+1u] - sub[{ofs}*4u+1u];
	sum[2u] += add[{ofs}*4u+2u] - sub[{ofs}*4u+2u];
	sum[3u] += add[{ofs}*4u+3u] - sub[{ofs}*4u+3u];
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	/* This function is very slow since using divisiion in loop. Function written in assembly should be used. */
	tjs_int half_n = n >> 1u;
EOF

$content = <<EOF;
{
	dest[{ofs}] =
		(((sum[0u] + half_n) / n)       )+
		(((sum[1u] + half_n) / n) << 8u  )+
		(((sum[2u] + half_n) / n) << 16u )+
		(((sum[3u] + half_n) / n) << 24u );

	sum[0u] += add[{ofs}*4u+0u] - sub[{ofs}*4u+0u];
	sum[1u] += add[{ofs}*4u+1u] - sub[{ofs}*4u+1u];
	sum[2u] += add[{ofs}*4u+2u] - sub[{ofs}*4u+2u];
	sum[3u] += add[{ofs}*4u+3u] - sub[{ofs}*4u+3u];
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_d_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	/* This function is very slow since using divisiion in loop. Function written in assembly should be used. */
	tjs_int half_n = n >> 1u;
EOF

$content = <<EOF;
{
	tjs_int a = ((sum[3u] + half_n) / n);
	tjs_uint8 * t = TVPDivTable + (a << 8u);
	dest[{ofs}] =
		(t[(sum[0u] + half_n) / n]       )+
		(t[(sum[1u] + half_n) / n] << 8u  )+
		(t[(sum[2u] + half_n) / n] << 16u )+
		(a << 24u );

	sum[0u] += add[{ofs}*4u+0u] - sub[{ofs}*4u+0u];
	sum[1u] += add[{ofs}*4u+1u] - sub[{ofs}*4u+1u];
	sum[2u] += add[{ofs}*4u+2u] - sub[{ofs}*4u+2u];
	sum[3u] += add[{ofs}*4u+3u] - sub[{ofs}*4u+3u];
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------
;# UD/LR flip
;#-----------------------------------------------------------------

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSwapLine8_c, (tjs_uint8 *line1, tjs_uint8 *line2, tjs_int len))
{
	#define swap_tmp_buf_size 256u
	tjs_uint8 swap_tmp_buf[swap_tmp_buf_size];
	while(len > 0)
	{
		tjs_int le = len < swap_tmp_buf_size ? len : swap_tmp_buf_size;
		memcpy(swap_tmp_buf, line1, le);
		memcpy(line1, line2, le);
		memcpy(line2, swap_tmp_buf, le);
		line1 += le;
		line2 += le;
		len -= le;
	}
	#undef swap_tmp_buf_size
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPSwapLine32_c, (tjs_uint32 *line1, tjs_uint32 *line2, tjs_int len))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 tmp, tmp2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 tmp;
EOF
}

$content = <<EOF;
	tmp = line1[{ofs}];;
	line1[{ofs}] = line2[{ofs}];;
	line2[{ofs}] = tmp;;
EOF

$content2 = <<EOF;
	tmp2 = line1[{ofs}];;
	line1[{ofs}] = line2[{ofs}];;
	line2[{ofs}] = tmp2;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 8);


print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPReverse8_c, (tjs_uint8 *pixels, tjs_int len))
{
	tjs_uint8 *pixels2 = pixels + len -1u;
	len/=2u;
EOF

$content = <<EOF;
{
	tjs_uint8 tmp = *pixels;
	*pixels = *pixels2;
	*pixels2 = tmp;
	pixels2 --;
	pixels++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPReverse32_c, (tjs_uint32 *pixels, tjs_int len))
{
	tjs_uint32 *pixels2 = pixels + len -1u;
	len/=2u;
EOF

$content = <<EOF;
{
	tjs_uint32 tmp = *pixels;
	*pixels = *pixels2;
	*pixels2 = tmp;
	pixels2 --;
	pixels++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}



;#-----------------------------------------------------------------
;# grayscale conversion
;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPDoGrayScale_c, (tjs_uint32 *dest, tjs_int len))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint32 s1, d1, s2, d2;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint32 s1, d1;
EOF
}

$content = <<EOF;
	s1 = dest[{ofs}];;
	d1 = (s1&0xffu)*19u;;
	d1 += ((s1 >> 8u)&0xffu)*183u;;
	d1 += ((s1 >> 16u)&0xffu)*54u;;
	d1 = (d1 >> 8u) * 0x10101u + (s1 & 0xff000000u);;
	dest[{ofs}] = d1;;
EOF

$content2 = <<EOF;
	s2 = dest[{ofs}];;
	d2 = (s2&0xffu)*19u;;
	d2 += ((s2 >> 8u)&0xffu)*183u;;
	d2 += ((s2 >> 16u)&0xffu)*54u;;
	d2 = (d2 >> 8u) * 0x10101u + (s2 & 0xff000000u);;
	dest[{ofs}] = d2;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# BGRA to RGBA
;#-----------------------------------------------------------------







print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRedBlueSwap_c, (tjs_uint32 *dest, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 s = *dest;
	*dest = ( ( ( s & 0xff0000u ) >> 16u ) | ( s & 0xff00ff00u ) | ( ( s & 0xffu ) << 16u ) );
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPRedBlueSwapCopy_c, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))
{
EOF

$content = <<EOF;
{
	tjs_uint32 s = *src;
	*dest = ( ( ( s & 0xff0000u ) >> 16u ) | ( s & 0xff00ff00u ) | ( ( s & 0xffu ) << 16u ) );
	src++;
	dest++;
}
EOF

&loop_unroll_c($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------
;# Gamma Adjustment
;#-----------------------------------------------------------------

print FH <<EOF;
/*[*/
#pragma pack(push, 4)
typedef struct
{
	tjs_uint8 B[256];
	tjs_uint8 G[256];
	tjs_uint8 R[256];
} tTVPGLGammaAdjustTempData;
#pragma pack(pop)
/*]*/
EOF

print FC <<EOF;


/*export*/
TVP_GL_FUNC_DECL(void, TVPInitGammaAdjustTempData_c, (tTVPGLGammaAdjustTempData *temp, const tTVPGLGammaAdjustData *data))
{
	/* make table */

	double ramp = data->RCeil - data->RFloor;
	double gamp = data->GCeil - data->GFloor;
	double bamp = data->BCeil - data->BFloor;

	double rgamma = 1.0/data->RGamma; /* we assume data.?Gamma is a non-zero value here */
	double ggamma = 1.0/data->GGamma;
	double bgamma = 1.0/data->BGamma;

	int i;
	for(i=0;i<256;i++)
	{
#if 1
		double rate = (double)i/255.0;
		int n;
		n = (int)(pow(rate, rgamma)*ramp+0.5+(double)data->RFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->R[i]= n;
		n = (int)(pow(rate, ggamma)*gamp+0.5+(double)data->GFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->G[i]= n;
		n = (int)(pow(rate, bgamma)*bamp+0.5+(double)data->BFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->B[i]= n;
#else
		// pow(x, y) == exp(y * log(x))
		double rate = log((double)i/255.0);
		int n;
		n = (int)(exp(rate * rgamma)*ramp+0.5+(double)data->RFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->R[i]= n;
		n = (int)(exp(rate * ggamma)*gamp+0.5+(double)data->GFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->G[i]= n;
		n = (int)(exp(rate * bgamma)*bamp+0.5+(double)data->BFloor);
		if(n<0) n=0; else if(n>255) n=255;
		temp->B[i]= n;
#endif
	}
}

/*export*/
TVP_GL_FUNC_DECL(void, TVPUninitGammaAdjustTempData_c, (tTVPGLGammaAdjustTempData *temp))
{
	/* nothing to do */
}
EOF



print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdjustGamma_c, (tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp))
{
	tjs_uint32 d1, t1;
EOF

$content = <<EOF;
	d1 = dest[{ofs}];;
	if(d1 > 0x00ffffffu)
	{
		/* process only non-fully-transparent pixel */
		t1 = temp->B[d1 & 0xffu];;
		d1 >>= 8u;;
		t1 += (temp->G[d1 & 0xffu]<<8u);;
		d1 >>= 8u;;
		t1 += (temp->R[d1 & 0xffu]<<16u);;
		t1 += ((d1 & 0xff00u) << 16u);;
		dest[{ofs}] = t1;;
	}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPAdjustGamma_a_c, (tjs_uint32 *dest, tjs_int len, tTVPGLGammaAdjustTempData *temp))
{
	/* gamma adjustment for additive alpha */
EOF

$content = <<EOF;
{
	tjs_uint32 d;
	tjs_int alpha;
	tjs_int alpha_adj;
	tjs_int recip;
	tjs_int t, d_tmp;

	d = dest[{ofs}];

	if(d >= 0xff000000u)
	{
		/* completely opaque */
		t = d & 0xffu;
		d_tmp =   temp->B[t];
		t = (d>>8u) & 0xffu;
		d_tmp |=  temp->G[t] << 8u;
		t = (d>>16u) & 0xffu; 
		d_tmp |=  temp->R[t] << 16u;
		d_tmp |= 0xff000000u;
		dest[{ofs}] = d_tmp;
	}
	else if(d != 0u)
	{
		/* not completely transparent */
		alpha = d >> 24u;
		alpha_adj = alpha + (alpha >> 7u);
		recip = TVPRecipTable256_16[alpha];

		/* B */
		t = d & 0xffu;
		if(t > alpha)
			d_tmp = (temp->B[255u] * alpha_adj >> 8u) + t - alpha;
		else
			d_tmp = temp->B[recip * t >> 8u] * alpha_adj >> 8u;
		/* G */
		t = (d>>8u) & 0xffu; 
		if(t > alpha)
			d_tmp |= ((temp->G[255u] * alpha_adj >> 8u) + t - alpha) << 8u;
		else
			d_tmp |= (temp->G[recip * t >> 8u] * alpha_adj >> 8u) << 8u;
		/* R */
		t = (d>>16u) & 0xffu; 
		if(t > alpha)
			d_tmp |= ((temp->R[255u] * alpha_adj >> 8u) + t - alpha) << 16u;
		else
			d_tmp |= (temp->R[recip * t >> 8u] * alpha_adj >> 8u) << 16u;
		/* A */
		d_tmp |= d & 0xff000000u;

		dest[{ofs}] = d_tmp;
	}
}
EOF

&loop_unroll_c_2($content, 'len', 2);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------
;# simple blur for character data
;#-----------------------------------------------------------------



if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurMulCopy65_c, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_int a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_int a;
EOF
}

$content = <<EOF;
	a = (src[{ofs}] * level >> 18u);;
	if(a>=64u) a = 64u;;
	dest[{ofs}] = a;;
EOF

$content2 = <<EOF;
	b = (src[{ofs}] * level >> 18u);;
	if(b>=64u) b = 64u;;
	dest[{ofs}] = b;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurAddMulCopy65_c, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_int a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_int a;
EOF
}

$content = <<EOF;
	a = dest[{ofs}] +(src[{ofs}] * level >> 18u);;
	if(a>=64u) a = 64u;;
	dest[{ofs}] = a;;
EOF

$content2 = <<EOF;
	b = dest[{ofs}] +(src[{ofs}] * level >> 18u);;
	if(b>=64u) b = 64u;;
	dest[{ofs}] = b;;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 4);

print FC <<EOF;
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


print FC <<EOF;

/* fast_int_hypot from http://demo.and.or.jp/makedemo/effect/math/hypot/fast_hypot.c */
TVP_GL_FUNC_STATIC_DECL(tjs_uint, fast_int_hypot, (tjs_int lx, tjs_int ly))
{
	tjs_uint len1, len2,t,length;

/*	lx = abs(lx); */
/*	ly = abs(ly); */
	if(lx<0) lx = -lx;
	if(ly<0) ly = -ly;
	/*
		CWD
		XOR EAX,EDX
		SUB EAX,EDX
	*/
	
	if (lx >= ly)
	{
		len1 = lx ; len2 = ly;
	}
	else
	{
		len1 = ly ; len2 = lx;
	}

	t = len2 + (len2 >> 1u) ;
	length = len1 - (len1 >> 5u) - (len1 >> 7u) + (t >> 2u) + (t >> 6u) ;
	return length;
}

EOF

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/* simple blur for character data */
/* shuld be more optimized */

/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurCopy65_c, (tjs_uint8 *dest, tjs_int destpitch, tjs_int destwidth, tjs_int destheight, const tjs_uint8 * src, tjs_int srcpitch, tjs_int srcwidth, tjs_int srcheight, tjs_int blurwidth, tjs_int blurlevel))
{
	tjs_int lvsum, x, y;

	/* clear destination */
	memset(dest, 0u, destpitch*destheight);

	/* compute filter level */
	lvsum = 0u;
	for(y = -blurwidth; y <= blurwidth; y++)
	{
		for(x = -blurwidth; x <= blurwidth; x++)
		{
			tjs_int len = TVP_GL_FUNCNAME(fast_int_hypot)(x, y);
			if(len <= blurwidth)
				lvsum += (blurwidth - len +1u);
		}
	}

	if(lvsum) lvsum = (1u<<18u)/lvsum; else lvsum=(1u<<18u);

	/* apply */
	for(y = -blurwidth; y <= blurwidth; y++)
	{
		for(x = -blurwidth; x <= blurwidth; x++)
		{
			tjs_int len = TVP_GL_FUNCNAME(fast_int_hypot)(x, y);
			if(len <= blurwidth)
			{
				tjs_int sy;

				len = blurwidth - len +1u;
				len *= lvsum;
				len *= blurlevel;
				len >>= 8u;
				for(sy = 0; sy < srcheight; sy++)
				{
					TVPChBlurAddMulCopy65(dest + (y + sy + blurwidth)*destpitch + x + blurwidth, 
						src + sy * srcpitch, srcwidth, len);
				}
			}
		}
	}
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}


if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurMulCopy_c, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level) )
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_int a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_int a;
EOF
}
print FC <<EOF;
	{
EOF

if($should_unroll == 1) {
print FC <<EOF;
		int ___index = 0u;
EOF
	print FC <<EOF;
		len -= (4u-1u);

		while(___index < len)
		{
			a = (src[(___index+(0u*2u))] * level >> 18u);
			b = (src[(___index+(0u*2u+1u))] * level >> 18u);
			if(a>=255u) a = 255u;
			if(b>=255u) b = 255u;
			dest[(___index+(0u*2u))] = a;
			dest[(___index+(0u*2u+1u))] = b;
			a = (src[(___index+(1u*2u))] * level >> 18u);
			b = (src[(___index+(1u*2u+1u))] * level >> 18u);
			if(a>=255u) a = 255u;
			if(b>=255u) b = 255u;
			dest[(___index+(1u*2u))] = a;
			dest[(___index+(1u*2u+1u))] = b;
			___index += 4u;
		}

		len += (4u-1u);
EOF
print FC <<EOF;
		while(___index < len)
		{
			a = (src[___index] * level >> 18u);;
			if(a>=255u) a = 255u;
			dest[___index] = a;;
			___index ++;
		}
EOF
}
else
{
print FC <<EOF;
		for(int ___index = 0u; ___index < len; ___index++)
		{
			a = (src[___index] * level >> 18u);;
			if(a>=255u) a = 255u;
			dest[___index] = a;;
		}
EOF
}

print FC <<EOF;
	}
}

EOF

if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurAddMulCopy_c, (tjs_uint8 *dest, const tjs_uint8 *src, tjs_int len, tjs_int level) )
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_int a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_int a;
EOF
}

print FC <<EOF;
	{
EOF

if($should_unroll == 1) {
print FC <<EOF;
		int ___index = 0u;
EOF
	print FC <<EOF;
		len -= (4u-1u);

		while(___index < len)
		{
			a = dest[(___index+(0u*2u))] +(src[(___index+(0u*2u))] * level >> 18u);
			b = dest[(___index+(0u*2u+1u))] +(src[(___index+(0u*2u+1u))] * level >> 18u);
			if(a>=255u) a = 255u;
			if(b>=255u) b = 255u;
			dest[(___index+(0u*2u))] = a;
			dest[(___index+(0u*2u+1u))] = b;
			a = dest[(___index+(1u*2u))] +(src[(___index+(1u*2u))] * level >> 18u);
			b = dest[(___index+(1u*2u+1u))] +(src[(___index+(1u*2u+1u))] * level >> 18u);
			if(a>=255u) a = 255u;
			if(b>=255u) b = 255u;
			dest[(___index+(1u*2u))] = a;
			dest[(___index+(1u*2u+1u))] = b;
			___index += 4u;
		}

		len += (4u-1u);
EOF
print FC <<EOF;
		while(___index < len)
		{
			a = dest[___index] +(src[___index] * level >> 18u);;
			if(a>=255u) a = 255u;;
			dest[___index] = a;;
			___index ++;
		}
EOF
}
else
{
print FC <<EOF;
		for(int ___index = 0u; ___index < len; ___index++)
		{
			a = dest[___index] +(src[___index] * level >> 18u);;
			if(a>=255u) a = 255u;;
			dest[___index] = a;;
			___index ++;
		}
EOF
}


print FC <<EOF;
	}
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

if($skip_template_funcs == 1)
{
print FC <<EOF;
#if 0
EOF
}
print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPChBlurCopy_c, (tjs_uint8 *dest, tjs_int destpitch, tjs_int destwidth, tjs_int destheight, const tjs_uint8 * src, tjs_int srcpitch, tjs_int srcwidth, tjs_int srcheight, tjs_int blurwidth, tjs_int blurlevel) )
{
	tjs_int lvsum, x, y;

	/* clear destination */
	memset(dest, 0u, destpitch*destheight);

	/* compute filter level */
	lvsum = 0;
	for(y = -blurwidth; y <= blurwidth; y++)
	{
		for(x = -blurwidth; x <= blurwidth; x++)
		{
			tjs_int len = TVP_GL_FUNCNAME(fast_int_hypot)(x, y);
			if(len <= blurwidth)
				lvsum += (blurwidth - len +1);
		}
	}

	if(lvsum) lvsum = (1u<<18u)/lvsum; else lvsum=(1u<<18u);

	/* apply */
	for(y = -blurwidth; y <= blurwidth; y++)
	{
		for(x = -blurwidth; x <= blurwidth; x++)
		{
			tjs_int len = TVP_GL_FUNCNAME(fast_int_hypot)(x, y);
			if(len <= blurwidth)
			{
				tjs_int sy;

				len = blurwidth - len +1u;
				len *= lvsum;
				len *= blurlevel;
				len >>= 8u;
				for(sy = 0u; sy < srcheight; sy++)
				{
					TVPChBlurAddMulCopy(dest + (y + sy + blurwidth)*destpitch + x + blurwidth, 
						src + sy * srcpitch, srcwidth, len);
				}
			}
		}
	}
}

EOF
if($skip_template_funcs == 1)
{
print FC <<EOF;
#endif
EOF
}

;#-----------------------------------------------------------------
;# pixel format conversion
;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand1BitTo8BitPal_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
	tjs_uint8 p[2];
	tjs_uint8 *d=dest, *dlim;
	tjs_uint8 b;

	p[0u] = pal[0u]&0xffu, p[1u] = pal[1u]&0xffu;
	dlim = dest + len-7u;
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = p[(tjs_uint)(b&(tjs_uint)0x80u)>>7u];
		d[1u] = p[(tjs_uint)(b&(tjs_uint)0x40u)>>6u];
		d[2u] = p[(tjs_uint)(b&(tjs_uint)0x20u)>>5u];
		d[3u] = p[(tjs_uint)(b&(tjs_uint)0x10u)>>4u];
		d[4u] = p[(tjs_uint)(b&(tjs_uint)0x08u)>>3u];
		d[5u] = p[(tjs_uint)(b&(tjs_uint)0x04u)>>2u];
		d[6u] = p[(tjs_uint)(b&(tjs_uint)0x02u)>>1u];
		d[7u] = p[(tjs_uint)(b&(tjs_uint)0x01u)   ];
		d += 8u;
	}
	dlim = dest + len;
	b = *buf;
	while(d<dlim)
	{
		*(d++) = (b&0x80u) ? p[1u] : p[0u];
		b<<=1u;
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand1BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len))
{
	tjs_uint8 *d=dest, *dlim;
	tjs_uint8 b;

	dlim = dest + len-7u;
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = (tjs_uint8)((b&(tjs_uint)0x80u)>>7u);
		d[1u] = (tjs_uint8)((b&(tjs_uint)0x40u)>>6u);
		d[2u] = (tjs_uint8)((b&(tjs_uint)0x20u)>>5u);
		d[3u] = (tjs_uint8)((b&(tjs_uint)0x10u)>>4u);
		d[4u] = (tjs_uint8)((b&(tjs_uint)0x08u)>>3u);
		d[5u] = (tjs_uint8)((b&(tjs_uint)0x04u)>>2u);
		d[6u] = (tjs_uint8)((b&(tjs_uint)0x02u)>>1u);
		d[7u] = (tjs_uint8)((b&(tjs_uint)0x01u)   );
		d += 8u;
	}
	dlim = dest + len;
	b = *buf;
	while(d<dlim)
	{
		*(d++) = (b&0x80u) ? 1u : 0u;
		b<<=1u;
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand1BitTo32BitPal_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
	tjs_uint32 p[2u];
	tjs_uint32 *d=dest, *dlim;
	tjs_uint8 b;

	p[0u] = pal[0u], p[1u] = pal[1u];
	dlim = dest + len-7u;
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = p[(tjs_uint)(b&(tjs_uint)0x80u)>>7u];
		d[1u] = p[(tjs_uint)(b&(tjs_uint)0x40u)>>6u];
		d[2u] = p[(tjs_uint)(b&(tjs_uint)0x20u)>>5u];
		d[3u] = p[(tjs_uint)(b&(tjs_uint)0x10u)>>4u];
		d[4u] = p[(tjs_uint)(b&(tjs_uint)0x08u)>>3u];
		d[5u] = p[(tjs_uint)(b&(tjs_uint)0x04u)>>2u];
		d[6u] = p[(tjs_uint)(b&(tjs_uint)0x02u)>>1u];
		d[7u] = p[(tjs_uint)(b&(tjs_uint)0x01u)   ];
		d += 8u;
	}
	dlim = dest + len;
	b = *buf;
	while(d<dlim)
	{
		*(d++) = (b&0x80u) ? p[1u] : p[0u];
		b<<=1u;
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand4BitTo8BitPal_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
	tjs_uint8 *d=dest, *dlim;
	tjs_uint8 b;

	dlim = dest + (len & ~1u);
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = (tjs_uint8)pal[(b&0xf0u)>>4u];
		d[1u] = (tjs_uint8)pal[b&0x0fu];
		d += 2u;
	}
	if(len & 1u)
	{
		b = *buf;
		if(d<dlim) *d = (tjs_uint8)pal[(b&0xf0u)>>4u];
	}
}
EOF
;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand4BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len))
{
	tjs_uint8 *d=dest, *dlim;
	tjs_uint8 b;

	dlim = dest + (len & ~1u);
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = (tjs_uint8)((b&0xf0u)>>4u);
		d[1u] = (tjs_uint8)(b&0x0fu);
		d += 2u;
	}
	if(len & 1u)
	{
		b = *buf;
		if(d<dlim) *d = (tjs_uint8)((b&0xf0u)>>4u);
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand4BitTo32BitPal_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
	tjs_uint32 *d=dest, *dlim;
	tjs_uint8 b;

	dlim = dest + (len & ~1u);
	while(d < dlim)
	{
		b = *(buf++);
		d[0u] = pal[(b&0xf0u)>>4u];
		d[1u] = pal[b&0x0fu];
		d += 2u;
	}
	if(len & 1u)
	{
		b = *buf;
		*d = pal[(b&0xf0u)>>4u];
	}
}
EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand8BitTo8BitPal_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
EOF

$content = <<EOF;
{
	dest[{ofs}] = pal[buf[{ofs}]]&0xffu;
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------


print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLExpand8BitTo32BitPal_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len, const tjs_uint32 *pal))
{
EOF

$content = <<EOF;
{
	dest[{ofs}] = pal[buf[{ofs}]];
}
EOF

&loop_unroll_c_2($content, 'len', 8);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPExpand8BitTo32BitGray_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len))
{
EOF
if($should_unroll == 1)
{
	print FC <<EOF;
	tjs_uint8 a, b;
EOF
}

if($should_unroll == 0)
{
	print FC <<EOF;
	tjs_uint8 a;
EOF
}

$content = <<EOF;
	a = buf[{ofs}];;
	dest[{ofs}] = 0xff000000u + (a * 0x10101u);;
EOF

$content2 = <<EOF;
	b = buf[{ofs}];;
	dest[{ofs}] = 0xff000000u + (b * 0x10101u);;
EOF

&loop_unroll_c_int_2($content, $content2, 'len', 8);

print FC <<EOF;
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert15BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint16 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint16 s = *(tjs_uint8*)(buf+{ofs}) << 8u + *((tjs_uint8*)(buf+{ofs})+1u);
#else
	tjs_uint16 s = buf[{ofs}];
#endif
	dest[{ofs}] =
		((s&0x7c00u)*56u+ (s&0x03e0u)*(187u<<5u)+ (s&0x001fu)*(21u<<10u)) >> 15u;
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert15BitTo32Bit_c, (tjs_uint32 *dest, const tjs_uint16 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint16 s = *(tjs_uint8*)(buf+{ofs}) << 8u + *((tjs_uint8*)(buf+{ofs})+1u);
#else
	tjs_uint16 s = buf[{ofs}];
#endif
	tjs_int r = s&0x7c00u;
	tjs_int g = s&0x03e0u;
	tjs_int b = s&0x001fu;
	dest[{ofs}] = 0xff000000u +
		(r <<  9u) + ((r&0x7000u)<<4u) +
		(g <<  6u) + ((g&0x0380u)<<1u) +
		(b <<  3u) + (b>>2u);
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;
# define compose_grayscale(r,g,b) ((unsigned char)((((tjs_int)(b)*19u + (tjs_int)(g)*183u + (tjs_int)(r)*54u)>>8u)))
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert24BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint8 *buf, tjs_int len))
{
	tjs_uint8 *slimglim = dest + len;
EOF

if($should_unroll == 1) {
	print FC <<EOF;
		tjs_uint8 *slimglims = slimglim - 3u;
		while(dest < slimglims)
		{
			dest[0u] = compose_grayscale(buf[2u], buf[1u], buf[0u]);
			dest[1u] = compose_grayscale(buf[5u], buf[4u], buf[3u]);
			dest[2u] = compose_grayscale(buf[8u], buf[7u], buf[6u]);
			dest[3u] = compose_grayscale(buf[11u], buf[10u], buf[9u]);
			dest += 4u;
			buf += 12u;
		}
EOF
}

print FC <<EOF;
	while(dest < slimglim)
	{
		dest[0u] = compose_grayscale(buf[2u], buf[1u], buf[0u]);
		dest ++;
		buf += 3u;
	}
}

EOF


;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert24BitTo32Bit_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len))
{
	tjs_uint32 *slimglim = dest + len;
EOF

if($should_unroll == 1) {
	print FC <<EOF;
	tjs_uint32 *slimglims = slimglim - 7u;
	while(dest < slimglims)
	{
#if TJS_HOST_IS_BIG_ENDIAN
		dest[0u] = 0xff000000u + buf[0u] + (buf[1u]<<8u) + (buf[2u]<<16u);
		dest[1u] = 0xff000000u + buf[3u] + (buf[4u]<<8u) + (buf[5u]<<16u);
		dest[2u] = 0xff000000u + buf[6u] + (buf[7u]<<8u) + (buf[8u]<<16u);
		dest[3u] = 0xff000000u + buf[9u] + (buf[10u]<<8u) + (buf[11u]<<16u);
		dest += 4u;
		buf += 12u;
		dest[0u] = 0xff000000u + buf[0u] + (buf[1u]<<8u) + (buf[2u]<<16u);
		dest[1u] = 0xff000000u + buf[3u] + (buf[4u]<<8u) + (buf[5u]<<16u);
		dest[2u] = 0xff000000u + buf[6u] + (buf[7u]<<8u) + (buf[8u]<<16u);
		dest[3u] = 0xff000000u + buf[9u] + (buf[10u]<<8u) + (buf[11u]<<16u);
		dest += 4u;
		buf += 12u;
#else
		tjs_uint32 a = *(tjs_uint32*)buf, b;
		tjs_uint32 c = *(tjs_uint32*)(buf+12u), d;
		dest[0u] = 0xff000000u + (a & 0x00ffffffu);
		dest[4u] = 0xff000000u + (c & 0x00ffffffu);
		b = *(tjs_uint32*)(buf+4u);
		d = *(tjs_uint32*)(buf+16u);
		dest[1u] = 0xff000000u + ((a >> 24u) + ((b & 0xffffu)<<8u));
		dest[5u] = 0xff000000u + ((c >> 24u) + ((d & 0xffffu)<<8u));
		a = *(tjs_uint32*)(buf+8u);
		c = *(tjs_uint32*)(buf+20u);
		dest[2u] = 0xff000000u + ((b >> 16u) + ((a & 0xffu)<<16u));
		dest[6u] = 0xff000000u + ((d >> 16u) + ((c & 0xffu)<<16u));
		dest[3u] = 0xff000000u + (a >> 8u);
		dest[7u] = 0xff000000u + (c >> 8u);
		dest += 8u;
		buf += 24u;
#endif
	}
EOF
}

print FC <<EOF;
	while(dest < slimglim)
	{
#if TJS_HOST_IS_BIG_ENDIAN
		*(dest++) = 0xff000000u + buf[0u] + (buf[1u]<<8u) + (buf[2u]<<16u);
#else
		*(dest++) = 0xff000000u + buf[0u] + (buf[1u]<<8u) + (buf[2u]<<16u);
#endif
		buf += 3u;
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPConvert24BitTo32Bit_c, (tjs_uint32 *dest, const tjs_uint8 *buf, tjs_int len))
{
	/* this function does not matter the host endian */
	tjs_uint32 *slimglim = dest + len;
EOF

if($should_unroll == 1) {
	print FC <<EOF;
	tjs_uint32 *slimglims = slimglim - 7u;
	while(dest < slimglims)
	{
		tjs_uint32 a = *(tjs_uint32*)buf, b;
		tjs_uint32 c = *(tjs_uint32*)(buf+12u), d;
		dest[0u] = 0xff000000u + (a & 0x00ffffffu);
		dest[4u] = 0xff000000u + (c & 0x00ffffffu);
		b = *(tjs_uint32*)(buf+4u);
		d = *(tjs_uint32*)(buf+16u);
		dest[1u] = 0xff000000u + ((a >> 24u) + ((b & 0xffffu)<<8u));
		dest[5u] = 0xff000000u + ((c >> 24u) + ((d & 0xffffu)<<8u));
		a = *(tjs_uint32*)(buf+8u);
		c = *(tjs_uint32*)(buf+20u);
		dest[2u] = 0xff000000u + ((b >> 16u) + ((a & 0xffu)<<16u));
		dest[6u] = 0xff000000u + ((d >> 16u) + ((c & 0xffu)<<16u));
		dest[3u] = 0xff000000u + (a >> 8u);
		dest[7u] = 0xff000000u + (c >> 8u);
		dest += 8u;
		buf += 24u;
	}
EOF
}

print FC <<EOF;
	while(dest < slimglim)
	{
		*(dest++) = 0xff000000u + buf[0u] + (buf[1u]<<8u) + (buf[2u]<<16u);
		buf += 3u;
	}
}
EOF

;#-----------------------------------------------------------------

print FC <<EOF;
/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert32BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint32 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = compose_grayscale(d&0xffu, (d&0xff00u)>>8u, (d&0xff0000u)>>16u);
#else
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = compose_grayscale((d&0xff0000u)>>16u, (d&0xff00u)>>8u, d&0xffu);
#endif
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;
}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert32BitTo32Bit_NoneAlpha_c, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = 0xff000000u + ((d&0xff00u)<<8u) +  ((d&0xff0000u)>>8u) + ((d&0xff000000u)>>24u);
#else
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = d | 0xff000000u;
#endif
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF

;#-----------------------------------------------------------------


print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert32BitTo32Bit_MulAddAlpha_c, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = ((d&0xffu)<<24u) + ((d&0xff00u)<<8u) +  ((d&0xff0000u)>>8u) + ((d&0xff000000u)>>24u);
#else
	tjs_uint32 d = buf[{ofs}];
	dest[{ofs}] = d;
#endif
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPBLConvert32BitTo32Bit_AddAlpha_c, (tjs_uint32 *dest, const tjs_uint32 *buf, tjs_int len))
{
EOF

$content = <<EOF;
{
#if TJS_HOST_IS_BIG_ENDIAN
	tjs_uint32 d = buf[{ofs}];
	tjs_uint8 *t = TVPDivTable + ((d & 0xffu)<<8u);
	dest[{ofs}] = ((d&0xffu)<<24u) + (t[(d&0xff00u)>>8u]<<16u) +  (t[(d&0xff0000u)>>16u]<<8u) + (t[(d&0xff000000u)>>24u]);
#else
	tjs_uint32 d = buf[{ofs}];
	tjs_uint8 *t = TVPDivTable + ((d>>16u) & 0xff00u);
	dest[{ofs}] = (d&0xff000000u) + (t[(d&0xff0000u)>>16u]<<16u) + (t[(d&0xff00u)>>8u]<<8u) + (t[d&0xffu]);
#endif
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF

;#-----------------------------------------------------------------
;# conversion of 32bpp->16bpp(565/555) with dithering
;#-----------------------------------------------------------------


print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPDither32BitTo16Bit565_c, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs))
{

tjs_uint8 *line = TVPDitherTable_5_6[yofs & 0x03u][0u][0u];
tjs_int x = (xofs & 0x03u) << 9u;


EOF

$content = <<EOF;
{
tjs_uint32 v = *src;
*dest = (line[x + ((v >> 16u) & 0xffu)] << 11u)+  (line[x + (v & 0xffu)]) +
	(line[x + 256u + ((v >> 8u) & 0xffu)] << 5u);
dest++;
src++;
x+= 0x200u;
x &= 0x600u;
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF

;#-----------------------------------------------------------------

print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPDither32BitTo16Bit555_c, (tjs_uint16 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs))
{

tjs_uint8 *line = TVPDitherTable_5_6[yofs & 0x03u][0u][0u];
tjs_int x = (xofs & 0x03u) << 9u;


EOF

$content = <<EOF;
{
tjs_uint32 v = *src;
*dest = (line[x + ((v >> 16u) & 0xffu)] << 10u) + (line[x + (v & 0xffu)]) +
	(line[x + ((v >> 8u) & 0xffu)] << 5u);
dest++;
src++;
x+= 0x200u;
x &= 0x600u;
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF

;#-----------------------------------------------------------------
;# conversion of 32bpp->8bpp(6u*7u*6u) with dithering
;#-----------------------------------------------------------------


print FC <<EOF;

/*export*/
TVP_GL_FUNC_DECL(void, TVPDither32BitTo8Bit_c, (tjs_uint8 *dest, const tjs_uint32 *src, tjs_int len, tjs_int xofs, tjs_int yofs))
{

tjs_uint8 *line = &(TVPDitherTable_676[0u][yofs & 0x03u][0u][0u]);
tjs_int x = (xofs & 0x03u) << 8u;


EOF

$content = <<EOF;
{
tjs_uint32 v = *src;
*dest = (line[x + ((v >> 16u) & 0xffu)])+ (line[(256u * 16u * 2u) + x + (v & 0xffu)]) +
	(line[(16u * 256u) + x + ((v >> 8u) & 0xffu)]);
dest++;
src++;
x += 0x100u;
x &= 0x300u;
}
EOF

&loop_unroll_c_2($content, 'len', 4);

print FC <<EOF;

}

EOF



;#-----------------------------------------------------------------
;# tlg5 lossless graphics decompressor
;#-----------------------------------------------------------------

print FC <<EOF;


/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG5ComposeColors3To4_c, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const * buf, tjs_int width))
{
	tjs_int x;
	tjs_uint8 pc[3u];
	tjs_uint8 c[3u];
	pc[0u] = pc[1u] = pc[2u] = 0u;
	for(x = 0u; x < width; x++)
	{
		c[0u] = buf[0u][x];
		c[1u] = buf[1u][x];
		c[2u] = buf[2u][x];
		c[0u] += c[1u]; c[2u] += c[1u];
		*(tjs_uint32 *)outp =
								((((pc[0u] += c[0u]) + upper[0u]) & 0xffu)      ) +
								((((pc[1u] += c[1u]) + upper[1u]) & 0xffu) <<  8u) +
								((((pc[2u] += c[2u]) + upper[2u]) & 0xffu) << 16u) +
								0xff000000u;
		outp += 4u;
		upper += 4u;
	}
}

/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG5ComposeColors4To4_c, (tjs_uint8 *outp, const tjs_uint8 *upper, tjs_uint8 * const* buf, tjs_int width))
{
	tjs_int x;
	tjs_uint8 pc[4u];
	tjs_uint8 c[4u];
	pc[0u] = pc[1u] = pc[2u] = pc[3u] = 0u;
	for(x = 0u; x < width; x++)
	{
		c[0u] = buf[0u][x];
		c[1u] = buf[1u][x];
		c[2u] = buf[2u][x];
		c[3u] = buf[3u][x];
		c[0u] += c[1u]; c[2u] += c[1u];
		*(tjs_uint32 *)outp =
								((((pc[0u] += c[0u]) + upper[0u]) & 0xffu)      ) +
								((((pc[1u] += c[1u]) + upper[1u]) & 0xffu) <<  8u) +
								((((pc[2u] += c[2u]) + upper[2u]) & 0xffu) << 16u) +
								((((pc[3u] += c[3u]) + upper[3u]) & 0xffu) << 24u);
		outp += 4u;
		upper += 4u;
	}
}

/*export*/
TVP_GL_FUNC_DECL(tjs_int, TVPTLG5DecompressSlide_c, (tjs_uint8 *out, const tjs_uint8 *in, tjs_int insize, tjs_uint8 *text, tjs_int initialr))
{
	tjs_int r = initialr;
	tjs_uint flags = 0u;
	const tjs_uint8 *inlim = in + insize;
	while(in < inlim)
	{
		if(((flags >>= 1u) & 256u) == 0u)
		{
			flags = 0u[in++] | 0xff00u;
		}
		if(flags & 1u)
		{
			tjs_int mpos = in[0u] | ((in[1u] & 0xfu) << 8u);
			tjs_int mlen = (in[1u] & 0xf0u) >> 4u;
			in += 2u;
			mlen += 3u;
			if(mlen == 18u) mlen += 0u[in++];

			while(mlen--)
			{
				0u[out++] = text[r++] = text[mpos++];
				mpos &= (4096u - 1u);
				r &= (4096u - 1u);
			}
		}
		else
		{
			unsigned char c = 0u[in++];
			0u[out++] = c;
			text[r++] = c;
/*			0u[out++] = text[r++] = 0u[in++];*/
			r &= (4096u - 1u);
		}
	}
	return r;
}

EOF


;#-----------------------------------------------------------------
;# tlg6 lossless/near-lossless graphics decompressor
;#-----------------------------------------------------------------

print FC <<EOF;

#if TJS_HOST_IS_BIG_ENDIAN
	#define TVP_TLG6_BYTEOF(a, x) (((tjs_uint8*)(a))[(x)])

	#define TVP_TLG6_FETCH_32BITS(addr) ((tjs_uint32)TVP_TLG6_BYTEOF((addr), 0u) +  \\
									((tjs_uint32)TVP_TLG6_BYTEOF((addr), 1u) << 8u) + \\
									((tjs_uint32)TVP_TLG6_BYTEOF((addr), 2u) << 16u) + \\
									((tjs_uint32)TVP_TLG6_BYTEOF((addr), 3u) << 24u) )
#else
	#define TVP_TLG6_FETCH_32BITS(addr) (*(tjs_uint32*)addr)
#endif



/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG6DecodeGolombValuesForFirst_c, (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool))
{
	/*
		decode values packed in "bit_pool".
		values are coded using golomb code.

		"ForFirst" function do dword access to pixelbuf,
		clearing with zero except for blue (least siginificant byte).
	*/

	int n = TVP_TLG6_GOLOMB_N_COUNT - 1u; /* output counter */
	int a = 0u; /* summary of absolute values of errors */

	tjs_int bit_pos = 1u;
	tjs_uint8 zero = (*bit_pool & 1u)?0u:1u;

	tjs_int8 * limit = pixelbuf + pixel_count*4u;

	while(pixelbuf < limit)
	{
		/* get running count */
		int count;

		{
			tjs_uint32 t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
			tjs_int b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
			int bit_count = b;
			while(!b)
			{
				bit_count += TVP_TLG6_LeadingZeroTable_BITS;
				bit_pos += TVP_TLG6_LeadingZeroTable_BITS;
				bit_pool += bit_pos >> 3u;
				bit_pos &= 7u;
				t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
				b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
				bit_count += b;
			}


			bit_pos += b;
			bit_pool += bit_pos >> 3u;
			bit_pos &= 7u;

			bit_count --;
			count = 1u << bit_count;
			count += ((TVP_TLG6_FETCH_32BITS(bit_pool) >> (bit_pos)) & (count-1u));

			bit_pos += bit_count;
			bit_pool += bit_pos >> 3u;
			bit_pos &= 7u;

		}

		if(zero)
		{
			/* zero values */

			/* fill distination with zero */
			do { *(tjs_uint32*)pixelbuf = 0u; pixelbuf+=4u; } while(--count);

			zero ^= 1u;
		}
		else
		{
			/* non-zero values */

			/* fill distination with glomb code */

			do
			{
				int k = TVPTLG6GolombBitLengthTable[a][n], v, sign;

				tjs_uint32 t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
				tjs_int bit_count;
				tjs_int b;
				if(t)
				{
					b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
					bit_count = b;
					while(!b)
					{
						bit_count += TVP_TLG6_LeadingZeroTable_BITS;
						bit_pos += TVP_TLG6_LeadingZeroTable_BITS;
						bit_pool += bit_pos >> 3u;
						bit_pos &= 7u;
						t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
						b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
						bit_count += b;
					}
					bit_count --;
				}
				else
				{
					bit_pool += 5u;
					bit_count = bit_pool[-1u];
					bit_pos = 0u;
					t = TVP_TLG6_FETCH_32BITS(bit_pool);
					b = 0u;
				}


				v = (bit_count << k) + ((t >> b) & ((1u<<k)-1u));
				sign = (v & 1u) - 1u;
				v >>= 1u;
				a += v;
				*(tjs_uint32*)pixelbuf = (unsigned char) ((v ^ sign) + sign + 1u);
				pixelbuf += 4u;

				bit_pos += b;
				bit_pos += k;
				bit_pool += bit_pos >> 3u;
				bit_pos &= 7u;

				if (--n < 0u) {
					a >>= 1u;  n = TVP_TLG6_GOLOMB_N_COUNT - 1u;
				}
			} while(--count);
			zero ^= 1u;
		}
	}
}

/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG6DecodeGolombValues_c, (tjs_int8 *pixelbuf, tjs_int pixel_count, tjs_uint8 *bit_pool))
{
	/*
		decode values packed in "bit_pool".
		values are coded using golomb code.
	*/

	int n = TVP_TLG6_GOLOMB_N_COUNT - 1u; /* output counter */
	int a = 0u; /* summary of absolute values of errors */

	tjs_int bit_pos = 1u;
	tjs_uint8 zero = (*bit_pool & 1u)?0u:1u;

	tjs_int8 * limit = pixelbuf + pixel_count*4u;

	while(pixelbuf < limit)
	{
		/* get running count */
		int count;

		{
			tjs_uint32 t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
			tjs_int b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
			int bit_count = b;
			while(!b)
			{
				bit_count += TVP_TLG6_LeadingZeroTable_BITS;
				bit_pos += TVP_TLG6_LeadingZeroTable_BITS;
				bit_pool += bit_pos >> 3u;
				bit_pos &= 7u;
				t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
				b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
				bit_count += b;
			}


			bit_pos += b;
			bit_pool += bit_pos >> 3u;
			bit_pos &= 7u;

			bit_count --;
			count = 1u << bit_count;
			count += ((TVP_TLG6_FETCH_32BITS(bit_pool) >> (bit_pos)) & (count-1u));

			bit_pos += bit_count;
			bit_pool += bit_pos >> 3u;
			bit_pos &= 7u;

		}

		if(zero)
		{
			/* zero values */

			/* fill distination with zero */
			do { *pixelbuf = 0u; pixelbuf+=4u; } while(--count);

			zero ^= 1u;
		}
		else
		{
			/* non-zero values */

			/* fill distination with glomb code */

			do
			{
				int k = TVPTLG6GolombBitLengthTable[a][n], v, sign;

				tjs_uint32 t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
				tjs_int bit_count;
				tjs_int b;
				if(t)
				{
					b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
					bit_count = b;
					while(!b)
					{
						bit_count += TVP_TLG6_LeadingZeroTable_BITS;
						bit_pos += TVP_TLG6_LeadingZeroTable_BITS;
						bit_pool += bit_pos >> 3u;
						bit_pos &= 7u;
						t = TVP_TLG6_FETCH_32BITS(bit_pool) >> bit_pos;
						b = TVPTLG6LeadingZeroTable[t&(TVP_TLG6_LeadingZeroTable_SIZE-1u)];
						bit_count += b;
					}
					bit_count --;
				}
				else
				{
					bit_pool += 5u;
					bit_count = bit_pool[-1u];
					bit_pos = 0u;
					t = TVP_TLG6_FETCH_32BITS(bit_pool);
					b = 0u;
				}


				v = (bit_count << k) + ((t >> b) & ((1u<<k)-1u));
				sign = (v & 1u) - 1u;
				v >>= 1u;
				a += v;
				*pixelbuf = (char) ((v ^ sign) + sign + 1u);
				pixelbuf += 4u;

				bit_pos += b;
				bit_pos += k;
				bit_pool += bit_pos >> 3u;
				bit_pos &= 7u;

				if (--n < 0u) {
					a >>= 1u;  n = TVP_TLG6_GOLOMB_N_COUNT - 1u;
				}
			} while(--count);
			zero ^= 1u;
		}
	}
}


/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, make_gt_mask, (tjs_uint32 a, tjs_uint32 b))
{
	tjs_uint32 tmp2 = ~b;
	tjs_uint32 tmp = ((a & tmp2) + (((a ^ tmp2) >> 1u) & 0x7f7f7f7fu) ) & 0x80808080u;
	tmp = ((tmp >> 7u) + 0x7f7f7f7fu) ^ 0x7f7f7f7fu;
	return tmp;
}
/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, packed_bytes_add, (tjs_uint32 a, tjs_uint32 b))
{
	tjs_uint32 tmp = (((a & b)<<1u) + ((a ^ b) & 0xfefefefeu) ) & 0x01010100u;
	return a+b-tmp;
}
/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, med2, (tjs_uint32 a, tjs_uint32 b, tjs_uint32 c))
{
	/* do Median Edge Detector   thx, Mr. sugi  at    kirikiri.info */
	tjs_uint32 aa_gt_bb = TVP_GL_FUNCNAME(make_gt_mask)(a, b);
	tjs_uint32 a_xor_b_and_aa_gt_bb = ((a ^ b) & aa_gt_bb);
	tjs_uint32 aa = a_xor_b_and_aa_gt_bb ^ a;
	tjs_uint32 bb = a_xor_b_and_aa_gt_bb ^ b;
	tjs_uint32 n = TVP_GL_FUNCNAME(make_gt_mask)(c, bb);
	tjs_uint32 nn = TVP_GL_FUNCNAME(make_gt_mask)(aa, c);
	tjs_uint32 m = ~(n | nn);
	return (n & aa) | (nn & bb) | ((bb & m) - (c & m) + (aa & m));
}
/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, med, (tjs_uint32 a, tjs_uint32 b, tjs_uint32 c, tjs_uint32 v))
{
	return TVP_GL_FUNCNAME(packed_bytes_add)(TVP_GL_FUNCNAME(med2)(a, b, c), v);
}

#define TLG6_AVG_PACKED(x, y) ((((x) & (y)) + ((((x) ^ (y)) & 0xfefefefeu) >> 1u)) +\\
			(((x)^(y))&0x01010101u))

/*not export*/
TVP_GL_FUNC_INLINE_DECL(tjs_uint32, avg, (tjs_uint32 a, tjs_uint32 b, tjs_uint32 c, tjs_uint32 v))
{
	return TVP_GL_FUNCNAME(packed_bytes_add)(TLG6_AVG_PACKED(a, b), v);
}

#define TVP_TLG6_DO_CHROMA_DECODE_PROTO(B, G, R, A, POST_INCREMENT) do \\
			{ \\
				tjs_uint32 u = *prevline; \\
				p = TVP_GL_FUNCNAME(med)(p, u, up, \\
					(0xff0000u & ((R)<<16u)) + (0xff00u & ((G)<<8u)) + (0xffu & (B)) + ((A) << 24u) ); \\
				up = u; \\
				*curline = p; \\
				curline ++; \\
				prevline ++; \\
				POST_INCREMENT \\
			} while(--w);
#define TVP_TLG6_DO_CHROMA_DECODE_PROTO2(B, G, R, A, POST_INCREMENT) do \\
			{ \\
				tjs_uint32 u = *prevline; \\
				p = TVP_GL_FUNCNAME(avg)(p, u, up, \\
					(0xff0000u & ((R)<<16u)) + (0xff00u & ((G)<<8u)) + (0xffu & (B)) + ((A) << 24u) ); \\
				up = u; \\
				*curline = p; \\
				curline ++; \\
				prevline ++; \\
				POST_INCREMENT \\
			} while(--w);
#define TVP_TLG6_DO_CHROMA_DECODE(N, R, G, B) case (N<<1u): \\
	TVP_TLG6_DO_CHROMA_DECODE_PROTO(R, G, B, IA, {in+=step;}) break; \\
	case (N<<1u)+1u: \\
	TVP_TLG6_DO_CHROMA_DECODE_PROTO2(R, G, B, IA, {in+=step;}) break;

/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG6DecodeLineGeneric_c, (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int start_block, tjs_int block_limit, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *in, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir))
{
	/*
		chroma/luminosity decoding
		(this does reordering, color correlation filter, MED/AVG  at a time)
	*/
	tjs_uint32 p, up;
	int step, i;

	if(start_block)
	{
		prevline += start_block * TVP_TLG6_W_BLOCK_SIZE;
		curline  += start_block * TVP_TLG6_W_BLOCK_SIZE;
		p  = curline[-1u];
		up = prevline[-1u];
	}
	else
	{
		p = up = initialp;
	}

	in += skipblockbytes * start_block;
	step = (dir&1u)?1u:-1u;

	for(i = start_block; i < block_limit; i ++)
	{
		int w = width - i*TVP_TLG6_W_BLOCK_SIZE, ww;
		if(w > TVP_TLG6_W_BLOCK_SIZE) w = TVP_TLG6_W_BLOCK_SIZE;
		ww = w;
		if(step==-1u) in += ww-1u;
		if(i&1u) in += oddskip * ww;
		switch(filtertypes[i])
		{
#define IA	(char)((*in>>24u)&0xffu)
#define IR	(char)((*in>>16u)&0xffu)
#define IG  (char)((*in>>8u )&0xffu)
#define IB  (char)((*in     )&0xffu)
		TVP_TLG6_DO_CHROMA_DECODE( 0u, IB, IG, IR); 
		TVP_TLG6_DO_CHROMA_DECODE( 1u, IB+IG, IG, IR+IG); 
		TVP_TLG6_DO_CHROMA_DECODE( 2u, IB, IG+IB, IR+IB+IG); 
		TVP_TLG6_DO_CHROMA_DECODE( 3u, IB+IR+IG, IG+IR, IR); 
		TVP_TLG6_DO_CHROMA_DECODE( 4u, IB+IR, IG+IB+IR, IR+IB+IR+IG); 
		TVP_TLG6_DO_CHROMA_DECODE( 5u, IB+IR, IG+IB+IR, IR); 
		TVP_TLG6_DO_CHROMA_DECODE( 6u, IB+IG, IG, IR); 
		TVP_TLG6_DO_CHROMA_DECODE( 7u, IB, IG+IB, IR); 
		TVP_TLG6_DO_CHROMA_DECODE( 8u, IB, IG, IR+IG); 
		TVP_TLG6_DO_CHROMA_DECODE( 9u, IB+IG+IR+IB, IG+IR+IB, IR+IB); 
		TVP_TLG6_DO_CHROMA_DECODE(10u, IB+IR, IG+IR, IR); 
		TVP_TLG6_DO_CHROMA_DECODE(11u, IB, IG+IB, IR+IB); 
		TVP_TLG6_DO_CHROMA_DECODE(12u, IB, IG+IR+IB, IR+IB); 
		TVP_TLG6_DO_CHROMA_DECODE(13u, IB+IG, IG+IR+IB+IG, IR+IB+IG); 
		TVP_TLG6_DO_CHROMA_DECODE(14u, IB+IG+IR, IG+IR, IR+IB+IG+IR); 
		TVP_TLG6_DO_CHROMA_DECODE(15u, IB, IG+(IB<<1u), IR+(IB<<1u));

		default: return;
		}
		if(step == 1u)
			in += skipblockbytes - ww;
		else
			in += skipblockbytes + 1u;
		if(i&1u) in -= oddskip * ww;
#undef IR
#undef IG
#undef IB
	}
}

/*export*/
TVP_GL_FUNC_DECL(void, TVPTLG6DecodeLine_c, (tjs_uint32 *prevline, tjs_uint32 *curline, tjs_int width, tjs_int block_count, tjs_uint8 *filtertypes, tjs_int skipblockbytes, tjs_uint32 *in, tjs_uint32 initialp, tjs_int oddskip, tjs_int dir))
{
	TVPTLG6DecodeLineGeneric(prevline, curline, width, 0u, block_count,
		filtertypes, skipblockbytes, in, initialp, oddskip, dir);
}


EOF

;#-----------------------------------------------------------------
;# process tvpps.c
;#-----------------------------------------------------------------

open PSC, "tvpps.c" or die;

$exportlist = '';
while($line = <PSC>)
{
	if($line =~ /^\#define\s+TVPPS_FUNC_NORM\s+(\w+)/)
	{
		;# normal function
		$exportlist .= "/*export*/\n";
		$exportlist .= "TVP_GL_FUNC_DECL(void, $1, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))\n{}\n";
	}
	elsif($line =~ /^\#define\s+TVPPS_FUNC_O\s+(\w+)/)
	{
		;# with opacity 
		$exportlist .= "/*export*/\n";
		$exportlist .= "TVP_GL_FUNC_DECL(void, $1, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))\n{}\n";
	}
	elsif($line =~ /^\#define\s+TVPPS_FUNC_HDA\s+(\w+)/)
	{
		;# with hda
		$exportlist .= "/*export*/\n";
		$exportlist .= "TVP_GL_FUNC_DECL(void, $1, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len))\n{}\n";
	}
	elsif($line =~ /^\#define\s+TVPPS_FUNC_HDA_O\s+(\w+)/)
	{
		;# with hda opacity
		$exportlist .= "/*export*/\n";
		$exportlist .= "TVP_GL_FUNC_DECL(void, $1, (tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa))\n{}\n";
	}

	print FC $line;
}

print FC "\n\n";
print FC "#if 0\n";
print FC "/* dummy definitions to be checked in gengl.pl */\n";
print FC "$exportlist\n";
print FC "#endif\n";
print FC "\n\n";

;#-----------------------------------------------------------------



;#-----------------------------------------------------------------
;# write the footer
;#-----------------------------------------------------------------

;#-----------------------------------------------------------------
close FC;

&get_function_list("tvpgl.c");

open FC, ">>tvpgl.c";

foreach $each (@function_list)
{
	$temp = $each;
	$temp =~ s/TVP_GL_FUNC_DECL\((.*?)\s*\,\s*(.*?)_c\s*\,/TVP_GL_FUNC_PTR_DECL\($1\, $2\, /;
	print FC "$temp;\n";
}


print FC <<EOF;

/* suffix "_c" : function is written in C */

/*export*/
TVP_GL_FUNC_DECL(void, TVPInitTVPGL, ())
{
EOF

foreach $each (@function_list)
{
	$temp = $each;
	$temp =~ /TVP_GL_FUNC_DECL\((.*?)\s*\,\s*(.*?)_c\s*\,/;
	print FC "\t${2} = TVP_GL_FUNCNAME(${2}_c);\n";
}

print FC <<EOF;

	TVP_GL_FUNCNAME(TVPCreateTable)();
}

/*export*/
TVP_GL_FUNC_DECL(void, TVPUninitTVPGL, ())
{
	TVP_GL_FUNCNAME(TVPDestroyTable)();
}
/*end of the file*/

EOF
close FC;
;#-----------------------------------------------------------------
print FH "/* begin function list */\n";

foreach $each (@function_list)
{
	$temp = $each;
	$temp =~ s/TVP_GL_FUNC_DECL\((.*?)\s*\,\s*(.*?)_c\s*\,/TVP_GL_FUNC_PTR_EXTERN_DECL\($1\, $2\, /;
	print FH "$temp;\n";
}
print FH "/* end function list */\n";

print FH <<EOF;

TVP_GL_FUNC_EXTERN_DECL(void, TVPInitTVPGL, ());
TVP_GL_FUNC_EXTERN_DECL(void, TVPUninitTVPGL, ());
/*[*/
#ifdef __cplusplus
 }
#endif
/*]*/
/* some utilities */
/*[*/
#define TVP_RGB2COLOR(r,g,b) ((((r)<<16u) + ((g)<<8u) + (b)) | 0xff000000u)
#define TVP_RGBA2COLOR(r,g,b,a) \\
	(((a)<<24u) +  (((r)<<16u) + ((g)<<8u) + (b)))
/*]*/

#endif
/* end of the file */
EOF
;#-----------------------------------------------------------------
<<COMMENT;
open FE, ">tvpgl_exporter.cpp";

print FE "/*\n$gpl\n*/\n";

print FE <<EOF;

/* TVP GL function exporter for plug-ins */
/* this file is always generated by gengl.pl rev. $rev */

#include "tjsCommHead.h"

#include "tvpgl.h"
#include "PluginImpl.h"

void TVPGLExportFunctions()
{
EOF

$count = 0;
foreach $each (@function_list)
{
	$temp = $each;
	$temp =~ /TVP_GL_FUNC_DECL\((.*?)\s*\,\s*(.*?)_c\s*\,/;
	print FE "\tTVPAddExportFunction(TJS_W(\"$2\"),  (void*)$2);\n";
	$count ++;
}


print FE <<EOF;
}
EOF

close FE;

open FE, ">tvpgl_exporter.h";
print FE "/*\n$gpl\n*/\n";

print FE <<EOF;

/* TVP GL function exporter for plug-ins */
/* this file is always generated by gengl.pl rev. $rev */


extern void TVPGLExportFunctions();

EOF
close FE;
COMMENT
;#-----------------------------------------------------------------
