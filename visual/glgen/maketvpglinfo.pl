;# Create file containing information about tvpgl functions

$copyright = <<EOF;
/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2019-2019 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
// See details of license at "license.txt" //
//                                         //
/////////////////////////////////////////////
/* This file contains information about tvpgl functions. */
/* This file is generated from maketvpglinfo.pl . */
/* Modifications directly to this file will be lost. */
EOF

open OFH , ">tvpgl_info.h";

print OFH $copyright;

sub normalize_string
{
	local($str, $ret, @array, @array2);
	$str = $_[0];
	$str =~ s/^\s*(.*?)\s*$/$1/;
	$str =~ s/\*\*/\* \*/g;
	$str =~ s/\*\*/\* \*/g;
	@array = split(/\s|\b/, $str);
	foreach $str (@array)
	{
		if($str !~ /\s/ && $str ne '')
		{
			push(@array2, $str);
		}
	}
	$str = join(' ', @array2);
	return $str;
}

sub except_arg_names
{
	local($args, $arg, @array);
	$args = $_[0];
	@array = split(/,/, $args);
	$args = '';
	foreach $arg ( @array )
	{
		$arg =~ s/^\s*(.*?)\s*$/$1/;
		if($arg =~ /^(.*)=(.*)$/)
		{
			$arg = $1;
		}
		$arg =~ s/^\s*(.*?)\s*$/$1/;
		$arg =~ s/(.*?)(\w+)$/$1/;
		$arg =~ s/^\s*(.*?)\s*$/$1/;
		$args .= ',' if $args ne '';
		$args .= normalize_string($1);
	}
	return $args;
}

sub get_arg_names
{
	local($args, $arg, @array);
	$args = $_[0];
	@array = split(/,/, $args);
	$args = '';
	foreach $arg ( @array )
	{
		$arg =~ s/^\s*(.*?)\s*$/$1/;
		if($arg =~ /^(.*)=(.*)$/)
		{
			$arg = $1;
		}
		$arg =~ s/^\s*(.*?)\s*$/$1/;
		$arg =~ /(\w+)$/;
		$args .= ', ' if $args ne '';
		$args .= $1;
	}
	return $args;
}

sub make_exp_stub
{
	local($rettype, $name, $arg, $type, $prefix, $isconst, $str, $noreturn, $h, $argnames, $exceptargnames);
	$rettype = $_[0];
	$name = $_[1];
	$arg = $_[2];

	$rettype =~ s/\n/ /gs;
	$rettype =~ s/\t/ /gs;
	$name =~ s/\n/ /gs;
	$name =~ s/\t/ /gs;
	$arg =~ s/\n/ /gs;
	$arg =~ s/\t/ /gs;

	$func_exp_name = normalize_string($rettype).' '.
		'::'.normalize_string($name).'('.except_arg_names($arg).')';

	$argnames = get_arg_names($arg);
	$exceptargnames = except_arg_names($arg);

print OFH <<EOF;
	x($name, "$func_exp_name", $rettype, ($arg)) \\
EOF

}

sub process_exp_stub
{
	local($file);
	$file = $_[0];

	open FK, $file;
	$content = <FK>;

	while($content =~ /TVP_GL_FUNC_PTR_EXTERN_DECL\(\s*(.*?)\s*,\s*(.*?)\s*,\s*\((.*?)\)\s*\)/sg)
	{
		&make_exp_stub($1, $2, $3);
	}
}

undef($/);

print OFH <<EOF;
#include "tvpgl.h"
#ifndef TVPGL_EXPAND_MACRO
#define TVPGL_EXPAND_MACRO(x) \\
EOF

process_exp_stub("tvpgl.h");
print OFH <<EOF;

#endif
EOF

close OFH;
