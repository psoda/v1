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
#ifndef GUI_PSODA_H
#define GUI_PSODA_H
#include "GUIPSODA.h"
#include "Block.h"
#include <map>
#include <vector>
#include <string>
#include <set>

/*
 * Class:      PSODA
 * Method:    getBaseDir
 */
JNIEXPORT string getBaseDir(string pathToFile);

/*
 * Class:      PSODA
 * Method:    setGraphEnabled
 */
JNIEXPORT void JNICALL Java_gui_PSODA_setGraphEnabled(bool value);

/*
 * Class:     PSODA
 * Method:    freeBlocks
 * Signature: 
 */
JNIEXPORT void JNICALL Java_gui_PSODA_freeBlocks(vector<Block*>& blocks);

/*
 * Class:     PSODA
 * Method:    writeToGui
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_writeToGui
  (const char*);

/*
 * Class:     PSODA
 * Method:    passConsensusTreeToGui
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gui_PSODA_passConsensusTreeToGui
  (const char*);

/**
 * Converts a jobjectArray to a vector<string>
 */
vector<string> jobjectArrayToVector(JNIEnv * env, jobjectArray origArray);

/**
 * Converts a jstring to a string
 */
string jstringToString(JNIEnv * env, jstring origString);

/**
 * Converts a set<string> to a jobjectArray
 */
jobjectArray setToJObjectArray(JNIEnv * env, set<string>& origSet);

/**
 * Converts a vector<string> to a jobjectArray
 */
jobjectArray vectorToJObjectArray(JNIEnv * env, vector<string>& origVector);

/**
 * Pairs correspondin entries in the two vectors into a single map 
 */
map<string, string> vectorsToMap(const vector<string>& nameVector, const vector<string>& valueVector);

/**
 * Initiates the process of opening a file chooser from the gui and returns the absolute path to the file
 */
JNIEXPORT string JNICALL Java_gui_PSODA_guiFindFile();

#endif
