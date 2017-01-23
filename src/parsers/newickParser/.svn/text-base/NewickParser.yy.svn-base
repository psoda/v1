%{
  #include "NewickLexicalAnalyzer.h"
  #include "NewickTreeParser.h"
  #include "QTree.h"
  #include "QNode.h"
  #include "PsodaPrinter.h"
  #include <string>
  #define YYERROR_VERBOSE 1
  #define YYPARSE_PARAM parser
  #define YYLEX_PARAM ((NewickTreeParser*)parser)->yyscanner
  #define NTP ((NewickTreeParser*) parser)
  //  #define LINE_NO newickget_lineno(newick_yyscanner)
  #define LINE_NO newick_linenum

  int newick_linenum;
  void *newick_yyscanner; 
  string filename;

  char *newickget_text(void *yyscanner);
  int newickget_lineno(void *yyscanner);
  void newickset_lineno(int n, void *yyscanner);

  union YYSTYPE;
  int newicklex(YYSTYPE *yylval_param, void *yyscanner);

  //for bison
  int newickerror() { PsodaPrinter::getInstance()->writeError("\nNewick Parser Error in %s line %d, %s\n", (char*)filename.c_str(), LINE_NO, newickget_text(newick_yyscanner)); return -1; }
  int newickerror( const char* errorMessage ) { PsodaPrinter::getInstance()->writeError("\nNewick Parser Error in %s line %d, %s\n", (char*)filename.c_str(), LINE_NO, errorMessage); return -1; }
  void newickset_scanner(void* newScanner) { newick_yyscanner = newScanner; }
  void newickset_filename(string newName) { filename = newName; }


%}

%debug
%pure_parser
%name-prefix="newick"
%file-prefix="NewickParser"
%verbose

%union 
{
    int intval;
    char *stringval;
    double doubleval;
    QNode *nodeval;
}

%start START;
%token LEFTPARENSY
%token RIGHTPARENSY
%token COMMASY
%token TRANSLATESY
%token <doubleval> REALNUMBERSY
%token <stringval> NAMESY
%token <intval> NUMBERSY
%token <stringval> IDENTSY
%token SEMISY
%token NEXUSSY
%token BEGINSY
%token TREESSY
%token TREESY
%token EQUALSY
%token FUNKYSY
%token COLONSY
%token ENDSY

%type <doubleval> branch_length
%type <stringval> name
%type <nodeval> leaf_name sub_name

%%
START:
header
treelist
trailer
;

header:
NEXUSSY
BEGINSY TREESSY SEMISY
translate
|
;

translate:
TRANSLATESY taxaList SEMISY 
|
;


trailer:
ENDSY SEMISY
|
;

taxaList:
taxaListItem
|taxaListItem COMMASY taxaList
|
;

taxaListItem:
NUMBERSY { NTP->biggestNodeNumber($1); } NAMESY { NTP->setTranslation($1, $3); }
;


treelist:
treelist tree
|tree
;

tree:
TREESY { NTP->qtree = new QTree(); } IDENTSY EQUALSY optionalfunkysymbol subTree SEMISY { NTP->finishTree();  NTP->uploadAndResetTree(); }
|{ NTP->qtree = new QTree(); } subTree optionalbranchlength optionalsemicolon { NTP->finishTree();  NTP->uploadAndResetTree(); }
;

optionalfunkysymbol:
FUNKYSY
|
;

optionalbranchlength:
COLONSY REALNUMBERSY 
|COLONSY NUMBERSY 
| 
;

optionalsemicolon:
SEMISY
|
;

subTree:
 interiorNode
;

interiorNode:
LEFTPARENSY { NTP->addInteriorNode(); }
branchlist
RIGHTPARENSY 
;

name: 
NUMBERSY {$$ = NTP->buildLabel($<intval>1);}
| REALNUMBERSY{$$ = NTP->buildLabel($<doubleval>1);}
| IDENTSY{$$ = NTP->buildLabel($<stringval>1);}
|{ $$ = NULL;}
;

branchlist:
branch 
| branch COMMASY { NTP->nextNode(); } branchlist
;

branch:
sub_name branch_length {  NTP->setBranchLength($<nodeval>1, $<doubleval>2);  }
| leaf_name branch_length { NTP->setBranchLength($<nodeval>1, $<doubleval>2); }
;

branch_length:
COLONSY REALNUMBERSY { $$ = $<doubleval>2; }
|COLONSY NUMBERSY { $$ = (double)$<intval>2; }
| { $$ = -1.0 }
;

sub_name:
subTree name { NTP->setLabel($<stringval>2); $$ = NTP->popInteriorNode();}
;

leaf_name: 
NUMBERSY { $$ = NTP->addLeafNode($<intval>1); }
| IDENTSY { $$ = NTP->addLeafNode($<stringval>1); }
/* dissallowing unnamed leaves
|
*/
;

%%

