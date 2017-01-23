#include "ParsingInfo.h"
#include "PsodaPrinter.h"
#include "OperationExpression.h"
#include "LiteralExpression.h"
#include "IdentExpression.h"
#include "NameExpression.h"
#include "FileTools.h"

using namespace std;

ParsingInfo::ParsingInfo(string _pathToDir, string _filename, PsodaBlocks& blocksToPopulate, Environment* userDefinedCommandsToPopulate, ParsingInfo* base) :
  yyscanner(NULL),
  baseParsingInfo(base),
  pathToDir(_pathToDir),
  filename(_filename),
  currentLocation("", 1),
  blocks(&blocksToPopulate),
  constructStack(),
  commandStack(),
  callExpressions(),
  identStack(),
  setCommandStack(),
  currentName(),
  currentUserDefinedCommand(NULL),
  currentParam(NULL),
  userDefinedCommands(userDefinedCommandsToPopulate) {
  return;
}

ParsingInfo::~ParsingInfo() {
  while (commandStack.size()) {
    delete commandStack.top();
    commandStack.pop();
  }
}

Dataset& ParsingInfo::getCurrentDataset() const {
  return ((DataBlock*)blocks->top())->getDataset();
}

PsodaProgram& ParsingInfo::getCurrentProgram() const {
  if (baseParsingInfo) {
    return baseParsingInfo->getCurrentProgram();
  } else {
    return ((ProgramBlock*)blocks->top())->getProgram();
  }
}

CommandNode* ParsingInfo::getCurrentCommand() const {
  return commandStack.top();
}

char* ParsingInfo::getCurrentSetCommand() const {
  return setCommandStack.top();
}

IfElse* ParsingInfo::getCurrentIfElse() const {
  return (IfElse*)constructStack.top();
}

WhileLoop* ParsingInfo::getCurrentWhileLoop() const {
  return (WhileLoop*)constructStack.top();
}

PsodaConstruct* ParsingInfo::getCurrentConstruct() const {
  return (PsodaConstruct*)constructStack.top();
}

WeightsData* ParsingInfo::getCurrentWeightsParam() const {
  return (WeightsData*)currentParam;
}

const Location& ParsingInfo::getLocation() const {
  return currentLocation;
}

char* ParsingInfo::getCurrentIdent() const {
  return identStack.top();
}

string ParsingInfo::getCurrentName() const {
  return currentName.str();
}

void*& ParsingInfo::getScanner() {
  return yyscanner;
}

CallExpression* ParsingInfo::getCurrentCallExpression() const {
  return callExpressions.top();
}

Environment* ParsingInfo::getUserDefinedCommands() const {
  return userDefinedCommands;
}

UserDefinedCommand* ParsingInfo::getCurrentUserDefinedCommand() const {
  return currentUserDefinedCommand;
}

string ParsingInfo::getPathToFile() const {
  return FileTools::buildPath(pathToDir, filename);
}

string ParsingInfo::getFilename() const {
  return filename;
}

string ParsingInfo::getDir() const {
  return pathToDir;
}

int ParsingInfo::getCommandStackSize() const {
  return commandStack.size();
}

bool ParsingInfo::isParsingFile(string pathToQuestionDir, string questionFilename) const {
  string questionPath = FileTools::buildPath(pathToQuestionDir, questionFilename);
  string thisPath = getPathToFile();
  if (FileTools::pathEqual(questionPath, thisPath)) {
    return true;
  } else if (baseParsingInfo) {
    return baseParsingInfo->isParsingFile(pathToQuestionDir, questionFilename);
  } else {
    return false;
  }
}

void ParsingInfo::setCurrentParam(void* newParam) {
  currentParam = newParam;
}

void ParsingInfo::setLocation(const Location& location) {
  currentLocation = location;
}

void ParsingInfo::pushBlock(Block* newBlock) {
  blocks->push(newBlock);
}

void ParsingInfo::addCommand(CommandNode* newCommand) {
  if (currentUserDefinedCommand) {
    currentUserDefinedCommand->getBody().addNode(newCommand);
  } else {
    getCurrentProgram().addNode(newCommand);
  }
}

