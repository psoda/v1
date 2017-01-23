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
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#define MAKEFILE_IN "Commands.am"
#define INSTRUCTION_HEADER_OUT "Instructions.h"
#define REGISTRY_OUT "RegisterInstructions.h"
#define REGISTRY_LINE_SIZE 100

using namespace std;

int main(char* argv[] __attribute__((unused)), int argc  __attribute__((unused))) {
  // open the .h file and parse out the commands
  ifstream registryFile(MAKEFILE_IN);
  char thisLine[REGISTRY_LINE_SIZE];
  int lineNum = 0;
  ostringstream instructionsHeaderOutStream;
  ostringstream registerCommandsOutStream;
  ostringstream makefileOutStream;
  makefileOutStream << "psodacommands = ";
  const char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  while (registryFile.good()) {
    lineNum++;
    registryFile.getline(thisLine, REGISTRY_LINE_SIZE);
    string thisLineString = thisLine;

    // Skip the first line that says psodacommands = 
    if (thisLineString.find('=') != string::npos) continue;

    // Skip any lines that don't have a class name
    string::size_type firstChar = thisLineString.find_first_of(alphabet);
    if (firstChar == string::npos) continue;

    // If there isn't a .cpp that is an error
    string::size_type lastChar = thisLineString.find_first_not_of(alphabet, firstChar);
    if (lastChar == string::npos) {
	cerr << "Error building command registry (line " << lineNum << ")" << endl;
	registryFile.close();
	exit(1);
    }

    string className = thisLineString.substr(firstChar, lastChar - firstChar);
    if (className == "") continue;
    else {
      instructionsHeaderOutStream << "#include \"" << className << ".h\"" << endl; 
      registerCommandsOutStream << "Interpreter::getInstance()->registerCommand(new " << className << ");" << endl; 
    }
  }
  makefileOutStream << endl;
  registryFile.close();

  ofstream registryOut(REGISTRY_OUT);
  registryOut << registerCommandsOutStream.str();
  registryOut.close();

  ofstream instructionsHeaderOut(INSTRUCTION_HEADER_OUT);
  instructionsHeaderOut << instructionsHeaderOutStream.str();
  instructionsHeaderOut.close();

}
