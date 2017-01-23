%require "2.2"
%{
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PaupParser.h"
#include "IntCastOp.h"
#include "BoolCastOp.h"
#include "StringCastOp.h"
#include "RealNumberCastOp.h"
#include "AdditionOp.h"
#include "Block.h"
#include "BoolLiteral.h"
#include "RealNumberLiteral.h"
#include "BootstrapInstr.h"
#include "BreakInstr.h"
#include "CallExpression.h"
#include "PsodaConstruct.h"
#include "EmptyInstr.h"
#include "IncrementInstr.h"
#include "UpdateInstr.h"
#include "SetInstr.h"
#include "DecrementInstr.h"
#include "CatOp.h"
#include "ConTreeInstr.h"
#include "DataBlock.h"
#include "Dataset.h"
#include "Data.h"
#include "DivisionOp.h"
#include "EqualityOp.h"
#include "ExecuteInstr.h"
#include "Expression.h"
#include "GreaterThanOp.h"
#include "GreaterThanOrEqualOp.h"
#include "IdentExpression.h"
#include "IfElse.h"
#include "InequalityOp.h"
#include "Interpreter.h"
#include "IntLiteral.h"
#include "LScoresInstr.h"
#include "LessThanOp.h"
#include "LessThanOrEqualOp.h"
#include "LiteralExpression.h"
#include "LogicalAndOp.h"
#include "LogicalNotOp.h"
#include "LogicalOrOp.h"
#include "ModOp.h"
#include "MultiplicationOp.h"
#include "NameExpression.h"
#include "NegationOp.h"
#include "NewickTreeParser.h"
#include "OperationExpression.h"
#include "PScoresInstr.h"
#include "ParenthesesOp.h"
#include "PostDecrOp.h"
#include "PostIncrOp.h"
#include "PreDecrOp.h"
#include "PreIncrOp.h"
#include "ProgramBlock.h"
#include "PsodaBlocks.h"
#include "PsodaError.h"
#include "PsodaPrinter.h"
#include "PsodaProgram.h"
#include "ReturnInstr.h"
#include "SSDataset.h"
#include "StringLiteral.h"
#include "SubtractionOp.h"
#include "TreeBlock.h"
#include "UndefinedLiteral.h"
#include "UserDefinedCommand.h"
#include "InlineUserDefinedCommand.h"
#include "WeightsInstr.h"
#include "WeightsData.h"
#include "WeightsResetInstr.h"
#include "WhileLoop.h"
#include "ParsingInfo.h"
#include "FileTools.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <string>
#include <vector>
  

#define YYMAXDEPTH 20000
#define YYERROR_VERBOSE
#define YYPARSE_PARAM parsingInfo
#define P_INFO ((ParsingInfo*)YYPARSE_PARAM)
#define YYLEX_PARAM P_INFO->getScanner()
#define LINE_NO paupLexget_lineno(P_INFO->getScanner())
#define LOCATION(x,y) Location(P_INFO->getPathToFile(), x.first_line, x.first_column, y.last_line, y.last_column)

#define YYLLOC_DEFAULT(Current, Rhs, N)					\
  do {									\
    if (N) {								\
      (Current).first_line = YYRHSLOC(Rhs, 1).first_line;		\
      (Current).first_column = YYRHSLOC(Rhs, 1).first_column;		\
      (Current).last_line = YYRHSLOC(Rhs, N).last_line;			\
      (Current).last_column = YYRHSLOC(Rhs, N).last_column;		\
    } else {								\
      (Current).first_line = (Current).last_line =			\
	YYRHSLOC(Rhs, 0).last_line;					\
      (Current).first_column = (Current).last_column =			\
	YYRHSLOC(Rhs, 0).last_column;					\
    }									\
    P_INFO->setLocation(						\
			Location(					\
				 P_INFO->getPathToFile(),		\
				 (Current).first_line,			\
				 (Current).first_column,		\
				 (Current).last_line,			\
				 (Current).last_column			\
				 )					\
			);						\
    if (P_INFO->getCommandStackSize() > 0) {				\
      P_INFO->getCurrentCommand()->setLocation(P_INFO->getLocation());	\
    }									\
  } while (0)  
  
  using namespace std;

  union YYSTYPE;

  int paupLexlex(YYSTYPE *yylval_param, YYLTYPE* yylloc_param, void *yyscanner);
  char *paupLexget_text(void *yyscanner);
  int paupLexget_lineno(void *yyscanner);

#ifdef __cplusplus
  extern "C"
  {
#endif
    int paupLexwrap() { return 1; }
#ifdef __cplusplus
  }
#endif

  int yyerror(YYLTYPE* locp, void* YYPARSE_PARAM, const char* errorMessage) {
    if (P_INFO->getLocation().getLastLine() == paupLexget_lineno(P_INFO->getScanner())) {
      throw PsodaError(errorMessage, P_INFO->getLocation());
    } else {
      throw PsodaError(errorMessage, Location(P_INFO->getPathToFile(), paupLexget_lineno(P_INFO->getScanner())));
    }
  }

  char *stripWhiteSpace(char *s)
  {
      int i, j = 0;
      for (i = 0; s[i] != '\0'; i++)
      {
          switch (s[i])
          {
              case ' ':
              case '\n':
              case '\r':
              case '\t':
                  break;
              default:
                  s[j++] = s[i];
          }
      }
      s[j] = '\0';
      return s;
  }
              

%}

