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

#include "PSODA.h"
#include "PsodaRunner.h"
#include "PsodaException.h"
#include "QTreeRepository.h"
#include "Interpreter.h"
#include "SignalHandlers.h"
#include "GuiPrintBuffer.h"
#include "PsodaReturn.h"
#include "PsodaWarning.h"
#include "PsodaError.h"
#include "PsodaPrinter.h"
#include "InteractiveInstr.h"
#include <vector>
#include <iostream>
#include <deque>

extern double PSODA_maxmemory; // The max java vm memory or OS memory we can use for trees

// Must be global to function
JavaVM* jvm;
static jobject globalObj;
static jclass globalCls;

/*
 * Class:     gui_PSODA
 * Method:    initInterpreter
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_initInterpreter(JNIEnv * env, jobject obj, jlong vmsize) {
  env->GetJavaVM(&jvm);
  globalObj = env->NewGlobalRef(obj);
  jclass cls = env->GetObjectClass(obj);
  globalCls = (jclass)env->NewGlobalRef(cls);
  gQuit = false;

PSODA_maxmemory = vmsize; // The max java vm memory or OS memory we can use for trees
//printf("VMSIZE %f\n",PSODA_maxmemory);
  static GuiPrintBuffer myGuiPrintBuffer;
	
  PsodaPrinter::getInstance()->clearBufferList();
  PsodaPrinter::getInstance()->addPrintBuffer(&myGuiPrintBuffer);
  Interpreter::getInstance()->clear();
}

/*
 * Class:     gui_PSODA
 * Method:    interpretFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_interpretFile(JNIEnv * env, jobject obj, jstring fileName) {
  // Convert fileName to char * //
  //	env->GetJavaVM(&jvm);
  gQuit = false;
  const char* file;
  jboolean iscopy;
  iscopy = JNI_FALSE;
  file = (env)->GetStringUTFChars(fileName, &iscopy);

  // DEBUG: Remove these exceptions for product release
  jthrowable exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
  }
  // 

  // Get method ID for Java String class //
  //	jclass cls = env->GetObjectClass(obj);

  // Save global variables //
  //	globalObj = env->NewGlobalRef(obj);
  //	globalCls = (jclass)env->NewGlobalRef(cls);

  //	GuiPrintBuffer myGuiPrintBuffer;
	
  //	PsodaPrinter::getInstance()->clearBufferList();
  //	PsodaPrinter::getInstance()->addPrintBuffer(&myGuiPrintBuffer);
  PsodaRunner::run(file);

  (env)->ReleaseStringUTFChars(fileName, file);
  env->DeleteGlobalRef(obj);
}


/*
 * Class:      PSODA
 * Method:    setGraphEnabled
 */
JNIEXPORT void JNICALL Java_gui_PSODA_setGraphEnabled(bool value) {
  union {
    JNIEnv* env;
    void* env_p;
  };

  jvm->AttachCurrentThread(&env_p, NULL);
  
  jmethodID methID = env->GetMethodID(globalCls, "setGraphEnabled", "(Z)V");

  jthrowable exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  jboolean graphEnabled;

  if(value){
    graphEnabled = JNI_TRUE;
  }
  else{
    graphEnabled = JNI_FALSE;
  }

  env->CallVoidMethod(globalObj, methID, graphEnabled);
  exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }
}

