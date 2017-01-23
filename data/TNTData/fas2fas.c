/*
Copyright (C) 2008 Pablo A. Goloboff

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc., 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.

Email: pablogolo@csnat.edu.ar
Mail: Pablo A. Goloboff, INSUE, Instituto Miguel Lillo, Miguel Lillo 205, 4000 S.M. de Tucuman, Argentina. 
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
FILE * inpf[30] , * curinput , * opsf = NULL ;
int numinputs = 0 ;
unsigned long int userlength = 0 ;
int itsahybrid , unacceptable ;
int max_acceptable_diffs = 3 ;
double threshold = 0.95 ; 
unsigned long int dafsize , bytes_read ; 

int num_unaccepts = 0 ;
#define MAX_NUM_UNACCEPTS 100 
char * unaccept[ MAX_NUM_UNACCEPTS ] ; 

#define MAXSEQS 100000 
char ** names ;
char ** seqs ;
unsigned long int * islike ; 

unsigned long int seqsread = 0 , seqsinfile = 0 ; 

#define MAXNAMELEN 250 
#define MAXSEQLENGTH 25000
char tmpname[MAXNAMELEN] , tmpbuff[MAXNAMELEN];
char tmpseq[MAXSEQLENGTH] ;

typedef struct 
      { int up , diag , lef ; 
        int min ; } Stringcomptyp ; 
Stringcomptyp ** cellcost ; 
int gapcost = 1 , gapextcost = 1 ; 
int suscost = 1 ;


#ifdef LINUX
int getch ( void )
{
    return getc ( stdin ) ;
}    
#endif

int sizit ( void )
{
    struct stat buf ;
    fstat ( fileno ( curinput ) , &buf ) ;
    dafsize = buf.st_size ;
    bytes_read = 0 ; 
}

void dildit ( void )
{
    static int prv = 0 ; 
    double fract ;
    unsigned long int ifract ; 
    fract = ( ( double ) bytes_read / dafsize ) * 100 ;
    ifract = fract ; 
    if ( ifract == prv ) return ;
    prv = ifract ; 
    fprintf ( stderr , "\b\b\b\b%3i%%" , ifract ) ; 
}

int cget ( FILE * where )
{
    ++ bytes_read ; 
    return getc ( where ) ; 
}

void outer ( char * txt , int i )
{
    fprintf ( stderr , txt , i ) ;
    getc ( stdin ) ; 
    exit ( 1 ) ;
}    

void * myalloc ( unsigned long int siz )
{
    void * p = malloc ( siz ) ;
    if ( p == NULL ) outer ( "OOPS! --ran out of memory, reading data for sequence nr. %i" , seqsinfile ) ; 
    return p ; 
}

void gotoarrow ( void )
{
    int i = ' ' ; 
    while ( isspace ( i ) && !feof ( curinput ) ) i = cget ( curinput ) ;
    if ( feof ( curinput ) ) return ; 
    if ( i != '>' ) outer ( "Expected \">\" for taxon %i" , seqsinfile ) ;
}    

void stuffit ( int strinlen )
{
    if ( strinlen < 6 ) strinlen = 6 ; 
    names[seqsread] = myalloc ( ( strinlen + 1 ) * sizeof ( char ) ) ;
    strcpy ( names[seqsread] , tmpname ) ;
}

void rdtaxname ( void )
{
    char * cp = tmpname - 1 , * cpp ;
    int i = 'a' , found_cuadruple , x ;
    int numunderscores = 0 , secunderscore ;
    int strinlen ; 
    while ( !isspace ( i ) ) 
        * ++ cp = i = cget ( curinput ) ;
    * cp = '\0' ;
    ridofstuff () ;
    if ( unacceptable || itsahybrid ) return ; 
    strinlen = strlen ( tmpname ) ;
    cp = tmpname ;
    while ( * cp ) {
        if ( * cp == '_' )
           if ( ++ numunderscores == 2 ) secunderscore = cp - tmpname ;
        ++ cp ; }
    if ( strinlen - secunderscore < 4 ) {
        for ( i = 1 ; i < secunderscore ; ++ i )
              tmpname[i] = tolower( tmpname[i] ) ; 
        stuffit ( strinlen ) ; 
        return ; }
    i = secunderscore + 1 ; 
    if ( tmpname[i]=='_' && tmpname[i+1]=='_' && tmpname[i+2]=='_' ) {
        for ( i = 1 ; i < secunderscore ; ++ i )
              tmpname[i] = tolower( tmpname[i] ) ; 
        stuffit ( strinlen ) ;
        return ; }
    found_cuadruple = 0 ;
    i = secunderscore ; 
    while ( !found_cuadruple ) {
        if ( strinlen - i < 4 ) break ;
        x = tmpname[i+4] ;
        tmpname[i+4] = '\0' ;
        cp = tmpname + i ;
        if ( !strcmp ( cp , "____" ) )
             found_cuadruple = cp - tmpname ;
        tmpname[i+4] = x ; 
        ++ i ; }
    for ( i = 1 ; i < found_cuadruple ; ++ i )
         tmpname[i] = tolower( tmpname[i] ) ; 
    cpp = tmpname + secunderscore ;
    while ( * cp ) * cpp ++ = * cp ++ ;
    * cpp = '\0' ; 
    stuffit ( cp - tmpname ) ;
    cp = tmpname ;
    return ; 
}

void rdsequence ( void ) 
{
    char * cp = tmpseq ;
    int i , len ;
    while ( isspace ( i = cget ( curinput ) ) ) ;
    while ( i != '>' /* !isspace ( i ) */ && !feof ( curinput ) ) {
        if ( !isspace ( i ) ) 
             * cp ++ = i ;
        i = cget ( curinput ) ; }
    * cp = '\0' ;
    if ( i == '>' ) ungetc ( i , curinput ) ; 
    len = cp - tmpseq ; 
    seqs[seqsread] = myalloc ( ( len + 1 ) * sizeof ( char ) ) ;
    strcpy ( seqs[seqsread], tmpseq ) ;
    return ; 
}

