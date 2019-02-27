#!/bin/sh
#$Id$
#Copyright (c) 2015-2018 Pierre Pronchery <khorben@defora.org>
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
CONFIGSH="${0%/platform.sh}/../config.sh"
DESTDIR=
LDSOCONF="/etc/ld.so.conf"
PREFIX="/usr/local"
PROGNAME="platform.sh"
SOEXT=".so"
#executables
UNAME="uname"
[ -f "$CONFIGSH" ] && . "$CONFIGSH"


#functions
#platform_library
_platform_library()
{
	library="$1"
	libdir=$(_platform_variable "LIBDIR")
	path="/lib:/usr/lib:$libdir"

	if [ -f "$DESTDIR$LDSOCONF" ]; then
		paths=$(_library_ldsoconf "$DESTDIR$LDSOCONF")
		#XXX breaks on whitespace
		for p in $paths; do
			path="$path:$p"
		done
	fi
	(IFS=:; for p in $path; do
		if [ -f "$DESTDIR$p/lib$library$SOEXT" ]; then
			echo "-l$library"
			return
		fi
	done)
}

_library_ldsoconf()
{
	ldsoconf="$1"

	while read line; do
		case "$line" in
			"#"*)
				;;
			"include "*)
				#remove trailing comments
				line="${line%#*}"

				#recurse into the file included
				#XXX does not support globbing
				filename="${ldsoconf%/*}/${line#include }"
				[ -f "$filename" ] &&
					_library_ldsoconf "$filename"
				;;
			*)
				#remove trailing comments
				line="${line%#*}"
				echo "$line"
				;;
		esac
	done < "$ldsoconf"
}


#platform_variable
_platform_variable()
{
	variable="$1"
	context="$2"

	case "$variable" in
		BINDIR)
			echo "$PREFIX/bin"
			;;
		DATADIR)
			echo "$PREFIX/share"
			;;
		LIBUKERNEL_*|NATIVE_*|UKERNEL_*|ULOADER_*)
			[ -n "$context" ] || context=`$UNAME -m`
			case "$context" in
				amd64)
					"_platform_variable_$context" "$variable"
					;;
				i386|i486|i586|i686)
					"_platform_variable_i386" "$variable"
					;;
			esac
			;;
		LIBDIR)
			echo "$PREFIX/lib"
			;;
		MANDIR)
			if [ "$PREFIX" = "/usr/pkg" ]; then
				echo "$PREFIX/man"
			else
				echo "$PREFIX/share/man"
			fi
			;;
		PREFIX)
			echo "$PREFIX"
			;;
		SBINDIR)
			echo "$PREFIX/sbin"
			;;
		SYSCONFDIR)
			if [ "$PREFIX" = "/usr" ]; then
				echo "/etc"
			else
				echo "$PREFIX/etc"
			fi
			;;
	esac
}

_platform_variable_amd64()
{
	variable="$1"
	extra=
	platform=$($UNAME -s)

	case "$variable" in
		LIBUKERNEL_CFLAGS|UKERNEL_CFLAGS)
			if [ "$platform" = "OpenBSD" ]; then
				extra=" -fno-stack-protector"
			else
				extra=" -fstack-protector"
			fi
			echo "-ffreestanding -fPIC -mno-red-zone$extra"
			;;
		NATIVE_CFLAGS)
			if [ "$platform" = "OpenBSD" ]; then
				extra=" -fno-stack-protector"
			else
				extra=" -fstack-protector"
			fi
			echo "-ffreestanding -fPIC$extra"
			;;
		ULOADER_CFLAGS)
			if [ "$platform" = "OpenBSD" ]; then
				extra=" -fno-stack-protector"
			else
				extra=" -fstack-protector"
			fi
			echo "-m32 -ffreestanding -fPIC$extra"
			;;
		LIBUKERNEL_LDFLAGS|NATIVE_LDFLAGS)
			echo "-nostdlib -static"
			;;
		UKERNEL_LDFLAGS)
			echo "-nostdlib -shared -static -T ${prepend}src/arch/amd64/uKernel.ld"
			;;
		ULOADER_LDFLAGS)
			echo "-m32 -nostdlib -static -T ${prepend}src/arch/i386/uKernel.ld"
			;;
	esac
}

_platform_variable_i386()
{
	variable="$1"
	extra=
	platform=$($UNAME -s)

	case "$variable" in
		LIBUKERNEL_CFLAGS|NATIVE_CFLAGS|UKERNEL_CFLAGS|ULOADER_CFLAGS)
			if [ "$platform" = "OpenBSD" ]; then
				extra=" -fno-stack-protector"
			else
				extra=" -fstack-protector"
			fi
			echo "-ffreestanding -fPIC$extra"
			;;
		LIBUKERNEL_LDFLAGS|NATIVE_LDFLAGS)
			echo "-nostdlib -static"
			;;
		UKERNEL_LDFLAGS)
			echo "-nostdlib -pie -static -T ${prepend}src/arch/i386/uKernel.ld"
			;;
		ULOADER_LDFLAGS)
			echo "-nostdlib -static -T ${prepend}src/arch/i386/uKernel.ld"
			;;
	esac
}


#usage
_usage()
{
	echo "Usage: $PROGNAME -l library" 1>&2
	echo "       $PROGNAME -V variable [-C context]" 1>&2
	return 1
}


#main
prepend="${0%tools/platform.sh}"
context=
type=
variable=
while getopts "C:l:O:V:" name; do
	case "$name" in
		C)
			context="$OPTARG"
			;;
		O)
			export "${OPTARG%%=*}"="${OPTARG#*=}"
			;;
		V)
			type="variable"
			variable="$OPTARG"
			;;
		l)
			type="library"
			library="$OPTARG"
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -ne 0 ]; then
	_usage
	exit $?
fi

case "$type" in
	library)
		"_platform_$type" "$library"
		;;
	variable)
		"_platform_$type" "$variable" "$context"
		;;
	*)
		_usage
		exit $?
		;;
esac
