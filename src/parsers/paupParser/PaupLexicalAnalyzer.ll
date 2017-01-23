%{
#include "PaupParser.hpp"
#include <string>

using namespace std;

//#define QECHO ECHO
#define QECHO
#define YY_NEVER_INTERACTIVE 1
#define MATRIXFRAMESIZE 15000
#define PUSH_EXPR yy_push_state(expressions, yyscanner)
#define PUSH_NAME yy_push_state(name, yyscanner)
#define YY_DECL int yylex (YYSTYPE *yylval_param, YYLTYPE* yylloc_param, void* yyscanner) 

union YYSTYPE;
struct YYLTYPE;
string currentString;
int taxanum = 0;
int reftaxanum = 0;
int sstaxanum = 0;
int refsstaxanum = 0;

%}

%option prefix="paupLex"
%option bison-locations yylineno
%option reentrant
%option stack
%option nounistd
%option never-interactive
%x comment
%x include
%x endsequence
%x endsequencematrix
%x escaped_char
%x expressions
%x data
%x matrix
%x name
%x psoda
%x refdata
%x refendsequence
%x refendsequencematrix
%x refmatrix
%x refsequence
%x refsequences
%x refsequences2
%x refsequences3
%x refsequences4
%x sequence
%x sequences
%x sequences2
%x sequences3
%x sequences4
%x ssdata
%x ssendsequence
%x ssendsequencematrix
%x ssmatrix
%x sssequence
%x sssequences
%x sssequences2
%x sssequences3
%x sssequences4
%x refssdata
%x refssendsequence
%x refssendsequencematrix
%x refssmatrix
%x refsssequence
%x refsssequences
%x refsssequences2
%x refsssequences3
%x refsssequences4
%x string_literal
%x taxa
%x top
%x translate
%x trees
%x weights
%x weights_list
%x begin_block
%x command_def
%x varcast