int istruncof ( char * deque , char * que )
{
    int i , j ; 
    while ( * que ) {
       i = tolower ( * que ++ ) ;
       j = tolower ( * deque ++ ) ; 
       if ( i != j ) return 0 ; }
    return 1 ; 
}

void ridofstuff ( void )
{
    char * a = tmpbuff ;
    char * cp = tmpname ;
    int i ; 
    int havesomepars = 0 ;
    itsahybrid = unacceptable = 0 ; 
    if ( islower ( * cp ) ) { unacceptable = 1 ; return ; }
    /*  Get rid of single quotes  */ 
    if ( * cp == '\'' ) {
        while ( * cp ) {
            * a ++ = * ++ cp ;
            if ( * cp == '\'' ) a -- ; }
        strcpy ( tmpname , tmpbuff ) ; }
    /*  Get rid of parentheses, or _x_ */ 
    a = tmpbuff ;
    cp = tmpname ;
    havesomepars = 0 ; 
    while ( * cp ) {
        for ( i = 0 ; i < num_unaccepts && !unacceptable ; ++ i ) 
            if ( istruncof ( cp , unaccept[i] ) )
                unacceptable = 1 ; 
        if ( * cp == '(' ) {
            havesomepars = 1 ; 
            while ( * cp && * cp != ')' ) ++ cp ;
            ++ cp ;
            if ( a > tmpbuff && * ( a - 1 ) == '_' && * cp == '_' ) -- a ; }
        if ( * cp == '_' ) {
            i = cp[3] ;
            cp[3] = '\0' ;
            if ( !strcmp ( cp , "_x_" ) ) itsahybrid = 1 ;
            cp[3] = i ; }
        * a ++ = * cp ++ ; }
    * a = '\0' ; 
    if ( itsahybrid || unacceptable ) return ;
    if ( havesomepars ) strcpy ( tmpname , tmpbuff ) ; 
    return ; 
}

void parse_strings_to_trash ( void )
{
    char tmpopt[50] ;
    char * cp ;
    int i = ' ' ;
    while ( !feof ( opsf ) ) {
        cp = tmpopt - 1 ; 
        while ( isspace ( i ) && !feof ( opsf ) ) i = cget ( opsf ) ;
        while ( !isspace ( i ) && !feof ( opsf ) ) {
            * ++ cp = i ;
            i = cget ( opsf ) ; }
        * ++ cp = '\0' ;
        if ( feof ( opsf ) && cp == tmpopt ) break ; 
        unaccept[num_unaccepts] = myalloc ( ( strlen ( tmpopt ) + 1 ) * sizeof ( char ) ) ; 
        strcpy ( unaccept[num_unaccepts++] , tmpopt ) ;
        fprintf ( stderr , "Will trash any name containing \"%s\"\n" , tmpopt ) ;
        i = cget ( opsf ) ; }
    fclose ( opsf ) ;
}           

