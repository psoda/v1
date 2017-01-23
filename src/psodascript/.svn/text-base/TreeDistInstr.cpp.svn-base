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
#include "TreeDistInstr.h"
#include "Interpreter.h"
#include "PsodaWarning.h"
#include "RF.h"

using namespace std;

TreeDistInstr::TreeDistInstr() : BuiltInCommand() {
    initDefaultValue("dummy", "notused", "This is just here for fun");

}

TreeDistInstr::~TreeDistInstr() {
  return;
}

void TreeDistInstr::execute(Environment* baseEnv) {
	
	QTreeRepository* treeRepository = Interpreter::getInstance()->qtreeRepository();
	RF dist_mat(treeRepository);
	printf("\t");
	for(int i=0;i<dist_mat.numTrees();i++)
		printf("%d ",i+1);
	printf("\n");
	
	for(int i=0;i<dist_mat.numTrees();i++)
	{
		printf("%d\t",i+1);
		for(int j=0;j<i;j++)
		{
			printf("%d ",dist_mat(i,j));
		}
		printf("-\n");
	}
  return;
}

void TreeDistInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

string TreeDistInstr::getName() const {
  return "treedist";
}
