include ${top_srcdir}/Include.mk

EXTRA_DIST= NewickLexicalAnalyzer.ll NewickParser.yy

lib_LTLIBRARIES=libsodaNewickParser.la

noinst_HEADERS=\
    NewickLexicalAnalyzer.h\
    NewickTreeParser.h

libsodaNewickParser_la_SOURCES=\
    NewickParser.cpp\
    NewickLexicalAnalyzer.cpp\
    NewickTreeParser.cpp

NewickParser.hpp NewickParser.cpp: NewickParser.yy
	bison -d -o NewickParser.cpp NewickParser.yy

NewickLexicalAnalyzer.cpp: NewickLexicalAnalyzer.ll
	flex -t NewickLexicalAnalyzer.ll > NewickLexicalAnalyzer.cpp

CLEANFILES=NewickParser.cpp NewickParser.hpp NewickParser.output NewickLexicalAnalyzer.cpp
