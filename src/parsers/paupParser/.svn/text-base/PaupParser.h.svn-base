#ifndef PAUP_PARSER_H
#define PAUP_PARSER_H

#include "PaupParser.hpp"

typedef void* yyscan_t;
typedef struct yy_buffer_state* YY_BUFFER_STATE;
typedef unsigned int yy_size_t;

int paupLexparse (void* YYPARSE_PARAM);
void paupLexset_in(FILE* inFile, void* yyscanner);
void paupLexset_extra(void* user_defined, yyscan_t yyscanner);
void paupLexset_lloc(YYLTYPE* yylloc_param, yyscan_t yyscanner);
int paupLexlex_init(void** yyscanner);
int paupLexlex_destroy(void* yyscanner);
YY_BUFFER_STATE paupLex_scan_buffer(char* base, yy_size_t size, yyscan_t yyscanner);

#endif
