Cross-compiling with GCC
========================

Some compilers, like GCC, need to be built for a specific platform in order to
compile code suitable there. This document gathers the different steps required
for this purpose. Three major steps are required:

* install the dependencies
* patch and compile binutils
* patch and compile GCC

For convenience, a helper script is available in `cross-gcc.sh`.

Dependencies
------------

Binutils and GCC require the following software components to be available in
the system prior to their installation:

| Component | Notes                |
| --------- | -------------------- |
| Make      | GNU Make is required |
| Bison     |                      |
| Flex      |                      |
| GMP       |                      |
| MPC       |                      |
| MPFR      |                      |
| Texinfo   | Optional             |

Please refer to the manual of your local system for information on how to find
and install these components as required.

Binutils
--------

Download the latest version of binutils; at the time of writing this document it
is version 2.32:

    $ wget https://ftpmirror.gnu.org/binutils/binutils-2.32.tar.gz

To configure binutils it is necessary to choose a target. It can be chosen
according to the following table:

| Architecture | Target              |
| ------------ | ------------------- |
| amd64        | amd64-none-deforaos |
| i386         | i686-none-deforaos  |

Modify the environment to reflect this choice; in this case for the i386
architecture:

    $ TARGET="i686-none-deforaos"
    $ PREFIX="$HOME/opt/cross-gcc-$TARGET"
    $ PATH="$PREFIX/bin:$PATH"

Extract, patch, configure, and build binutils in a dedicated tree:

    $ tar xzf binutils-2.32.tar.gz
    $ (cd binutils-2.32 && patch -p1 < ../doc/patch-binutils_2.32.diff)
    $ (cd binutils-2.32/ld && aclocal && automake)
    $ mkdir binutils-build
    $ (cd binutils-build && ../binutils-2.32/configure --target="$TARGET" \
      --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror)
    $ (cd binutils-build && make)
    $ (cd binutils-build && make install)

GCC
---

Download the latest version of GCC; at the time of writing this document it is
version 8.3.0:

    $ wget https://ftpmirror.gnu.org/gcc/gcc-8.3.0/gcc-8.3.0.tar.gz

Just the same, GCC has to be configured for the target chosen.

Extract, patch, configure, and build GCC in a dedicated tree:

    $ tar xzf gcc-8.3.0.tar.gz
    $ (cd gcc-8.3.0 && patch -p1 < ../doc/patch-gcc_8.3.0.diff)
    $ mkdir gcc-build
    $ (cd gcc-build && ../gcc-8.3.0/configure --target="$TARGET" \
      --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ \
      --without-headers
    $ (cd gcc-build && make all-gcc)
    $ (cd gcc-build && make all-target-libgcc)
    $ (cd gcc-build && make install-gcc)
    $ (cd gcc-build && make install-target-libgcc)

Using the cross-compiler
------------------------

Make sure the compiler binaries are available in your PATH:

    $ export TARGET="i686-none-deforaos"
    $ export PATH="$HOME/opt/cross-gcc-$TARGET/bin:$PATH"

The compiler can then be used directly. For instance, its version can be
obtained by invoking the following:

    $ $TARGET-gcc --version

Troubleshooting
---------------

In case the combination of binutils and GCC does not build together, it is
possible to refer to the following table and choose a combination known to have
worked before: <https://wiki.osdev.org/Cross-Compiler_Successful_Builds>
