/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_PIC_I8259A_H
# define UKERNEL_DRIVERS_PIC_I8259A_H


/* constants */
# define I8259A_MASTER_BASE		0x20
# define I8259A_MASTER_COMMAND		I8259A_MASTER_BASE
# define I8259A_MASTER_DATA		(I8259A_MASTER_BASE + 1)
# define I8259A_SLAVE_BASE		0xa0
# define I8259A_SLAVE_COMMAND		I8259A_SLAVE_BASE
# define I8259A_SLAVE_DATA		(I8259A_SLAVE_BASE + 1)

# define I8259A_COMMAND_ICW1_ICW4	0x01
# define I8259A_COMMAND_ICW1_INIT	0x10
# define I8259A_COMMAND_ICW4_8086	0x01
# define I8259A_COMMAND_EOI		0x20

#endif /* !UKERNEL_DRIVERS_PIC_I8259A_H */
