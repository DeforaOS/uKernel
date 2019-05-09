#!/bin/sh -e
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
BINUTILS_FLAGS=
BINUTILS_VERSION="2.32"
GCC_FLAGS=
GCC_TARGETS="all-gcc all-target-libgcc install-gcc install-target-libgcc"
GCC_VERSION="8.3.0"
GZEXT="gz"
MIRROR="https://ftpmirror.gnu.org"
TARGET="i686-elf"
#executables
MAKE="make"
MKDIR="mkdir -p"
TAR="tar"
WGET="wget"


#functions
#main

#Modify the environment to reflect the port chosen
PREFIX="$HOME/opt/cross-gcc-$TARGET"
PATH="$PREFIX/bin:$PATH"

#Download binutils
[ -f "binutils-$BINUTILS_VERSION.tar.$GZEXT" ] ||
	$WGET "$MIRROR/binutils/binutils-$BINUTILS_VERSION.tar.$GZEXT"

#Extract, configure, and build binutils in a dedicated tree
[ -d "binutils-$BINUTILS_VERSION" ] ||
	$TAR xzvf "binutils-$BINUTILS_VERSION.tar.$GZEXT"
case "$TARGET" in
	aarch64-elf|amd64-elf|sparc64-elf)
		BINUTILS_FLAGS="$BINUTILS_FLAGS --enable-multilib"
		;;
esac
$MKDIR "binutils-$TARGET"
(cd "binutils-$TARGET" && "../binutils-$BINUTILS_VERSION/configure" \
	--target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls \
	--disable-werror $BINUTILS_FLAGS)
(cd "binutils-$TARGET" && $MAKE)
(cd "binutils-$TARGET" && $MAKE install)

#Download GCC
[ -f "gcc-$GCC_VERSION.tar.$GZEXT" ] ||
	$WGET "$MIRROR/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.$GZEXT"

#Extract, configure, and build GCC in a dedicated tree
[ -d "gcc-$GCC_VERSION" ] ||
	$TAR xzvf "gcc-$GCC_VERSION.tar.$GZEXT"
case "$TARGET" in
	aarch64-elf)
		GCC_FLAGS="$GCC_FLAGS --with-multilib-list=lp64,ilp32"
		;;
	amd64-elf|sparc64-elf)
		GCC_FLAGS="$GCC_FLAGS --with-multilib-list=m64,m32"
		;;
esac
$MKDIR "gcc-$TARGET"
(cd "gcc-$TARGET" && "../gcc-$GCC_VERSION/configure" --target="$TARGET" \
	--prefix="$PREFIX" --disable-nls --enable-languages=c,c++ \
	--without-headers $GCC_FLAGS)
for target in $GCC_TARGETS; do
	(cd "gcc-$TARGET" && $MAKE "$target")
done
