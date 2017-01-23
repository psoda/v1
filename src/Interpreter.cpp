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
#include "Interpreter.h"

#include "Data.h"
#include "CommandRegister.h"
#include "Parsimony.h"
#include "FastParsimony.h"
#include "Likelihood.h"
#include "PsodaPrinter.h"
#include "Environment.h"
#include <fstream>
#include <assert.h>
#include "NewickTreeParser.h"
#include "InteractiveInstr.h"
#include "UndefinedVariableException.h"
#include "UndefinedLiteral.h"



Interpreter::Interpreter() :
  mBuiltInCommands(),
  mUserDefinedCommands(&mBuiltInCommands),
  mPopularCommands(),
  mGlobalEnv(),
  mDefaultVarEnv(&mGlobalEnv)
{
  init();
}

Interpreter::Interpreter(const Interpreter& a)
:
	mRefDataset(a.mRefDataset),
	mSSDataset(a.mSSDataset),
	mRefSSDataset(a.mRefSSDataset),
	mQTreeRepository(a.mQTreeRepository),
	mNTP(a.mNTP),
	mBaseDir(a.mBaseDir),
	mAccumulator(a.mAccumulator),
	mGlobalEnv(a.mGlobalEnv),
	mDefaultVarEnv(a.mDefaultVarEnv)
{
}

Interpreter& Interpreter::operator=(const Interpreter& a)
{
	mRefDataset = a.mRefDataset;
	mSSDataset = a.mSSDataset;
	mRefSSDataset = a.mRefSSDataset;
	mQTreeRepository=a.mQTreeRepository;
	mNTP = a.mNTP;
	mBaseDir = a.mBaseDir;
	mAccumulator = a.mAccumulator;
	mGlobalEnv = a.mGlobalEnv;
	mDefaultVarEnv = a.mDefaultVarEnv;
	return *this;
}

Interpreter::~Interpreter() {
  clean();
}

void Interpreter::init() {
  mBuiltInCommandsAreRegistered = false;
  mRefDataset = NULL;
  mSSDataset = NULL;
  mRefSSDataset = NULL;
  mQTreeRepository = new QTreeRepository;
  mAccumulator = new QTreeRepository;
  mNTP = NULL;
  mBaseDir = ".";
  mGlobalEnv.clear();
  mDefaultVarEnv.clear();
  setDefaults();
  initPopularCommands();
}

void Interpreter::initPopularCommands() {
  mPopularCommands.clear();
  mPopularCommands.push_back("loaddata");
  mPopularCommands.push_back("align");
  mPopularCommands.push_back("hsearch");
  mPopularCommands.push_back("help");
  mPopularCommands.push_back("bibliography");
  mPopularCommands.push_back("source");
}

void Interpreter::registerBuiltInCommands() {
  if (!mBuiltInCommandsAreRegistered) {
    mBuiltInCommandsAreRegistered = true;
    CommandRegister::registerCommands();
  }
}

void Interpreter::clean() {

  if (mQTreeRepository) {
    delete mQTreeRepository;
    mQTreeRepository = NULL;
  }

  if (mAccumulator) {
    delete mAccumulator;
    mAccumulator = NULL;
  }

  if (mNTP) {
    delete mNTP;
    mNTP = NULL;
  }

  // Destroy the built-in commands
  std::set<string> definedCommands;
  mBuiltInCommands.getAllReadableNames(definedCommands);
  set<string>::iterator commandNameItr = definedCommands.begin();
  set<string>::iterator commandNameEnd = definedCommands.end();
  for (; commandNameItr != commandNameEnd; commandNameItr++) {
    Data* data = (Data*) &mBuiltInCommands.lookup(*commandNameItr);
    PsodaCommand* commandPointer = (PsodaCommand*) data->getData();
    delete commandPointer;
    commandPointer = NULL;
    mBuiltInCommands.unset(*commandNameItr);
  }

  // Destroy the user-defined commands
  definedCommands.clear();
  mUserDefinedCommands.getAllReadableNames(definedCommands);
  commandNameItr = definedCommands.begin();
  commandNameEnd = definedCommands.end();
  for (; commandNameItr != commandNameEnd; commandNameItr++) {
    Data* data = (Data*) &mUserDefinedCommands.lookup(*commandNameItr);
    PsodaCommand* commandPointer = (PsodaCommand*) data->getData();
    delete commandPointer;
    commandPointer = NULL;
    mUserDefinedCommands.unset(*commandNameItr);
  }

  mBuiltInCommands.clear();
  mUserDefinedCommands.clear();
  mPopularCommands.clear();
  mGlobalEnv.clear();
  mDefaultVarEnv.clear();
  
}
void Interpreter::printCommands() {

	PsodaPrinter::getInstance()->write("Possible Commands Include:\n");
  // Print the built-in commands
  std::set<string> definedCommands;
  mBuiltInCommands.getAllReadableNames(definedCommands);
  set<string>::iterator commandNameItr = definedCommands.begin();
  set<string>::iterator commandNameEnd = definedCommands.end();
  for (; commandNameItr != commandNameEnd; commandNameItr++) {
    Data* data = (Data*) &mBuiltInCommands.lookup(*commandNameItr);
    PsodaCommand* commandPointer = (PsodaCommand*) data->getData();
		if (commandPointer) {
			PsodaPrinter::getInstance()->write("%s", commandPointer->getHelp().c_str());
		} 
  }

}