%name-prefix="paupLex"
%locations
%pure_parser                      
%expect 0
%union 
{
  int intval;
  void* expressionval;
  void* literalval;
  void* operatorval;
  double doubleval;
  const char* stringval;
};
%verbose
%parse-param {void* YYPARSE_PARAM}
%lex-param {void* YYLEX_PARAM}
%start NEXUS

%left LOGICALORSY
%left LOGICALANDSY
%left NEQSY DBLEQSY
%left LTEQSY GTEQSY LTSY GTSY
%left CATSY
%left ADDSY SUBSY
%left MULSY DIVSY MODSY
%left LOGICALNOTSY
%left PREINCRSY PREDECRSY
%left UNARYINCRSY UNARYDECRSY

%token <intval> TAXONNUMBERSY
%token <intval> NEWICKNUMBERSY
%token <doubleval> REALNUMBERSY
%token <stringval> IDENTSY
%token <stringval> NEWICKNAMESY
%token <stringval> NUMBERSY
%token <stringval> STRINGSY
%token <stringval> TEXT
%token <stringval> TOP_IDENTSY
%token <stringval> SEQDATASY
%token <stringval> TREEBLOCKSTRING
%token STARSY
%token CALLSY
%token INTCASTSY
%token BOOLCASTSY
%token STRINGCASTSY
%token REALNUMBERCASTSY
%token REFERENCESY
%token SEQUENCE_STARTSY
%token NAMESY
%token ALLSY
%token ASSUMPTIONSSY
%token BACKUPREPOSITORYSY
%token BANDBSY
%token BEGINSY
%token BOOTSTRAPSY
%token BREAKSY
%token BSLASHSY
%token CHAR
%token CHARACTERSSY
%token CODONSY
%token COLONSY
%token COMBINEREPOSITORIESSY
%token RANDOMMORPHSY
%token COMMASY
/*
%token CONTREESY
*/
%token DATASY
%token DATATYPESY
%token DECREMENTSY
%token DEFAULTSY
%token DIMENSIONSSY
%token DISTANCESSY
%token DNASEQSY
%token DNASY
%token DSETSY
%token PRINTSY
%token HELPSY
%token ELSESY
%token ELSIFSY
%token ENDDNASEQSY
%token ENDIFSY
%token ENDLINESY
%token ENDNAMESY
%token ENDSY
%token ENDWHILESY
%token EQSY
%token EQUALSY
 //%token EXECUTESY
%token FALSESY
%token FORMATSY
%token FUNKYSY
%token GAPSY
%token EQUATESY
%token GETBESTSCORESY
%token GETNUMCHARSSY
%token GETTREESSY
%token GETWEIGHTSLENGTHSY
%token GRAPHSY
%token HSEARCHSY
%token ALIGNSY
%token IFSY
%token INCREMENTSY
%token INTERLEAVESY
%token LEFTCURLYSY
%token LEFTPARENSY
%token LOGSY
/*
%token LSCORESSY
*/
%token LSETSY
%token MATCHCHARSY
%token MATRIXSY
%token MINUSSY
%token MISSINGSY
%token NCHARSY
%token NEWTAXASY
%token NEXUSSY
%token NJINSTRSY
%token NTAXSY
%token NUCLEOTIDESY
%token PAUPSY
%token PERIODSY
%token PRINTMATRIXSY
%token PROTEINSY
/*
%token PSCORESSY
*/
%token PSODASY
%token QUITSY
%token RANDOMSY
%token REFDATASY
%token RESETSY
%token RETURNSY
%token RIGHTPARENSY
%token RIGHTCURLYSY
%token RNASY
%token RSEARCHSY // Ratchet Search
%token SAVETREESSY
%token SEMISY
%token SEQUENCESSY // A List of Sequences that may not all have the same length
%token SETSSY
%token SETSY
%token SLASHSY
%token SRANDOMSY
%token SSDATASY
%token REFSSDATASY
%token STANDARDSY
%token TAXASY
%token TAXLABELSSY
%token TIMESY
%token TRANSLATESY
%token TREEINFOSY
%token TREENAMESY
%token TREESSY
%token TREESY
%token TRUESY
%token VARSY
%token VARCASTSY
%token WEIGHTSSY
%token WHILESY
%token NONINTERLEAVESY

%%

NEXUS:
NEXUSSY blocks 
| { P_INFO->pushBlock(new DataBlock()); P_INFO->getCurrentDataset().datatype() = Dataset::DNA_DATATYPE; } sequences_entry_list
;

blocks:
block blocks
|
;

block:
BEGINSY block_declaration ENDSY SEMISY
|BEGINSY block_tree SEMISY
;

block_declaration:
block_characters
|block_data
|block_assumptions
|block_sets
|block_distances
|block_psoda
|block_refdata
|block_ssdata
|block_refssdata
;

block_taxa:
TAXASY SEMISY { P_INFO->pushBlock(new DataBlock()); } taxa_body
;

block_data:
DATASY SEMISY { P_INFO->pushBlock(new DataBlock()); } data_block_header block_data_data
;

block_ssdata:
SSDATASY SEMISY { P_INFO->pushBlock( new DataBlock(SS_BLOCK)); P_INFO->getCurrentDataset().datatype()= Dataset::SECONDARY_STRUCTURE_DATATYPE; } data_block_header block_data_data
;