/*
 * Class:     PSODA
 * Method:    pausePSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_pausePSODA(JNIEnv * env __attribute__((unused)), jobject obj __attribute__((unused))){
  pausePSODA = 1;
}

/*
 * Class:     PSODA
 * Method:    resumePSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_resumePSODA(JNIEnv * env __attribute__((unused)), jobject obj __attribute__((unused))){
  pausePSODA = 0;
}

/*
 * Class:     PSODA
 * Method:    killPSODA
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_killPSODA(JNIEnv * env __attribute__((unused)), jobject obj __attribute__((unused))){
    gQuit = true;
}

/*
 * Class:     gui_PSODA
 * Method:    getDefinedCommandNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getDefinedCommandNames(JNIEnv *env, jclass __attribute__((unused))) {
  set<string> commandNames = Interpreter::getInstance()->getDefinedCommandNames();
  return setToJObjectArray(env, commandNames);
}

/*
 * Class:     gui_PSODA
 * Method:    getPopularCommandNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getPopularCommandNames(JNIEnv *env, jclass __attribute__((unused))) {
  vector<string> popularCommandNames = Interpreter::getInstance()->getPopularCommandNames();
  return vectorToJObjectArray(env, popularCommandNames);
}

/*
 * Class:     PSODA
 * Method:    getTrees
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getTrees(JNIEnv * env, jobject obj __attribute__((unused))){
  jobjectArray treeArray = NULL;
  if(Interpreter::getInstance()->qtreeRepository() != NULL &&
     Interpreter::getInstance()->qtreeRepository()->numTrees() > 0){
    int maxtrees = (int)(Interpreter::getInstance()->qtreeRepository()->numTrees() + 1);
    // Find the Java String class so JNI can use it to create an array of Strings
    jclass cls = env->FindClass("java/lang/String");

    // DEBUG: Remove these exceptions for product release
    jthrowable exception = env->ExceptionOccurred();
    if(exception){
      env->ExceptionDescribe();
    }
    //

    // Create String Array
    treeArray = (jobjectArray)env->NewObjectArray(maxtrees, cls, NULL);

    // DEBUG: Remove these exceptions for product release
    exception = env->ExceptionOccurred();
    if(exception){
      env->ExceptionDescribe();
      exit(0);
    }
    //

    //		char treeString[30000];
    char score[350];
    QTree* nextTree = NULL;
    const deque<QTree *>* treeList = Interpreter::getInstance()->qtreeRepository()->getTrees();
    deque<QTree *>::const_iterator treeIt;

    // If there are any trees in the repository, copy them into a java array and pass to gui
    int count = 0;
    if( treeList->size() > 0){
      for(treeIt = treeList->begin(); treeIt != treeList->end(); treeIt++){
	nextTree = *treeIt;
	jstring jTreeString = env->NewStringUTF((const char*)nextTree->treeStr());

	// DEBUG: Remove these exceptions for product release
	jthrowable exception = env->ExceptionOccurred();
	if(exception){
	  env->ExceptionDescribe();
	  exit(0);
	}
	//

	env->SetObjectArrayElement(treeArray, count, jTreeString);

	// DEBUG: Remove these exceptions for product release
	exception = env->ExceptionOccurred();
	if(exception){
	  env->ExceptionDescribe();
	  exit(0);
	}
	//
	env->DeleteLocalRef(jTreeString);
	count++;
      }
      // Attach the current score for the tree repository to the end of the list to display
      // in "Trees" tab of gui
	
      jstring jScore;
      if(nextTree->isScored()){
	sprintf(score, "%.0lf", nextTree->getScore());
	jScore = env->NewStringUTF((const char*)score);
      }
      else{
	sprintf(score, "%.0lf", 0.0);
	jScore = env->NewStringUTF((const char*)score);
      }

      // DEBUG: Remove these exceptions for product release
      jthrowable exception = env->ExceptionOccurred();
      if(exception){
	env->ExceptionDescribe();
	exit(0);
      }
      // 

      env->SetObjectArrayElement(treeArray, count, jScore);

      // DEBUG: Remove these exceptions for product release
      exception = env->ExceptionOccurred();
      if(exception){
	env->ExceptionDescribe();
	exit(0);
      }
      // 

      env->DeleteLocalRef(jScore);
    }
  }

  /* Debug
     if(treeArray != NULL && env->GetArrayLength(treeArray) > 1){
     //printf("Length: %d\n", env->GetArrayLength(treeArray));
     for(int i = 0; i < env->GetArrayLength(treeArray); i++){
     jstring jst = (jstring)env->GetObjectArrayElement(treeArray,i);
     const char* st = env->GetStringUTFChars(jst, NULL);
     printf("treeArray[%d]: %s\n", i, st);
     }
     }
  */
  return treeArray;
}