double doneedwunsch ( char * ap , char * bp ) 
{
    int wid , hei , i , j , dacos ; 
    char * app , * bpp ; 
    char * abecs , * bbecs , * anp , * bnp ;
    double val ;
    static int mademem = 0 ; 
    int HIGH = 10000000 ; 
    wid = strlen ( ap ) ; 
    hei = strlen ( bp ) ; 
    if ( !mademem ) {
         cellcost = myalloc ( 100 * sizeof ( Stringcomptyp * ) ) ;
         for ( i = 0 ; i < 100 ; ++ i )
            cellcost[i] = myalloc ( 100 * sizeof ( Stringcomptyp ) ) ; 
         mademem = 1 ; }
    if ( hei >= 99 || wid >= 99 )
        outer ( "String for sp. %i is too long to use string-matching" , 0 ) ; 
    cellcost[0][0].min = cellcost[0][0].diag = 0 ; 
    cellcost[0][0].up = cellcost[0][0].lef = HIGH ; 
    bpp = bp ; 
    for ( j = 0 ; j < hei ; ++ j ) {
       app = ap ; 
       for ( i = 0 ; i < wid ; ++ i ) {
          if ( !i && !j ) {
             continue ; }
          dacos = 0 ; 
          if ( * app != * bpp ) dacos = suscost ; 
          if ( j ) {
            if ( cellcost[i][j-1].min == cellcost[i][j-1].up )  
               cellcost[i][j].up = cellcost[i][j-1].min + gapextcost ; 
            else 
               cellcost[i][j].up = cellcost[i][j-1].min + gapcost ; }
          else cellcost[i][j].up = cellcost[i][j].diag = HIGH ; 
          if ( i ) {
            if ( cellcost[i-1][j].min == cellcost[i-1][j].lef ) 
               cellcost[i][j].lef = cellcost[i-1][j].min + gapextcost ; 
            else 
               cellcost[i][j].lef = cellcost[i-1][j].min + gapcost ; }
          else cellcost[i][j].lef = cellcost[i][j].diag = HIGH ; 
          if ( i && j ) cellcost[i][j].diag = cellcost[i-1][j-1].min + dacos ; 
          dacos = cellcost[i][j].diag ; 
          if ( dacos > cellcost[i][j].up ) dacos = cellcost[i][j].up ; 
          if ( dacos > cellcost[i][j].lef ) dacos = cellcost[i][j].lef ; 
          cellcost[i][j].min = dacos ; 
          ++ app ; }
       ++ bpp ; }
    dacos = cellcost[wid-1][hei-1].min ;
    if ( dacos > max_acceptable_diffs ) return 0 ;
    if ( dacos == 0 ) return 1 ; 
    if ( wid > hei ) hei = wid ; 
    val = ( double ) dacos / ( double ) hei ;
    val = 1 - val ; 
    return val ; 
} 

int absneedwunsch ( char * ap , char * bp ) 
{
    int wid , hei , i , j , dacos ; 
    char * app , * bpp ; 
    char * abecs , * bbecs , * anp , * bnp ;
    double val ;
    static int mademem = 0 ; 
    int HIGH = 10000000 ; 
    wid = strlen ( ap ) ; 
    hei = strlen ( bp ) ; 
    if ( !mademem ) {
         cellcost = myalloc ( 100 * sizeof ( Stringcomptyp * ) ) ;
         for ( i = 0 ; i < 100 ; ++ i )
            cellcost[i] = myalloc ( 100 * sizeof ( Stringcomptyp ) ) ; 
         mademem = 1 ; }
    if ( hei >= 99 || wid >= 99 )
        outer ( "String for sp. %i is too long to use string-matching" , 0 ) ; 
    cellcost[0][0].min = cellcost[0][0].diag = 0 ; 
    cellcost[0][0].up = cellcost[0][0].lef = HIGH ; 
    bpp = bp ; 
    for ( j = 0 ; j < hei ; ++ j ) {
       app = ap ; 
       for ( i = 0 ; i < wid ; ++ i ) {
          if ( !i && !j ) {
             continue ; }
          dacos = 0 ; 
          if ( * app != * bpp ) dacos = suscost ; 
          if ( j ) {
            if ( cellcost[i][j-1].min == cellcost[i][j-1].up )  
               cellcost[i][j].up = cellcost[i][j-1].min + gapextcost ; 
            else 
               cellcost[i][j].up = cellcost[i][j-1].min + gapcost ; }
          else cellcost[i][j].up = cellcost[i][j].diag = HIGH ; 
          if ( i ) {
            if ( cellcost[i-1][j].min == cellcost[i-1][j].lef ) 
               cellcost[i][j].lef = cellcost[i-1][j].min + gapextcost ; 
            else 
               cellcost[i][j].lef = cellcost[i-1][j].min + gapcost ; }
          else cellcost[i][j].lef = cellcost[i][j].diag = HIGH ; 
          if ( i && j ) cellcost[i][j].diag = cellcost[i-1][j-1].min + dacos ; 
          dacos = cellcost[i][j].diag ; 
          if ( dacos > cellcost[i][j].up ) dacos = cellcost[i][j].up ; 
          if ( dacos > cellcost[i][j].lef ) dacos = cellcost[i][j].lef ; 
          cellcost[i][j].min = dacos ; 
          ++ app ; }
       ++ bpp ; }
    dacos = cellcost[wid-1][hei-1].min ;
    return dacos ; 
} 

