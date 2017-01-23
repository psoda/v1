#ifndef NEWICK_LEXICAL_ANAYLIZER_H
#define NEWICK_LEXICAL_ANAYLIZER_H
#include <stdio.h>
typedef void* yyscan_t;
typedef void* newickscan_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
//typedef unsigned int yy_size_t;
typedef size_t yy_size_t;

int newicklex_init( newickscan_t* );
//int newicklex( int*,  newickscan_t );
int newicklex( int*, newickscan_t );
int newicklex_destroy( newickscan_t );
void newickset_extra( void* user_defined, newickscan_t yyscanner );
void newickset_in( FILE* user_defined, newickscan_t yyscanner );
YY_BUFFER_STATE newick_scan_buffer  (char * base, yy_size_t  size , yyscan_t yyscanner);
#endif

