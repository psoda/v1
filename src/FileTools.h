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
#ifndef FILE_TOOLS_H
#define FILE_TOOLS_H

#include <string>

using namespace std;

class FileTools {

 private:


 public:

  /**
   * Get the (relative) directory in which the file is
   */
  static string getBaseDir(string pathToFile);
  static string getFilename(string pathToFile);
  static bool pathEqual(string filename1, string filename2, string baseDir = "");
  static string buildPath(string baseDir, string filename);
  static bool isAbsolute(string filename);
  static string resolveInteralRelativeMarks(const string& path);

};

#endif