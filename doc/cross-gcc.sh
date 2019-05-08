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
BINUTILS_TARGETS="all install"
BINUTILS_VERSION="2.32"
GCC_FLAGS=
GCC_TARGETS="all-gcc all-target-libgcc install-gcc install-target-libgcc"
GCC_VERSION="8.3.0"
GZEXT="gz"
MIRROR="https://ftpmirror.gnu.org"
TARGET="i686-deforaos-elf"
#executables
ACLOCAL="aclocal"
AUTOMAKE="automake"
CAT="cat"
MAKE="make"
MKDIR="mkdir -p"
PATCH="patch"
TAR="tar"
WGET="wget"


#functions
#binutils
_binutils()
{
	#Download binutils
	[ -f "binutils-$BINUTILS_VERSION.tar.$GZEXT" ] ||
		$WGET "$MIRROR/binutils/binutils-$BINUTILS_VERSION.tar.$GZEXT"

	#Extract binutils
	[ -d "binutils-$BINUTILS_VERSION" ] ||
		$TAR xzvf "binutils-$BINUTILS_VERSION.tar.$GZEXT"

	#Extend binutils for DeforaOS
	$CAT > "binutils-$BINUTILS_VERSION/ld/emulparams/elf_i386_deforaos.sh" << EOF
. \${srcdir}/emulparams/elf_i386.sh
GENERATE_SHLIB_SCRIPT=yes
GENERATE_PIE_SCRIPT=yes
EOF
	$CAT > "binutils-$BINUTILS_VERSION/ld/emulparams/elf_x86_64_deforaos.sh" << EOF
. \${srcdir}/emulparams/elf_x86_64_deforaos.sh
EOF
	(cd "binutils-$BINUTILS_VERSION" && $PATCH -p1) << EOF
--- binutils-2.32/ld/Makefile.am.orig	2019-05-08 01:05:33.000000000 +0200
+++ binutils-2.32/ld/Makefile.am	2019-05-08 01:08:58.000000000 +0200
@@ -285,6 +285,7 @@
 	eelf_i386.c \\
 	eelf_i386_be.c \\
 	eelf_i386_chaos.c \\
+	eelf_i386_deforaos.c \\
 	eelf_i386_fbsd.c \\
 	eelf_i386_ldso.c \\
 	eelf_i386_nacl.c \\
@@ -455,6 +456,7 @@
 	eelf_k1om_fbsd.c \\
 	eelf_x86_64.c \\
 	eelf_x86_64_cloudabi.c \\
+	eelf_x86_64_deforaos.c \\
 	eelf_x86_64_fbsd.c \\
 	eelf_x86_64_nacl.c \\
 	eelf_x86_64_sol2.c \\
@@ -1295,6 +1297,10 @@
 eelf_i386_chaos.c: \$(srcdir)/emulparams/elf_i386_chaos.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf_chaos.sc \${GEN_DEPENDS}
 
+eelf_i386_deforaos.c: \$(srcdir)/emulparams/elf_i386_deforaos.sh \\
+  \$(ELF_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
+	\${GENSCRIPTS} elf_i386_deforaos "\$(tdir_elf_i386_deforaos)"
+
 eelf_i386_fbsd.c: \$(srcdir)/emulparams/elf_i386_fbsd.sh \\
   \$(srcdir)/emulparams/elf_i386.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
@@ -1853,6 +1859,10 @@
   \$(srcdir)/emulparams/elf_x86_64.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
 
+eelf_x86_64_deforaos.c: \$(srcdir)/emulparams/elf_x86_64_deforaos.sh \\
+  \$(ELF_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
+	\${GENSCRIPTS} elf_x86_64_deforaos "\$(tdir_elf_x86_64_deforaos)"
+
 eelf_x86_64_fbsd.c: \$(srcdir)/emulparams/elf_x86_64_fbsd.sh \\
   \$(srcdir)/emulparams/elf_x86_64.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
--- binutils-2.32/gas/configure.tgt.orig	2019-05-08 00:02:09.000000000 +0200
+++ binutils-2.32/gas/configure.tgt	2019-05-08 00:02:12.000000000 +0200
@@ -260,6 +260,7 @@
   i386-*-chaos)				fmt=elf ;;
   i386-*-rdos*)				fmt=elf ;;
   i386-*-darwin*)			fmt=macho ;;
