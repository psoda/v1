include ${top_srcdir}/Include.mk

EXTRA_DIST= PaupLexicalAnalyzer.ll PaupParser.yy

lib_LTLIBRARIES=libsodaPaupParser.la

libsodaPaupParser_la_SOURCES=\
    PaupParser.cpp\
    PaupLexicalAnalyzer.cpp

PaupLexicalAnalyzer.cpp: PaupLexicalAnalyzer.ll
	flex -t PaupLexicalAnalyzer.ll > PaupLexicalAnalyzer.cpp

PaupParser.cpp: PaupParser.yy
	bison -d -o PaupParser.cpp PaupParser.yy

CLEANFILES= PaupParser.cpp PaupParser.hpp PaupLexicalAnalyzer.cpp
