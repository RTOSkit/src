#	$NetBSD: Makefile,v 1.5 2012/04/04 10:42:04 joerg Exp $

LIB=	clangAST

.include <bsd.init.mk>

.PATH: ${CLANG_SRCDIR}/lib/AST

SRCS+=	APValue.cpp \
	ASTConsumer.cpp \
	ASTContext.cpp \
	ASTDiagnostic.cpp \
	ASTImporter.cpp \
	AttrImpl.cpp \
	CXXInheritance.cpp \
	Decl.cpp \
	DeclarationName.cpp \
	DeclBase.cpp \
	DeclCXX.cpp \
	DeclFriend.cpp \
	DeclGroup.cpp \
	DeclObjC.cpp \
	DeclPrinter.cpp \
	DeclTemplate.cpp \
	DumpXML.cpp \
	Expr.cpp \
	ExprClassification.cpp \
	ExprConstant.cpp \
	ExprCXX.cpp \
	ExternalASTSource.cpp \
	InheritViz.cpp \
	ItaniumCXXABI.cpp \
	ItaniumMangle.cpp \
	LambdaMangleContext.cpp \
	Mangle.cpp \
	MicrosoftCXXABI.cpp \
	MicrosoftMangle.cpp \
	NSAPI.cpp \
	NestedNameSpecifier.cpp \
	ParentMap.cpp \
	RecordLayout.cpp \
	RecordLayoutBuilder.cpp \
	SelectorLocationsKind.cpp \
	Stmt.cpp \
	StmtDumper.cpp \
	StmtIterator.cpp \
	StmtPrinter.cpp \
	StmtProfile.cpp \
	StmtViz.cpp \
	TemplateBase.cpp \
	TemplateName.cpp \
	Type.cpp \
	TypeLoc.cpp \
	TypePrinter.cpp \
	VTTBuilder.cpp \
	VTableBuilder.cpp

.if defined(HOSTLIB)
.include <bsd.hostlib.mk>
.else
.include <bsd.lib.mk>
.endif