int checkit ( int which )
{
    int isamatch = 0 , matchto , endofa , endofb , absdiff ;
    int i , j , k , answer , warnit , q ; 
    char * ais , * bis , toa , tob , lenis , * accstartsa , * accstartsb , * accstartswas ;
    int alldashes ; 
    double resemblance ; 
    for ( i = which ; i -- && !isamatch ; ) {
        lenis = strlen ( ais = names[i] ) ;
        k = 0 ; 
        while ( k ++ < ( lenis - 4 ) ) {
            ++ ais ;
            for ( j = 0 , alldashes = 1 ; j < 4 && alldashes ; ++ j )
                 if ( ais[j] != '_' ) alldashes = 0 ;
            if ( alldashes ) { accstartsa = ais + 4 ; break ; }}
        lenis = strlen ( bis = names[which] ) ;
        k = 0 ; 
        while ( k ++ < ( lenis - 4 ) ) {
            ++ bis ;
            for ( j = 0 , alldashes = 1 ; j < 4 && alldashes ; ++ j )
                 if ( bis[j] != '_' ) alldashes = 0 ;
            if ( alldashes ) { accstartsb = bis + 4 ; break ; }}
        toa = * ais ; * ais = '\0' ; 
        tob = * bis ; * bis = '\0' ;
        endofa = names[i][5] ;
        endofb = names[which][5] ;
        names[i][5]= names[which][5] = '\0' ;
        absdiff = absneedwunsch ( names[i] , names[which] ) ; 
        names[i][5] = endofa ;
        names[which][5] = endofb ;
        if ( absdiff > 2 ) resemblance = 0 ;
        else 
           resemblance = doneedwunsch ( names[i] , names[which] ) ;
        * ais = toa ;
        * bis = tob ; 
        if ( resemblance == 1 ) {
                isamatch = 1 ;
                matchto = i ; }
        else if ( resemblance >= threshold ) {
                   accstartswas = accstartsa ; 
                   for ( k = i ; k -- && !isamatch ; ) {
                         lenis = strlen ( ais = names[k] ) ;
                         q = 0 ; 
                         while ( q ++ < ( lenis - 4 ) ) {
                                ++ ais ;
                                for ( j = 0 , alldashes = 1 ; j < 4 && alldashes ; ++ j )
                                     if ( ais[j] != '_' ) alldashes = 0 ;
                                if ( alldashes ) { accstartsa = ais + 4 ; break ; }}
                         toa = * ais ; * ais = '\0' ; 
                         tob = * bis ; * bis = '\0' ;
                         if ( !strcmp ( names[which] , names[k] ) ) {
                             isamatch = 1 ;
                             matchto = k ; }
                         * ais = toa ;
                         * bis = tob ; }
                   if ( !isamatch ) {
                      accstartsa = accstartswas ; 
                      fprintf ( stderr , "\r                                           \nFound name %s\n" , names[which] ) ;
                      fprintf ( stderr ,   "similar to %s\n" , names[i] ) ;
                      fprintf ( stderr , "Do you want to consider them the same (y/n)?" ) ;
                      answer = 0 ;
                      while ( answer != 'y' && answer != 'n' ) 
                          answer = tolower ( getch ( ) ) ;
                      fprintf ( stderr , "\n" ) ; 
                      if ( answer == 'y' ) {
                          isamatch = 1 ;
                          matchto = i ; }}}
        if ( isamatch ) {
            while ( * accstartsa != '@' && * accstartsa ) ++ accstartsa ;
            if ( * accstartsa == '@' ) {
                ++ accstartsa ;
                j = accstartsa [ 4 ] ; 
                accstartsa [ 4 ] = '\0' ; }
            while ( * accstartsb != '@' && * accstartsb ) ++ accstartsb ;
            if ( * accstartsb == '@' ) {
                ++ accstartsb ;
                k = accstartsb [ 4 ] ; 
                accstartsb [ 4 ] = '\0' ; }
            warnit = 0 ;
            if ( strcmp ( accstartsa , accstartsb ) ) warnit = 1 ;
            accstartsa[ 4 ] = j ; 
            accstartsb[ 4 ] = k ;
            if ( warnit ) {
                fprintf ( stderr , "\r                                                      \nWARNING: \n   %s\n   %s\n\ndiffer only in kingdom, but considered the same!\n(press any key to continue)\n" , names[which] , names[matchto] ) ;
                getch ( ) ;
                fprintf ( stderr , "\n" ) ; }}}
    if ( !isamatch ) islike[which] = which ;
    else islike[which] = islike[matchto] ;
    return  ( !isamatch ) ;
}    

