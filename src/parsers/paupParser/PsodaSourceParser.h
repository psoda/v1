#ifndef PSODA_SOURCE_PARSER_H
#define PSODA_SOURCE_PARSER_H

#include "ParsingInfo.h"

using namespace std;

class PsodaSourceParser
{
  
 public:
  
  static int parseFilename(const char* dir, const char* inputfile, PsodaBlocks& blocksVector, Environment* userDefinedCommandsToPopulate, ParsingInfo* baseParsingInfo = NULL);
  static int parseBuffer(const char* baseDir, const string& source, PsodaBlocks& blocksVector, Environment* userDefinedCommandsToPopulate, ParsingInfo* baseParsingInfo = NULL);
  
};

#endif

