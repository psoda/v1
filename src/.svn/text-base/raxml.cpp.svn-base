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
#include "Dataset.h"
#include "QNodeInfo.h"
#include "QTree.h"
//#include "Weights.h"
#include <fstream>
#include "QTreeRepository.h"
#include "NewickTreeParser.h"
#include "PsodaPrinter.h"

#ifdef WIN32
#include "process.h"
#endif

#define RAX_NARGS 30 // Max Args - there shouldnt be more than this

void  raxml_score(QTree *qtree)
{
    Interpreter::getInstance()->dataset()->printMatrix("Unlikelyx45q.phy");

// Save out the current tree so it can be scored
    ofstream outtree("Unlikelyx45q.intree");
    qtree->updateTreeString(0, true);
    char *tstr=qtree->treeStr();
    outtree << tstr << ";\n";
    outtree.close();
// Unlink the result files just in case they are still around
    unlink("RAxML_result.foobar");
    unlink("RAxML_info.foobar");
    int rval;

// The command line is:
// -t userStartingTree
// -m substitutionModel
// -f e optimize model parameters and branch lengths for tree given with -t
// -n outputFileName
// -s sequenceFileName
//

#if defined(WIN32) 
    char command_line[] = "RAxML-7.0.4\\raxmlHPC.exe";
#else
    char command_line[] = "RAxML-7.0.4/raxmlHPC -m GTRGAMMA -f e -n foobar -s Unlikelyx45q.phy -t Unlikelyx45q.intree > /dev/null";
#endif

    string fullCommand(psodaHome);
    fullCommand += command_line;

#ifdef WIN32
    rval = spawnlp (_P_WAIT,fullCommand.c_str(), fullCommand.c_str(), "-m" , "GTRGAMMA", "-f", "e", "-n", "foobar", "-s", "Unlikelyx45q.phy", "-t", "Unlikelyx45q.intree", ">/dev/null", NULL);
#else
    system(fullCommand.c_str());
#endif

// Now read the likelihood value
    ifstream ScoreFile("RAxML_info.foobar");
    if(!ScoreFile) {
      printf("raxml failed\n");
    }
    #define LINELEN 200
    char buff[LINELEN];
    float lvalue = 0;
    while(!ScoreFile.eof()) {
      ScoreFile.getline(buff,LINELEN);
      rval = sscanf(buff, "Final GAMMA  likelihood: %f", &lvalue);
      if(rval == 1) {
        break;
      } 
    }
// Now read the tree
    ifstream TreeFile("RAxML_result.foobar");
    #define LINELEN 200
    string treestr;
    TreeFile >> treestr;
    qtree->setScore(-lvalue);
    qtree->setTreeStr((char *)treestr.c_str());
    unlink("Unlikelyx45q.phy");
    unlink("Unlikelyx45q.intree");
    unlink("RAxML_result.foobar");
    unlink("RAxML_info.foobar");
}

void raxml_search(QTreeRepository &qtreeRepository, int raxml_iterations, string raxargs)
{
    Interpreter::getInstance()->dataset()->printMatrix("Unlikelyx45q.phy");
// Save out the current tree as a starting place
    ofstream outtree("Unlikelyx45q.intree");
    QTree *qtree = qtreeRepository.popTree();
    if(qtree == NULL) {
      return;
    }
    unlink("RAxML_result.foobar");
    unlink("RAxML_info.foobar");

    qtree->updateTreeString(0, true);
    char *tstr=qtree->treeStr();
    outtree << tstr << ";\n";
    outtree.close();
    delete(qtree);

// The command line is:
// -t userStartingTree
// -m substitutionModel
// -f d search with new hill-climbing algorithm
// -n outputFileName
// -s sequenceFileName
//
#if defined(WIN32) 
#if defined(GUI)
    char commandLineBase[] = "RAxML-7.0.4\raxmlHPC.exe -m GTRGAMMA -n foobar -s Unlikelyx45q.phy -t Unlikelyx45q.intree ";
#else
    char commandLineBase[] = "RAxML-7.0.4/raxmlHPC.exe -m GTRGAMMA -n foobar -s Unlikelyx45q.phy -t Unlikelyx45q.intree ";
#endif
#else
    char commandLineBase[] = "RAxML-7.0.4/raxmlHPC -m GTRGAMMA -n foobar -s Unlikelyx45q.phy -t Unlikelyx45q.intree ";
#endif

    string fullCommand(psodaHome);
    fullCommand += commandLineBase;
    fullCommand += raxargs;

//    printf("Raxcmd %s\n", fullCommand.c_str());
// Execute the command, read the output and send it to PsodaPrinter
    FILE * pipeToRaxml = popen(fullCommand.c_str(), "r");
    if(pipeToRaxml == NULL) {
      return;
    }

#define BUFFLEN 1024
    char buffer[BUFFLEN+1];
    int num;

    memset(buffer, 0, sizeof(buffer)+1); // Make sure it is null terminated
    while( num = fread( buffer, sizeof( char ), BUFFLEN, pipeToRaxml ) ) {
      PsodaPrinter::getInstance()->write("%s",buffer);
      memset(buffer, 0, sizeof(buffer)+1); // Make sure it is null terminated
    }

// Now read the likelihood value
    ifstream ScoreFile("RAxML_info.foobar");
    if(!ScoreFile) {
      printf("error opening raxml score file\n");
      return;
    }
    #define LINELEN 200
    char buff[LINELEN];
    float lvalue = 0;
    int rval;
    while(!ScoreFile.eof()) {
      ScoreFile.getline(buff,LINELEN);
      rval = sscanf(buff, "Likelihood   : %f", &lvalue);
      if(rval == 1) {
        break;
      } 
    }
// Now read the tree
    ifstream TreeFile("RAxML_result.foobar");
    #define LINELEN 200
    string treestr;
    TreeFile >> treestr;
    NewickTreeParser* ntp = new NewickTreeParser(&qtreeRepository);
    QTree * qt = ntp->parseBuffer(treestr.c_str(), strlen(treestr.c_str()), false);
    qt->setScore(-lvalue);
    qt->setTreeStr((char *)treestr.c_str());

// Nuke the tree repository so it will only have this tree
    qtreeRepository.removeAll();
    qtreeRepository.addTree(qt);
    unlink("Unlikelyx45q.phy");
    unlink("Unlikelyx45q.intree");
    unlink("RAxML_result.foobar");
    unlink("RAxML_info.foobar");
}

