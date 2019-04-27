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
BINUTILS_VERSION="2.32"
GCC_VERSION="8.3.0"
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
[ -f "binutils-$BINUTILS_VERSION.tar.gz" ] ||
	$WGET "$MIRROR/pub/binutils/releases/binutils-$BINUTILS_VERSION.tar.gz"

#Extract, configure, and build binutils in a dedicated tree
$TAR xzvf "binutils-$BINUTILS_VERSION.tar.gz"
$MKDIR binutils-build
(cd binutils-build && "../binutils-$BINUTILS_VERSION/configure" \
	--target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls \
	--disable-werror)
(cd binutils-build && $MAKE)
(cd binutils-build && $MAKE install)

#Download GCC
[ -f "gcc-$GCC_VERSION.tar.gz" ] ||
	$WGET "$MIRROR/pub/gcc/releases/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz"

#Extract, configure, and build GCC in a dedicated tree
$TAR xzvf "gcc-$GCC_VERSION.tar.gz"
$MKDIR gcc-build
(cd gcc-build && "../gcc-$GCC_VERSION/configure" --target="$TARGET" \
	--prefix="$PREFIX" --disable-nls --enable-languages=c,c++ \
	--without-headers)
(cd gcc-build && $MAKE all-gcc)
(cd gcc-build && $MAKE all-target-libgcc)
(cd gcc-build && $MAKE install-gcc)
(cd gcc-build && $MAKE install-target-libgcc)
