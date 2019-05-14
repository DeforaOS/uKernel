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
PROGNAME="cross-gcc.sh"
TARGET=
#executables
ACLOCAL="aclocal"
AUTOMAKE="automake"
CAT="cat"
MAKE="make"
MAKE_FLAGS=
MKDIR="mkdir -p"
PATCH="patch"
PORT="i386"
TAR="tar"
TAR_FLAGS="-xzf"
UNAME="uname"
WGET="wget"


#functions
#binutils
_binutils()
{
	version="$1"

	#Download binutils
	[ -f "binutils-$version.tar.$GZEXT" ] ||
		$WGET "$MIRROR/binutils/binutils-$version.tar.$GZEXT"

	#Extract binutils
	if [ ! -d "binutils-$version" ]; then
		$TAR $TAR_FLAGS "binutils-$version.tar.$GZEXT"

		#Patch binutils for DeforaOS
		(cd "binutils-$version" && $PATCH -p1) << EOF
--- binutils-2.32/ld/emulparams/elf_i386_deforaos.sh.orig	1970-01-01 01:00:00.000000000 +0100
+++ binutils-2.32/ld/emulparams/elf_i386_deforaos.sh	2019-05-09 17:30:46.000000000 +0200
@@ -0,0 +1,3 @@
+. \${srcdir}/emulparams/elf_i386.sh
+GENERATE_SHLIB_SCRIPT=yes
+GENERATE_PIE_SCRIPT=yes
--- binutils-2.32/ld/emulparams/elf_x86_64_deforaos.sh.orig	1970-01-01 01:00:00.000000000 +0100
+++ binutils-2.32/ld/emulparams/elf_x86_64_deforaos.sh	2019-05-09 17:30:46.000000000 +0200
@@ -0,0 +1 @@
+. \${srcdir}/emulparams/elf_x86_64_deforaos.sh
--- binutils-2.32/ld/Makefile.am.orig	2019-01-19 17:01:33.000000000 +0100
+++ binutils-2.32/ld/Makefile.am	2019-05-09 17:30:46.000000000 +0200
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
+  \$(srcdir)/emulparams/elf_i386.sh \\
+  \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
+
 eelf_i386_fbsd.c: \$(srcdir)/emulparams/elf_i386_fbsd.sh \\
   \$(srcdir)/emulparams/elf_i386.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
@@ -1853,6 +1859,10 @@
   \$(srcdir)/emulparams/elf_x86_64.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
 
+eelf_x86_64_deforaos.c: \$(srcdir)/emulparams/elf_x86_64_deforaos.sh \\
+  \$(srcdir)/emulparams/elf_x86_64.sh \\
+  \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
+
 eelf_x86_64_fbsd.c: \$(srcdir)/emulparams/elf_x86_64_fbsd.sh \\
   \$(srcdir)/emulparams/elf_x86_64.sh \\
   \$(ELF_X86_DEPS) \$(srcdir)/scripttempl/elf.sc \${GEN_DEPENDS}
--- binutils-2.32/ld/configure.tgt.orig	2019-05-11 04:48:32.000000000 +0200
+++ binutils-2.32/ld/configure.tgt	2019-05-11 05:58:31.000000000 +0200
@@ -240,10 +240,13 @@
 i[3-7]86-*-lynxos*)	targ_emul=i386lynx ;;
 i[3-7]86-*-aros*)	targ_emul=elf_i386
 			targ_extra_emuls=elf_iamcu ;;
+i[3-7]86-*-deforaos*)	targ_emul=elf_i386
+			targ_extra_emuls=elf_iamcu ;;
 i[3-7]86-*-rdos*)	targ_emul=elf_i386
 			targ_extra_emuls=elf_iamcu ;;
 x86_64-*-rdos*)		targ_emul=elf64rdos ;;
 x86_64-*-cloudabi*)	targ_emul=elf_x86_64_cloudabi ;;
