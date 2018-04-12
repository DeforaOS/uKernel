/* $Id$ */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Creating_a_64-bit_kernel */

/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;

	.text : AT(ADDR(.text) - 1M)
	{
		_code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data : AT(ADDR(.data) - 1M)
	{
		_data = .;
		*(.data)
		. = ALIGN(4096);
	}

	_edata = .;

	.eh_frame : AT(ADDR(.eh_frame) - 1M)
	{
		_ehframe = .;
		*(.eh_frame)
		. = ALIGN(4096);
	}

	.bss : AT(ADDR(.bss) - 1M)
	{
		_bss = .;
		*(.bss)

		/*
		 * You usually need to include generated COMMON symbols
		 * under kernel BSS section or use gcc's -fno-common
		 */

		*(COMMON)
		. = ALIGN(4096);
	}

	_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}
