#	$NetBSD: Makefile,v 1.14 2008/11/04 23:58:40 dbj Exp $

LIBISPRIVATE=	yes

LIB=		backend

SRCS=		${G_OBJS:.o=.c} ${G_out_file:T}

CPPFLAGS+=	-I. -I${GCCARCH} ${G_ALL_CFLAGS:M-D*} ${G_INCLUDES:M-I*:N-I.*} \
		-DTARGET_NAME=\"${MACHINE_GNU_PLATFORM}\"
HOST_CPPFLAGS+=	-I. -I${GCCARCH} ${G_ALL_CFLAGS:M-D*} ${G_INCLUDES:M-I*:N-I.*}
DPSRCS+=	insn-attr.h insn-codes.h insn-config.h insn-flags.h tree-check.h
CPPFLAGS.prefix.c+=     -DPREFIX=\"/usr\"

.include <bsd.lib.mk>

# For early builders.
EXTRADEPENDSEARLY=	${G_BUILD_EARLY_SUPPORT:.o=.lo} ${G_BUILD_RTL:.o=.lo} \
			${G_BUILD_ERRORS:.o=.lo}

# Don't auto-frob .y or .l files.
.l.c .y.c .y.h:
	@true

#
# Independent generation programs.
#
CLEANFILES+=	gengenrtl genrtl.c genrtl.h \
		gengtype gtyp-gen.h gtype-desc.c gtype-desc.h \
		gtype-c.h gtype-cp.h gtype-f.h gtype-objc.h \
		genflags genflags.h \
		gencheck tree-check.h \
		insn-preds.c genpreds tm-preds.h tm_p.h cs-tm_p.h \
		genmodes insn-modes.c insn-modes.h min-insn-modes.c \
		auto-host.h build-print-rtl.c \
		build-rtl.c build-errors.c build-varray.c \
		build-bitmap.c build-ggc-none.c \
		gt-*.h *.o *.lo \
		cs-config.h \
		gcov-iov gcov-iov.h \

#
# These files have special needs
#
.for f in \
	build-errors build-ggc-none build-print-rtl build-rtl build-varray dummy-conditions gcov-iov genattr genattrtab genautomata gencheck genchecksum gencodes genconditions genconfig genconstants genemit genextract genflags gengenrtl gengtype gengtype-lex gengtype-yacc genmddeps genmodes genopinit genoutput genpeep genpreds genrecog gensupport insn-conditions min-insn-modes read-rtl
CPPFLAGS.${f}.c=		-DGENERATOR_FILE
.endfor

# XXX
COPTS.varasm.c=			-Wno-error


# Headers that host objects depend on (except gen*rtl*)
HH_NORTL=	${G_tm_file_list} ${G_build_xm_include_list}
HH=		${HH_NORTL} genrtl.h insn-modes.h

#
# Generate the various header files we need.
#
genrtl.c: gengenrtl
	${_MKTARGET_CREATE}
	./gengenrtl >${.TARGET}
genrtl.h: gengenrtl
	${_MKTARGET_CREATE}
	./gengenrtl -h >${.TARGET}
gengenrtl.lo: ${HH_NORTL}
gengenrtl: gengenrtl.lo build-errors.lo
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

tm-preds.h: genpreds ${G_md_file}
	${_MKTARGET_CREATE}
	./genpreds -h ${G_md_file} >${.TARGET}
insn-preds.c: genpreds ${G_md_file}
	${_MKTARGET_CREATE}
	./genpreds ${G_md_file} >${.TARGET}
genpreds.lo: ${HH}
genpreds: genpreds.lo ${EXTRADEPENDSEARLY}
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

tree-check.h: gencheck
	${_MKTARGET_CREATE}
	./gencheck >${.TARGET}
gencheck.lo: ${HH}
gencheck: gencheck.lo
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

gtype-desc.c: gtype-desc.h
gtype-desc.h: gengtype
	${_MKTARGET_CREATE}
	rm -f auto-host.h
	ln -s ${.CURDIR}/../arch/${MACHINE_ARCH}/auto-host.h .
	./gengtype
# gengtype is the real need for options.h
gengtype.lo gengtype-lex.lo gengtype-yacc.lo: ${HH} gtyp-gen.h config.h options.h
gengtype: gengtype.lo gengtype-lex.lo gengtype-yacc.lo build-errors.lo
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