+x86_64-*-deforaos*)	targ_emul=elf_x86_64 ;;
 i[3-7]86-*-bsd)		targ_emul=i386bsd ;;
 i[3-7]86-*-bsd386)	targ_emul=i386bsd ;;
 i[3-7]86-*-bsdi*)	targ_emul=i386bsd ;;
--- binutils-2.32/gas/configure.tgt.orig	2019-01-19 17:01:33.000000000 +0100
+++ binutils-2.32/gas/configure.tgt	2019-05-09 17:30:46.000000000 +0200
@@ -260,6 +260,7 @@
   i386-*-chaos)				fmt=elf ;;
   i386-*-rdos*)				fmt=elf ;;
   i386-*-darwin*)			fmt=macho ;;
+  i386-*-deforaos*)			fmt=elf ;;
 
   ia16-*-elf*)				fmt=elf ;;
 
--- binutils-2.32/bfd/config.bfd.orig	2019-01-19 17:01:32.000000000 +0100
+++ binutils-2.32/bfd/config.bfd	2019-05-11 05:49:12.000000000 +0200
@@ -66,6 +66,7 @@
 
 case \$targ in
  *-*-netbsdelf* | i[3-7]86-*-netbsd*-gnu* | i[3-7]86-*-knetbsd*-gnu | \\
+ *-*-deforaos* | \\
  mips*-*-irix5* | mips*-*-irix6*)
     # Not obsolete
     ;;
@@ -589,6 +590,11 @@
     targ_defvec=i386_elf32_vec
     targ_selvecs=iamcu_elf32_vec
     ;;
+  i[3-7]86-*-deforaos*)
+    targ_defvec=i386_elf32_vec
+    targ_selvecs="iamcu_elf32_vec"
+    targ64_selvecs="x86_64_elf64_vec l1om_elf64_vec k1om_elf64_vec"
+    ;;
   i[3-7]86-*-dicos*)
     targ_defvec=i386_elf32_vec
     targ_selvecs=iamcu_elf32_vec
@@ -663,6 +669,11 @@
     targ_archs="\$targ_archs bfd_powerpc_arch bfd_rs6000_arch"
     want64=true
     ;;
+  x86_64-*-deforaos*)
+    targ_defvec=x86_64_elf64_vec
+    targ_selvecs="i386_elf32_vec iamcu_elf32_vec x86_64_elf32_vec l1om_elf64_vec k1om_elf64_vec"
+    want64=true
+    ;;
   x86_64-*-dicos*)
     targ_defvec=x86_64_elf64_vec
     targ_selvecs="i386_elf32_vec iamcu_elf32_vec l1om_elf64_vec k1om_elf64_vec"
--- binutils-2.32/config.sub.orig	2019-05-11 04:38:21.000000000 +0200
+++ binutils-2.32/config.sub	2019-05-11 04:38:24.000000000 +0200
@@ -135,6 +135,7 @@
 			| linux-newlib* | linux-musl* | linux-uclibc* | uclinux-uclibc* \\
 			| uclinux-gnu* | kfreebsd*-gnu* | knetbsd*-gnu* | netbsd*-gnu* \\
 			| netbsd*-eabi* | kopensolaris*-gnu* | cloudabi*-eabi* \\
+			| deforaos*-elf* \\
 			| storm-chaos* | os2-emx* | rtmk-nova*)
 				basic_machine=\$field1
 				os=\$maybe_os
@@ -1341,7 +1342,7 @@
 	     | hpux* | unos* | osf* | luna* | dgux* | auroraux* | solaris* \\
 	     | sym* | kopensolaris* | plan9* \\
 	     | amigaos* | amigados* | msdos* | newsos* | unicos* | aof* \\
