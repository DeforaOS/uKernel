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
[ -n "$ARCH" ] || ARCH="$(uname -m)"
case "$ARCH" in
	x86_64)
		ARCH="amd64"
		;;
esac
[ -n "$OBJDIR" ] || OBJDIR="./"
PROGNAME="grub.sh"
UKERNELBIN="${OBJDIR}../src/kernel/uKernel.bin"
#executables
DATE="date"
GRUBFILE="grub-file"
UNAME="uname"


#functions
#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}

_grub()
{
	ret=0

	if [ -z "$ARCH" ]; then
		_error "Could not determine the platform"
		return $?
	fi
	$DATE
	echo
	case "$ARCH" in
		amd64|i?86)
			_info "Testing multiboot conformance ($ARCH)"
			$GRUBFILE --is-x86-multiboot "$OBJDIR$UKERNELBIN"
			#FIXME look for multiboot2 instead?
			#$GRUBFILE --is-x86-multiboot2 "$OBJDIR$UKERNELBIN"
			ret=$?
			if [ $ret -eq 127 ]; then
				_error "Cannot test: $GRUBFILE not available (ignored)"
				ret=0
			elif [ $ret -ne 0 ]; then
				_error "$OBJDIR$UKERNELBIN: Not compliant with multiboot"
				ret=$?
			fi
			;;
		*)
			_info "$ARCH: Unsupported platform (ignored)"
			;;
	esac
	if [ $ret -eq 0 ]; then
		echo "OK"
	else
		echo "FAIL"
	fi
	return $ret
}


#info
_info()
{
	echo "$PROGNAME: $@"
}


#usage
_usage()
{
	echo "Usage: $PROGNAME [-c] target..." 1>&2
	return 1
}


#main
clean=0
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
if [ $# -lt 1 ]; then
	_usage
	exit $?
fi

#clean
[ $clean -ne 0 ] && exit 0

exec 3>&1
while [ $# -gt 0 ]; do
	target="$1"
	shift

	_grub > "$target"					|| exit 2
done
