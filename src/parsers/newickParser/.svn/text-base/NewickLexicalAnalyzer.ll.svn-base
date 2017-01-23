%{
#include "NewickTreeParser.h"
#include "NewickParser.hpp"
extern int yylval;
extern int newick_linenum;
extern void* newick_yyscanner;
//%option bison-bridge
//%option bison-locations
//%option header-file="FILE"
#define YY_NEVER_INTERACTIVE 1
union YYSTYPE;
#define YY_DECL int yylex(YYSTYPE *yylval_param, void *yyscanner)
#define NP ((NewickTreeParser* ) newickget_extra( yyscanner ) )
#define LINE_NO newickget_lineno(newick_yyscanner)
//#define QECHO ECHO
#define QECHO 
int comment_caller;
%}

%option outfile="NewickLexicalAnalyzer.cpp"
%option reentrant 
%option yylineno
%option noyywrap
%option prefix="newick"
%option nounistd

%x top
%x comment
%x trees
%x translate
                         
%%
"\n"                        { QECHO; newick_linenum = yylineno; }
[ \r\t]+                    { QECHO; }
"#"[Nn][Ee][Xx][Uu][Ss]     { QECHO; BEGIN(top); return NEXUSSY; }

"("                   	{ QECHO; return LEFTPARENSY; }
")"                   	{ QECHO; return RIGHTPARENSY; }
","                   	{ QECHO; return COMMASY; }
":"                   	{ QECHO; return COLONSY; }
";"                   	{ QECHO; return SEMISY; }
[\.][0-9]+               { QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
0[\.][0-9]*   	{ QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
[1-9][0-9]*[\.][0-9]*   	{ QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
[1-9][0-9]*             	{ QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY ; }
-[1-9][0-9]*             	{ QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY ; }
[A-Za-z0-9\-_.\/\|\+]+  { QECHO; yylval_param->stringval = yytext; return IDENTSY; }

<top>[Bb][Ee][Gg][Ii][Nn]   { QECHO; return BEGINSY; }
<top>[Tt][Rr][Ee][Ee][Ss]   { QECHO; BEGIN(trees); return TREESSY; }
<top>";"                    { QECHO; return SEMISY; }
<top>\n                     { QECHO; newick_linenum = yylineno; }
<top>[ \r\t]+               { QECHO; }

<*>[[]                   { QECHO; comment_caller = YY_START; BEGIN(comment); }
<comment>[^]\n]*         { QECHO; }
<comment>\n              { QECHO; newick_linenum = yylineno + 1; }
<comment>[]]             { QECHO; BEGIN(comment_caller); }

<trees>[Tt][Rr][Aa][Nn][Ss][Ll][Aa][Tt][Ee] { QECHO; BEGIN(translate); return TRANSLATESY; }
<translate>[1-9][0-9]*                      { QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY; }
<translate>-[1-9][0-9]*                      { QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY; }
<translate>[A-Za-z0-9\-_.\/\|\+]+      { QECHO; yylval_param->stringval = yytext; return NAMESY; }
<translate>";"                              { QECHO; BEGIN(trees);  return SEMISY; }
<translate>\n                               { QECHO; newick_linenum = yylineno; }
<translate>[ \r\t]+                         { QECHO; }
<translate>","                              { QECHO; return COMMASY; }

<trees>"("                   	{ QECHO; return LEFTPARENSY; }
<trees>")"                   	{ QECHO; return RIGHTPARENSY; }
<trees>","                   	{ QECHO; return COMMASY; }
<trees>[Tt][Rr][Ee][Ee]      	{ QECHO; return TREESY; }
<trees>"="                   	{ QECHO; return EQUALSY; }
trees>"[&U]"                	{ QECHO; return FUNKYSY; /* Unrooted trees */} 
<trees>"[&R]"                	{ QECHO; return FUNKYSY; /* Rooted trees   */}
<trees>":"                   	{ QECHO; return COLONSY; }
<trees>[\.][0-9]+               { QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
<trees>0[\.][0-9]+               { QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
<trees>[1-9][0-9]*[\.][0-9]*   	{ QECHO; yylval_param->doubleval = atof(yytext); return REALNUMBERSY ; }
<trees>[1-9][0-9]*             	{ QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY ; }
<trees>-[1-9][0-9]*             	{ QECHO; yylval_param->intval = atoi(yytext); return NUMBERSY ; }
<trees>";"                   	{ QECHO; return SEMISY; }
<trees>\n                    	{ QECHO; newick_linenum = yylineno; }
<trees>[ \r\t]+              	{ QECHO; }
<trees>[Ee][Nn][Dd]          	{ QECHO; BEGIN(top); return ENDSY; }
<trees>[A-Za-z0-9\-_.\/\|\+]+    { QECHO; yylval_param->stringval = yytext; return IDENTSY; }

%%