int main ( int argc , char ** argv )
{
    double val ;
    time_t now , before ; 
    int i , j ; 
    before = clock () ;
    for ( i = 1 ; i < argc ; ++ i ) {
        if ( !strcmp ( argv[i] , "-l" ) ) {
            if ( argc < i + 2 ) outer ( "Needs length after -l" , 0 ) ;
            userlength = atoi ( argv[++i] ) ; }
        else if ( !strcmp ( argv[i] , "-t" ) ) {
            if ( argc < i + 2 ) outer ( "Needs filename after -t" , 0 ) ;
            opsf = fopen ( argv[++i] , "rb" ) ; }
        else if ( !strcmp ( argv[i] , "-s" ) ) {
            if ( argc < i + 2 ) outer ( "Needs similarity after -s" , 0 ) ;
            threshold = atof ( argv[++i] ) ;
            fprintf ( stderr , "\nUsing similarity of %f\n" , threshold ) ; }
        else if ( !strcmp ( argv[i] , "-m" ) ) {
            if ( argc < i + 2 ) outer ( "Needs maximum edit cost after -m" , 0 ) ;
            max_acceptable_diffs = atoi ( argv[++i] ) ; }
        else 
            if ( ( inpf[numinputs++] = fopen ( argv[i] , "rb" ) ) == NULL )
               outer ( "Cannot open input file nr %i" , i ) ; }
    if ( !numinputs ) 
        outer ( "No input file specified" "\n\nOther options:\n  -t filename   file with strings to trash (no quotes!)\n  -l N   discard sequences shorter than N\n  -s S  ask for names with similarity greater than S\n" , 0 ) ;
    seqs = myalloc ( MAXSEQS * sizeof ( char * ) ) ;
    names = myalloc ( MAXSEQS * sizeof ( char * ) ) ;
    islike = myalloc ( MAXSEQS * sizeof ( unsigned long int ) ) ;
    if ( opsf != NULL ) parse_strings_to_trash () ; 
    for ( i = 0 ; i < numinputs ; ++ i ) {
        curinput = inpf [ i ] ;
        sizit () ;
        fprintf ( stderr , "\rReading file %3i ...     \n" , i + 1 ) ; 
        while ( 1 ) {
            gotoarrow( ) ;
            if ( feof ( curinput ) ) break ; 
            rdtaxname ( ) ;
            rdsequence ( ) ;
            seqsinfile ++ ; 
            if ( itsahybrid ) {
                fprintf ( stderr , "\rIgnoring %i (it's a hybrid)\n" , seqsinfile ) ;
                continue ; }
            if ( unacceptable ) {
                fprintf ( stderr , "\rIgnoring %i (not acceptable)\n" , seqsinfile ) ;
                continue ; }
            ++ seqsread ; 
            dildit () ; }}
   fprintf ( stderr , "\rFinished parsing %i files...\nChecking unique names...    " , numinputs ) ; 
   for ( i = 0 ; i < seqsread ; ++ i ) {
         fprintf ( stderr , "\b\b\b\b%3i%%" , ( ( i * 100 ) / seqsread ) ) ;  
         checkit ( i ) ; }
   savem () ; 
   fprintf ( stderr , "\rDone!                                          " ) ;
   now = clock () ;
   val = ( double ) ( now - before ) / CLK_TCK ;
   fprintf ( stderr , "\n%.2f secs.\n" , val ) ; 
   exit ( 0 ) ;
}

void savem ( void )
{
    int i , j , leni , lenj ;
    for ( i = 0 ; i < seqsread ; ++ i ) {
        leni = strlen ( seqs[i] ) ;
        if ( userlength && leni < userlength ) islike[i] = -1 ; 
        for ( j = i ; ++ j < seqsread && islike [ i ] == i ; ) {
            if ( islike[j] != i ) continue ;
            lenj = strlen ( seqs[j] ) ;
            if ( leni < lenj ) 
                islike [ i ] = islike [ j ] = j ; }}
    for ( i = 0 ; i < seqsread ; ++ i )
       if ( islike[i] == i ) 
           fprintf ( stdout , ">%s\n%s\n" , names[i] , seqs[i] ) ;
    return ; 
}                