%{
  static int colIndex = 1;
  static int charIndex = 0;
  static int last_line = 1;

  #define YY_USER_ACTION \
    yylloc_param->first_line = last_line;\
    yylloc_param->first_column = colIndex;\
    colIndex += yyleng;\
    charIndex += yyleng;\
    yylloc_param->last_line = last_line; \
    yylloc_param->last_column = colIndex - 1;

  #define NEW_LINE \
    colIndex = 1;\
    last_line = yylineno;\
    yylloc_param->last_line = last_line
%}
%%
"#"[Nn][Ee][Xx][Uu][Ss]        			{ QECHO; yy_push_state(top,yyscanner); return NEXUSSY; }
">"						{ QECHO; yy_push_state(sequences, yyscanner); yy_push_state(sequences2,yyscanner); return SEQUENCE_STARTSY; }
<string_literal>[^\"\\\n]+		{ QECHO; currentString += string(yytext); }
<trees>[Ee][Nn][Dd]			{ QECHO; currentString += string(yytext) + ";\n"; yylval_param->stringval = currentString.c_str(); yy_pop_state(yyscanner); return TREEBLOCKSTRING; }
<trees>";" 				{ QECHO; currentString += string(yytext); }
<trees>[\n]				{ QECHO; currentString += string(yytext); }
<trees>[ \t\r]				{ QECHO; currentString += string(yytext); }
<trees>[^; \t\r\n]			{ QECHO; currentString += string(yytext); }

<*>[[]			 		{ QECHO; yy_push_state(comment,yyscanner); }
<*>[ \r\t]+				{ QECHO; }
<top,psoda>";"      			{ QECHO; return SEMISY; }
<string_literal>\n			{ QECHO; NEW_LINE; currentString += string(yytext); }
<string_literal>"\\"			{ QECHO; yy_push_state(escaped_char, yyscanner); }
<string_literal>"\""			{ QECHO; yy_pop_state(yyscanner); yytext = (char*)currentString.c_str(); return STRINGSY; }
<sequences3>\n			            { QECHO; yy_pop_state(yyscanner); yy_pop_state(yyscanner); yy_push_state(sequences4,yyscanner); }
<refsequences3>\n			{ QECHO; yy_pop_state(yyscanner); yy_pop_state(yyscanner); yy_push_state(refsequences4,yyscanner); }
<sssequences3>\n			{ QECHO; yy_pop_state(yyscanner); yy_pop_state(yyscanner); yy_push_state(sssequences4,yyscanner); }
<refsssequences3>\n			{ QECHO; yy_pop_state(yyscanner); yy_pop_state(yyscanner); yy_push_state(refsssequences4,yyscanner); }
<*>\n					{ QECHO; NEW_LINE; }

<psoda>[Ee][Nn][Dd][Ww][Hh][Ii][Ll][Ee]	{ QECHO; return ENDWHILESY; }
<psoda>[Ee][Nn][Dd][Ii][Ff]		{ QECHO; return ENDIFSY; }
<top,psoda>[Ee][Nn][Dd]       		{ QECHO; return ENDSY; }
<top,psoda>[Bb][Ee][Gg][Ii][Nn]         { QECHO; yy_push_state(begin_block, yyscanner); return BEGINSY; }

<begin_block>[Tt][Rr][Ee][Ee][Ss]				{ QECHO; yy_pop_state(yyscanner); yy_push_state(trees,yyscanner); currentString = "begin " + string(yytext); }
<begin_block>[Tt][Aa][Xx][Aa]          				{ QECHO; yy_pop_state(yyscanner); yy_push_state(taxa,yyscanner); return TAXASY; }
<begin_block>[Pp][Aa][Uu][Pp]					{ QECHO; yy_pop_state(yyscanner); yy_push_state(psoda,yyscanner); return PAUPSY; }
<begin_block>[Pp][Ss][Oo][Dd][Aa]				{ QECHO; yy_pop_state(yyscanner); yy_push_state(psoda,yyscanner); return PSODASY; }
<begin_block>[Cc][Hh][Aa][Rr][Aa][Cc][Rt][Ee][Rr][Ss]   	{ QECHO; yy_pop_state(yyscanner); yy_push_state(data,yyscanner); return CHARACTERSSY; }
<begin_block>[Dd][Aa][Tt][Aa]          				{ QECHO; yy_pop_state(yyscanner); yy_push_state(data,yyscanner); return DATASY; }
<begin_block>[Rr][Ee][Ff][Dd][Aa][Tt][Aa]      			{ QECHO; yy_pop_state(yyscanner); yy_push_state(refdata,yyscanner); return REFDATASY; }
<begin_block>[Ss][Ss][Dd][Aa][Tt][Aa]      			{ QECHO; yy_pop_state(yyscanner); yy_push_state(ssdata,yyscanner); return SSDATASY; }
<begin_block>[Ss][Ee][Cc][Oo][Nn][Dd][Aa][Rr][Yy][ _]?[Ss][Tt][Rr][Uu][Cc][Tt][Uu][Rr][Ee][Ss]?          				{ QECHO; yy_pop_state(yyscanner); yy_push_state(ssdata,yyscanner); return SSDATASY; }
<begin_block>[Rr][Ee][Ff][Ss][Ss][Dd][Aa][Tt][Aa]      		{ QECHO; yy_pop_state(yyscanner); yy_push_state(refssdata,yyscanner); return REFSSDATASY; }
<begin_block>[Aa][Ss][Ss][Uu][Mm][Pp][Tt][Ii][Oo][Nn][Ss]       { QECHO; yy_pop_state(yyscanner); return ASSUMPTIONSSY; }
<begin_block>[Ss][Ee][Tt][Ss]          				{ QECHO; yy_pop_state(yyscanner); return SETSSY; }
<begin_block>[Dd][Ii][Ss][Tt][Aa][Nn][Cc][Ee][Ss]       	{ QECHO; yy_pop_state(yyscanner); return DISTANCESSY; }
<begin_block>"*"  						{ QECHO; return STARSY; }
<begin_block>[\$_0-9a-zA-Z]+  					{ QECHO; yy_pop_state(yyscanner); yy_push_state(command_def, yyscanner); return TOP_IDENTSY; }



<command_def>"="	    				{ QECHO; return EQSY; }
<command_def>"&"             				{ QECHO; return REFERENCESY; }
<command_def>","             				{ QECHO; return COMMASY; }
<command_def>[-]?[0-9]+[\.][0-9]+			{ QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
<command_def>[-]?[0-9]+ 				{ QECHO; return NUMBERSY; }
<command_def>[Ii][Nn][Tt][ \t\r\n]*[(]			{ QECHO; PUSH_EXPR; return INTCASTSY; }       
<command_def>[Bb][Oo][Oo][Ll][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return BOOLCASTSY; }      
<command_def>[Ss][Tt][Rr][Ii][Nn][Gg][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return STRINGCASTSY; }    
<command_def>[Dd][Oo][Uu][Bb][Ll][Ee][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return REALNUMBERCASTSY; }
<command_def>[\$_0-9a-zA-Z]+[ \t\r\n]*[(]		{ QECHO; return CALLSY; }
<command_def>[\$_0-9a-zA-Z]+  				{ QECHO; return IDENTSY; }


<psoda>[Ww][Hh][Ii][Ll][Ee]			{ QECHO; return WHILESY; }
<psoda>[Ii][Ff]					{ QECHO; return IFSY; }
<psoda>[Ee][Ll][Ss][Ii][Ff]			{ QECHO; return ELSIFSY; }
<psoda>[Ee][Ll][Ss][Ee]				{ QECHO; return ELSESY; }
<psoda>[Bb][Rr][Ee][Aa][Kk]			{ QECHO; return BREAKSY; }
<psoda>[Vv][Aa][Rr] 	 			{ QECHO; return VARSY; }
<psoda>[Rr][Ee][Tt][Uu][Rr][Nn]			{ QECHO; PUSH_EXPR; return RETURNSY; }
<psoda>[Hh][Ee][Ll][Pp]				{ QECHO; PUSH_EXPR; return HELPSY; }
<psoda>[Pp][Rr][Ii][Nn][Tt]			{ QECHO; PUSH_EXPR; return PRINTSY; }
<psoda>[Ww][Ee][Ii][Gg][Hh][Tt][Ss]		{ QECHO; yy_push_state(weights,yyscanner); return WEIGHTSSY;}
<psoda>[Ww][Tt][Ss]				{ QECHO; yy_push_state(weights,yyscanner); return WEIGHTSSY;}
<psoda,expressions,name>[-]?[0-9]+[\.][0-9]+	{ QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY; }
<psoda,expressions,name>[-]?[0-9]+ 		{ QECHO; return NUMBERSY; }

<name>"/"	    				{ QECHO; return SLASHSY; }
<name>"="	    				{ QECHO; yy_pop_state(yyscanner); PUSH_EXPR; return EQSY; }
<name,command_def>";"             		{ QECHO; yy_pop_state(yyscanner); return SEMISY; }
<name,command_def>"("             		{ QECHO; return LEFTPARENSY; }
<name,command_def>")"             		{ QECHO; return RIGHTPARENSY; }
<name>[Aa][Ll][Ll]    				{ QECHO; return ALLSY; }
<name>"{"	    				{ QECHO; return LEFTCURLYSY; }
<name>"}"	    				{ QECHO; return RIGHTCURLYSY; }
<name>[Ii][Nn][Tt][ \t\r\n]*[(]			{ QECHO; PUSH_EXPR; return INTCASTSY; }	       
<name>[Bb][Oo][Oo][Ll][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return BOOLCASTSY; }	       
<name>[Ss][Tt][Rr][Ii][Nn][Gg][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return STRINGCASTSY; }    
<name>[Dd][Oo][Uu][Bb][Ll][Ee][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return REALNUMBERCASTSY; }
<name,command_def>[\$_0-9a-zA-Z]+[ \t\r\n]*[(]	{ QECHO; return CALLSY; }
<name,command_def>[\$_0-9a-zA-Z]+  		{ QECHO; return IDENTSY; }

<psoda>[Ss][Ee][Tt]				{ QECHO; PUSH_NAME; return SETSY; }
<psoda>[Bb][Oo][Oo][Tt][Ss][Tt][Rr][Aa][Pp]	{ QECHO; PUSH_NAME; return BOOTSTRAPSY; }
    /*
<psoda>[Cc][Oo][Nn][Tt][Rr][Ee][Ee]		{ QECHO; PUSH_NAME; return CONTREESY; }
<psoda>[Pp][Ss][Cc][Oo][Rr][Ee][Ss]		{ QECHO; PUSH_NAME; return PSCORESSY;}
<psoda>[Ll][Ss][Cc][Oo][Rr][Ee][Ss]		{ QECHO; PUSH_NAME; return LSCORESSY; }
    */
 

    /*
<psoda>[Ee][Xx][Ee][Cc][Uu][Tt][Ee]   		{ QECHO; PUSH_EXPR; return EXECUTESY; }
<expressions>[Bb][Oo][Oo][Tt][Ss][Tt][Rr][Aa][Pp]	{ QECHO;            return EXPR_BOOTSTRAPSY; }
<expressions>[Cc][Oo][Nn][Tt][Rr][Ee][Ee]		{ QECHO;            return EXPR_CONTREESY; }
<expressions>[Pp][Ss][Cc][Oo][Rr][Ee][Ss]		{ QECHO;            return EXPR_PSCORESSY;}
<expressions>[Ll][Ss][Cc][Oo][Rr][Ee][Ss]		{ QECHO;            return EXPR_LSCORESSY; }
<expressions>[Ee][Xx][Ee][Cc][Uu][Tt][Ee]   		{ QECHO;            return EXPR_EXECUTESY; }
    */

<expressions>"++"[\$_0-9a-zA-Z]+    	{ QECHO; return PREINCRSY; }
<expressions>"--"[\$_0-9a-zA-Z]+	{ QECHO; return PREDECRSY; }
<psoda,expressions>"++"		   	{ QECHO; return UNARYINCRSY; }
<psoda,expressions>"--"		   	{ QECHO; return UNARYDECRSY; }
<expressions,command_def>[Tt][Rr][Uu][Ee]		{ QECHO; return TRUESY; }
<expressions,command_def>[Ff][Aa][Ll][Ss][Ee]   	{ QECHO; return FALSESY; }
<expressions>"&&"       		{ QECHO; return LOGICALANDSY; }
<expressions>"||"       	        { QECHO; return LOGICALORSY; }
<expressions>"!="       		{ QECHO; return NEQSY; }
<expressions>"=="       		{ QECHO; return DBLEQSY; }
<expressions>"<="       	        { QECHO; return LTEQSY; }
<expressions>">="       		{ QECHO; return GTEQSY; }
<expressions>"="        		{ QECHO; return EQSY; }
<expressions>"!"			{ QECHO; return LOGICALNOTSY; }
<expressions>"<"		        { QECHO; return LTSY; }
<expressions>">"        		{ QECHO; return GTSY; }
<expressions>"*"			{ QECHO; return MULSY; }
<expressions>"/"		        { QECHO; return DIVSY; }
<expressions>"%"		        { QECHO; return MODSY; }
<expressions>"."		        { QECHO; return CATSY; }
<expressions>"+"	          	{ QECHO; return ADDSY; }
<expressions>"-"			{ QECHO; return SUBSY; }
<psoda,expressions,command_def>"\""	{ QECHO; currentString = ""; yy_push_state(string_literal, yyscanner); }
<psoda,expressions>"("			{ QECHO; PUSH_EXPR; return LEFTPARENSY; }
<expressions>")"        		{ QECHO; yy_pop_state(yyscanner); return RIGHTPARENSY; }
<expressions>","			{ QECHO; return COMMASY; }
<expressions>";"             		{ QECHO; yy_pop_state(yyscanner); return SEMISY; }

<escaped_char>"n"			{ QECHO; currentString += "\n"; yy_pop_state(yyscanner); }
<escaped_char>"r"			{ QECHO; currentString += "\r"; yy_pop_state(yyscanner); }
<escaped_char>"t"			{ QECHO; currentString += "\t"; yy_pop_state(yyscanner); }
<escaped_char>[^nrt]			{ QECHO; currentString += string(yytext); yy_pop_state(yyscanner); }

<comment>[^]\n]*	 		{ QECHO; }
<comment>[]]		 		{ QECHO; yy_pop_state(yyscanner); }

<taxa>[Ee][Nn][Dd]           				{ QECHO; yy_pop_state(yyscanner); return ENDSY; }
<taxa>[Dd][Ii][Mm][Ee][Nn][Ss][Ii][Oo][Nn][Ss]  	{ QECHO; return DIMENSIONSSY; }
<taxa>[Tt][Aa][Xx][Ll][Aa][Bb][Ee][Ll][Ss]    		{ QECHO; return TAXLABELSSY; }
<taxa>[Nn][Tt][Aa][Xx]			   		{ QECHO; return NTAXSY; }
<taxa>"="				   		{ QECHO; return EQSY; }
<taxa>";"				   		{ QECHO; return SEMISY; }
<taxa>[0-9]+		        			{ QECHO; return NUMBERSY; }
<taxa>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; return NAMESY; }

<data>[Mm][Aa][Tt][Rr][Ii][Xx]        			{ QECHO; yy_push_state(matrix,yyscanner); QECHO; return MATRIXSY; }
<data>[Ss][Ee][Qq][Uu][Ee][Nn][Cc][Ee][Ss]  		{ QECHO; yy_push_state(sequences,yyscanner); return SEQUENCESSY; }

<refdata>[Mm][Aa][Tt][Rr][Ii][Xx]        		{ QECHO; yy_push_state(refmatrix,yyscanner); QECHO; return MATRIXSY; }
<refdata>[Ss][Ee][Qq][Uu][Ee][Nn][Cc][Ee][Ss]  		{ QECHO; yy_push_state(refsequences,yyscanner); return SEQUENCESSY; }

<ssdata>[Mm][Aa][Tt][Rr][Ii][Xx]        		{ QECHO; yy_push_state(ssmatrix,yyscanner); QECHO; return MATRIXSY; }
<ssdata>[Ss][Ee][Qq][Uu][Ee][Nn][Cc][Ee][Ss]  		{ QECHO; yy_push_state(sssequences,yyscanner); return SEQUENCESSY; }

<refssdata>[Mm][Aa][Tt][Rr][Ii][Xx]        		{ QECHO; yy_push_state(refssmatrix,yyscanner); QECHO; return MATRIXSY; }
<refssdata>[Ss][Ee][Qq][Uu][Ee][Nn][Cc][Ee][Ss]  	{ QECHO; yy_push_state(refsssequences,yyscanner); return SEQUENCESSY; }

<data,refdata,ssdata,refssdata>[Dd][Ii][Mm][Ee][Nn][Ss][Ii][Oo][Nn][Ss]  	{ QECHO; return DIMENSIONSSY; }
<data,refdata,ssdata,refssdata>[Tt][Aa][Xx][Ll][Aa][Bb][Ee][Ll][Ss]    		{ QECHO; return TAXLABELSSY; }
<data,refdata,ssdata,refssdata>[Nn][Ee][Ww][Tt][Aa][Xx][Aa]          		{ QECHO; return NEWTAXASY;  }
<data,refdata,ssdata,refssdata>[Nn][Tt][Aa][Xx]          			{ QECHO; return NTAXSY;  }
<data,refdata,ssdata,refssdata>[Nn][Cc][Hh][Aa][Rr][Ss]?       			{ QECHO; return NCHARSY;  }
<data,refdata,ssdata,refssdata>[Ff][Oo][Rr][Mm][Aa][Tt]        			{ QECHO; return FORMATSY; }
<data,refdata,ssdata,refssdata>[Gg][Aa][Pp]           				{ QECHO; return GAPSY; }
<data,refdata,ssdata,refssdata>[Ee][Qq][Uu][Aa][Tt][Ee] 				{ QECHO; return EQUATESY; }
<data,refdata,ssdata,refssdata>[Mm][Ii][Ss][Ss][Ii][Nn][Gg]       		{ QECHO; return MISSINGSY; }
<data,refdata,ssdata,refssdata>[Mm][Aa][Tt][Cc][Hh][Cc][Hh][Aa][Rr]    		{ QECHO; return MATCHCHARSY; }
<data,refdata,ssdata,refssdata>[Dd][Aa][Tt][Aa][Tt][Yy][Pp][Ee]      		{ QECHO; return DATATYPESY; }
<data,refdata,ssdata,refssdata>[Nn][Uu][Cc][Ll][Ee][Oo][Tt][Ii][Dd][Ee]		{ QECHO; return NUCLEOTIDESY; }
<data,refdata,ssdata,refssdata>[Ss][Tt][Aa][Nn][Dd][Aa][Rr][Dd]			{ QECHO; return STANDARDSY; }
<data,refdata,ssdata,refssdata>[Dd][Nn][Aa]					{ QECHO; return DNASY; }
<data,refdata,ssdata,refssdata>[Rr][Nn][Aa]					{ QECHO; return RNASY; }
<data,refdata,ssdata,refssdata>[Cc][Oo][Dd][Oo][Nn][Ss]?				{ QECHO; return CODONSY; }
<data,refdata,ssdata,refssdata>[Ee][Nn][Dd]					{ QECHO; yy_push_state(top,yyscanner); return ENDSY; }
<data,refdata,ssdata,refssdata>[Pp][Rr][Oo][Tt][Ee][Ii][Nn][Ss]?			{ QECHO; return PROTEINSY; } 
<data,refdata,ssdata,refssdata>[Ii][Nn][Tt][Ee][Rr][Ll][Ee][Aa][Vv][Ee]		{ QECHO; return INTERLEAVESY; }
<data,refdata,ssdata,refssdata>[Nn][Oo][Nn][Ii][Nn][Tt][Ee][Rr][Ll][Ee][Aa][Vv][Ee]		{ QECHO; return NONINTERLEAVESY; }  
<data,refdata,ssdata,refssdata>"="             					{ QECHO; return EQSY; }
<data,refdata,ssdata,refssdata>";"             					{ QECHO; return SEMISY; }
<data,refdata,ssdata,refssdata>[0-9]+          					{ QECHO; return NUMBERSY; }
<data,refdata,ssdata,refssdata>.               					{ QECHO; return CHAR; }
<data,refdata,ssdata,refssdata>[A-Za-z0-9\-_.\|\:\+]+					{ QECHO; return NAMESY; }

<matrix>[A-Za-z0-9\-_.()\/\|\:\+]+							{ QECHO; yy_push_state(sequence,yyscanner); /*printf("In Matrix name with %s\n", yytext);*/ return NAMESY; }
<matrix>";"             				{ QECHO; yy_push_state(data,yyscanner); return SEMISY; }

<sequence>[^ \r\t\n]+				{ QECHO; if ((++taxanum)%MATRIXFRAMESIZE==0) {yy_push_state(endsequencematrix,yyscanner);}else{yy_push_state(matrix,yyscanner);} return DNASEQSY; }
							
<endsequencematrix>[A-Za-z0-9\-_.\|\:\+]+			{ QECHO; yy_push_state(endsequence,yyscanner); return ENDNAMESY; }
<endsequencematrix>";"             			{ QECHO; yy_push_state(data,yyscanner); return SEMISY; }
<endsequencematrix>[^ \r\t\n]+		{ QECHO; yy_push_state(matrix,yyscanner); return ENDDNASEQSY; }
							
<endsequence>[^ \r\t\n]+			{ QECHO; yy_push_state(matrix,yyscanner); return ENDDNASEQSY; }
							
<sequences>">"						{ QECHO; yy_push_state(sequences2,yyscanner); return SEQUENCE_STARTSY; }
<sequences2>[A-Za-z0-9\-_.()\/\|\:\+]+	{ QECHO; yy_push_state(sequences3,yyscanner); return NAMESY; }
<sequences3>[^\n]*					{ QECHO; }
<sequences4>[^;\>]+			{ QECHO; return SEQDATASY; }
<sequences4>">"						{ QECHO; yy_pop_state(yyscanner); yy_push_state(sequences2,yyscanner); return SEQUENCE_STARTSY; }
<sequences4>;						{ QECHO; yy_pop_state(yyscanner); yy_push_state(data,yyscanner); return SEMISY; }



<refmatrix>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; yy_push_state(refsequence,yyscanner); return NAMESY; }
<refmatrix>";"             				{ QECHO; yy_push_state(refdata,yyscanner); return SEMISY; }

<refsequence>[^ \r\t\n]+			{ QECHO; if ((++reftaxanum)%MATRIXFRAMESIZE==0) {yy_push_state(refendsequencematrix,yyscanner);}else{yy_push_state(refmatrix,yyscanner);} return DNASEQSY; }

<refendsequencematrix>[A-Za-z0-9\-_.\|\:\+]+			{ QECHO; yy_push_state(refendsequence,yyscanner); return ENDNAMESY; }
<refendsequencematrix>";"             		{ QECHO; yy_push_state(refdata,yyscanner); return SEMISY; }
<refendsequencematrix>[^ \r\t\n]+	{ QECHO; yy_push_state(refmatrix,yyscanner); return ENDDNASEQSY; }

<refendsequence>[^ \r\t\n]+		{ QECHO; yy_push_state(refmatrix,yyscanner); return ENDDNASEQSY; }

<refsequences>">"					{ QECHO; yy_push_state(refsequences2,yyscanner); return SEQUENCE_STARTSY; }
<refsequences2>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; yy_push_state(refsequences3,yyscanner); return NAMESY; }
<refsequences3>[^\n]*				{ QECHO; }
<refsequences4>[^;\>]+			{ QECHO; return SEQDATASY; }
<refsequences4>">"					{ QECHO; yy_pop_state(yyscanner); yy_push_state(refsequences2,yyscanner); return SEQUENCE_STARTSY; }
<refsequences4>;					{ QECHO; yy_pop_state(yyscanner); yy_push_state(refdata,yyscanner); return SEMISY; }



<ssmatrix>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; yy_push_state(sssequence,yyscanner); return NAMESY; }
<ssmatrix>";"             				{ QECHO; yy_push_state(ssdata,yyscanner); return SEMISY; }

<sssequence>[^ \r\t\n]+			{ QECHO; if ((++sstaxanum)%MATRIXFRAMESIZE==0) {yy_push_state(ssendsequencematrix,yyscanner);}else{yy_push_state(ssmatrix,yyscanner);} return DNASEQSY; }

<ssendsequencematrix>[A-Za-z0-9\-_.\|\:\+]+			{ QECHO; yy_push_state(ssendsequence,yyscanner); return ENDNAMESY; }
<ssendsequencematrix>";"             		{ QECHO; yy_push_state(ssdata,yyscanner); return SEMISY; }
<ssendsequencematrix>[^ \r\t\n]+	{ QECHO; yy_push_state(ssmatrix,yyscanner); return ENDDNASEQSY; }

<ssendsequence>[^ \r\t\n]+		{ QECHO; yy_push_state(ssmatrix,yyscanner); return ENDDNASEQSY; }

<sssequences>">"					{ QECHO; yy_push_state(sssequences2,yyscanner); return SEQUENCE_STARTSY; }
<sssequences2>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; yy_push_state(sssequences3,yyscanner); return NAMESY; }
<sssequences3>[^\n]*				{ QECHO; }
<sssequences4>[^;\>]+			{ QECHO; return SEQDATASY; }
<sssequences4>">"					{ QECHO; yy_pop_state(yyscanner); yy_push_state(sssequences2,yyscanner); return SEQUENCE_STARTSY; }
<sssequences4>;					{ QECHO; yy_pop_state(yyscanner); yy_push_state(ssdata,yyscanner); return SEMISY; }



