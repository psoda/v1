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
#include "UPGMAInstr.h"
#include "QSearchBase.h"
#include "Dist.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"


using namespace std;

UPGMAInstr::UPGMAInstr() : BuiltInCommand() {
  setDescription("Generate a UPGMA tree.");
  return;
}

UPGMAInstr::~UPGMAInstr() {
  return;
}

void UPGMAInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void UPGMAInstr::execute(Environment* baseEnv __attribute__((unused)) ) 
{
    QTreeRepository* treeRepository = Interpreter::getInstance()->qtreeRepository();
    QSearchBase *search = new DistSearch();
    search->search(*treeRepository, NULL, NULL, 1);
#ifdef GUI
        PsodaPrinter::getInstance()->write("## UPGMA Completed Successfully\n");
#endif
}

string UPGMAInstr::getName() const {
  return "upgma";
}
