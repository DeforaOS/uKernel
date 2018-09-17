#!/bin/sh
#$Id$
#Copyright (c) 2018 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS uKernel
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



#variables
[ -n "$ARCH" ] || ARCH=$(uname -m)
PLATFORMCONF="../arch/$ARCH/platform.conf"
[ -f "$PLATFORMCONF" ] || PLATFORMCONF="platform.conf"
PROGNAME="platform.sh"


#functions
#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}


#platform
_platform()
{
	ret=0
	line=0
	buses=
	devices=

	#output the header
	echo "/* \$Id\$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


"

	#output the includes
	while read name type at busname bus args; do
		line=$((line + 1))

		#convert comments and ignore empty lines
		case "$name" in
			"#"*)
				echo -n "/* "
				[ -n "${name#?}" ] && echo -n "${name#?}"
				[ -n "$type" ] && echo -n " $type"
				[ -n "$at" ] && echo -n " $at"
				[ -n "$busname" ] && echo -n " $busname"
				[ -n "$bus" ] && echo -n " $bus"
				[ -n "$args" ] && echo -n " $args"
				echo " */"
				continue
				;;
			"")
				continue
				;;
		esac

		#check for syntax errors
		if [ "$at" != "at" -o "$bus" != "bus" ] &&
				[ -n "$at" -o -n "$busname" -o -n "$bus" ]; then
			_error "Syntax error at line $line"
			ret=$?
		fi

		case "$type" in
			bus)
				buses="$buses
$name $type $busname $args"
				;;
			*)
				devices="$devices
$name $type $busname $args"
				;;
		esac
	done

	#output the includes
	echo "#include <stddef.h>"
	echo "$devices" | while read name type busname args; do
		[ -n "$name" ] || continue
		echo "#include <kernel/drivers/${type}.h>"
	done

	#begin the function
	echo "
	
/* private */
/* prototypes */
static void _platform_init_arch(void);


/* functions */
/* platform_init */
static void _platform_init_arch(void)
{"

	#list the buses
	echo "$buses" | while read name type busname args; do
		[ -n "$name" ] || continue
		echo "	ukBus * ${name}bus;"
	done

	#initialize the buses
	echo
	echo "$buses" | while read name type busname args; do
		[ -n "$name" ] || continue
		if [ -z "$busname" ]; then
			echo "	${name}${type} = ${type}_init(NULL, \"${name}\");"
		else
			echo "	${name}${type} = ${type}_init(${busname}bus, \"${name}\");"
		fi
	done

	#initialize the devices
	echo "$devices" | while read name type busname args; do
		[ -n "$name" ] || continue
		if [ -z "$busname" ]; then
			echo "	${type}_init(NULL, \"${name}\");"
		else
			echo "	${type}_init(${busname}bus, \"${name}\");"
		fi
	done

	#end the function
	echo "}"

	return $ret
}


#usage
_usage()
{
	echo "Usage: $PROGNAME target" 1>&2
	return 1
}


#main
while getopts "cO:P:" name; do
	case "$name" in
		c)
			clean=1
			;;
		O)
			export "${OPTARG%%=*}"="${OPTARG#*=}"
			;;
		P)
			#XXX ignored for compatibility
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -ne 1 ]; then
	_usage
	exit $?
fi
#XXX breaks OBJDIR
_platform < "$PLATFORMCONF" > "$1"
