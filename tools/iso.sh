#!/bin/sh
#$Id$
#Copyright (c) 2019 Pierre Pronchery <khorben@defora.org>
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
	i[3456]86)
		ARCH="i386"
		;;
	amd64|x86_64)
		ARCH="amd64"
		;;
	*)
		exit 0
		;;
esac
[ -n "$OBJDIR" ] || OBJDIR="./"
PROGNAME="iso.sh"
UKERNELBIN="${OBJDIR}kernel/uKernel.bin"
ULOADERBIN="${OBJDIR}loader/uLoader.bin"
#executables
CAT="cat"
DATE="date"
DEBUG="_debug"
GRUBMKRESCUE="grub-mkrescue"
INSTALL="install"
MKDIR="mkdir -p"
MKTEMP="mktemp"
RM="rm -f"
RMDIR="rmdir"
UNAME="uname"


#functions
#debug
_debug()
{
	echo "$@" 1>&2
	$@
}


#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}


#iso
_iso()
{
	target="$1"

	_info "$target: Creating ISO image ($ARCH)"
	tmpdir=$($DEBUG $MKTEMP -d)
	if [ $? -ne 0 ]; then
		_error "$target: Could not create temporary directory"
		return 2
	fi
	$DEBUG $MKDIR -- "$tmpdir/boot/grub"
	$DEBUG $INSTALL -m 0644 "$UKERNELBIN" "$tmpdir/boot/uKernel.bin"
	$DEBUG $INSTALL -m 0644 "$ULOADERBIN" "$tmpdir/boot/uLoader.bin"
	$CAT > "$tmpdir/boot/grub/grub.cfg" << EOF
menuentry "DeforaOS" {
	multiboot /boot/uKernel.bin
}
EOF
	$DEBUG $GRUBMKRESCUE -o "$target" "$tmpdir"
	ret=$?
	$DEBUG $RM -- "$tmpdir/boot/uKernel.bin" "$tmpdir/boot/uLoader.bin" \
		"$tmpdir/boot/grub/grub.cfg"
	$DEBUG $RMDIR -- "$tmpdir/boot/grub" "$tmpdir/boot" "$tmpdir"
	if [ $ret -eq 127 ]; then
		_error "$target: Could not create ISO image: $GRUBMKRESCUE not available (ignored)"
		ret=0
	elif [ $ret -ne 0 ]; then
		_error "$target: Could not create ISO image"
		ret=$?
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

	_iso "$target"						|| exit 2
done