block_refssdata:
REFSSDATASY SEMISY { P_INFO->pushBlock( new DataBlock(REF_SS_BLOCK)); P_INFO->getCurrentDataset().datatype() = Dataset::SECONDARY_STRUCTURE_DATATYPE; } data_block_header block_data_data
;

block_refdata:
REFDATASY SEMISY { P_INFO->pushBlock( new DataBlock(REF_BLOCK)); } data_block_header block_data_data
;

block_characters:
block_taxa ENDSY SEMISY BEGINSY CHARACTERSSY SEMISY characters_proper
|CHARACTERSSY SEMISY { P_INFO->pushBlock(new DataBlock()); } characters_proper
;

characters_proper:
character_block_header block_data_data
;

taxa_body:
taxa_item taxa_body
|
;

taxa_item:
DIMENSIONSSY dimensions SEMISY
|taxalabels
;

taxalabels:
TAXLABELSSY taxa_names SEMISY
;

taxa_names:
taxa_names taxon_name
|
;

taxon_name:
NAMESY
;

data_block_header:
header_items
;

header_items:
header_item header_items
|
;

header_item:
 DIMENSIONSSY  dimensions  SEMISY 
| FORMATSY  format_list  SEMISY 
| taxalabels  
;

character_block_header:
header_items
;

block_data_data:
matrix
|sequences
;

dimensions:
dimension dimensions
|
;

dimension:
ntax
|nchar
;

ntax:
 numtax  EQSY  NUMBERSY  { P_INFO->getCurrentDataset().setntaxa(atoi(paupLexget_text(P_INFO->getScanner()))); }
;

numtax:
 NEWTAXASY  NTAXSY 
|  NTAXSY 
;

nchar:
 NCHARSY  EQSY  NUMBERSY  { P_INFO->getCurrentDataset().setnchars(atoi(paupLexget_text(P_INFO->getScanner()))); }
;

format_list:
format_item format_list
|
;

format_item:
DATATYPESY EQSY format_item_datatype
| GAPSY EQSY CHAR       { P_INFO->getCurrentDataset().gapchar() =     (paupLexget_text(P_INFO->getScanner()))[0]; } 
| MISSINGSY EQSY CHAR   { P_INFO->getCurrentDataset().missingchar() = (paupLexget_text(P_INFO->getScanner()))[0]; } 
| MATCHCHARSY EQSY CHAR { P_INFO->getCurrentDataset().matchchar() =   (paupLexget_text(P_INFO->getScanner()))[0]; }
| INTERLEAVESY          
| NONINTERLEAVESY		
| EQUATESY EQSY chars
| NAMESY EQSY chars
;

chars:
char chars
|char
;

char:
EQSY
|CHAR
;

format_item_datatype:
NUCLEOTIDESY  { P_INFO->getCurrentDataset().datatype() = Dataset::NUCLEOTIDE_DATATYPE; }
|DNASY        { P_INFO->getCurrentDataset().datatype() = Dataset::DNA_DATATYPE; }
|STANDARDSY   { P_INFO->getCurrentDataset().datatype() = Dataset::STANDARD_DATATYPE; }
|RNASY        { P_INFO->getCurrentDataset().datatype() = Dataset::RNA_DATATYPE; }
|PROTEINSY    { P_INFO->getCurrentDataset().datatype() = Dataset::PROTEIN_DATATYPE; }
|CODONSY      { P_INFO->getCurrentDataset().datatype() = Dataset::PROTEIN_CODING_DNA_DATATYPE; }
;

matrix:
MATRIXSY { P_INFO->getCurrentDataset().dataformat() = Dataset::ALIGNED_DATAFORMAT; } matrixpiecelist
;

matrixpiecelist:
matrixpiece matrixpiecelist
|
;

matrixpiece:
sequencelist endseq
;

sequencelist:
sequence sequencelist
|
;

sequence:
NAMESY {
  P_INFO->getCurrentDataset().addName( (paupLexget_text(P_INFO->getScanner())) );
  //fprintf(stderr,"Read taxon [%s]\n", paupLexget_text(P_INFO->getScanner()));
} DNASEQSY {
  P_INFO->checkCharLength(paupLexget_text(P_INFO->getScanner()), paupLexget_lineno(P_INFO->getScanner()));
  P_INFO->getCurrentDataset().addCharacters( (paupLexget_text(P_INFO->getScanner())) );
  //fprintf(stderr,"Read dna sequence [%s]\n", paupLexget_text(P_INFO->getScanner()));
}
;


endseq:
ENDNAMESY {
  P_INFO->getCurrentDataset().addName( (paupLexget_text(P_INFO->getScanner())) );
} ENDDNASEQSY {
  P_INFO->checkCharLength(paupLexget_text(P_INFO->getScanner()), paupLexget_lineno(P_INFO->getScanner()));
  P_INFO->getCurrentDataset().addCharacters( (paupLexget_text(P_INFO->getScanner())) );
}
|SEMISY
;

sequences:
SEQUENCESSY { P_INFO->getCurrentDataset().dataformat() = Dataset::UNALIGNED_DATAFORMAT; } sequences_entry_list SEMISY
;

sequences_entry_list:
sequences_entry sequences_entry_list
|
;

sequences_entry:
SEQUENCE_STARTSY NAMESY { P_INFO->getCurrentDataset().addName( (paupLexget_text(P_INFO->getScanner())) ); } sequences_data
;