void ParsingInfo::beginConstruct(PsodaConstruct* newConstruct) {
  if (currentUserDefinedCommand) {
    currentUserDefinedCommand->getBody().beginNode(newConstruct);
  } else {
    getCurrentProgram().beginNode(newConstruct);
  }
}

void ParsingInfo::endConstruct() {
  if (currentUserDefinedCommand) {
    currentUserDefinedCommand->getBody().endNode();
  } else {
    getCurrentProgram().endNode();
  }
}

void ParsingInfo::pushConstruct(PsodaConstruct* newConstruct) {
  constructStack.push(newConstruct);
}

void ParsingInfo::popConstruct() {
  constructStack.pop();
}

void ParsingInfo::pushCommand(CommandNode* newCommand) {
  commandStack.push(newCommand);
}

void ParsingInfo::popCommand() {
  commandStack.pop();
}

void ParsingInfo::pushSetCommand(const char* newSetParamName) {
  setCommandStack.push(strdup(newSetParamName));
}

void ParsingInfo::freeSetCommand() {
  free(setCommandStack.top());
  setCommandStack.pop();
}

void ParsingInfo::pushIdent(const char* newSetParamName) {
  identStack.push(strdup(newSetParamName));
}

void ParsingInfo::freeIdent() {
  free(identStack.top());
  identStack.pop();
}

void ParsingInfo::setCurrentName(const char* name) {
  currentName.str("");
  currentName << name;
}

void ParsingInfo::pushCallExpression(CallExpression* newCallExpression) {
  callExpressions.push(newCallExpression);
}

void ParsingInfo::popCallExpression() {
  callExpressions.pop();
}

void ParsingInfo::addCurrentUserDefinedCommand() {
  UserDefinedCommand* newUserDefinedCommand = currentUserDefinedCommand;
  string name = newUserDefinedCommand->getName();
  if (userDefinedCommands->canRead(name)) {
    if (PSODA_VERBOSE) {
      string errorMessage = "Function \"";
      errorMessage += name;
      errorMessage += "\" was already defined";
      PsodaWarning myWarning(errorMessage);
      PsodaPrinter::getInstance()->write("%s\n", myWarning.toString().c_str());
    }
  } else {
    Data myCommandHolder(newUserDefinedCommand);
    userDefinedCommands->set(name, &myCommandHolder);
  }
}

void ParsingInfo::setCurrentUserDefinedCommand(UserDefinedCommand* newUserDefinedCommand) {
  currentUserDefinedCommand = newUserDefinedCommand;
}

Expression* ParsingInfo::buildOperationExpression(void* op, void* operand1, void* operand2, Location location) {
  Expression* newExpression = new OperationExpression();
  SET_LOCATION(location,newExpression);
  ((OperationExpression*)newExpression)->setOperator((Operator*)op);
  ((OperationExpression*)newExpression)->setOperand1((Expression*)operand1);
  ((OperationExpression*)newExpression)->setOperand2((Expression*)operand2);
  return newExpression;
}  

Expression* ParsingInfo::buildNameExpression(string name, Location location) {
  Expression* newExpression = new NameExpression(name.c_str());
  SET_LOCATION(location,newExpression);
  return newExpression;
}  

Expression* ParsingInfo::buildIdentExpression(string name, Location location) {
  Expression* newExpression = new IdentExpression(name.c_str());
  SET_LOCATION(location,newExpression);
  return newExpression;
}  

Expression* ParsingInfo::buildLiteralExpression(void* lit, Location location) {
  Expression* newExpression = new LiteralExpression((Literal*)lit);
  SET_LOCATION(location,newExpression);
  return newExpression;
}  

void ParsingInfo::checkCharLength(string chars, int lineNumber) {
  int numCharsNeeded = getCurrentDataset().nchars();
  int numCharsGiven = chars.length();
  if (numCharsNeeded != numCharsGiven) {
    char buf[500];
    sprintf(buf, "Sequence of %d characters found on line %d does not match header which specifies nchars as %d",
	    numCharsGiven, lineNumber, numCharsNeeded);
    throw PsodaError(buf, Location(getPathToFile(), lineNumber));
  }
}  

