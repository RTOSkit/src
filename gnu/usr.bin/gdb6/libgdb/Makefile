#	$NetBSD: Makefile,v 1.12 2011/07/03 15:26:25 mrg Exp $

.include <bsd.own.mk>

.include "../Makefile.inc"

LIBISPRIVATE=	yes
LIB=		gdb

.include "${.CURDIR}/../arch/${MACHINE_ARCH}/defs.mk"

DIST=		${NETBSDSRCDIR}/gnu/dist/gdb6

SRCS=		${G_LIBGDB_OBS:.o=.c}
GCPPFLAGS=	${G_INTERNAL_CFLAGS}
CPPFLAGS.bsd-kvm.c=	-D_KMEMUSER
CPPFLAGS+=	-I${.CURDIR} \
		-I${.CURDIR}/../arch/${MACHINE_ARCH} \
		-I${DIST}/gdb \
		-I${DIST}/gdb/config \
		-I${DIST}/include/opcode \
		-I${DIST} \
		-I${.CURDIR}/../bfd/arch/${MACHINE_ARCH} \
		-I${DIST}/bfd \
		-I${DIST}/include \
		${GCPPFLAGS:M-D*}

# XXX inf-ptrace.c has some bogus (PTRACE_TYPE_ARG3) casts, but in 
# code that's never exercised on NetBSD.
CPPFLAGS.inf-ptrace.c=	-Wno-error

.include "../../Makefile.inc"
.include <bsd.lib.mk>

.PATH: ${DIST}/gdb ${DIST}/gdb/mi ${DIST}/gdb/cli ${DIST}/gdb/tui \
    ${DIST}/gdb/signals ${DIST}/gdb/doc ${.CURDIR}/../arch/${MACHINE_ARCH}

#
COPTS+=	-Wno-stack-protector

# Don't run yacc/bison or lex
.y.c .y.h .l.c:

.if ${MACHINE} == "amiga"
COPTS.dwarf2-frame.c+=-O1
.endif

.if defined(HAVE_GCC) && HAVE_GCC == 4 && ${MACHINE} == "sun2"
COPTS.dwarf2-frame.c+=-O0
.endif