Interpreter* Interpreter::getInstance() {
  static Interpreter mInstance;
  mInstance.registerBuiltInCommands();
  return &mInstance;
}

void Interpreter::clear() {
  clean();
  init();
  registerBuiltInCommands();
}

void Interpreter::registerCommand(PsodaCommand* command) {
  Data thisCommandHolder(command);
  mBuiltInCommands.set(command->getName(), &thisCommandHolder);
}

set<string> Interpreter::getDefinedCommandNames() {
  set<string> returnSet;
  mUserDefinedCommands.getAllReadableNames(returnSet);
  return returnSet;
}

vector<string>& Interpreter::getPopularCommandNames() {
  return mPopularCommands;		
}

Environment* Interpreter::getUserDefinedCommandEnv() {
  return &mUserDefinedCommands;
}

void Interpreter::doSet(string command, string name, Literal* value) {
  PsodaCommand* thisCommand = getFunction(command);
  if (thisCommand && thisCommand->hasParam(name)) {
    thisCommand->setDefaultValue(name, value);
  }
}

void Interpreter::doSet(string name, Literal* value) {
  set<string> definedCommands;
  mUserDefinedCommands.getAllReadableNames(definedCommands);
  set<string>::iterator commandNameItr = definedCommands.begin();
  set<string>::iterator commandNameEnd = definedCommands.end();
  for (; commandNameItr != commandNameEnd; commandNameItr++) {
    doSet(*commandNameItr, name, value);
  }
}

PsodaCommand* Interpreter::getFunction(string name) const {
  try {
    Data* data = (Data*)&(mUserDefinedCommands.lookup(name));
    return (PsodaCommand*)data->getData();
  } catch (UndefinedVariableException& e) {
    return NULL;
  }
}

void Interpreter::interpret(PsodaBlocks* blocks) {
  assert(blocks);
  //iterate through the blocks and interpret each one
  while (blocks->hasNext()) {
    blocks->next()->interpretBlock();
  }
}

DatasetMap& Interpreter::getDatasetMap() {
  return mDatasetMap;
}

TreeMap& Interpreter::getTreeMap() {
  return mTreeMap;
}

Dataset* Interpreter::dataset() const {
  return mDatasetMap.getCurrent();
}

void Interpreter::setDataset(Dataset* dataset) {
  mDatasetMap.setCurrent(dataset);
}

void Interpreter::installDataset(Dataset* dataset) {
  assert(dataset);
  setDataset(dataset);
}


void Interpreter::setRefDataset(Dataset* dataset){
  mDatasetMap.setD(dataset, "mRefDataset");
  mRefDataset = dataset;
}

void Interpreter::installRefDataset(Dataset* dataset){
  assert(dataset);
  setRefDataset(dataset);
}

Dataset* Interpreter::refDataset() const {
  return mDatasetMap.getDataset("mRefDataset");
}


SSDataset* Interpreter::ssDataset() const {
	//return mSSDataset;
    return (SSDataset*) mDatasetMap.getDataset("mSSDataset");
}

void Interpreter::setSSDataset(SSDataset* ssDataset) {
  mDatasetMap.setD(ssDataset, "mSSDataset");
  mSSDataset = ssDataset;
}

void Interpreter::installSSDataset(SSDataset* ssDataset) {
  assert(ssDataset);
  setSSDataset(ssDataset);
}

SSDataset* Interpreter::refSSDataset() const {
  return (SSDataset*) mDatasetMap.getDataset("mRefSSDataset");
}

void Interpreter::setRefSSDataset(SSDataset* refSSDataset) {
  mDatasetMap.setD(refSSDataset, "mRefSSDataset");
  mRefSSDataset = refSSDataset;
}

void Interpreter::installRefSSDataset(SSDataset* refSSDataset) {
  assert(refSSDataset);
  setRefSSDataset(refSSDataset);
}

QTreeRepository*& Interpreter::qtreeRepository() {
  return mQTreeRepository;
}

QTreeRepository* Interpreter::qtreeRepository() const {
  return mQTreeRepository;
}

void Interpreter::setAccumulator(QTreeRepository* newAccumulator) {
  mAccumulator = newAccumulator;
}

QTreeRepository* Interpreter::getAccumulator() const {
  return mAccumulator;
}

void Interpreter::setBaseDir(const string newBaseDir) {
  mBaseDir = newBaseDir;
}

const string Interpreter::getBaseDir() const {
  return mBaseDir;
}

NewickTreeParser*& Interpreter::NTP() {
  return mNTP;
}






/**
 * @param Evaluator Criteria
 * @return The Proper Typed and Constructed Evaluator
 */
