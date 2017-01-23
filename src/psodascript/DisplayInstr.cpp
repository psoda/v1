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
#include "DisplayInstr.h"
#include "Interpreter.h"
#include "PsodaWarning.h"
/*Visualization Styles*/
#include "MDS.h"
#include "Hypersphere.h"
/*Terminal Styles*/
#include "gnuPlotTerminal.h"
#include "AnalysisTerminal.h"

using namespace std;

DisplayInstr::DisplayInstr() : BuiltInCommand() {
  setDescription("Display a visualization of the tree space\n");
	initDefaultValue("style","MDS","Which display style should be used");
	addParamOption("style","Hypersphere");
	initDefaultValue("terminal","gnuplot","How should the visualization be displayed");
	addParamOption("terminal","gui");
	addParamOption("terminal","analysis");
}

DisplayInstr::~DisplayInstr() {
  return;
}

void DisplayInstr::execute(Environment* baseEnv) {
	VisualizationTerminal* term = NULL;
	
	if(baseEnv->lookupString("terminal") == "gnuplot")
	{
		term = new gnuPlotTerminal();
	}
	if(baseEnv->lookupString("terminal") == "analysis")
	{
		term = new AnalysisTerminal();
	}
	
	
	if(term == NULL)
	{
		printf("Invalid terminal %s, default gnuplot terminal used\n",(baseEnv->lookupString("terminal")).c_str());
		term = new gnuPlotTerminal();
	}	
	
	term->setOutput((char*)"Terminal.dat");
	
	VisualizationBase* vis = NULL;
	
	if(baseEnv->lookupString("style") == "mds")
	{
		vis = new MDS();
	}
	if(baseEnv->lookupString("style") == "hypersphere")
	{
		vis = new Hypersphere();
	}
	if(vis == NULL)
	{
		vis = new MDS();
		printf("Invalid style %s, default MDS visualization used\n",(baseEnv->lookupString("style")).c_str());
	}
	
	vis->AddTrees(Interpreter::getInstance()->qtreeRepository());
	term->display(vis->visualization());
	
  return;
}

void DisplayInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

string DisplayInstr::getName() const {
  return "display";
}