gtyp-gen.h: Makefile
	${_MKTARGET_CREATE}
	(\
	echo "static const char *srcdir = \"$(G_GTFILES_SRCDIR)\";"	; \
	echo "static const char *lang_files[] = {"			; \
	for f in $(G_GTFILES_FILES_FILES); do echo "\"$$f\", "; done	; \
	echo "NULL };"							; \
	echo "static const char *langs_for_lang_files[] = {"		; \
	for f in $(G_GTFILES_FILES_LANGS); do echo "\"$$f\", "; done 	; \
	echo "NULL };"							; \
	echo "static const char *all_files[] = {"			; \
	for f in $(G_GTFILES); do echo "\"$$f\", "; done		; \
	echo " NULL };"							; \
	echo "static const char *lang_dir_names[] = {"			; \
	echo "\"c\", "							; \
	for f in $(G_GTFILES_LANG_DIR_NAMES); do echo "\"$$f\", "; done	; \
	echo "NULL };"							; \
	) >${.TARGET}

insn-modes.c: genmodes
	${_MKTARGET_CREATE}
	./genmodes >${.TARGET}
insn-modes.h: genmodes
	${_MKTARGET_CREATE}
	./genmodes -h >${.TARGET}
min-insn-modes.c: genmodes
	${_MKTARGET_CREATE}
	./genmodes -m >${.TARGET}
genmodes.lo: ${HH_NORTL}
genmodes: genmodes.lo build-errors.lo
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

gcov-iov.h: gcov-iov
	${_MKTARGET_CREATE}
	./gcov-iov "${BASEVER}" "${DEVPHASE}" >${.TARGET}
gcov-iov.lo: ${HH_NORTL}
gcov-iov: gcov-iov.lo
	${_MKTARGET_CREATE}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}

#config.h: Makefile
#	${_MKTARGET_CREATE}
#	TM_DEFINES="$(G_tm_defines)" \
#	HEADERS="$(G_host_xm_file)" XM_DEFINES="$(G_host_xm_defines)" \
#	TARGET_CPU_DEFAULT="$(G_target_cpu_default)" \
#	${HOST_SH} ${GNUHOSTDIST}/gcc/mkconfig.sh ${.TARGET}

tm_p.h: Makefile
	${_MKTARGET_CREATE}
	TARGET_CPU_DEFAULT="" \
	HEADERS="$(G_tm_p_include_list)" DEFINES="" \
	${HOST_SH} ${GNUHOSTDIST}/gcc/mkconfig.sh ${.TARGET}

CLEANFILES+=	options.h options.c optionlist
optionlist: ${G_ALL_OPT_FILES} ${GCCARCH}/defs.mk Makefile ${DIST}/gcc/opt-gather.awk
	${TOOL_AWK} -f ${DIST}/gcc/opt-gather.awk ${G_ALL_OPT_FILES} > ${.TARGET}

options.c: optionlist ${DIST}/gcc/opt-functions.awk ${DIST}/gcc/optc-gen.awk
	${TOOL_AWK} -f ${DIST}/gcc/opt-functions.awk -f ${DIST}/gcc/optc-gen.awk \
		-v header_name="config.h system.h coretypes.h tm.h" < optionlist > ${.TARGET}

options.h: optionlist ${DIST}/gcc/opt-functions.awk ${DIST}/gcc/opth-gen.awk
	${TOOL_AWK} -f ${DIST}/gcc/opt-functions.awk -f ${DIST}/gcc/opth-gen.awk \
		< optionlist > ${.TARGET}

options.o: options.c coretypes.h opts.h intl.h
#options.o: options.c ${G_CONFIG_H} ${G_SYSTEM_H} coretypes.h ${G_TM_H} opts.h intl.h

# Yuck, we have to run configure to generate this one...
CLEANFILES+=	auto-build.h
auto-build.h: Makefile
	${_MKTARGET_CREATE}
	rm -rf .ab && \
	mkdir .ab && \
	(cd .ab && \
		AWK=${TOOL_AWK:Q} \
		CC=${HOST_CC:Q} CFLAGS=${HOST_CFLAGS:Q} \
		MAKE=${MAKE:Q} \
		CONFIG_SHELL=${HOST_SH:Q} \
		${HOST_SH} ${GNUHOSTDIST}/gcc/configure \
			--build=`${HOST_SH} ${GNUHOSTDIST}/config.guess` \
			--host=`${HOST_SH} ${GNUHOSTDIST}/config.guess` \
			--target=${MACHINE_GNU_PLATFORM} && \
		mv auto-host.h ../auto-build.h) && \
	rm -rf .ab

#
# These are copies of files we build for the build host that are used
# by the genfoo programs.
#

build-rtl.c: rtl.c \
  ${G_RTL_H} real.h ${G_GCC_H} errors.h gtype-desc.h
	${_MKTARGET_CREATE}
	rm -f build-rtl.c
	${TOOL_SED} -e 's/<config[.]h/hconfig.h/' \
		${GNUHOSTDIST}/gcc/rtl.c >${.TARGET}
build-rtl.lo: ${HH}

