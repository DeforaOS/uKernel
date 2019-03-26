DeforaOS uKernel
================

About uKernel
-------------

uKernel is intended to be the kernel of the DeforaOS Operating System.

This project is in a very early stage and only targets the Intel 32-bit
architecture for the moment (x86). Support for the Intel 64-bit architecture is
in progress (amd64).

Compiling uKernel
-----------------

First, the Makefiles must be generated for the current platform. This can be
performed with `configure(1)` from the DeforaOS configure project, found at
<https://www.defora.org/os/project/16/configure> (or
<https://github.com/DeforaOS/configure>). The procedure is then as follows:

    $ configure

Please refer to the documentation of DeforaOS configure for further
instructions.

Once this done, to build uKernel for the Intel 32-bit architecture, simply run:

    $ make ARCH="i386" CC="/path/to/compiler"

To build uKernel for the Intel 64-bit architecture, this would be:

    $ make ARCH="amd64" CC="/path/to/compiler"

The "CC" parameter is only required if a compiler other than the default is
necessary. When using GCC, it is normally recommended to use a compiler
specifically targeted for this purpose.

The resulting kernel is located in `src/kernel/uKernel.bin`. On the i386 and
amd64 platforms, it produces a multiboot-compliant kernel file.

Note that it is also possible to build uKernel outside of the source tree, by
using the "OBJDIR" variable as follows:

    $ make ARCH="i386" OBJDIR="$PWD/obj-i386"

Testing uKernel
---------------

To test uKernel for the Intel 32-bit platform:

    $ qemu-system-i386 -kernel src/kernel/uKernel.bin

It is also possible to leverage the generic loader:

    $ qemu-system-i386 -kernel src/loader/uLoader.bin -initrd src/kernel/uKernel.bin

On the Intel 64-bit platform, the loader is actually required:

    $ qemu-system-x86_64 -kernel src/loader/uLoader.bin -initrd src/kernel/uKernel.bin

