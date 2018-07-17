/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_CLOCK_CMOS_H
# define UKERNEL_DRIVERS_CLOCK_CMOS_H


/* constants */
# define CMOS_REGISTER_SECONDS		0x00
# define CMOS_REGISTER_MINUTES		0x02
# define CMOS_REGISTER_HOURS		0x04
# define CMOS_REGISTER_DAY		0x07
# define CMOS_REGISTER_MONTH		0x08
# define CMOS_REGISTER_YEAR		0x09
# define CMOS_REGISTER_STATUS0		0x0a
# define CMOS_REGISTER_STATUS1		0x0b
# define CMOS_REGISTER_STATUS2		0x0c
# define CMOS_REGISTER_STATUS3		0x0d

# define CMOS_HOUR_PM			0x80

# define CMOS_STATUS0_UPDATING		0x80

# define CMOS_STATUS1_24HOUR		0x02
# define CMOS_STATUS1_BINARY_MODE	0x04

#endif /* !UKERNEL_DRIVERS_CLOCK_CMOS_H */