<refssmatrix>[A-Za-z0-9\-_.\|\:\+]+				{ QECHO; yy_push_state(refsssequence,yyscanner); return NAMESY; }
<refssmatrix>";"             				{ QECHO; yy_push_state(refssdata,yyscanner); return SEMISY; }

<refsssequence>[^ \r\t\n]+			{ QECHO; if ((++refsstaxanum)%MATRIXFRAMESIZE==0) {yy_push_state(refssendsequencematrix,yyscanner);}else{yy_push_state(refssmatrix,yyscanner);} return DNASEQSY; }

<refssendsequencematrix>[A-Za-z0-9\-_.\|\:\+]+		{ QECHO; yy_push_state(refssendsequence,yyscanner); return ENDNAMESY; }
<refssendsequencematrix>";"             		{ QECHO; yy_push_state(refssdata,yyscanner); return SEMISY; }
<refssendsequencematrix>[^ \r\t\n]+	{ QECHO; yy_push_state(refssmatrix,yyscanner); return ENDDNASEQSY; }

<refssendsequence>[^ \r\t\n]+		{ QECHO; yy_push_state(refssmatrix,yyscanner); return ENDDNASEQSY; }

<refsssequences>">"					{ QECHO; yy_push_state(refsssequences2,yyscanner); return SEQUENCE_STARTSY; }
<refsssequences2>[A-Za-z0-9\-_.\|\:\+]+			{ QECHO; yy_push_state(refsssequences3,yyscanner); return NAMESY; }
<refsssequences3>[^\n]*				{ QECHO; }
<refsssequences4>[^;\>]+			{ QECHO; return SEQDATASY; }
<refsssequences4>">"					{ QECHO; yy_pop_state(yyscanner); yy_push_state(refsssequences2,yyscanner); return SEQUENCE_STARTSY; }
<refsssequences4>;					{ QECHO; yy_pop_state(yyscanner); yy_push_state(refssdata,yyscanner); return SEMISY; }

