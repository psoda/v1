#ifndef PARSING_INFO_H
#define PARSING_INFO_H

#include "Location.h"
#include "PsodaBlocks.h"
#include "DataBlock.h"
#include "ProgramBlock.h"
#include "TreeBlock.h"
#include "CommandNode.h"
#include "CallExpression.h"
#include "UserDefinedCommand.h"
#include "PsodaProgram.h"
#include "Dataset.h"
#include "IfElse.h"
#include "WhileLoop.h"
#include "PsodaConstruct.h"
#include "WeightsData.h"
#include "Data.h"
#include "Environment.h"
#include <stack>
#include <sstream>

using namespace std;

#define SET_LOCATION(loc,elem) if (elem) { (elem)->setLocation(loc); }

/**
 * Holds most state information for the parser
 */
class ParsingInfo {

 private:
  
  /**
   * The scanner
   */
  void* yyscanner;

  /**
   * This keeps track of recursive includes so that we can check if the file we are including is already in the process of being parsed
   */
  ParsingInfo* baseParsingInfo;

  /**
   * The path to this file's directory
   */
  string pathToDir;

  /**
   * The name of the file being parsed
   */
  string filename;

  /**
   * The current location in the file
   */
  Location currentLocation;

  /**
   * A pointer to the PsodaBlocks that are being populated during this parse
   */
  PsodaBlocks* blocks;

  /**
   * A stack of the open constructs with the innermost construct on the top
   */
  stack<PsodaConstruct*> constructStack;

  /**
   * A stack of the open command with the innermost CommandNode on the top
   * There can be "nested" CommandNodes in the sense that parameters that are passed in
   * can be call expressions which each hold a CommandNode
   */
  stack<CommandNode*> commandStack;

  /**
   * Holds the open CallExpressions
   */
  stack<CallExpression*> callExpressions;

  /**
   * This stack holds copies of identifier parsed from the input file until we need them
   */
  stack<char*> identStack;

  /**
   * The top element of this stack is the name of the command on which the current set command is working
   */
  stack<char*> setCommandStack;

  /**
   * Holds a name for a name expression until it can be given to the NameExpression object
   */
  ostringstream currentName;

  /**
   * A pointer to the current UserDefinedCommand that is being built (this assumes that UserDefinedCommands should not be nested but I don't know that that is ever tested)
   */
  UserDefinedCommand* currentUserDefinedCommand;

  /**
   * Keeps track of a single object (like a weights list or an execute list that will be passed in as a parameter)
   */
  void* currentParam;

  /**
   * This environment holds all of the user defined commands that have been defined
   */
  Environment* userDefinedCommands;

 public:

  ParsingInfo(string pathToDir, string filename, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate, ParsingInfo* base = NULL);
  virtual ~ParsingInfo();

  void setLocation(const Location& location);
  void setCurrentParam(void* newParam);
  void setCurrentName(const char* name);
  void setCurrentUserDefinedCommand(UserDefinedCommand* newUserDefinedCommand);
  void addCurrentUserDefinedCommand();

  int getCommandStackSize() const;
  char* getCurrentSetCommand() const;
  char* getCurrentIdent() const;
  string getCurrentName() const;
  string getFilename() const;
  string getDir() const;
  string getPathToFile() const;
  void*& getScanner();
  CallExpression* getCurrentCallExpression() const;
  CommandNode* getCurrentCommand() const;
  PsodaConstruct* getCurrentConstruct() const;
  UserDefinedCommand* getCurrentUserDefinedCommand() const;
  DataBlock* getCurrentDataBlock() const;
  IfElse* getCurrentIfElse() const;
  WeightsData* getCurrentWeightsParam() const;
  WhileLoop* getCurrentWhileLoop() const;
  Dataset& getCurrentDataset() const;
  const Location& getLocation() const;
  PsodaProgram& getCurrentProgram() const;
  Environment* getUserDefinedCommands() const;

  bool isParsingFile(string filename, string baseDir = "") const;

  void pushCallExpression(CallExpression* newCallExpression);
  void pushSetCommand(const char* newSetParamName);
  void pushIdent(const char* newIdent);
  void pushCommand(CommandNode* newCommand);
  void pushBlock(Block* newBlock);
  void pushConstruct(PsodaConstruct* newConstruct);
  void addCommand(CommandNode* newNode);
  void beginConstruct(PsodaConstruct* newConstruct);

  void freeSetCommand();
  void freeIdent();
  void popCallExpression();
  void popCommand();
  void popConstruct();
  void endConstruct();

  Expression* buildOperationExpression(void* op, void* operand1, void* operand2, Location location);
  Expression* buildNameExpression(string name, Location location);
  Expression* buildIdentExpression(string name, Location location);
  Expression* buildLiteralExpression(void* lit, Location location);
  void checkCharLength(string chars, int lineNumber);

  //#define CUR_EXECUTE_LIST ((vector<Expression*>*)currentParam)
};
#endif