EvaluatorBase* Interpreter::createEvaluator( string criterion ) {

  EvaluatorBase* returnEvaluator = NULL;
  if ( criterion == "parsimony") {
    returnEvaluator = new Parsimony();
  } else if ( criterion == "likelihood" ) {
    returnEvaluator = new Likelihood();
  } else if ( criterion == "fastparsimony" ) {
    returnEvaluator = new FastParsimony();
  }
  else
  {
	 returnEvaluator = new Parsimony();
  }
  return returnEvaluator;
}

void Interpreter::saveTrees(int format, string filename) {
	string fullPathToFile = "";
        // Get the path relative to the base dir
	if (filename[0] != '/' && filename[0] != '\\') {
	  fullPathToFile = mBaseDir;
#if defined(WIN32) && defined(GUI)
	  fullPathToFile += "\\";
#else
	  fullPathToFile += "/";
#endif
	}
	fullPathToFile += filename;
	PsodaPrinter::getInstance()->write("Filename: %s\n", fullPathToFile.c_str());

	ofstream out;
	out.open((const char*) fullPathToFile.c_str(), ios::trunc);

	if (!out.bad())
	{
		if(format == NEXUS_TREE_FORMAT){
        		qtreeRepository()->print(out);
		}
		else{
			qtreeRepository()->printNewick(out);
		}
	}
	else
	{
		Debug(0,0,"Unable to open stream for saving trees\n");
	}
	out.close();
}

void Interpreter::printMatrix(Environment* env) {
  assert(env);
	string filename = env->lookupString("file");
	if (filename == "")
	{
	  filename="matrix.txt";
	}
	ofstream out(filename.c_str());
	dataset()->printMatrix();
	out.close();
}

void Interpreter::printScores(Environment* env, const string metric) {
  assert(env);
  string type = env->lookupString("PASSTYPE");
    double treeScore;
    QTree *nextTree;
    // Set up the dataset for qscore
    ofstream out;

    //Dataset* compressedMatrix = dataset();
    //assert(compressedMatrix);

    // End of Set up the dataset for qscore

    if (env->lookupString("file") != "")
    {
      out.open(env->lookupString("file").c_str(),ios::trunc);
    }
    EvaluatorBase* eval = createEvaluator(metric.c_str());

    const deque<QTree *>* treeList = qtreeRepository()->getTrees();
    deque<QTree *>::const_iterator treeIt;

    int i = 0;
    if( treeList->size() > 0){
	for(treeIt = treeList->begin(); treeIt != treeList->end(); treeIt++){
	    nextTree = *treeIt;
	    treeScore = eval->qscoreTree(nextTree);
	    PsodaPrinter::getInstance()->write("score %f\n",treeScore);
	    // printf("qscore %f\n",treeScore);
	    if (type == "single" || type == "") // If we need to write to a file
	    {
		if (env->lookupString("file") != "")
		{
		    out<<i++<<"\t"<<treeScore<<endl;
		}
	    }
	}
    }
    if (type == "single" || type == "")
        out.close();
    if (eval) {
      delete eval;
      eval = NULL;
    }
}

// print scores for all trees in the repository
void Interpreter::pscores(Environment* env) {
  printScores(env, "parsimony");
}

void Interpreter::lscores(Environment* env) {
  printScores(env, "likelihood");
}

void Interpreter::getTrees(Environment* env) {
  assert(env);
    qtreeRepository()->setMaxTrees(INT_MAX, dataset()->ntaxa());
	NTP()= new NewickTreeParser(qtreeRepository());


	// kpr_printf("Interpreter::getTrees NewickTreeParser created\n");

	string fullPathToFile = "";
	string treesFile = env->lookupString("file");
	//cout << "getTrees treesFile "<< treesFile << " BaseDir "<<mBaseDir<<"\n";
	if (treesFile[0] != '/' && treesFile[0] != '\\') {
	  fullPathToFile += mBaseDir;
#if defined(WIN32) && defined(GUI)
	  fullPathToFile += "\\";
#else
	  fullPathToFile += "/";
#endif
	}
	fullPathToFile += treesFile;
	string fileName = fullPathToFile;
	//cout << "getTrees fullPath "<< fullPathToFile << " fileName "<<fileName<<"\n";
	
	if (fileName == "")
	{
		PsodaPrinter::getInstance()->writeError("GetTrees::Error No filename given\n");
		return;
	}

	int ntpParseError = NTP()->parseFilename( fileName.c_str() );
	if (ntpParseError) {
	  throw PsodaException( "Error Parsing Tree File" );
	}
        //NTP()->qtree->print();
    // The NTP is passed the tree repository.  He should put the trees in,
    //  so we dont need to do it here.
	// qtreeRepository()->addTree(NTP()->qtree);

}

void Interpreter::setDefaults() {
  getGlobalEnv()->setString("searchStyle", "PAUP*");
  getGlobalEnv()->setString("endl", "\n");
  getGlobalEnv()->setString("endline", "\n");
  getGlobalEnv()->set("undef", UndefinedLiteral::getInstance());
}

Environment* Interpreter::getGlobalEnv() {
  return &mGlobalEnv;
}

Environment* Interpreter::getDefaultVarEnv() {
  return &mDefaultVarEnv;
}

//bool isStepwise;