+  i386-*-deforaos*)			fmt=elf ;;
 
   ia16-*-elf*)				fmt=elf ;;
 
--- binutils-2.32/bfd/config.bfd.orig	2019-05-08 00:02:27.000000000 +0200
+++ binutils-2.32/bfd/config.bfd	2019-05-08 00:26:59.000000000 +0200
@@ -652,6 +652,11 @@
     targ_selvecs=
     targ64_selvecs=x86_64_elf64_vec
     ;;
+  i[3-7]86-*-deforaos*)
+    targ_defvec=i386_elf32_vec
+    targ_selvecs=
+    targ64_selvecs=x86_64_elf64_vec
+    ;;
 #ifdef BFD64
   x86_64-*-cloudabi*)
     targ_defvec=x86_64_elf64_cloudabi_vec
@@ -718,6 +723,11 @@
     targ_selvecs=i386_elf32_vec
     want64=true
     ;;
+  x86_64-*-deforaos*)
+    targ_defvec=x86_64_elf64_vec
+    targ_selvecs=i386_elf32_vec
+    want64=true
+    ;;
 #endif
   i[3-7]86-*-lynxos*)
     targ_defvec=i386_elf32_vec
EOF
	(cd "binutils-$BINUTILS_VERSION/ld" && $ACLOCAL && $AUTOMAKE)

	#Configure and build binutils in a dedicated tree
	case "$TARGET" in
		aarch64-elf|aarch64-*-elf|amd64-elf|amd64-*-elf|sparc64-elf|sparc64-*-elf)
			BINUTILS_FLAGS="$BINUTILS_FLAGS --enable-multilib"
			;;
	esac
	$MKDIR "binutils-$TARGET"
	(cd "binutils-$TARGET" && "../binutils-$BINUTILS_VERSION/configure" \
		--target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls \
		--disable-werror $BINUTILS_FLAGS)
	for target in $BINUTILS_TARGETS; do
		(cd "binutils-$TARGET" && $MAKE "$target")
	done
}

_gcc()
{
	#Download GCC
	[ -f "gcc-$GCC_VERSION.tar.$GZEXT" ] ||
		$WGET "$MIRROR/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.$GZEXT"

	#Extract GCC
	[ -d "gcc-$GCC_VERSION" ] ||
		$TAR xzvf "gcc-$GCC_VERSION.tar.$GZEXT"

	#Extend GCC for DeforaOS
	$CAT > "gcc-$GCC_VERSION/gcc/config/deforaos.h" << EOF
/* Useful if you wish to make target-specific GCC changes. */
#undef TARGET_DEFORAOS
#define TARGET_DEFORAOS 1
 
#undef LIB_SPEC
#define LIB_SPEC "-lc" /* link against C standard library */
 
#undef LINK_SPEC
#define LINK_SPEC "%{shared:-shared} %{static:-static} %{!shared: %{!static: %{rdynamic:-export-dynamic}}}"

/* Files that are linked before user code.
   The %s tells GCC to look for these files in the library directory. */
#undef STARTFILE_SPEC
#define STARTFILE_SPEC "crt0.o%s crti.o%s crtbegin.o%s"
 
/* Files that are linked after user code. */
#undef ENDFILE_SPEC
#define ENDFILE_SPEC "crtend.o%s crtn.o%s"
 
/* Don't automatically add extern "C" { } around header files. */
#undef  NO_IMPLICIT_EXTERN_C
#define NO_IMPLICIT_EXTERN_C 1
 
/* Additional predefined macros. */
#undef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()       \\
  do {                                 \\
    builtin_define ("__DeforaOS__");   \\
    builtin_define ("__unix__");       \\
    builtin_assert ("system=deforaos");\\
    builtin_assert ("system=unix");    \\
    builtin_assert ("system=posix");   \\
  } while(0);
EOF
	$CAT > "gcc-$GCC_VERSION/gcc/config/i386/t-x86_64-elf" << EOF
MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone
EOF
	(cd "gcc-$GCC_VERSION" && $PATCH -p1) << EOF
--- gcc-8.3.0/libgcc/config.host.orig	2019-05-08 01:33:41.000000000 +0200
+++ gcc-8.3.0/libgcc/config.host	2019-05-08 01:37:37.000000000 +0200
@@ -603,6 +603,15 @@
 	tm_file="\$tm_file i386/darwin-lib.h"
 	extra_parts="\$extra_parts crtprec32.o crtprec64.o crtprec80.o crtfastmath.o"
 	;;
+i[34567]86-*-deforaos*)
+	extra_parts="crti.o crtbegin.o crtend.o crtn.o"
+	tmake_file="\$tmake_file i386/t-crtstuff t-crtstuff-pic t-libgcc-pic"
+	;;
+x86_64-*-deforaos*)
+	extra_parts="crti.o crtbegin.o crtend.o crtn.o"
+	tmake_file="\$tmake_file i386/t-crtstuff t-crtstuff-pic t-libgcc-pic"
+	tmake_file="\$tmake_file i386/t-x86_64-elf"
+	;;
 i[34567]86-*-elfiamcu)
 	tmake_file="\$tmake_file i386/t-crtstuff t-softfp-sfdftf i386/32/t-softfp i386/32/t-iamcu i386/t-softfp t-softfp t-dfprules"
 	;;
