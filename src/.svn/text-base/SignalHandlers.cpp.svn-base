/*
 * Copyright (C) <2009> <Quinn Snell, Mark Clement>
 *
 * This program is free software; you can redistribute it and/or modify it under the 
 * terms of the GNU General Public License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program (gpl.txt); 
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * More information can be obtained by accessing http://dna.cs.byu.edu/psoda
 */
#include "SignalHandlers.h"

#include "Interpreter.h"
#include "PsodaException.h"
#include "Globals.h"
#include <iostream>
#include <stdio.h>

#ifdef USEREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

void SodaSignalHandler(int signum);
void PsodaErrorSignalHandler(int signum) __attribute__ ((noreturn));
void PsodaSignalHandlerQuit(int signum) __attribute__ ((noreturn));
void SavingTreesAfterErrorHandler(int signum) __attribute__ ((noreturn));
void dumpTrees(Interpreter *interpreter);

#ifndef WIN32
#include <signal.h>
/*
void sigIntHandler ( int signal, siginfo_t* sigInfo, void* ucontext )
{
  SaveTrees ( GlobalInterpreter );
  exit ( 0 );
  return;
}
*/
#endif 

void RegisterSignalHandlers ( )
{
#ifndef GUI
#ifndef WIN32
  signal(SIGINT, SodaSignalHandler);
  //  signal(SIGILL, PsodaErrorSignalHandler);
  if (!(AlgorithmFlags & NOCATCHSIGBUS)) {
    //    signal(SIGBUS, PsodaErrorSignalHandler);
  }
  if (!(AlgorithmFlags & NOCATCHSIGSEGV)) {
    //    signal(SIGSEGV, PsodaErrorSignalHandler);
  }
#if 0
  struct sigaction sigintHandlerStruct;
  sigintHandlerStruct.sa_sigaction = sigIntHandler ;

#ifndef WIN32
  if ( sigaction ( SIGINT, &sigintHandlerStruct, NULL ) )
  {
    //ERROR CONDITION
  }
#endif
#endif
#endif
#endif
}

void dumpTrees(Interpreter *interpreter)
{
  char filename[80];
  //char formatInput[2];
  int format;
  unsigned int i;
  while(1){
    printf("Enter the filename: ");
    fgets(filename, 80, stdin); 
    // Find and strip off the newline char
    for (i = 0; i < strlen(filename); i++)
    {
      if (filename[i] == '\n')
        filename[i] = '\0';
    }

    if(strlen(filename) > 0){ break; }
  }
  while(1){
    printf("Choose file format (%d = Nexus, %d = Newick): ", NEXUS_TREE_FORMAT, NEWICK_TREE_FORMAT);
    
    if(scanf("%d", &format)){ while(getchar() != '\n'); break; }
    else{ while(getchar() != '\n'); }
  }
  interpreter->saveTrees(format, filename);
}

void PsodaErrorSignalHandler(int signum)
{
  char c;

#ifndef WIN32
  signal( SIGINT, PsodaSignalHandlerQuit ); //if they press Ctrl-C twice, let them quit
#endif
  //  perror("Error: ");
  printf( "\nBecause of an error, PSODA needs to exit.\nIf you want to save your trees before we exit, enter 'S', otherwise, please enter 'Q' to quit.\n\n");

  while (true) {
    printf( "Options: (S/s)ave trees, (Q/q)uit\n" );
    cin >> c;
printf("Character you typed was %c\n",c);
    switch( c ) {
    case 'S': 
    case 's': 
      printf( "Saving Trees......\n" );
#ifndef WIN32
      signal( SIGSEGV, SavingTreesAfterErrorHandler );
      signal( SIGBUS, SavingTreesAfterErrorHandler );
      signal( SIGILL, SavingTreesAfterErrorHandler );
#endif
      dumpTrees( Interpreter::getInstance() );
      exit(signum);
      break;
      
    case 'Q': 
    case 'q': 
      printf( "Exiting......\n" );
      exit(signum);
      break;
    /*default:
        fprintf(stderr, "PsodaErrorSignalHandler(): cin: %c", c); */
    }
    fflush(NULL);
#ifndef WIN32
    sleep(1);
#endif
  }
}

void SavingTreesAfterErrorHandler(int signum __attribute__ ((unused)) )
{
  printf( "There was an error while trying to save the trees.\nExiting Immediately.........\n" );
  exit(0);
}


void SodaSignalHandler(int signum __attribute__ ((unused)))
{
  char c;
  int done = 0;

#ifdef USEREADLINE
  if (RL_ISSTATE(RL_STATE_SIGHANDLER)) {
    break_readline_input = true;
    rl_done = 1;
    return;
  }
#endif

  //  signal( signum, SIG_IGN );
#ifndef WIN32
  signal( SIGINT, PsodaSignalHandlerQuit ); //if they press Ctrl-C twice, let them quit
#endif
  //  printf( "\n ### Signal detected --> " );
  //  printf( "<ctrl-c> " );

  while ( !done )
  {
    printf( "Options (C/c)ontinue, (S/s)ave trees, (Q/q)uit\n" );
    cin >> c;
printf("Character you typed was %c\n",c);
    switch( c )
    {
      case 'C': 
      case 'c': 
        printf( "Continuing......\n" );
#ifndef WIN32
        signal( SIGINT, SodaSignalHandler ); // reinstall self as signal handler
#endif
        done = 1;
        break;
      case 'S': 
      case 's': 
printf("the character in the switch was S\n");
        printf( "Saving Trees......\n" );
        dumpTrees( Interpreter::getInstance() );
#ifndef WIN32
        signal( SIGINT, SodaSignalHandler ); // reinstall self as signal handler
#endif
        done = 1;
        break;

      case 'Q': 
      case 'q': 
printf("the character in the switch was Q\n");
#ifdef GUI
		gStopPSODA();
#else
        printf( "Exiting......\n" );
        exit(signum);
#endif
        break;
    }
  }
}

void PsodaSignalHandlerQuit(int signum __attribute__ ((unused)))
{
  printf( "Quitting Immediately.........\n" );
  exit(0);
}