jobjectArray vectorToJObjectArray(JNIEnv * env, vector<string>& origVector) {
  jclass cls = env->FindClass("java/lang/String");
  jobjectArray returnArray = (jobjectArray)env->NewObjectArray(origVector.size(), cls, NULL);

  vector<string>::const_iterator itemItr = origVector.begin();
  vector<string>::const_iterator itemEnd = origVector.end();
  for(int curIndex = 0; itemItr != itemEnd; itemItr++, curIndex++) {
    string thisItem = *itemItr;
    jstring jItem = env->NewStringUTF((const char*)thisItem.c_str());
    env->SetObjectArrayElement(returnArray, curIndex, jItem);
    env->DeleteLocalRef(jItem);
  }

  return returnArray;
}

vector<string> jobjectArrayToVector(JNIEnv * env, jobjectArray origArray) {
  vector<string> returnVector;
  jint size = env->GetArrayLength(origArray);
  for(jint curIndex = 0; curIndex < size; curIndex++) {
    jstring jItem = (jstring) env->GetObjectArrayElement(origArray, curIndex);
    returnVector.push_back(jstringToString(env, jItem));
    env->DeleteLocalRef(jItem);
  }

  return returnVector;
}

jobjectArray setToJObjectArray(JNIEnv * env, set<string>& origSet) {
  jclass cls = env->FindClass("java/lang/String");
  jobjectArray returnArray = (jobjectArray)env->NewObjectArray(origSet.size(), cls, NULL);

  set<string>::const_iterator itemItr = origSet.begin();
  set<string>::const_iterator itemEnd = origSet.end();
  for(int curIndex = 0; itemItr != itemEnd; itemItr++, curIndex++) {
    string thisItem = *itemItr;
    jstring jItem = env->NewStringUTF((const char*)thisItem.c_str());
    env->SetObjectArrayElement(returnArray, curIndex, jItem);
    env->DeleteLocalRef(jItem);
  }

  return returnArray;
}

string jstringToString(JNIEnv * env, jstring origString) {
  jboolean iscopy = JNI_FALSE;
  const char* stringCopy = (env)->GetStringUTFChars(origString, &iscopy);
  string returnString = stringCopy;
  (env)->ReleaseStringUTFChars(origString, stringCopy);
  return returnString;
}

map<string, string> vectorsToMap(const vector<string>& nameVector, const vector<string>& valueVector) {
  int size1 = nameVector.size();
  int size2 = valueVector.size();

  // Take the minimum number of items
  int numItems = (size2 < size1) ? size2 : size1; 
  map<string, string> returnMap;
  for (int i = 0; i < numItems; i++) {
    string thisName = nameVector[i];
    string thisValue = valueVector[i];
    returnMap[thisName] = thisValue;
  }
  return returnMap;
}

