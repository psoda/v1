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
#include "SectorSearchInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"
#include "OrthoSectorSearch.h"
#include "PartitionSpaceSectorSearch.h"
#include "RandomSectorSearch.h"
#include "NoneSectorSearch.h"
#include "QTBR.h"
#include "Parsimony.h"

using namespace std;

SectorSearchInstr::SectorSearchInstr() : BuiltInCommand() {
  initDefaultValue("start", "stepwise", "Where should the starting trees for the Gradient Descent Search come from");
  initDefaultValue("ptmmin",30,"What is the smallest partial tree for PTM?");
  initDefaultValue("ptmmax",60,"What is the largest partial tree for PTM?");
  initDefaultValue("reps", 10,"How many search replicates should be used?");
  initDefaultValue("sreps", 5,"How many stepwise trees should be used by each replicate?");
  initDefaultValue("gtime", 45,"For how many seconds should global tbr run for each replicate?");
  initDefaultValue("ptime", 15,"For how many seconds should global tbr run with perturbed character weights for each replicate?");
  initDefaultValue("iterations", INT_MAX,"How many stepwise trees should be used by each replicate?");
  initDefaultValue("maxTrees", 1000, "The maximum number of trees with the best score to keep in all repository.  This may be limited by the amount of memory on your machine or in the Java Virtual Machine");
  initDefaultValue("type", "random","How should sectors be selected?");
  addParamOption("type","partition");
  addParamOption("type","ortho");
  addParamOption("type","none");
  initDefaultValue("keepSectorTree", "no","Should all TBRs be considered before changing trees while examining a sector?");
  initDefaultValue("fastSectorExit","no","Should a sector finish as soon as all TBRs on one tree are examined without improvement?");
  initDefaultValue("keepGlobalTree", "no","Should all TBRs be considered before changing trees during global TBR?");
  initDefaultValue("keepPerturbedTree", "no","Should all TBRs be considered before changing trees while the dataset is perturbed?");
  
/*
  initDefaultValue("criterion", "parsimony");
  initDefaultValue("nreps", 1);
  initDefaultValue("swap", "TBR", "What method should be used for swaping");
  initDefaultValue("wantRecursion", "FALSE");
  
  initDefaultValue("iterations", INT_MAX);
*/
}

SectorSearchInstr::~SectorSearchInstr() {
  return;
}

void SectorSearchInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);	
}

void SectorSearchInstr::execute(Environment* env) 
{
	SectorSearchBase* searchType;
	Parsimony eval;
	QTBR tbr;
	
	if ( env->lookupString("type") == "random" )
	{
		searchType = new RandomSectorSearch();
	}
	else if ( env->lookupString("type") == "ortho" )
	{
		searchType = new OrthoSectorSearch();

	}
	else if ( env->lookupString("type") == "partition" )
	{
		searchType = new PartitionSpaceSectorSearch();

	}
	else if ( env->lookupString("type") == "none" )
	{
		searchType = new NoneSectorSearch();
	}
	else
	{
		searchType = new RandomSectorSearch();
	}
	
	if(env->lookupString("ptmmin") != "") 
	{
		searchType->ptm_min() = atoi(env->lookupString("ptmmin").c_str());
	}
	else 
	{
		searchType->ptm_min() = 30;
	}
	
	if(env->lookupString("ptmmax") != "") 
	{
		searchType->ptm_max() = atoi(env->lookupString("ptmmax").c_str());
	}
	else 
	{
		searchType->ptm_max() = 60;
	}
	
	if(env->lookupString("reps") != "") 
	{
		searchType->replicates() = atoi(env->lookupString("reps").c_str());
	}
	else 
	{
		searchType->replicates() = 10;
	}
	
	if(env->lookupString("sreps") != "") 
	{
		searchType->stepwise_replicates() = atoi(env->lookupString("sreps").c_str());
	}
	else 
	{
		searchType->stepwise_replicates() = 10;
	}
	
	if(env->lookupString("gtime") != "") 
	{
		searchType->global_tbr_length() = atoi(env->lookupString("gtime").c_str());
	}
	else 
	{
		searchType->global_tbr_length() = 45;
	}
	
	if(env->lookupString("ptime") != "") 
	{
		searchType->global_perturbed_tbr_length() = atoi(env->lookupString("ptime").c_str());
	}
	else 
	{
		searchType->global_perturbed_tbr_length() = 15;
	}
	
	if(env->lookupString("maxTrees") != "") 
	{
		searchType->repo_size() = atoi(env->lookupString("maxTrees").c_str()) / searchType->replicates();
	}
	else 
	{
		searchType->repo_size() = 1000 / searchType->replicates();
	}
	
	int iter;
	if(env->lookupString("iterations") != "") 
	{
		iter = atoi(env->lookupString("iterations").c_str());
	}
	else 
	{
		iter = 10;
	}
	
	if(env->lookupString("keepSectorTree") != "yes") 
	{
		searchType->sector_keep_tree() = false;
	}
	else 
	{
		searchType->sector_keep_tree() = true;
	}
	
	if(env->lookupString("fastSectorExit") != "yes") 
	{
		searchType->fast_exit_sector_search() = false;
	}
	else 
	{
		searchType->fast_exit_sector_search() = true;
	}
	
	if(env->lookupString("keepGlobalTree") != "yes") 
	{
		searchType->global_keep_tree() = false;
	}
	else 
	{
		searchType->global_keep_tree() = true;
	}
	
	if(env->lookupString("keepPerturbedTree") != "yes") 
	{
		searchType->perturbed_keep_tree() = false;
	}
	else 
	{
		searchType->perturbed_keep_tree() = true;
	}
	
	searchType->search(*Interpreter::getInstance()->qtreeRepository(),&tbr,&eval,iter);
	delete searchType;
}



string SectorSearchInstr::getName() const {
  return "sectorsearch";
}
