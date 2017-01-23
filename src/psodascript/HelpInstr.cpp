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
#include "HelpInstr.h"
#include "PsodaPrinter.h"
#include "Interpreter.h"
#include "PsodaCommand.h"

using namespace std;

#define HELP_NAME_PARAM "*name"

HelpInstr::HelpInstr() : BuiltInCommand() {
  setDescription("Prints usage information about a given command (note: the usage for this command differs from the typical name=value paradigm)\nThe help command displays the usage for PsodaScript commands whether BuiltIn or UserDefined.\n\n\nExamples:\n  help \"print\";	[ displays the help information for the \'print\' command]\n  help;			[ displays the help information for the \'help\' command]\n");
  initDefaultValue(HELP_NAME_PARAM, "help", "the name (as a string) of the command for which to print the usage information" );
}

HelpInstr::~HelpInstr() {
  return;
}

void HelpInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
  execute(baseEnv);
}

void HelpInstr::execute(Environment* baseEnv) {
  string commandName = baseEnv->lookup(HELP_NAME_PARAM).toString();
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandName);
  if (command) {
    PsodaPrinter::getInstance()->write("%s", command->getHelp().c_str());
  } else {
    PsodaPrinter::getInstance()->write("%s", getMissingCommandHelp(commandName).c_str());
	  Interpreter::getInstance()->printCommands();
  }
}

string HelpInstr::getName() const {
  return "help";
}

string HelpInstr::getMissingCommandHelp(const string& commandName) {
  return "Help (" + commandName + ") \n\tThis command has not been defined.\n";
}

string HelpInstr::toCode(const map<string, string>& params) const {
  string returnString = getName();
  returnString += " (";
  map<string, string>::const_iterator commandNameFoundItr = params.find(HELP_NAME_PARAM);
  if (commandNameFoundItr != params.end()) {
    returnString += commandNameFoundItr->second;
  }
  returnString += ");";
  return returnString;
}

string HelpInstr::getUsage() const {
  string usage = getName() + " ";
  usage += HELP_NAME_PARAM;
  usage += ";";
  return usage;
}