-	     | aos* | aros* | cloudabi* | sortix* \\
+	     | aos* | aros* | cloudabi* | deforaos* | sortix* \\
 	     | nindy* | vxsim* | vxworks* | ebmon* | hms* | mvs* \\
 	     | clix* | riscos* | uniplus* | iris* | isc* | rtu* | xenix* \\
 	     | knetbsd* | mirbsd* | netbsd* \\
EOF
		(cd "binutils-$version/ld" && $ACLOCAL && $AUTOMAKE)
	fi

	#Configure and build binutils in a dedicated tree
	case "$TARGET" in
		aarch64-elf|aarch64-*-elf|amd64-elf|amd64-*-elf|sparc64-elf|sparc64-*-elf)
			BINUTILS_FLAGS="$BINUTILS_FLAGS --enable-multilib"
			;;
	esac
	$MKDIR "binutils-$TARGET"
	if [ ! -f "binutils-$TARGET/Makefile" ]; then
		(cd "binutils-$TARGET" &&
			"../binutils-$version/configure" \
			--target="$TARGET" --prefix="$PREFIX" --with-sysroot \
			--disable-nls --disable-werror $BINUTILS_FLAGS)
	fi
	for target in $BINUTILS_TARGETS; do
		(cd "binutils-$TARGET" && $MAKE $MAKE_FLAGS "$target")
	done
}


#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}


#gcc
_gcc()
{
	version="$1"

	#Download GCC
	[ -f "gcc-$version.tar.$GZEXT" ] ||
		$WGET "$MIRROR/gcc/gcc-$version/gcc-$version.tar.$GZEXT"

	#Extract GCC
	if [ ! -d "gcc-$version" ]; then
		$TAR $TAR_FLAGS "gcc-$version.tar.$GZEXT"

		#Extend GCC for DeforaOS
		$CAT > "gcc-$version/gcc/config/deforaos.h" << EOF
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
		$CAT > "gcc-$version/gcc/config/i386/t-x86_64-elf" << EOF
MULTILIB_OPTIONS = m64 m32 mno-red-zone
MULTILIB_DIRNAMES = m64 m32 no-red-zone
MULTILIB_OSDIRNAMES = ../lib ../lib/i386 ../lib/no-red-zone
EOF
		$CAT > "gcc-$version/gcc/config/i386/deforaos.h" << EOF
EOF
		$CAT > "gcc-$version/gcc/config/i386/deforaos64.h" << EOF
#if TARGET_64BIT_DEFAULT
# define MULTILIB_DEFAULTS { "m64" }
#else
# define MULTILIB_DEFAULTS { "m32" }
#endif
EOF

		#Patch GCC for DeforaOS
		(cd "gcc-$version" && $PATCH -p1) << EOF
--- gcc-8.3.0/libgcc/config.host.orig	2018-04-06 22:04:17.000000000 +0200
+++ gcc-8.3.0/libgcc/config.host	2019-05-11 04:54:00.000000000 +0200
@@ -603,6 +603,17 @@
 	tm_file="\$tm_file i386/darwin-lib.h"
 	extra_parts="\$extra_parts crtprec32.o crtprec64.o crtprec80.o crtfastmath.o"
 	;;
+i[34567]86-*-deforaos*)
+	tmake_file="\$tmake_file i386/t-crtstuff t-crtstuff-pic t-libgcc-pic"
+	tmake_file="\$tmake_file t-slibgcc t-slibgcc-elf-ver"
+	tmake_file="\$tmake_file t-slibgcc-libgcc"
+	extra_parts="crtbegin.o crtbeginS.o crtend.o crtendS.o crti.o crtn.o"
+	;;
+x86_64-*-deforaos*)
+	extra_parts="crti.o crtbegin.o crtend.o crtn.o"
+	tmake_file="\$tmake_file i386/t-crtstuff t-crtstuff-pic t-libgcc-pic"
+	tmake_file="\$tmake_file i386/t-x86_64-elf"
+	;;
 i[34567]86-*-elfiamcu)
 	tmake_file="\$tmake_file i386/t-crtstuff t-softfp-sfdftf i386/32/t-softfp i386/32/t-iamcu i386/t-softfp t-softfp t-dfprules"
 	;;
