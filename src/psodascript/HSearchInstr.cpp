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
#include "HSearchInstr.h"
#include "Interpreter.h"
#include "PsodaWarning.h"
#include "PsodaPrinter.h"

#include "InteractiveInstr.h"

#include "QSearchBase.h"
#include "QRetainedResultSearch.h"
#include "QTBR.h"
#include "RTBR.h"
#include "DPTBR.h"
#include "QTree.h"
#include "QStep.h"
#include "SectorStep.h"
#include "PartialTreeSearch.h"
#include "Dist.h"
#include "NJ.h"
#include "Timer.h"
#include "TreeResolve.h"
#include "Sectarian.h"
#include "AlternatingSearch.h"
#include "CommonCladeRefinement.h"
#include "EvaluatorBase.h"
#include "Globals.h"
#include "TreeFusion.h"


#ifdef GUI
#include "PSODA.h"
#endif

using namespace std;

HSearchInstr::HSearchInstr() : BuiltInCommand() {
  setDescription("Heuristic Search for Phylogenetic Trees with optimal score.");
  initDefaultValue("start", "stepwise", "Where should the starting tree for the HSearch come from: stepwise, nj=neighbor joining, upgma, current");
  //addParamOption("start", "random");
  addParamOption("start", "nj");
  addParamOption("start", "upgma");
  addParamOption("start", "current");
  addParamOption("start", "tf");
  addParamOption("start", "sector");
  addParamOption("start","pts");
  initDefaultValue("criterion", "parsimony", "Criterion can be \"likelihood\" or \"parsimony\"");
  addParamOption("criterion", "likelihood");
  initDefaultValue("nreps", 1,"Number of replicate trees created in stepwise tree creation");
  initDefaultValue("swap", "TBR", "What method should be used for swaping.  Possible values are TBR, sectarian(computes a window over the tree that is 5 brances long and computes the best resolution for those branches, holding everything else constant), resolve (computes a consensus tree, then uses TBR to create a bifurcating tree for all unresolved nodes), none");
  addParamOption("swap", "sectarian");
  addParamOption("swap", "resolve");
  addParamOption("swap", "none");
  initDefaultValue("wantRecursion", "FALSE", "Turn Recursive TBR on.  Find a tree using the most optimal subtrees");
  initDefaultValue("maxTrees", 1000, "The maximum number of trees with the best score to keep in the repository.  This may be limited by the amount of memory on your machine or in the Java Virtual Machine");
  initDefaultValue("iterations", INT_MAX, "The number of iterations to search through.  Each iteration takes one split point in the tree and computes all possible rearrangements of the subtrees.");
  initDefaultValue("aligned", "FALSE", "If this value is TRUE, then run the search even if the dataset is not aligned.  If the dataset is aligned, this parameter will not make a difference.");
}

HSearchInstr::~HSearchInstr() {
  return;
}

void HSearchInstr::execute(Environment* baseEnv) {
  //Interpreter::getInstance()->doCheckParsimony(baseEnv);
  doHeuristicSearch(baseEnv,Interpreter::getInstance()->qtreeRepository());
#ifdef GUI
    PsodaPrinter::getInstance()->write("## HSearch Completed Successfully\n");
#endif
}

void HSearchInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

string HSearchInstr::getName() const {
  return "hsearch";
}