--- gcc-8.3.0/gcc/config.gcc.orig	2019-05-08 01:21:50.000000000 +0200
+++ gcc-8.3.0/gcc/config.gcc	2019-05-08 01:27:44.000000000 +0200
@@ -676,6 +676,12 @@
     "" | yes | posix) thread_file='posix' ;;
   esac
   ;;
+*-*-deforaos*)
+  gas=yes
+  gnu_ld=yes
+  default_use_cxa_atexit=yes
+  use_gcc_stdint=wrap
+  ;;
 *-*-dragonfly*)
   gas=yes
   gnu_ld=yes
@@ -1490,6 +1496,12 @@
 	tmake_file="\${tmake_file} \${cpu_type}/t-darwin64 t-slibgcc"
 	tm_file="\${tm_file} \${cpu_type}/darwin64.h"
 	;;
+i[34567]86-*-deforaos*)
+	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h glibc-stdint.h i386/i386elf.h deforaos.h"
+	;;
+x86_64-*-deforaos*)
+	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h glibc-stdint.h i386/i386elf.h i386/x86-64.h deforaos.h"
+	;;
 i[34567]86-*-elfiamcu)
 	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/iamcu.h"
 	;;
--- gcc-8.3.0/config.sub.orig	2019-05-08 01:20:42.000000000 +0200
+++ gcc-8.3.0/config.sub	2019-05-08 01:20:44.000000000 +0200
@@ -1395,7 +1395,7 @@
 	      | -hpux* | -unos* | -osf* | -luna* | -dgux* | -auroraux* | -solaris* \\
 	      | -sym* | -kopensolaris* | -plan9* \\
 	      | -amigaos* | -amigados* | -msdos* | -newsos* | -unicos* | -aof* \\
-	      | -aos* | -aros* | -cloudabi* | -sortix* \\
+	      | -aos* | -aros* | -cloudabi* | -deforaos* | -sortix* \\
 	      | -nindy* | -vxsim* | -vxworks* | -ebmon* | -hms* | -mvs* \\
 	      | -clix* | -riscos* | -uniplus* | -iris* | -rtu* | -xenix* \\
 	      | -hiux* | -386bsd* | -knetbsd* | -mirbsd* | -netbsd* \\
EOF

	#Configure and build GCC in a dedicated tree
	case "$TARGET" in
		aarch64-elf|aarch64-*-elf)
			GCC_FLAGS="$GCC_FLAGS --with-multilib-list=lp64,ilp32"
			;;
		amd64-elf|amd64-*-elf|sparc64-elf|sparc64-*-elf)
			GCC_FLAGS="$GCC_FLAGS --with-multilib-list=m64,m32"
			;;
	esac
	$MKDIR "gcc-$TARGET"
	(cd "gcc-$TARGET" && "../gcc-$GCC_VERSION/configure" --target="$TARGET" \
		--prefix="$PREFIX" --disable-nls --enable-languages=c,c++ \
		--without-headers --enable-shared $GCC_FLAGS)
	for target in $GCC_TARGETS; do
		(cd "gcc-$TARGET" && $MAKE "$target")
	done
}

#main
#Modify the environment to reflect the port chosen
PREFIX="$HOME/opt/cross-gcc-$TARGET"
PATH="$PREFIX/bin:$PATH"

_binutils &&
_gcc