--- gcc-8.3.0/gcc/config.gcc.orig	2019-01-29 16:31:10.000000000 +0100
+++ gcc-8.3.0/gcc/config.gcc	2019-05-11 04:54:00.000000000 +0200
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
@@ -1490,6 +1496,15 @@
 	tmake_file="\${tmake_file} \${cpu_type}/t-darwin64 t-slibgcc"
 	tm_file="\${tm_file} \${cpu_type}/darwin64.h"
 	;;
+i[34567]86-*-deforaos*)
+	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h"
+	tm_file="\${tm_file} i386/i386elf.h deforaos.h i386/deforaos.h"
+	;;
+x86_64-*-deforaos*)
+	tmake_file="\${tmake_file} i386/t-x86_64-elf"
+	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h"
+	tm_file="\${tm_file} i386/i386elf.h i386/x86-64.h deforaos.h i386/deforaos64.h"
+	;;
 i[34567]86-*-elfiamcu)
 	tm_file="\${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/iamcu.h"
 	;;
--- gcc-8.3.0/config.sub.orig	2018-01-03 05:25:18.000000000 +0100
+++ gcc-8.3.0/config.sub	2019-05-11 04:54:00.000000000 +0200
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
	fi

	#Configure and build GCC in a dedicated tree
	case "$TARGET" in
		aarch64-elf|aarch64-*-elf)
			GCC_FLAGS="$GCC_FLAGS --with-multilib-list=lp64,ilp32"
			;;
		amd64-elf|amd64-*-elf)
			GCC_FLAGS="$GCC_FLAGS --with-multilib-list=m64,m32,no-red-zone"
			;;
		sparc64-elf|sparc64-*-elf)
			GCC_FLAGS="$GCC_FLAGS --with-multilib-list=m64,m32"
			;;
	esac
	$MKDIR "gcc-$TARGET"
	if [ ! -f "gcc-$TARGET/Makefile" ]; then
		(cd "gcc-$TARGET" && "../gcc-$version/configure" \
			--target="$TARGET" --prefix="$PREFIX" \
			--disable-nls --enable-languages=c,c++ \
			--without-headers $GCC_FLAGS)
	fi
	for target in $GCC_TARGETS; do
		(cd "gcc-$TARGET" && $MAKE $MAKE_FLAGS "$target")
	done
}


#platform
_platform()
{
	case $($UNAME -s) in
		NetBSD)
			GCC_FLAGS="$GCCFLAGS --with-gmp=/usr/pkg --with-mpfr=/usr/pkg"
			if [ -n "$LD_LIBRARY_PATH" ]; then
				LD_LIBRARY_PATH="/usr/pkg/lib:$LD_LIBRARY_PATH"
			else
				LD_LIBRARY_PATH="/usr/pkg/lib"
			fi
			export LD_LIBRARY_PATH
			MAKE="gmake"
			;;
	esac
}


#usage
_usage()
{
	echo "Usage: $PROGNAME [-p port]" 1>&2
	return 1
}


#main
#Modify the environment to reflect the port chosen
PREFIX="$HOME/opt/cross-gcc-$TARGET"
PATH="$PREFIX/bin:$PATH"
while getopts "O:p:" name; do
	case "$name" in
		O)
			export "${OPTARG%%=*}"="${OPTARG#*=}"
			;;
		p)
			PORT="$OPTARG"
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

case "$PORT" in
	i386)
		TARGET="i686-none-deforaos"
		;;
	amd64)
		TARGET="amd64-none-deforaos"
		;;
	*)
		_error "$PORT: Port not supported"
		exit $?
		;;
esac

_platform &&
_binutils "$BINUTILS_VERSION" &&
_gcc "$GCC_VERSION"
