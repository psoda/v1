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
#ifdef GUI

#include "GuiCommandNode.h"
#include "PsodaPrinter.h"
#include "PsodaReturn.h"
#include "PsodaError.h"
#include "PsodaWarning.h"
#include "PsodaException.h"
#include "PSODA.h"
#include "InteractiveInstr.h"
#include "Interpreter.h"

/*
 * Class:     gui_GuiCommandNode
 * Method:    runCommand
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gui_GuiCommandNode_runCommand(JNIEnv *env, jclass cls __attribute__((unused)), jstring code) {
  string codeCopy = jstringToString(env, code);
  gQuit = false;
  try {
    InteractiveInstr::executeCode(codeCopy, Interpreter::getInstance()->getDefaultVarEnv());
  	PsodaPrinter::getInstance()->write("\n");
  } catch (PsodaReturn& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  } catch (PsodaError& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  } catch (PsodaWarning& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  } catch (PsodaException& e) {
    PsodaPrinter::getInstance()->write("\n%s\n", e.toString().c_str());
  }
}

/*
 * Class:     gui_GuiCommandNode
 * Method:    toCode
 * Signature: (Ljava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gui_GuiCommandNode_toCode(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName, jobjectArray paramNames, jobjectArray paramValues) {
  string commandNameCopy = jstringToString(env, commandName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);
  vector<string> paramNamesVector = jobjectArrayToVector(env, paramNames);
  vector<string> paramValuesVector = jobjectArrayToVector(env, paramValues);
  map<string, string> params = vectorsToMap(paramNamesVector, paramValuesVector);  
  if (command) {
    return env->NewStringUTF(command->toCode(params).c_str());
  } else {
    return env->NewStringUTF(";");
  }
}

/*
 * Class:     gui_GuiCommandNode
 * Method:    getParamNames
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_GuiCommandNode_getParamNames(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName) {
  string commandNameCopy = jstringToString(env, commandName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);

  vector<string> paramNames;
  if (command) {
    paramNames = command->getParamNames();
  }

  return vectorToJObjectArray(env, paramNames);

}

/*
 * Class:     gui_GuiCommandNode
 * Method:    getParamDescriptions
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_GuiCommandNode_getParamDescriptions(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName) {
  string commandNameCopy = jstringToString(env, commandName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);

  vector<string> paramDescriptions;
  if (command) {
    paramDescriptions = command->getParamDescriptions();
  }

  return vectorToJObjectArray(env, paramDescriptions);

}

/*
 * Class:     gui_GuiCommandNode
 * Method:    getFileParams
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_GuiCommandNode_getFileParamNames(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName) {
  string commandNameCopy = jstringToString(env, commandName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);

  set<string> fileParamNames;
  if (command) {
	fileParamNames = command->getFileParamNames();
  }

  return setToJObjectArray(env, fileParamNames);

}

/*
 * Class:     gui_GuiCommandNode
 * Method:    getParamOptions
 * Signature: (Ljava/lang/String;Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_GuiCommandNode_getParamOptions(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName, jstring paramName) {
  string commandNameCopy = jstringToString(env, commandName);
  string paramNameCopy = jstringToString(env, paramName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);

  set<string> paramOptions;
  if (command) {
    paramOptions = command->getParamOptions(paramNameCopy);
  }

  return setToJObjectArray(env, paramOptions);

}

/*
 * Class:     gui_GuiCommandNode
 * Method:    getDefaultValues
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_GuiCommandNode_getDefaultValues(JNIEnv *env, jclass cls __attribute__((unused)), jstring commandName) {
  string commandNameCopy = jstringToString(env, commandName);
  PsodaCommand* command = Interpreter::getInstance()->getFunction(commandNameCopy);

  vector<string> paramDefaultValues;
  if (command) {
    paramDefaultValues = command->getDefaultValues();
  }

  return vectorToJObjectArray(env, paramDefaultValues);

}

#endif

