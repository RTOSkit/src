#	$NetBSD: Makefile.inc,v 1.14 2007/03/31 06:03:13 matt Exp $

RELOC=0x3f0000
.PATH:	${.CURDIR}/../../vax ${.CURDIR}/../common

CPPFLAGS+=-I.
CPPFLAGS+=-I${.CURDIR}/../../../../
CPPFLAGS+=-I${.CURDIR}/../../
CPPFLAGS+=-I${.CURDIR}/../common
CPPFLAGS+=-I${.CURDIR}/../../include
CPPFLAGS+=-DRELOC=${RELOC}

WARNS?=	1
CFLAGS+=-ffreestanding -fno-pic
AFLAGS+=-fno-pic

MKMAN=no

LIBCRTBEGIN=
LIBCRTEND=  

.if ${MACHINE} == "vax"
.PHONY: machine-links
beforedepend: machine-links
machine-links:
	@rm -f machine && ln -s ${S}/arch/${MACHINE}/include machine
	@rm -f ${MACHINE_ARCH} && ln -s ${S}/arch/${MACHINE_ARCH}/include ${MACHINE_ARCH}
.NOPATH: machine ${MACHINE_ARCH}
CLEANFILES+= machine ${MACHINE_ARCH}
.endif