<weights>";"             		{ QECHO; yy_pop_state(yyscanner); return SEMISY; }
<weights>[0-9]+          		{ QECHO; return NUMBERSY; }
<weights>"-"				{ QECHO; return MINUSSY;}
<weights>","				{ QECHO; return COMMASY;}
<weights>":"				{ QECHO; yy_push_state(weights_list,yyscanner); return COLONSY;}
<weights>"\\" 				{ QECHO; return BSLASHSY;}
<weights>"." 				{ QECHO; return PERIODSY;}
<weights>[Aa][Ll][Ll]			{ QECHO; return ALLSY;}
<weights>[Aa][Ll]			{ QECHO; return ALLSY;}
<weights>[Aa]				{ QECHO; return ALLSY;}
<weights>[Rr][Ee][Ss][Ee][Tt]		{ QECHO; return RESETSY;}
<weights>[Rr][Ee][Ss][Ee]		{ QECHO; return RESETSY;}
<weights>[Rr][Ee][Ss]			{ QECHO; return RESETSY;}
<weights>[Rr][Ee]			{ QECHO; return RESETSY;}
<weights>[Rr]				{ QECHO; return RESETSY;}
<weights>"("         			{ QECHO; yy_push_state(expressions,yyscanner); return LEFTPARENSY; }