sequences_data:
SEQDATASY { P_INFO->getCurrentDataset().addCharacters( stripWhiteSpace(paupLexget_text(P_INFO->getScanner())) ); } sequences_data
|
;


block_psoda:
psoda_sym SEMISY { P_INFO->pushBlock(new ProgramBlock()); } construct_body
;

psoda_sym:
PSODASY
|PAUPSY
;

//Adding constructs
construct_body:
psoda_elements
;

psoda_elements:
psoda_element psoda_elements
|
;

psoda_element:
psoda_statement { SET_LOCATION(LOCATION(@1,@1),P_INFO->getCurrentCommand()); P_INFO->addCommand(P_INFO->getCurrentCommand()); P_INFO->popCommand(); P_INFO->pushCommand(new CommandNode()); }
|set_command { P_INFO->freeSetCommand(); }
|construct
;

construct:
if_construct 		{ P_INFO->endConstruct(); SET_LOCATION(LOCATION(@1,@1),P_INFO->getCurrentConstruct()); P_INFO->popConstruct(); }
|while_construct	{ P_INFO->endConstruct(); SET_LOCATION(LOCATION(@1,@1),P_INFO->getCurrentConstruct()); P_INFO->popConstruct(); }
|function
;

function:
BEGINSY top_ident {
  UserDefinedCommand* newUserDefinedCommand = new UserDefinedCommand();
  newUserDefinedCommand->setName($<stringval>2);
  P_INFO->setCurrentUserDefinedCommand(newUserDefinedCommand);
} free_ident defaults SEMISY construct_body ENDSY SEMISY {
  try {
    P_INFO->addCurrentUserDefinedCommand();
  } catch (PsodaError e) {
    e.setLocation(LOCATION(@2,@2));
    throw e;
  }
  P_INFO->setCurrentUserDefinedCommand(NULL);
} 
|BEGINSY STARSY top_ident {
  InlineUserDefinedCommand* newUserDefinedCommand = new InlineUserDefinedCommand();
  newUserDefinedCommand->setName($<stringval>3);
  P_INFO->setCurrentUserDefinedCommand(newUserDefinedCommand);
} free_ident defaults SEMISY construct_body ENDSY SEMISY {
  try {
    P_INFO->addCurrentUserDefinedCommand();
  } catch (PsodaError e) {
    e.setLocation(LOCATION(@2,@2));
    throw e;
  }
  P_INFO->setCurrentUserDefinedCommand(NULL);
} 
;

defaults:
no_comma_defaults
|comma_defaults
|LEFTPARENSY comma_defaults RIGHTPARENSY
|LEFTPARENSY no_comma_defaults RIGHTPARENSY
|LEFTPARENSY RIGHTPARENSY
|
;

no_comma_defaults:
param_default no_comma_defaults
|param_default
;

comma_defaults:
param_default comma_defaults_tail
;

comma_defaults_tail:
COMMASY param_default comma_defaults_tail
|COMMASY param_default
;

param_default:
ident EQSY literal_value {
  Literal* myLitPtr = (Literal*) $<literalval>3;
  P_INFO->getCurrentUserDefinedCommand()->initDefaultValue($<stringval>1, myLitPtr);
  delete myLitPtr;
  myLitPtr = NULL;
} free_ident
|ident REFERENCESY {
//  printf("Reference found: %s\n", $<stringval>1);
  P_INFO->getCurrentUserDefinedCommand()->declareReference($<stringval>1);
  P_INFO->getCurrentUserDefinedCommand()->initDefaultValue($<stringval>1, UndefinedLiteral::getInstance());
} free_ident
|ident REFERENCESY EQSY literal_value {
//  printf("Reference found: %s\n", $<stringval>1);
  P_INFO->getCurrentUserDefinedCommand()->declareReference($<stringval>1);
  Literal* myLitPtr = (Literal*) $<literalval>4;
  P_INFO->getCurrentUserDefinedCommand()->initDefaultValue($<stringval>1, myLitPtr);
  delete myLitPtr;
  myLitPtr = NULL;
} free_ident
;

literal_value:
NUMBERSY        { $<literalval>$ = new IntLiteral(atoi(paupLexget_text(P_INFO->getScanner()))); }
|REALNUMBERSY   { $<literalval>$ = new RealNumberLiteral($<doubleval>1); }
|STRINGSY       { $<literalval>$ = new StringLiteral(paupLexget_text(P_INFO->getScanner())); }
|TEXT           { $<literalval>$ = new StringLiteral(paupLexget_text(P_INFO->getScanner())); }
|TRUESY		{ $<literalval>$ = new BoolLiteral(true); }
|FALSESY        { $<literalval>$ = new BoolLiteral(false); }
;

while_construct:
WHILESY { P_INFO->pushConstruct(new WhileLoop()); P_INFO->beginConstruct(P_INFO->getCurrentConstruct()); } condition {
  P_INFO->getCurrentWhileLoop()->setCondition((Expression*)$<expressionval>3);
}
construct_body ENDWHILESY SEMISY
;

if_construct:
IFSY { P_INFO->pushConstruct(new IfElse()); P_INFO->beginConstruct(P_INFO->getCurrentConstruct()); } condition {
  P_INFO->getCurrentIfElse()->setCondition((Expression*)$<expressionval>3);
}
construct_body if_tail
;

