#!/bin/sh
#$Id$
#Copyright (c) 2018 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS uKernel



#variables
PROGNAME="grub.sh"
#executables
GRUBFILE="grub-file"
UNAME="uname"


#functions
#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}


#info
_info()
{
	echo "$PROGNAME: $@"
}


#main
case $($UNAME -m) in
	amd64|i?86)
		_info "Testing multiboot conformance"
		$GRUBFILE --is-x86-multiboot ../uKernel.bin
		res=$?
		if [ $res -eq 127 ]; then
			_error "Cannot test: $GRUBFILE not available (ignored)"
			exit 0
		elif [ $req -ne 0 ]; then
			_error ""
			exit $?
		fi
		;;
	*)
		;;
esac
