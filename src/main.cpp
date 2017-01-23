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
#include "PsodaRunner.h"
#include "ConsolePrintBuffer.h"
#include "PsodaPrinter.h"
#include <stdio.h>
#ifndef WIN32
#include <sys/resource.h>
#endif

extern double PSODA_maxmemory; // The max java vm memory or OS memory we can use for trees

int main(int argc, char * argv[]) {
  int argIndex = 1;


  ConsolePrintBuffer myConsolePrintBuffer;
  PsodaPrinter::getInstance()->clearBufferList();
  PsodaPrinter::getInstance()->addPrintBuffer(&myConsolePrintBuffer);
  Interpreter::getInstance()->clear();
  
  PSODA_DEBUG = 0;
  PSODA_VERBOSE = 0;
  

#ifndef WIN32
  struct rlimit rlp;
  getrlimit(RLIMIT_DATA, &rlp);

  PSODA_maxmemory = rlp.rlim_cur; // The max java vm memory or OS memory we can use for trees
//  printf("Memory Limit %f\n",PSODA_maxmemory);
#endif

  while( argc >= 2 &&
	 argIndex < argc){
    
    // The next argument should be a hex value to put in the flags
    if(strcmp(argv[argIndex],"-f") == 0) {
      if( argc > argIndex + 1 &&
	  argv[argIndex + 1][0] != '-'){
	AlgorithmFlags = strtol(argv[argIndex+1],0,16);
	printf("Turning on algorithm flags = 0x%X\n",AlgorithmFlags);
	// Make it look like the flags field wasnt here
	for( int i = argIndex + 2; i < argc; i++){
	  argv[i - 2] = argv[i];
	}
	argc -= 2;
      }else{
	printf("Note, algorithm flags unchanged (= 0x%X)\n",AlgorithmFlags);
	for( int i = argIndex + 1; i < argc; i++){
	  argv[i - 1] = argv[i];
	}
	argc -= 1;
      }
    }else if(strcmp(argv[argIndex],"-v") == 0){
      PSODA_VERBOSE = 1;
      bool hasNumber = true;
      if( argc <= argIndex + 1){
	hasNumber = false;
      }
      for( int i = 0; hasNumber == true && i < (int)strlen( argv[argIndex + 1]); i++){
	if( argv[argIndex + 1][0] < '0' ||
	    argv[argIndex + 1][0] > '9'){
	  hasNumber = false;
	  break;
	}
      }
      if( hasNumber ){
	PSODA_VERBOSE = atoi( argv[ argIndex + 1]);
	// Make it look like the flags field wasnt here
	for( int i = argIndex + 2; i < argc; i++){
	  argv[i - 2] = argv[i];
	}
	argc -= 2;
      }else{
	for( int i = argIndex + 1; i < argc; i++){
	  argv[i - 1] = argv[i];
	}
	argc -= 1;
      }
      printf("Setting PSODA_VERBOSE level to %d\n", PSODA_VERBOSE);
    }else if(strcmp(argv[argIndex],"-d") == 0){
      PSODA_DEBUG = 1;
      bool hasNumber = true;
      if( argc <= argIndex + 1){
	hasNumber = false;
      }
      for( int i = 0; hasNumber == true && i < (int)strlen( argv[argIndex + 1]); i++){
	if( argv[argIndex + 1][0] < '0' ||
	    argv[argIndex + 1][0] > '9'){
	  hasNumber = false;
	  break;
	}
      }
      if( hasNumber ){
	PSODA_DEBUG = atoi( argv[ argIndex + 1]);
	// Make it look like the flags field wasnt here
	for( int i = argIndex + 2; i < argc; i++){
	  argv[i - 2] = argv[i];
	}
	argc -= 2;
      }else{
	for( int i = argIndex + 1; i < argc; i++){
	  argv[i - 1] = argv[i];
	}
	argc -= 1;
      }
      printf("Setting PSODA_DEBUG level to %d\n", PSODA_DEBUG);
    }else if(strncmp(argv[argIndex],"-h", 2) == 0 ||
	     strncmp(argv[argIndex],"--h", 3) == 0){
      PsodaPrinter::getInstance()->write("\nUSAGE: <nexus file> [-v[ <number>]] [-d[ <number>]]\n\n");
      return 0;
    }else{
      argIndex++;
    }
  }

// Set up the psodaHome global variable to point to the base of the installation
	char *rval = getenv("PSODA_HOME");
	if(rval != NULL) {
		psodaHome = rval;
		printf("PSODA_HOME set to %s\n",psodaHome);
	}
  
  if( argc == 1){
    char noName[1] = { '\0' };
    return PsodaRunner::run(noName);
  } else if ( argc > 1 ) {
    char* filename = argv[1];
    return PsodaRunner::run(filename);
  }

}

