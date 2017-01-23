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
#ifndef PSODA_INTERPRETER_H
#define PSODA_INTERPRETER_H

#include "QTreeRepository.h"
#include "PsodaBlocks.h"
#include "Expression.h"
#include "DatasetMap.h"
#include "TreeMap.h"
#include <map>
#include <stack>

using namespace std;

class Dataset;
class SSDataset;
class SearchBase;
class EvaluatorBase;
class Tree;
class StandardTreeRepository;
class QTreeRepository;
class NewickTreeParser;
class PrintBuffer;
class PsodaCommand;

class Interpreter {
  
 private:
  /**
   * Initializes the interpreter
   */
  void init();

  /**
   * Destroys all stored interpreter data (called by the destructor)
   */
  void clean();

  /**
   * Initializes the list of popular commands
   */
  void initPopularCommands();

  /**
   * Default Constructor
   */
  Interpreter();
  Interpreter(const Interpreter&);
  
  Interpreter& operator=(const Interpreter&);
	
 public:
  /**
   * Destructor
   */
  virtual ~Interpreter();

  /**
   * Reinitialzes the interpreter as if it were newly created
   */
  void clear();

  /**
   * Returns a reference to a static Singleton interpreter
   * (the interpreter is created if null)
   *
   * @return A reference to the Singleton Interpreter
   */
  static Interpreter* getInstance();

  /**
   * Adds the given command to the builtInCommands environment
   */
  void registerCommand(PsodaCommand* command);

  /**
   * Print all of the current commands
   */
  void printCommands();

  /**
   * Gets the interpreter's default variable environment
   * 
   * @return A pointer to the interpreter's global variable env environment
   */
  Environment* getDefaultVarEnv();

  /**
   * Gets the interpreter's global scope environment
   * 
   */
  Environment* getGlobalEnv();
  
  /**
   * Sets the map of currently available UserDefinedInstructions
   */
  void setFunctions(const map<string, PsodaCommand*>*);

  /**
   * Gets a function
   *
   * @return A pointer to the UserDefinedInstruction with the given name (NULL if not found)
   */
  PsodaCommand* getFunction(string name) const;

  /**
   * Runs the provided blocks through the interpreter
   *
   * @param blocks      The blocks to be interpreted
   */
  void interpret(PsodaBlocks* blocks);

  /**
   * Sets the base directory.  PSODA will look for external files (referenced in the nexus file)
   * relative to this directory.  Typically this should be the directory of the file that was parsed.
   *
   * @param newBaseDir The new base directory
   */
  void setBaseDir(const string newBaseDir);

  /**
   * Get the directory of the file that was parsed
   *
   * @return The directory of the file that was parsed
   */
  const string getBaseDir() const;

  /**
   * Sets any general default values
   */
  void setDefaults();

  virtual EvaluatorBase* createEvaluator( string criterion );

  NewickTreeParser*& NTP();

  QTreeRepository*& qtreeRepository();
  QTreeRepository* qtreeRepository() const;

  /**
   * Set a new Accumulator
   */
  void setAccumulator(QTreeRepository* newAccumulator);

  /**
   * Access the Accumulator
   */
  QTreeRepository* getAccumulator() const;

  //  string getParameter( const char* name ) const;

  DatasetMap& getDatasetMap();
  TreeMap& getTreeMap();

  void setDataset(Dataset* dataset);
  void installDataset(Dataset* dataset);
  Dataset* dataset() const;

  void setRefDataset(Dataset* dataset);
  void installRefDataset(Dataset* dataset);
  Dataset* refDataset() const;
  
  void setSSDataset(SSDataset* ssDataset);
  void installSSDataset(SSDataset* ssDataset);
  SSDataset* ssDataset() const;
  
  void setRefSSDataset(SSDataset* refSSDataset);
  void installRefSSDataset(SSDataset* refSSDataset);
  SSDataset* refSSDataset() const;
  


  

  
  /**
   * Set the default value on the given command
   */
  void doSet(string command, string name, Literal* value);

  /**
   * Set the default value on all commmands that recognive the given parameter name
   */
  void doSet(string name, Literal* value);

  
 

  void printMatrix(Environment* env);
  void printScores(Environment* env, const string);
  void pscores(Environment* env);
  void lscores(Environment* env);
  void getTrees(Environment* env);
  void saveTrees(int format, string filename);

  /**
   * Registers each of the built in commands in "InstructionRegistry.h"
   */
  void registerBuiltInCommands();

  /**
   * Returns a pointer to the userDefinedCommands environment (which extends the builtInCommands environment)
   */
  Environment* getUserDefinedCommandEnv();

  /**
   * Returns a set containing the names of all currently defined commands
   */
  set<string> getDefinedCommandNames();

  /**
   * Returns a set containing the names of the popular commands
   */
  vector<string>& getPopularCommandNames();

 private:

  Dataset* mRefDataset;   /* reference data set (for metrics) */
  SSDataset* mSSDataset;  /* secondary structure data set */
  SSDataset* mRefSSDataset;  /* secondary structure reference data set */
  

  QTreeRepository* mQTreeRepository;
  NewickTreeParser* mNTP;

  /**
   * The directory of the file being run
   * (all references to external files should be resolved relative to this path)
   */
  string mBaseDir;

  /**
   * An extra QTreeRepository to hold on to trees
   */
  QTreeRepository* mAccumulator;

  /**
   * Holds the built in commands
   */
  Environment mBuiltInCommands;

  /**
   * Holds the user defined commands and extends the built in commands
   */
  Environment mUserDefinedCommands;

  /**
   * A list of commands that should be listed before the other commands (for instance in the gui's commands menu)
   */
  vector<string> mPopularCommands;

  /**
   * The global environment holds global variables
   */
  Environment mGlobalEnv;
    
  /**
   * The InteractiveInstr should use this varEnv if an environment isn't provided 
   */
  Environment mDefaultVarEnv;
    
  /**
   * is set to true after the built in commands are registered
   */
  bool mBuiltInCommandsAreRegistered;

  /**
   * A map containing parsed datablocks that can be identified by name and used
   */
  DatasetMap mDatasetMap;
  
  /**
   * A map containing parsed trees that can be identified by name and used
   */
  TreeMap mTreeMap;

};

extern bool isStepwise;
#endif

