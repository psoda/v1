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
#include "FileTools.h"
#include "PsodaPrinter.h"
#include "Globals.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

string FileTools::getBaseDir(string pathToFile) {
  string returnDir = "";
#if defined(WIN32) && defined(GUI)
  string::size_type lastSlashIndex = pathToFile.rfind('\\');
#else
  string::size_type lastSlashIndex = pathToFile.rfind('/');
#endif
  if (lastSlashIndex == string::npos) {
    returnDir = ".";
  } else {
    returnDir = pathToFile.substr(0, lastSlashIndex);
  }
  return returnDir;
}

string FileTools::getFilename(string pathToFile) {
  string filename = "";
#if defined(WIN32) && defined(GUI)
  string::size_type lastSlashIndex = pathToFile.rfind('\\');
#else
  string::size_type lastSlashIndex = pathToFile.rfind('/');
#endif
  if (lastSlashIndex != string::npos && lastSlashIndex < (pathToFile.length() - 1)) {
    filename = pathToFile.substr(lastSlashIndex + 1);
    return filename;
  } else {
    return pathToFile;
  }
}

bool FileTools::isAbsolute(string filename) {
#if defined(WIN32) && defined(GUI)
  if (filename[0] == '\\') {
#else
  if (filename[0] == '/') {
#endif
    return true;
  } else {
    return false;
  }
}

string FileTools::buildPath(string baseDir, string filename) {
  if (isAbsolute(filename)) {
    return filename;
  } else if (baseDir == "") {
    baseDir = ".";
  }

  string returnString = baseDir;
  if (filename != "") {
#if defined(WIN32) && defined(GUI)
    returnString += "\\";
#else
    returnString += "/";
#endif
    returnString += filename;
  }

  return resolveInteralRelativeMarks(returnString);
}

string FileTools::resolveInteralRelativeMarks(const string& path) {
  ostringstream finalPath;
  vector<string> dirStack;
  string::size_type nextIndexToRead = 0;
  string::size_type stringLength = path.length();
  string filename = "";
  if (isAbsolute(path)) {
#if defined(WIN32) && defined(GUI)
    finalPath << "\\";
#else
    finalPath << "/";
#endif
  }
  while (nextIndexToRead < (stringLength - 1)) {
    string::size_type nextSlashIndex = path.find_first_of("\\/", nextIndexToRead);
    if (nextSlashIndex != string::npos) {
      string::size_type lengthOfDirName = nextSlashIndex - nextIndexToRead;
      string thisDirName = path.substr(nextIndexToRead, lengthOfDirName);
      nextIndexToRead = nextSlashIndex + 1;
      // stay in the same directory
      if (thisDirName == "." || thisDirName == "") {
	continue;
      } else if (thisDirName == "..") {
	if (dirStack.size()) {
	  // take off the last directory entered
	  dirStack.pop_back();
	} else {
	  // add a "go to parent directory" to the beginning of the final path
#if defined(WIN32) && defined(GUI)
	  finalPath << "..\\";
#else
	  finalPath << "../";
#endif
	}
      } else {
	// if it isn't a relative path indicator, just add it to the stack
	dirStack.push_back(thisDirName);
      }
    } else {
      // The rest must be the filename so just keep everything after the alreadyReadThroughIndex + 1
      filename = path.substr(nextIndexToRead);
      break;
    }
  }
  // Now finish building the directory;
  vector<string>::iterator dirNameItr = dirStack.begin();
  vector<string>::iterator dirNameEnd = dirStack.end();
  for (; dirNameItr != dirNameEnd; dirNameItr++) {
#if defined(WIN32) && defined(GUI)
    finalPath << *dirNameItr << "\\";
#else
    finalPath << *dirNameItr << "/";
#endif
  }
  finalPath << filename;
  return finalPath.str();
}

bool FileTools::pathEqual(string filename1, string filename2, string baseDir) {
  string path1 = buildPath(baseDir, filename1);
  string path2 = buildPath(baseDir, filename2);
  struct stat buf1;
  if (stat(path1.c_str(), &buf1) < 0) {
    if (PSODA_VERBOSE) {
      PsodaPrinter::getInstance()->write("Warning: %s (%s)\n", strerror(errno), path1.c_str());
    }
    return false;
  }
  struct stat buf2;
  if (stat(path2.c_str(), &buf2) < 0) {
    if (PSODA_VERBOSE) {
      PsodaPrinter::getInstance()->write("Warning: %s (%s)\n", strerror(errno), path2.c_str());
    }
    return false;
  }
  return (buf1.st_ino == buf2.st_ino);
}
