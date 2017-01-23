#include "PsodaSourceParser.h"
#include "PaupParser.h"
#include "ParsingInfo.h"
#include "PsodaPrinter.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>

/**
 * Parses a file
 *
 * @param inputfile The name of the file to be parsed
 */
int PsodaSourceParser::parseFilename(const char* dir, const char* inputfile, PsodaBlocks& blocksVector, Environment* userDefinedCommandsToPopulate, ParsingInfo* baseParsingInfo) {
  ParsingInfo pInfo(dir, inputfile, blocksVector, userDefinedCommandsToPopulate, baseParsingInfo); // create a new parsing info on the stack
  pInfo.pushCommand(new CommandNode());
  string pathToFile = pInfo.getPathToFile();
  paupLexlex_init(&pInfo.getScanner());
  
  if( PSODA_VERBOSE){
    PsodaPrinter::getInstance()->write("Reading %s\n" , pathToFile.c_str() ) ;
  }
  
  struct stat buf;
  stat(pathToFile.c_str(), &buf);
  if (access(pathToFile.c_str(), F_OK | R_OK) < 0 || !S_ISREG(buf.st_mode)) {
    paupLexlex_destroy(pInfo.getScanner());
    pInfo.getScanner() = NULL;
    ostringstream message;
    message << "Invalid input file: " << pInfo.getPathToFile() << endl;
    throw PsodaError(message.str());
    return -1;
  } else {
    FILE* fileIn = fopen(pathToFile.c_str(), "r");
    paupLexset_in(fileIn, pInfo.getScanner());
    paupLexparse(&pInfo);
    paupLexlex_destroy(pInfo.getScanner());
    pInfo.getScanner() = NULL;
    fclose(fileIn);
    return 0;
  }
}

/**
 * Parses a string
 */
int PsodaSourceParser::parseBuffer(const char* baseDir, const string& source, PsodaBlocks& blocksVector, Environment* userDefinedCommandsToPopulate, ParsingInfo* baseParsingInfo) {
  ParsingInfo pInfo(baseDir, "", blocksVector, userDefinedCommandsToPopulate, baseParsingInfo); // create a new parsing info on the stack
  paupLexlex_init(&pInfo.getScanner());
  pInfo.pushCommand(new CommandNode());
  string pathToFile = pInfo.getPathToFile();
  FILE* tmpFile = tmpfile();
  fputs(source.c_str(), tmpFile);
  rewind(tmpFile);
  paupLexset_in(tmpFile, pInfo.getScanner());
  int parserResultCode = paupLexparse(&pInfo);
  paupLexlex_destroy(pInfo.getScanner());
  pInfo.getScanner() = NULL;
  fclose(tmpFile);
  tmpFile = NULL;
    /*
  int sourceLength = source.length();
  int lengthWithTwoEnddingNullChars = sourceLength + 2;
  char *buf = new char[lengthWithTwoEnddingNullChars];
  memcpy(buf, source.data(), sourceLength);
  memset(buf + sourceLength, '\0', 2);
  //  YYLTYPE yylloc;
  //  yylloc.first_line = yylloc.last_line = 1;
  //  yylloc.first_column = yylloc.last_column = 0;
  //  paupLexset_lloc(&yylloc, pInfo.getScanner());
  //  paupLexset_in(NULL, pInfo.getScanner());
  paupLex_scan_buffer(const_cast<char*>(buf), lengthWithTwoEnddingNullChars, pInfo.getScanner());
  paupLexset_extra(&pInfo, pInfo.getScanner());
  int parserResultCode = paupLexparse(&pInfo);
  paupLexlex_destroy(pInfo.getScanner());
  pInfo.getScanner() = NULL;
  delete [] buf;
    */
  return parserResultCode;
}
