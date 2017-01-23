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
#include "AlignmentCommand.h"
#include "StringLiteral.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

//#define COMMAND_HELP_DIR "doc/help/commands/"
//#define INPUT_BUF_SIZE 1024

AlignmentCommand::AlignmentCommand() : BuiltInCommand(), result(NULL) {
  return;
}

AlignmentCommand::AlignmentCommand(AlignmentCommand& old):BuiltInCommand(old) {
  return;
}

AlignmentCommand::~AlignmentCommand()
{}

void AlignmentCommand::setResult(Literal* newResult) {
  if (result) {
    delete result;
    result = NULL;
  }
  result = newResult;
}

Literal* AlignmentCommand::getResult() {
  return result;
}

string AlignmentCommand::getHelp() const {
  ostringstream helpStream;
  helpStream << PsodaCommand::getHelp();
  /*
  string helpFilename = COMMAND_HELP_DIR + StringLiteral::toLowerCase(getName()) + ".txt";
  ifstream helpFile(helpFilename.c_str(), ios::in);
  if (helpFile.good()) {
    char inBuf[INPUT_BUF_SIZE];
    do {
      helpFile.read(inBuf, INPUT_BUF_SIZE);
      int numRead = helpFile.gcount();
      inBuf[numRead] = '\0';
      helpStream << inBuf;
    } while (helpFile.good());
    helpFile.close();
  }
  */
  return helpStream.str();
}

/**
 * Get gap open & gap ext penalties
 */
void AlignmentCommand::getGapSettings(Environment* env, float* gapOpen, float* gapOpenHelix, float* gapOpenBetaStrand, float* gapOpenLoop, float* gapExt, bool* penalizeLeadingGaps, int* gapDistanceSetting){
  
  getGapSettings(env, gapOpen, gapExt, penalizeLeadingGaps, gapDistanceSetting);

  if( env->lookupString("gapopena") == ""){
    *gapOpenHelix = *gapOpen;
  }else{
    *gapOpenHelix = strtod( env->lookupString("gapopena").c_str(), (char**)NULL);
  }

  if( env->lookupString("gapopenb") == ""){
    *gapOpenBetaStrand = *gapOpen;
  }else{
    *gapOpenBetaStrand = strtod( env->lookupString("gapopenb").c_str(), (char**)NULL);
  }

  if( env->lookupString("gapopenl") == ""){
    *gapOpenLoop = *gapOpen;
  }else{
    *gapOpenLoop = strtod( env->lookupString("gapopenl").c_str(), (char**)NULL);
  }


  if( *gapOpenHelix > 0.0){
    *gapOpenHelix *= -1.0;
  }
  if( *gapOpenBetaStrand > 0.0){
    *gapOpenBetaStrand *= -1.0;
  }
  if( *gapOpenLoop > 0.0){
    *gapOpenLoop *= -1.0;
  }


} /* END getGapSettings() */



/**
 * Get gap open & gap ext penalties.
 */
void AlignmentCommand::getGapSettings(Environment* env, float* gapOpen, float* gapExt, bool* penalizeLeadingGaps, int* gapDistanceSetting){
  assert(env);
  Dataset::Datatype dataType = Interpreter::getInstance()->dataset()->datatype();
  
  if( env->lookupString("gapopen") == ""){
    if ( dataType == Dataset::PROTEIN_DATATYPE){
      *gapOpen = -10.0; /* default gap open penalty */
    }else if( dataType == Dataset::DNA_DATATYPE ||
	      dataType == Dataset::NUCLEOTIDE_DATATYPE ||
	      dataType == Dataset::RNA_DATATYPE){
      *gapOpen = -15.0; /* default gap open penalty */
    }else if( dataType == Dataset::CODON_DATATYPE){
      *gapOpen = -15.0; /* future: refine */ /* default gap open penalty */
    }else{
      throw PsodaError("ERROR: Unknown datatype for align() (setting default gapopen penalty");
    }
  }else{
    *gapOpen = strtod( env->lookupString("gapopen").c_str(), (char**)NULL);
  }

  if( env->lookupString("gapext") == ""){
    if ( dataType == Dataset::PROTEIN_DATATYPE){
      *gapExt = -0.2;  /* default gap extension penalty */
    }else if( dataType == Dataset::DNA_DATATYPE ||
	      dataType == Dataset::NUCLEOTIDE_DATATYPE ||
	      dataType == Dataset::RNA_DATATYPE){
      *gapExt = -6.66;  /* default gap extension penalty */
    }else if( dataType == Dataset::CODON_DATATYPE){
      *gapExt = -6.66;  /* future: refine */ /* default gap extension penalty */
    }else{
      throw PsodaError("ERROR: Unknown datatype for align() (setting default gapext penalty");
    }
  }else{
    *gapExt = strtod( env->lookupString("gapext").c_str(), (char**)NULL);
  }
  
  if( *gapOpen > 0.0){
    *gapOpen *= -1.0;
  }
  if( *gapExt > 0.0){
    *gapExt *= -1.0;
  }
  
  /* get endgap (penalizeLeadingGaps) */
  if( env->lookupString("endgap") == "y" ||
      env->lookupString("endgap") == "Y" ||
      env->lookupString("endgap") == "yes" ||
      env->lookupString("endgap") == "YES"){
    *penalizeLeadingGaps = true;
  }else{
    *penalizeLeadingGaps = false;
  }

  if( gapDistanceSetting != NULL){
    if( env->lookupString("gapdist") != ""){
      *gapDistanceSetting = strtol( env->lookupString("gapdist").c_str(), (char**)NULL, 10);
    }
  }
  
} /* END getGapSettings(Environment* env, float* gapOpen, float* gapExt, bool* penalizeLeadingGaps, int* gapDistanceSetting) */