/*
 * Class:     PSODA
 * Method:    getTaxaNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_gui_PSODA_getTaxaNames(JNIEnv * env, jobject obj __attribute__((unused))){
  vector<string>& taxonNames = Interpreter::getInstance()->dataset()->taxonNames();
  return vectorToJObjectArray(env, taxonNames);
}

/*
 * Class:     PSODA
 * Method:    saveTrees
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jboolean JNICALL Java_gui_PSODA_saveTrees(JNIEnv * env, jobject obj __attribute__((unused)), jstring fileName, jstring stringFormat){
  jboolean success = false;
  if(Interpreter::getInstance()->qtreeRepository() != NULL &&
     Interpreter::getInstance()->qtreeRepository()->numTrees() > 0){
    const char* file;
    const char* tmpFormat;
    int format = 0;
    jboolean iscopy;
    iscopy = JNI_FALSE;
    file = (env)->GetStringUTFChars(fileName, &iscopy);

    tmpFormat = env->GetStringUTFChars(stringFormat, &iscopy);

	
    if(strncmp(tmpFormat, "nexus", 5) == 0){
      format = 1;
    }
    else{
      format = 2;
    }

    jthrowable exception = env->ExceptionOccurred();
    if(exception){
      env->ExceptionDescribe();
      exit(0);
    }

    //Interpreter::getInstance()->setParameter("saveTreesFile", file);
    Interpreter::getInstance()->saveTrees(format, file);
    success = true;
    (env)->ReleaseStringUTFChars(fileName, file);
  }
  return success;
}

/*
 * Class:     PSODA
 * Method:    saveAlign
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jboolean JNICALL Java_gui_PSODA_saveAlign(JNIEnv * env, jobject obj __attribute__((unused)), jstring fileName, jstring stringFormat){
  jboolean success = false;
  if(Interpreter::getInstance()->dataset() != NULL) {
    const char* file;
    const char* tmpFormat;
    jboolean iscopy;
    iscopy = JNI_FALSE;
    file = (env)->GetStringUTFChars(fileName, &iscopy);

    tmpFormat = env->GetStringUTFChars(stringFormat, &iscopy);

	
    jthrowable exception = env->ExceptionOccurred();
    if(exception){
      env->ExceptionDescribe();
      exit(0);
    }
    Interpreter::getInstance()->dataset()->printSeqs(tmpFormat,file);
    success = true;
    (env)->ReleaseStringUTFChars(fileName, file);
  }
  return success;
}

/*
 * Class:     PSODA
 * Method:    writeTreeSAAPPSODA
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT void JNICALL Java_gui_PSODA_writeTreeSAAPPSODA(JNIEnv * env, jobject obj __attribute__((unused)), jstring fileName){
  if(Interpreter::getInstance()->qtreeRepository() != NULL &&
     Interpreter::getInstance()->qtreeRepository()->numTrees() > 0){
    const char* file;
    jboolean iscopy;
    iscopy = JNI_FALSE;
    file = (env)->GetStringUTFChars(fileName, &iscopy);

    jthrowable exception = env->ExceptionOccurred();
    if(exception){
      env->ExceptionDescribe();
      exit(0);
    }

    Interpreter::getInstance()->dataset()->printMatrixNexus(file, 0);
    (env)->ReleaseStringUTFChars(fileName, file);
  }
}

/*
 * Class:     PSODA
 * Method:    storeJavaEnvironment
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_writeToGui(const char* str){
  union {
    JNIEnv* env;
    void* env_p;
  };

  jvm->AttachCurrentThread(&env_p, NULL);

  jmethodID methID = env->GetMethodID(globalCls, "writeStringToGui", "(Ljava/lang/String;)V");

  jthrowable exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  jboolean iscopy;
  iscopy = JNI_FALSE;

  //cout << "Str: " << str << endl;
  jstring string = env->NewStringUTF((const char*)str);
  exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  env->CallVoidMethod(globalObj, methID, string);
  exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }
  env->DeleteLocalRef(string);
}

JNIEXPORT string JNICALL Java_gui_PSODA_guiFindFile() {
  union {
    JNIEnv* env;
    void* env_p;
  };

  jvm->AttachCurrentThread(&env_p, NULL);

  jmethodID methID = env->GetMethodID(globalCls, "guiFindFile", "()Ljava/lang/String;");

  jthrowable exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  jboolean iscopy;
  iscopy = JNI_FALSE;

  jstring absolutePathToFile = (jstring) env->CallObjectMethod(globalObj, methID);
  string returnPath = jstringToString(env, absolutePathToFile);
  env->DeleteLocalRef(absolutePathToFile);
  return returnPath;
}

/*
 * Class:     PSODA
 * Method:    passConsensusTreeToGui
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_passConsensusTreeToGui(const char* treeString){
  union {
    JNIEnv* env;
    void* env_p;
  };

  jvm->AttachCurrentThread(&env_p, NULL);

  jmethodID methID = env->GetMethodID(globalCls, "openConsensusTreeInATV", "(Ljava/lang/String;)V");

  jthrowable exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  jboolean iscopy;
  iscopy = JNI_FALSE;

  //cout << "treeString: " << treeString << endl;
  jstring string = env->NewStringUTF((const char*)treeString);
  exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }

  env->CallVoidMethod(globalObj, methID, string);
  exception = env->ExceptionOccurred();
  if(exception){
    env->ExceptionDescribe();
    exit(0);
  }
  env->DeleteLocalRef(string);
}



#endif