if_tail:
ELSIFSY	 { P_INFO->getCurrentIfElse()->beginElsif(); } condition { 
  P_INFO->getCurrentIfElse()->setCondition((Expression*)$<expressionval>3);
} construct_body if_tail
|ELSESY	 { P_INFO->getCurrentIfElse()->beginElse(); } construct_body if_tail
|ENDIFSY SEMISY
;

condition:
LEFTPARENSY expression RIGHTPARENSY 		{ $<expressionval>$ = $<expressionval>2; }
;

expression:
or_expr                   			{ $<expressionval>$ = $<expressionval>1; }
;

or_expr:
or_expr or_operator and_expr 			{ $<expressionval>$ = P_INFO->buildOperationExpression($<operatorval>2, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|and_expr                                 	{ $<expressionval>$ = $<expressionval>1; }
;

and_expr:
and_expr and_operator rel_expr 			{ $<expressionval>$ = P_INFO->buildOperationExpression($<operatorval>2, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|rel_expr                                 	{ $<expressionval>$ = $<expressionval>1; }
;

rel_expr:
rel_expr rel_operator cat_expr 			{ $<expressionval>$ = P_INFO->buildOperationExpression($<operatorval>2, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|cat_expr   		                        { $<expressionval>$ = $<expressionval>1; }
;

cat_expr:
cat_expr CATSY arithmetic_expr			{ $<expressionval>$ = P_INFO->buildOperationExpression(new CatOp, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|arithmetic_expr   		                { $<expressionval>$ = $<expressionval>1; }
;

arithmetic_expr:
arithmetic_expr arithmetic_operator term        { $<expressionval>$ = P_INFO->buildOperationExpression($<operatorval>2, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|term                                           { $<expressionval>$ = $<expressionval>1; }
;

term:
term term_operator factor                       { $<expressionval>$ = P_INFO->buildOperationExpression($<operatorval>2, $<expressionval>1, $<expressionval>3, LOCATION(@1,@3)); }
|factor                                         { $<expressionval>$ = $<expressionval>1; }
;

factor:
SUBSY value                                     { $<expressionval>$ = P_INFO->buildOperationExpression(new NegationOp, $<expressionval>2, NULL, LOCATION(@1,@2)); }
|LOGICALNOTSY value                             { $<expressionval>$ = P_INFO->buildOperationExpression(new LogicalNotOp, $<expressionval>2, NULL, LOCATION(@1,@2)); }
|value                                          { $<expressionval>$ = $<expressionval>1; }
;

name:
ident {
  P_INFO->setCurrentName($<stringval>1);
  $<expressionval>$ = P_INFO->buildNameExpression(P_INFO->getCurrentName(), LOCATION(@1,@1));
  P_INFO->freeIdent();
}
|VARCASTSY ident RIGHTPARENSY {
  P_INFO->setCurrentName($<stringval>2);
  $<expressionval>$ = P_INFO->buildIdentExpression(P_INFO->getCurrentName(), LOCATION(@1,@1));
  P_INFO->freeIdent();
}
;

top_name:
top_ident              {
  P_INFO->setCurrentName($<stringval>1);
  $<expressionval>$ = P_INFO->buildNameExpression(P_INFO->getCurrentName(), LOCATION(@1,@1));
  P_INFO->freeIdent();
}
;

value:
PREINCRSY	 	{ $<expressionval>$ = P_INFO->buildOperationExpression(new PreIncrOp, new NameExpression((paupLexget_text(P_INFO->getScanner()) + 2)), NULL, LOCATION(@1,@1)); }
|PREDECRSY	 	{ $<expressionval>$ = P_INFO->buildOperationExpression(new PreDecrOp, new NameExpression((paupLexget_text(P_INFO->getScanner()) + 2)), NULL, LOCATION(@1,@1)); }
|call_expression        { $<expressionval>$ = $<expressionval>1; }
|name UNARYINCRSY       { $<expressionval>$ = P_INFO->buildOperationExpression(new PostIncrOp, $<expressionval>1, NULL, LOCATION(@1,@2)); }
|name UNARYDECRSY       { $<expressionval>$ = P_INFO->buildOperationExpression(new PostDecrOp, $<expressionval>1, NULL, LOCATION(@1,@2)); }
|name                   { $<expressionval>$ = $<expressionval>1; }
|literal_value          { $<expressionval>$ = P_INFO->buildLiteralExpression($<literalval>1, LOCATION(@1,@1)); }
|BOOLCASTSY expression RIGHTPARENSY { $<expressionval>$ = P_INFO->buildOperationExpression(new BoolCastOp, $<expressionval>2, NULL, LOCATION(@1,@3)); }
|INTCASTSY expression RIGHTPARENSY { $<expressionval>$ = P_INFO->buildOperationExpression(new IntCastOp, $<expressionval>2, NULL, LOCATION(@1,@3)); }
|REALNUMBERCASTSY expression RIGHTPARENSY { $<expressionval>$ = P_INFO->buildOperationExpression(new RealNumberCastOp, $<expressionval>2, NULL, LOCATION(@1,@3)); }
|STRINGCASTSY expression RIGHTPARENSY { $<expressionval>$ = P_INFO->buildOperationExpression(new StringCastOp, $<expressionval>2, NULL, LOCATION(@1,@3)); }
|LEFTPARENSY expression RIGHTPARENSY           { $<expressionval>$ = P_INFO->buildOperationExpression(new ParenthesesOp, $<expressionval>2, NULL, LOCATION(@1,@3)); }
;

call_expression:
CALLSY {
  string withParen;
  withParen = paupLexget_text(P_INFO->getScanner());
  int indexAfterName = withParen.find_first_of(" \t\n\r(");
  string justName = withParen.substr(0, indexAfterName); //We don't only want up until, so the length = indexAfterName
  P_INFO->pushCommand(new CommandNode());
  P_INFO->getCurrentCommand()->setCommandName(justName);
  CallExpression* newCallExpression = new CallExpression();
  newCallExpression->setCommandNode(P_INFO->getCurrentCommand());
  P_INFO->pushCallExpression(newCallExpression);
} call_tail {
  SET_LOCATION(LOCATION(@1,@1),P_INFO->getCurrentCallExpression());
  $<expressionval>$ = P_INFO->getCurrentCallExpression();
  P_INFO->popCallExpression();
  P_INFO->popCommand();
} 
;
 
call_tail:
generic_params RIGHTPARENSY
|generic_options RIGHTPARENSY
|RIGHTPARENSY
;

ident:
IDENTSY				{ P_INFO->pushIdent(paupLexget_text(P_INFO->getScanner())); $<stringval>$ = P_INFO->getCurrentIdent(); }
;

top_ident:
TOP_IDENTSY			{ P_INFO->pushIdent(paupLexget_text(P_INFO->getScanner())); $<stringval>$ = P_INFO->getCurrentIdent(); }
;

free_ident: {
  P_INFO->freeIdent();
}
;

arithmetic_operator:
add_operator                    { $<operatorval>$ = $<operatorval>1; }
|subtract_operator              { $<operatorval>$ = $<operatorval>1; }
;

term_operator:
multiply_operator               { $<operatorval>$ = $<operatorval>1; }
|divide_operator                { $<operatorval>$ = $<operatorval>1; }
;

rel_operator:
less_than_or_equal_operator     { $<operatorval>$ = $<operatorval>1; }
|greater_than_or_equal_operator { $<operatorval>$ = $<operatorval>1; }
|less_than_operator             { $<operatorval>$ = $<operatorval>1; }
|greater_than_operator          { $<operatorval>$ = $<operatorval>1; }
|inequality_operator            { $<operatorval>$ = $<operatorval>1; }
|equality_operator              { $<operatorval>$ = $<operatorval>1; }
;

add_operator:                   ADDSY        { $<operatorval>$ = new AdditionOp;           } ;
subtract_operator:              SUBSY        { $<operatorval>$ = new SubtractionOp;        } ;
multiply_operator:              MULSY        { $<operatorval>$ = new MultiplicationOp;     } ;
divide_operator:                DIVSY        { $<operatorval>$ = new DivisionOp;           } ;
divide_operator:                MODSY        { $<operatorval>$ = new ModOp;                } ;
and_operator:                   LOGICALANDSY { $<operatorval>$ = new LogicalAndOp;         } ;
or_operator:                    LOGICALORSY  { $<operatorval>$ = new LogicalOrOp;          } ;
inequality_operator:            NEQSY        { $<operatorval>$ = new InequalityOp;         } ;
equality_operator:              DBLEQSY      { $<operatorval>$ = new EqualityOp;           } ;
less_than_or_equal_operator:    LTEQSY       { $<operatorval>$ = new LessThanOrEqualOp;    } ;
greater_than_or_equal_operator: GTEQSY       { $<operatorval>$ = new GreaterThanOrEqualOp; } ;
less_than_operator:             LTSY         { $<operatorval>$ = new LessThanOp;           } ;
greater_than_operator:          GTSY         { $<operatorval>$ = new GreaterThanOp;        } ;

psoda_statement:
psoda_command SEMISY
;

psoda_command:
break_command		//no params
|semi_command    //no params (allow an empty statement)
|unarydecr_command	incr_decr_variable
|unaryincr_command	incr_decr_variable
|update_command
|var_declaration
|return_instruction
|print_command
|help_command
|weights_command      	weight_pair_list
|weights_reset_call
// |execute_call		execute_expression_list
|bootstrap_call		bootstrap_command_tail
|generic_command	generic_param_list 
//|psoda_tree_command_call       
//tree_command_tail
;

/*
expr_psoda_command_call:
expr_bootstrap_call	
|expr_execute_call	
|expr_psoda_tree_command_call
;
*/

/*
 psoda_tree_command_call:             contree_call|     
lscores_call|     
pscores_call;
*/
/*
expr_psoda_tree_command_call:   expr_contree_call|expr_lscores_call|expr_pscores_call;
*/

/*
expr_bootstrap_call:	EXPR_BOOTSTRAPSY	{ P_INFO->pushCommand(new CommandNode()); P_INFO->getCurrentCommand()->setCommandName("bootstrap"); };
*/
bootstrap_call:		     BOOTSTRAPSY	{ P_INFO->getCurrentCommand()->setCommandName("bootstrap"); };

/*
expr_contree_call:	EXPR_CONTREESY		{ P_INFO->pushCommand(new CommandNode()); P_INFO->getCurrentCommand()->setCommandName("contree"); };
*/
/*
contree_call:		     CONTREESY		{ P_INFO->getCurrentCommand()->setCommandName("contree"); };
*/

/*
expr_execute_call:	EXPR_EXECUTESY		{
  P_INFO->pushCommand(new CommandNode());
  P_INFO->getCurrentCommand()->setCommandName("execute");
  void* newParam = new vector<Expression*>();
  P_INFO->setCurrentParam(newParam);
  P_INFO->getCurrentCommand()->setParameter("*expressions", new Data(newParam));
};
*/
/*
execute_call:		     EXECUTESY		{
  P_INFO->getCurrentCommand()->setCommandName("execute");
  void* newParam = new vector<Expression*>();
  P_INFO->setCurrentParam(newParam);
  P_INFO->getCurrentCommand()->setParameter("*expressions", new Data(newParam));
};
*/
/*
expr_lscores_call:	EXPR_LSCORESSY		{ P_INFO->pushCommand(new CommandNode()); P_INFO->getCurrentCommand()->setCommandName("lscoressy"); };
*/
/*
lscores_call:		     LSCORESSY		{ P_INFO->getCurrentCommand()->setCommandName("lscores"); };
*/

/*
expr_pscores_call:	EXPR_PSCORESSY		{ P_INFO->pushCommand(new CommandNode()); P_INFO->getCurrentCommand()->setCommandName("pscores"); };
*/
/*
pscores_call:		     PSCORESSY		{ P_INFO->getCurrentCommand()->setCommandName("pscores"); };
*/

break_command:		BREAKSY			{ P_INFO->getCurrentCommand()->setCommandName("break"); };
semi_command:					{ P_INFO->getCurrentCommand()->setCommandName("*empty"); };
unarydecr_command:	UNARYDECRSY		{ P_INFO->getCurrentCommand()->setCommandName("*decrement"); };
unaryincr_command:	UNARYINCRSY		{ P_INFO->getCurrentCommand()->setCommandName("*increment"); };
weights_command:      	WEIGHTSSY      		{ P_INFO->getCurrentCommand()->setCommandName("weights"); void* newParam = new WeightsData(); P_INFO->getCurrentCommand()->setParameter("*weights", new Data(newParam)); P_INFO->setCurrentParam(newParam)};
weights_reset_call:  	WEIGHTSSY RESETSY       { P_INFO->getCurrentCommand()->setCommandName("*weightsreset"); };


return_instruction:
RETURNSY expression {
  P_INFO->getCurrentCommand()->setCommandName("return");
  P_INFO->getCurrentCommand()->setParameter("*value", (Expression*)$<expressionval>2);
  //  printf("Return Expression: %s\n", ($<expressionval>2)->toString().c_str());
}
|RETURNSY {
  P_INFO->getCurrentCommand()->setCommandName("return");
}
;

print_command:
PRINTSY expression {
  P_INFO->getCurrentCommand()->setCommandName("print");
  P_INFO->getCurrentCommand()->setParameter("*text", (Expression*)$<expressionval>2);
}
|PRINTSY {
  P_INFO->getCurrentCommand()->setCommandName("print");
}
;

help_command:
HELPSY expression {
  P_INFO->getCurrentCommand()->setCommandName("help");
  P_INFO->getCurrentCommand()->setParameter("*name", (Expression*)$<expressionval>2);
}
|HELPSY {
  P_INFO->getCurrentCommand()->setCommandName("help");
}
;

generic_command:
top_name {
  P_INFO->getCurrentCommand()->setCommandName(P_INFO->getCurrentName());
  delete ((NameExpression*)$<expressionval>1);
}
;

generic_param_list:
generic_options
|generic_params
|LEFTPARENSY generic_params RIGHTPARENSY
|LEFTPARENSY generic_options RIGHTPARENSY
|LEFTPARENSY RIGHTPARENSY
|
;

generic_options:
generic_param generic_options
|generic_param
;

generic_params:
generic_param generic_param_tail
;

generic_param_tail:
COMMASY generic_param generic_param_tail
|COMMASY generic_param
;

generic_param:
generic_param_name EQSY expression {
  P_INFO->getCurrentCommand()->setParamValue((Expression*)$<expressionval>3);
}
|generic_param_name {
  P_INFO->getCurrentCommand()->setParamValue(new LiteralExpression(new StringLiteral("yes")));
}
;

generic_param_name:
IDENTSY	{
  P_INFO->getCurrentCommand()->setParamName(paupLexget_text(P_INFO->getScanner()));
}
;

set_command:
SETSY {
  P_INFO->pushSetCommand("*all");
} set_param_list SEMISY
|SETSY LEFTCURLYSY ident RIGHTCURLYSY {
  P_INFO->pushSetCommand($<stringval>3);
} set_param_list SEMISY free_ident
;

set_param_list:
set_options
|LEFTPARENSY set_params RIGHTPARENSY
|LEFTPARENSY set_param set_options RIGHTPARENSY
|
;

set_options:
set_param set_options
|set_param
;

set_params:
set_param COMMASY set_params
|set_param
;

set_param:
ident EQSY expression {
  P_INFO->getCurrentCommand()->setCommandName("set");
  P_INFO->getCurrentCommand()->setParameter("*command", P_INFO->getCurrentSetCommand());
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
  P_INFO->getCurrentCommand()->setParameter("*value", (Expression*)$<expressionval>3);
  SET_LOCATION(LOCATION(@1,@3),P_INFO->getCurrentCommand());
  P_INFO->addCommand(P_INFO->getCurrentCommand());
  P_INFO->popCommand();
  P_INFO->pushCommand(new CommandNode());
} free_ident
|ident {
  P_INFO->getCurrentCommand()->setCommandName("set");
  P_INFO->getCurrentCommand()->setParameter("*command", P_INFO->getCurrentSetCommand());
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
  P_INFO->getCurrentCommand()->setParameter("*value", new LiteralExpression(new StringLiteral("yes")));
  SET_LOCATION(LOCATION(@1,@1),P_INFO->getCurrentCommand());
  P_INFO->addCommand(P_INFO->getCurrentCommand());
  P_INFO->popCommand();
  P_INFO->pushCommand(new CommandNode());
} free_ident
;

var_declaration:
VARSY top_ident EQSY expression {
  P_INFO->getCurrentCommand()->setCommandName("*var");
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>2);
  P_INFO->getCurrentCommand()->setParameter("*value", (Expression*)$<expressionval>4);
} free_ident
|VARSY top_ident {
  P_INFO->getCurrentCommand()->setCommandName("*var");
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>2);
  P_INFO->getCurrentCommand()->setParameter("*value", new LiteralExpression(new UndefinedLiteral()));
} free_ident
;

incr_decr_variable:
top_ident {
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
} free_ident
;

update_command:
top_ident EQSY expression {
  P_INFO->getCurrentCommand()->setCommandName("*update");
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
  P_INFO->getCurrentCommand()->setParameter("*value", (Expression*)$<expressionval>3);
} free_ident
|top_ident UNARYINCRSY {
  P_INFO->getCurrentCommand()->setCommandName("*increment");
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
} free_ident
|top_ident UNARYDECRSY {
  P_INFO->getCurrentCommand()->setCommandName("*decrement");
  P_INFO->getCurrentCommand()->setParameter("*name", $<stringval>1);
} free_ident
;

  /*
execute_expression_list:
execute_expression COMMASY execute_expression_list
|execute_expression
;

execute_expression:
expression {
  CUR_EXECUTE_LIST->push_back((Expression*)$<expressionval>1);
}
;
  */

bootstrap_command_tail:
generic_param_list SLASHSY generic_param_list
|generic_param_list
;

tree_command_tail:
treelist { /* set the *treelist parameter */ } SLASHSY generic_param_list
|treelist { /* set the *treelist parameter */ }
;

treelist:
trees
|ALLSY

trees:
tree trees
|
;

tree:
value
;

weight_value:
value                                           { $<expressionval>$ = $<expressionval>1; }
;

weight_pair_list:
weight_pair
|weight_pair COMMASY weight_pair_list
|
;

weight_pair:
value { P_INFO->getCurrentWeightsParam()->beginWeightPair((Expression*)$<expressionval>1); } COLONSY weight_list
;

weight_list:
{ P_INFO->getCurrentWeightsParam()->beginNumberListEntry(); } number_list_entry number_list
|ALLSY {
  P_INFO->getCurrentWeightsParam()->beginNumberListEntry();
  P_INFO->getCurrentWeightsParam()->setEntryStart(new LiteralExpression(new IntLiteral(1)));
  P_INFO->getCurrentWeightsParam()->setEntryOpenEnded();
  P_INFO->getCurrentWeightsParam()->setEntryIncr(new LiteralExpression(new IntLiteral(1)));
}
;

number_list:
{ P_INFO->getCurrentWeightsParam()->beginNumberListEntry(); } number_list_entry number_list
|
;

number_list_entry:
weight_value {
  P_INFO->getCurrentWeightsParam()->setEntryStart((Expression*)$<expressionval>1);
  P_INFO->getCurrentWeightsParam()->setEntryEnd(NULL);
  P_INFO->getCurrentWeightsParam()->setEntryIncr(new LiteralExpression(new IntLiteral(1))); 
}
|start_number MINUSSY end_number {
  P_INFO->getCurrentWeightsParam()->setEntryIncr(new LiteralExpression(new IntLiteral(1))); }
|start_number MINUSSY end_number BSLASHSY expression {
  P_INFO->getCurrentWeightsParam()->setEntryIncr((Expression*)$<expressionval>5); }
|start_number MINUSSY PERIODSY {
  P_INFO->getCurrentWeightsParam()->setEntryOpenEnded();
  P_INFO->getCurrentWeightsParam()->setEntryIncr(new LiteralExpression(new IntLiteral(1))); }
|start_number MINUSSY PERIODSY BSLASHSY expression {
  P_INFO->getCurrentWeightsParam()->setEntryOpenEnded();
  P_INFO->getCurrentWeightsParam()->setEntryIncr((Expression*)$<expressionval>5); }
 ;


start_number:
weight_value { P_INFO->getCurrentWeightsParam()->setEntryStart((Expression*)$<expressionval>1); }
;

end_number:
weight_value { P_INFO->getCurrentWeightsParam()->setEntryEnd((Expression*)$<expressionval>1); }
;

block_assumptions:
ASSUMPTIONSSY SEMISY assumptions
;

assumptions:
assumption assumptions
|
;

assumption:
SEMISY
;

block_sets:
SETSSY SEMISY sets
;

sets:
;

block_tree:
TREEBLOCKSTRING { P_INFO->pushBlock(new TreeBlock($1)); }
;

block_distances:
DISTANCESSY SEMISY distances
;

distances:
;
