#	$NetBSD: Makefile,v 1.1 2012/01/22 03:53:29 tsutsui Exp $

LIVEIMGBASE=	amd64live		# gives ${IMGBASE}.img

BOOTDISK=	sd0			# for USB flash etc.
IMAGEMB=	1536            	# 2GB USB flash memories aren't 2GiB
OMIT_SWAPIMG=	yes			# no need to write swap

.include "${.CURDIR}/../Makefile.liveimage"