void HSearchInstr::doHeuristicSearch(Environment* env,QTreeRepository* treeRepository) {
  assert(env);

	// This algorithm requires a Dataset and a QTreeRepository
	Dataset* data = Interpreter::getInstance()->dataset();

    // This evaluator is used for Qsearch
    EvaluatorBase *evaluator;
    // If we did a getTrees, then the repository will have trees in it from the file

	string evaluatorTypeName = env->lookupString("criterion");
	string initialTreeSearchTypeName = env->lookupString("start");
    string dataIsAligned = env->lookupString("aligned");
	QSearchBase *qsearch = NULL;
	QSearchBase *qinitialTreeSearch = NULL;
	QAlgorithmBase *qfinalsearchAlgorithm = NULL;
	QAlgorithmBase *qinitalsearchAlgorithm = NULL;
	bool doInitialSearch = false;
	bool doMainSearch = false;
	int iterations = 0;
	int nreps = 0;
	

	if (!data) {
#ifdef GUI
	  PsodaPrinter::getInstance()->write("Cannot perform hsearch without a current dataset.  Select a data file to open.");
	  string guifilename = Java_gui_PSODA_guiFindFile();
	  if (guifilename != "") {
	    string loadDataCode = "loaddata file = \"";
	    loadDataCode += guifilename;
	    loadDataCode += "\";";
	    PsodaPrinter::getInstance()->write("Running loaddata command: %s", loadDataCode.c_str());
	    InteractiveInstr::executeCode(loadDataCode);
	    data = Interpreter::getInstance()->dataset();
	  }
#endif
	}
	if (!data) {
	  throw PsodaWarning("Cannot perform hsearch without a current dataset.  Did you forget to give the data?  Skipping hsearch.");
	}

    //fprintf(stderr,"dataIsAligned = %s\n", dataIsAligned.c_str());
    if (dataIsAligned == "TRUE" || dataIsAligned == "true")
    {
        // Set the data format to Dataset::ALIGNED_DATAFORMAT
        data->dataformat() = Dataset::ALIGNED_DATAFORMAT;
    }

	// Check the data.  if it is not aligned, align it
	if (data->dataformat() != Dataset::ALIGNED_DATAFORMAT) {
	  string alignInstrCode = "align;";
	  PsodaPrinter::getInstance()->write("Current dataset is unaligned.  Running align command: %s", alignInstrCode.c_str());
	  InteractiveInstr::executeCode(alignInstrCode);
	  data = Interpreter::getInstance()->dataset();
	  if (data->dataformat() != Dataset::ALIGNED_DATAFORMAT) {
	    throw PsodaWarning("Cannot perform hsearch on unaligned dataset. Skipping hsearch.");
	  }
	}


	/*
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 * Hyrum's temporary solution.  Other solutions include requesting a compressed binary dataset, etc
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 *
	 */
	assert(data->dataformat() == Dataset::ALIGNED_DATAFORMAT);









	

	
	// Q - We pass an algorithm, evaluator and tree repository to a search
	// The search will return a repository with the results of the search in it

	// Create an evaluator.  This also initializes the datasets
	evaluator = Interpreter::getInstance()->createEvaluator( evaluatorTypeName );

  if(env->lookupString("NReps") != "") {
    nreps = atoi(env->lookupString("NReps").c_str());
  } else {
    nreps = 1;
  }
  //printf("\n nreps %d\n",nreps);

  //Setup algorithm
  if ( env->lookupString("swap") == "ptbr" )
  {
  }
  else if ( env->lookupString("swap") == "rtbr" )
  {
	/*
    printf( "RECURSIVE TBR" );
    StringLiteral searchStyle("PAUP*");
    env->set("searchStyle", &searchStyle);
    StringLiteral wantRecursion("TRUE");
    env->set("searchStyle", &wantRecursion);
	// If someone put in a set maxtrees command
	int maxtrees;
	if (env->lookupString("maxTrees") != "") {
	  maxtrees = atoi(env->lookupString("maxTrees").c_str());
	} else {
	  maxtrees = 1;
	}
	qtreeRepository()->maxTrees() = maxtrees;
	*/
	doMainSearch = true;
  }
  else if ( env->lookupString("swap") == "sectarian" )
  {
	doMainSearch = true;
  }
  else if ( env->lookupString("swap") == "none" )
  {
      printf("Swap NONE, not doing main search\n");
      doMainSearch = false;
  }
  else // Normal TBR
  {
	// If someone put in a set maxtrees command
	int maxtrees;
	if (env->lookupString("maxTrees") != "") {
	  maxtrees = atoi(env->lookupString("maxTrees").c_str());
	} else {
	  maxtrees = 1000;
	}
	doMainSearch = true;
	treeRepository->setMaxTrees(maxtrees, data->ntaxa());
  }
  //Setup initialTreeSearch
  //printf("InitialTreeSearchTypeName is [%s]\n", initialTreeSearchTypeName);
  if ( initialTreeSearchTypeName == "current" || initialTreeSearchTypeName == "1" )
  {
    qinitialTreeSearch = NULL;
    doInitialSearch = false;
  }
  else if ( initialTreeSearchTypeName == "stepwise" )
  {
    qinitialTreeSearch = new QStepwiseAdditionSearch();
    doInitialSearch = true;

  }
  else if ( initialTreeSearchTypeName == "sector" )
  {
    qinitialTreeSearch = new QSectorStepwiseAdditionSearch();
    doInitialSearch = true;

  }
  else if ( initialTreeSearchTypeName == "pts" )
  {
    qinitialTreeSearch = new PartialTreeSearch();
    doInitialSearch = true;

  }
  else if (initialTreeSearchTypeName == "nj" )
  {
    qinitialTreeSearch = new NJSearch();
    doInitialSearch = true;
  } 
  else if (initialTreeSearchTypeName == "upgma" )
  {
    qinitialTreeSearch = new DistSearch();
    doInitialSearch = true;
  } 
  else if (initialTreeSearchTypeName == "dptbr" )
  {
    qinitialTreeSearch = new DPTBR();
    doInitialSearch = true;
  } 
  else if (initialTreeSearchTypeName == "tf" )
  {
    qinitialTreeSearch = new TreeFusionSearch();
    doInitialSearch = true;
  } 
  else 
  {
    qinitialTreeSearch = NULL;
    doInitialSearch = false;
  }


  if (env->lookupString("swap") == "resolve")
	qsearch = new TreeResolveSearch();
  else if (env->lookupString("swap") == "alt-sectarian")
    qsearch = new AlternatingSearch();
  else if (env->lookupString("swap") == "ccr")
	qsearch = new CommonCladeRefinement();
  else
	qsearch = new QRetainedResultSearch();
  
  if ( env->lookupString("swap") == "rtbr" )
	qfinalsearchAlgorithm = new RTBR;
  else if ( env->lookupString("swap") == "sectarian" )
	qfinalsearchAlgorithm = new SectarianSearch;
  else
	qfinalsearchAlgorithm = new QTBR;
  
  qinitalsearchAlgorithm = new QTBR;
  /*
  printf("Weights-->");
  for (int i = 0; i < data->nchars(); i++)
  {
  	printf(" %d:%d ", i, search->evaluator()->data->weights()->operator[](i));
  }
  printf("\n");
  */

  
// Now call the main search
// Pass the algorithm, tree repository and evaluator to the initial search
    Dataset* parsimonyCompressedMatrix = data;
    if(!parsimonyCompressedMatrix) {
        printf("fatal error:  Dataset not found\n");
        exit(-1);
    }
    // The last parameter is iterations for the retainedResult search, it should probably be replicates here
    if(doInitialSearch)
    {
      Timer timer;
      if( PSODA_VERBOSE){ timer.start(); }
      qinitialTreeSearch->search( *treeRepository,  qinitalsearchAlgorithm, evaluator, nreps );
      if( PSODA_VERBOSE){
        TimerSecondMicros sm = timer.getCurrentSecondMicros();
        PsodaPrinter::getInstance()->write("%s took %li.%li\n",qinitialTreeSearch->name() , sm.seconds , sm.micros );
      }
    }
    if(doMainSearch)
    {
        Timer timer;
		if( PSODA_VERBOSE){ timer.start(); }
        iterations = atoi(env->lookupString("iterations").c_str());
        if(iterations <= 0)
	  	{
            iterations = INT_MAX;
	  	}
        qsearch->search( *treeRepository,  qfinalsearchAlgorithm, evaluator, iterations);
		if( PSODA_VERBOSE){
	  		TimerSecondMicros sm = timer.getCurrentSecondMicros();
        PsodaPrinter::getInstance()->write("%s took %li.%li\n",qsearch->name() , sm.seconds , sm.micros );
		}
    }

    if (qinitialTreeSearch) {
      delete qinitialTreeSearch;
      qinitialTreeSearch = NULL;
    }
    if (qsearch) {
      delete qsearch;
      qsearch = NULL;
    }
    if (qinitalsearchAlgorithm) {
      delete qinitalsearchAlgorithm;
      qinitalsearchAlgorithm = NULL;
    }
	if (qfinalsearchAlgorithm) {
      delete qfinalsearchAlgorithm;
      qfinalsearchAlgorithm = NULL;
    }
    if (evaluator) {
      delete evaluator;
      evaluator = NULL;
    }
	

}
