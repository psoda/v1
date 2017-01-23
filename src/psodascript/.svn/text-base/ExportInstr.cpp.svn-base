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
#include "ExportInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"


using namespace std;

ExportInstr::ExportInstr() : BuiltInCommand() {
  setDescription("Export the dataset with different formats\n");
  initFileDefaultValue("file", "");
  initDefaultValue("format", "fasta", "");
  addParamOption("format", "phylip");
  //addParamOption("format", "nexus");

  return;
}

ExportInstr::~ExportInstr() {
  return;
}

void ExportInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void ExportInstr::execute(Environment* baseEnv) {

    Dataset* dataset = Interpreter::getInstance()->dataset();
    string format = baseEnv->lookupString("format");
    string file = baseEnv->lookupString("file");
    dataset->printSeqs(format, file);

#ifdef GUI
        PsodaPrinter::getInstance()->write("## Export Completed Successfully\n");
#endif
}

string ExportInstr::getName() const {
  return "export";
}