<weights_list>[0-9]+          		{ QECHO; return NUMBERSY; }
<weights_list>"-"			{ QECHO; return MINUSSY;}
<weights_list>","			{ QECHO; yy_pop_state(yyscanner); return COMMASY;}
<weights_list>";"             		{ QECHO; yy_pop_state(yyscanner); yy_pop_state(yyscanner); return SEMISY; }
<weights_list>"\\" 			{ QECHO; return BSLASHSY;}
<weights_list>"." 			{ QECHO; return PERIODSY;}
<weights_list>[Aa][Ll][Ll]		{ QECHO; yy_pop_state(yyscanner); return ALLSY;}
<weights_list>[Aa][Ll]			{ QECHO; yy_pop_state(yyscanner); return ALLSY;}
<weights_list>"("         		{ QECHO; yy_push_state(expressions, yyscanner); return LEFTPARENSY; }

<psoda>[Ii][Nn][Tt][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return INTCASTSY; }	       
<psoda>[Bb][Oo][Oo][Ll][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return BOOLCASTSY; }	       
<psoda>[Ss][Tt][Rr][Ii][Nn][Gg][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return STRINGCASTSY; }    
<psoda>[Dd][Oo][Uu][Bb][Ll][Ee][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return REALNUMBERCASTSY; }
<psoda>[\$_0-9a-zA-Z]+  			{ QECHO; PUSH_EXPR; return TOP_IDENTSY; }
<expressions,weights,weights_list>[Ii][Nn][Tt][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return INTCASTSY; }	       
<expressions,weights,weights_list>[Bb][Oo][Oo][Ll][ \t\r\n]*[(]		{ QECHO; PUSH_EXPR; return BOOLCASTSY; }	       
<expressions,weights,weights_list>[Ss][Tt][Rr][Ii][Nn][Gg][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return STRINGCASTSY; }    
<expressions,weights,weights_list>[Dd][Oo][Uu][Bb][Ll][Ee][ \t\r\n]*[(]	{ QECHO; PUSH_EXPR; return REALNUMBERCASTSY; }
<expressions,weights,weights_list>[Vv][Aa][Rr][ \t\r\n]*[(] 			{ QECHO; yy_push_state(varcast,yyscanner); return VARCASTSY; }
<expressions,weights,weights_list>[\$_0-9a-zA-Z]+[ \t\r\n]*[(]			{ QECHO; PUSH_EXPR; return CALLSY; }
<expressions,weights,weights_list>[\$_0-9a-zA-Z]+ 				{ QECHO; return IDENTSY; }
<expressions>[A-Za-z0-9\/_][A-Za-z0-9\/_\-]*	        			{ QECHO; return TEXT; }
<expressions>[A-Za-z0-9\/_][A-Za-z0-9\.\/_\-]*[A-Za-z0-9\/_\-]+	        	{ QECHO; return TEXT; }

<varcast>[)]  				{ QECHO; yy_pop_state(yyscanner); return RIGHTPARENSY; }
<varcast>[\$_0-9a-zA-Z]+  		{ QECHO; return IDENTSY; }
%%