build-print-rtl.c: print-rtl.c \
  ${G_RTL_H} ${G_TREE_H} hard-reg-set.h ${G_BASIC_BLOCK_H}
	${_MKTARGET_CREATE}
	rm -f buid-print-rtl.c
	${TOOL_SED} -e 's/<config[.]h/hconfig.h/' \
		${GNUHOSTDIST}/gcc/print-rtl.c >${.TARGET}
build-print-rtl.lo: ${HH}

build-bitmap.c: bitmap.c \
  ${G_RTL_H} flags.h ${G_BASIC_BLOCK_H} ${REGS_H} ${G_GCC_H} gtype-desc.h
	${_MKTARGET_CREATE}
	rm -f build-bitmap.c
	${TOOL_SED} -e 's/<config[.]h/hconfig.h/' \
		${GNUHOSTDIST}/gcc/bitmap.c >${.TARGET}
build-bitmap.lo: ${HH}

build-errors.c: errors.c errors.h
	${_MKTARGET_CREATE}
	rm -f build-errors.c
	cat ${GNUHOSTDIST}/gcc/errors.c >${.TARGET}
build-errors.lo: ${HH_NORTL}

build-varray.c: varray.c \
  ${G_RTL_H} ${G_GCC_H} ${TREE_H} bitmap.h errors.h
	${_MKTARGET_CREATE}
	rm -f build-varray.c
	${TOOL_SED} -e 's/<config[.]h/hconfig.h/' \
		${GNUHOSTDIST}/gcc/varray.c >${.TARGET}
build-varray.lo: ${HH} gtype-desc.h

build-ggc-none.c: ggc-none.c \
  ${G_GCC_H} gtype-desc.h
	${_MKTARGET_CREATE}
	rm -f build-ggc-none.c
	${TOOL_SED} -e 's/<config[.]h/hconfig.h/' \
		${GNUHOSTDIST}/gcc/ggc-none.c >${.TARGET}
build-ggc-none.lo: ${HH}

#
# The `early' insn-foo generators
#
.for f in conditions.c constants.h
CLEANFILES+=		gen${f:R} insn-${f}

gen${f:R}.lo: gen${f:R}.c ${HH}
gen${f:R}: gen${f:R}.lo ${EXTRADEPENDSEARLY}
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}
insn-${f}: gen${f:R} ${G_md_file}
	${_MKTARGET_CREATE}
	./gen${f:R} ${G_md_file} >${.TARGET}
.endfor

#
# The normal insn-foo generators
#
.for f in gensupport.c read-rtl.c dummy-conditions.c
${f:R}.lo: ${HH}
.endfor

EXTRADEPENDS=		${G_BUILD_RTL:.o=.lo} ${G_BUILD_SUPPORT:.o=.lo} \
			${G_BUILD_PRINT:.o=.lo} ${G_BUILD_ERRORS:.o=.lo}
.for f in attr.h codes.h config.h emit.c extract.c flags.h opinit.c output.c \
	  peep.c recog.c
CLEANFILES+=		gen${f:R} insn-${f}

gen${f:R}.lo: ${HH}
gen${f:R}: gen${f:R}.lo ${EXTRADEPENDS} 
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY}
insn-${f}: gen${f:R} ${G_md_file}
	${_MKTARGET_CREATE}
	./gen${f:R} ${G_md_file} >${.TARGET}
.endfor

#
# "One of these things is not like the other"
#
CLEANFILES+=		genattrtab insn-attrtab.c
genattrtab.lo: ${HH} gtype-desc.h
genautomata.lo: ${HH}
genattrtab: genattrtab.lo genautomata.lo ${EXTRADEPENDS} ${G_BUILD_VARRAY:.o=.lo}
	${_MKTARGET_LINK}
	${HOST_LINK.c} -o ${.TARGET} ${.ALLSRC} ${NBCOMPATLIB} ${HOSTLIBIBERTY} -lm
insn-attrtab.c: genattrtab ${G_md_file}
	${_MKTARGET_CREATE}
	./genattrtab ${G_md_file} >${.TARGET}

#
# Required hard-coded dependancies.
#
genextract.lo: insn-config.h
insn-conditions.lo: insn-constants.h tm_p.h tm-preds.h
build-ggc-none.lo: gtype-desc.h
ggc-none.o: gtype-desc.h

insn-attrtab.o: insn-config.h
insn-extract.o: insn-config.h
toplev.d toplev.o: options.h
coverage.d: gcov-iov.h
gcov-io.h: gcov-iov.h

COPTS+=-Wno-stack-protector

.PATH: ${DIST}/gcc ${DIST}/libiberty ${GCCARCH} ${G_out_file:H} ${DIST}/include
