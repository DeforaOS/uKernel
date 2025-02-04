Cross-compiling with GCC
========================

Some compilers, like GCC, need to be built for a specific platform in order to
compile code suitable there. This document gathers the different steps required
for this purpose. Three major steps are required:

* install the dependencies
* compile binutils
* compile GCC

Dependencies
------------

Binutils and GCC require the following software components to be available in
the system prior to their installation:

| Component | Notes                      |
| --------- | -------------------------- |
| Make      | e.g., GNU Make or BSD Make |
| Bison     |                            |
| Flex      |                            |
| GMP       |                            |
| MPC       |                            |
| MPFR      |                            |
| Texinfo   |                            |

Please refer to the manual of your local system for information on how to find
and install these components as required.

Binutils
--------

Download the latest version of binutils; at the time of writing this document it
is version 2.32:

    $ wget https://ftpmirror.gnu.org/binutils/binutils-2.32.tar.gz

To configure binutils it is necessary to choose a target. It can be chosen
according to the following table:

| Port  | Target    |
| ----- | --------- |
| amd64 | amd64-elf |
| i386  | i686-elf  |

Modify the environment to reflect this choice; in this case for the i386 port:

    $ TARGET="i686-elf"
    $ PREFIX="$HOME/opt/cross-gcc-$TARGET"
    $ PATH="$PREFIX/bin:$PATH"

Extract, configure, and build binutils in a dedicated tree:

    $ tar xzf binutils-2.32.tar.gz
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

*Note:* GNU Make is required to build GCC.

Extract, configure, and build GCC in a dedicated tree:

    $ tar xzf gcc-8.3.0.tar.gz
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

    $ export TARGET="i686-elf"
    $ export PATH="$HOME/opt/cross-gcc-$TARGET/bin:$PATH"

The compiler can then be used directly. For instance, its version can be
obtained by invoking the following:

    $ $TARGET-gcc --version

Troubleshooting
---------------

In case the combination of binutils and GCC does not build together, it is
possible to refer to the following table and choose a combination known to have
worked before: <https://wiki.osdev.org/Cross-Compiler_Successful_Builds>
