#	$NetBSD: Makefile,v 1.5 2011/11/04 17:52:19 christos Exp $

.include <bsd.own.mk>
.include "../Makefile.inc"

GDBARCHDIR=${.CURDIR}/../../lib/libgdb/arch/${MACHINE_ARCH}
.include "${GDBARCHDIR}/defs.mk"

PROG=		gdb

SRCS=		gdb.c
CPPFLAGS+=	-I${.CURDIR} \
		-I${GDBARCHDIR} \
		-I${DIST}/gdb \
		-I${DIST}/gdb/config \
		-I${DIST}/include/opcode \
		-I${DIST} \
		-I${.CURDIR}/../../lib/libbfd/arch/${MACHINE_ARCH} \
		-I${DIST}/bfd \
		-I${DIST}/include \
		${GCPPFLAGS:M-D*}

TEXINFO=	gdb.texinfo gdbint.texinfo stabs.texinfo
INFOFLAGS=	-I${DIST}/gdb/doc -I${DIST}/gdb/mi \
		-I${DIST}/readline/doc -I${.CURDIR}/.. -DGDBN=${GDBN}

GDBvn.texi: ${DIST}/gdb/version.in
	echo "@set GDBVN $$(${TOOL_CAT} ${.ALLSRC})" > ${.TARGET}

gdb-cfg.texi: ${DIST}/gdb/doc/all-cfg.texi
	${TOOL_CAT} ${.ALLSRC} > ${.TARGET}

CLEANFILES+= GDBvn.texi gdb-cfg.texi
gdbint.info stabs.info gdb.info: gdb-cfg.texi GDBvn.texi

.include "../../Makefile.inc"
.include <bsd.info.mk>

LIBOPCODESDIR!=   cd ${.CURDIR}/../../lib/libopcodes   && ${PRINTOBJDIR}
LIBBFDDIR!=       cd ${.CURDIR}/../../lib/libbfd       && ${PRINTOBJDIR}
LIBIBERTYDIR!=    cd ${.CURDIR}/../../lib/libiberty    && ${PRINTOBJDIR}
LIBGDBDIR!=	  cd ${.CURDIR}/../../lib/libgdb       && ${PRINTOBJDIR}
LIBDECNUMBERDIR!= cd ${.CURDIR}/../../lib/libdecnumber && ${PRINTOBJDIR}

.if ${USE_LIBEDIT} == yes
LIBRL=-ledit
LIBRLDP=${LIBEDIT}
.else
LIBRLDIR!=  cd ${.CURDIR}/../../lib/libreadline && ${PRINTOBJDIR}
LIBRL=      -L${LIBRLDIR} -lreadline
LIBRLDP=    ${LIBRLDIR}/libreadline.a
.endif

LDADD+= -L${LIBGDBDIR} -lgdb
DPADD+= ${LIBGDBDIR}/libgdb.a

# Simulator support
.if ${MACHINE_ARCH} == powerpc || ${MACHINE_ARCH} == powerpcle || \
    ${MACHINE_ARCH} == powerpc64
SIMOBJ!=	cd ${.CURDIR}/../../lib/libsim && ${PRINTOBJDIR}
LDADD+=		-L${SIMOBJ} -lsim
DPADD+=		${SIMOBJ}/libsim.a
.elif ${MACHINE_CPU} == xxmips
SIMOBJ!=	cd ${.CURDIR}/../../lib/libsim && ${PRINTOBJDIR}
LDADD+=		-L${SIMOBJ} -lsim
DPADD+=		${SIMOBJ}/libsim.a
.endif

LDADD+= -L${LIBOPCODESDIR} -lopcodes \
	-L${LIBBFDDIR} -lbfd \
	-L${LIBDECNUMBERDIR} -ldecnumber \
	${LIBRL} \
	-lterminfo \
	-L${LIBIBERTYDIR} -liberty

DPADD+=	${LIBOPCODESDIR}/libopcodes.a \
	${LIBBFDDIR}/libbfd.a \
	${LIBDECNUMBERDIR}/libdecnumber.a \
	${LIBRLDP} \
	${LIBTERMINFO} \
	${LIBIBERTYDIR}/libiberty.a

LDADD+=		-lz -lcurses -lintl -lm -lkvm
DPADD+=		${LIBZ} ${LIBCURSES} ${LIBINTL} ${LIBM} ${LIBKVM}

LDADD+=                -lpthread_dbg

.include <bsd.prog.mk>

.PATH: ${DIST}/gdb ${DIST}/gdb/mi ${DIST}/gdb/cli ${DIST}/gdb/tui \
    ${DIST}/gdb/signals ${DIST}/gdb/doc ${.CURDIR}/../arch/${MACHINE_ARCH}
