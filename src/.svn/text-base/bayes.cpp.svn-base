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

#else
#include <signal.h>
#include <sys/wait.h>
#endif

#define BAYES_NARGS 30 // Max Args - there shouldnt be more than this

#ifndef WIN32
void timeout_catcher (int status)
{
    printf("mrbayes timeout %X\n",status);
}
#endif

#define DEFAULT_ITERATIONS 20;
#define MINFORCONSENSUS 4
#define MINTOTAL 6
#define STOPPINGPROB 0.10
void bayes_search(QTreeRepository &qtreeRepository, int bayes_iterations, string bayesargs)
{
    Interpreter::getInstance()->dataset()->printMatrixNexus("mrbayes.nex", true);
// Save out the current tree as a starting place
    ofstream outtree;
    outtree.open("mrbayes.nex", ios::ate|ios::out|ios::app);
    QTree *qtree = qtreeRepository.popTree();
    if(qtree == NULL) {
      return;
    }
    // If they didnt specify a max, set one
    if(bayes_iterations <=0) {
      bayes_iterations = DEFAULT_ITERATIONS;
    }

    // Get the tree string
    qtree->updateTreeString(0, true, false);
    char *tstr=qtree->treeStr();

    // Create the usertree command
    string treeString = "usertree=";
    treeString +=  tstr;
    treeString +=  ";\n";

    outtree << "\nEND;\n";
    outtree << "BEGIN mrbayes;\n";
    outtree << "set nowarn=yes;\n";
    outtree << "mcmcp savebrlens=yes;\n";
    outtree << "usertree = "<<tstr << ";\n";
    outtree << bayesargs;
#ifdef WIN32
    // If we are in windows, we wont know how to fork, so just execute it.
    outtree << treeString;
    outtree << "mcmc ngen="<<100*bayes_iterations<<" startingtree=user samplefreq=100 printfreq=100 nruns=1 nchains=2 savebrlens=yes;\n";
    outtree << "sumt contype=halfcompat burnin="<<bayes_iterations-MINFORCONSENSUS<<";";
#endif
    outtree << "END;";
    outtree.close();
    delete(qtree);

#ifdef WIN32
#ifdef GUI
    char commandLineBase[] = "mrbayes-3.1.2\\mb.exe";
#else // WIN32 but not GUI
    char commandLineBase[] = "mrbayes-3.1.2\\mb.exe";
#endif
#else // NOT WIN32
    char commandLineBase[] = "mrbayes-3.1.2/mb";
#endif

    string fullCommand(psodaHome);
    fullCommand += commandLineBase;

#ifdef WIN32
    int rval = spawnlp (_P_WAIT,fullCommand.c_str(), fullCommand.c_str(), "mrbayes.nex", NULL); // Run MrBayes
#else 
// Execute the command, read the output and send it to PsodaPrinter

  int fdsout[2];
  int fdsin[2];
  pid_t pid;

// Create pipes going both ways
  if(pipe (fdsout) != 0) {
    perror("pipe failed\n");
    exit(0);
  }
  if(pipe (fdsin) != 0) {
    perror("pipe failed\n");
    exit(0);
  }

  pid = fork (); /* Fork a child process.  */
  if (pid == (pid_t) 0) {
    // This is the child process.  
    close(fdsout[0]);
    dup2 (fdsout[1], STDOUT_FILENO); // Dup fds[1] to stdout 
    close(fdsin[1]);
    dup2 (fdsin[0], STDIN_FILENO); // Connect the read end of the pipe to standard input.
    execlp (fullCommand.c_str(), fullCommand.c_str(), NULL); // Run MrBayes
  } else {
    /* This is the parent process.  */
    FILE* output;
    FILE* input;

    /* Convert the write file descriptor to a FILE object, and write to it.  */
    if((output = fdopen (fdsin[1], "w")) == NULL) {
        perror("output fdopen failed\n");
        exit(0);
    }
    close(fdsin[0]);
    if((input = fdopen (fdsout[0], "r")) == NULL) {
        perror("input fdopen failed\n");
        exit(0);
    }
    close(fdsout[1]);
    fprintf (output, "execute mrbayes.nex \n");

#define MAXLINE 1024
    char buf[MAXLINE];
    float prob = 1.0;
    
    setbuf(stdout,0);
    setbuf(output,0);
    fprintf (output, "%s",bayesargs.c_str());
    fprintf (output, "set nowarn=yes\n");
    fprintf (output, "%s",treeString.c_str());
    fprintf (output, "mcmc ngen=100 startingtree=user;\n");
    fflush (output);
    int count = 0;
    int countbelow = 0;
// We want to keep going until we have at least the minimum number of trees and at least MINFORCONSENSUS below .10 probability for a minimum number
// Break out early if the user has set bayes_iterations
    while(((countbelow < MINFORCONSENSUS) || (count < MINTOTAL)) &&
          (count < bayes_iterations)) {
      while(fgets(buf, MAXLINE, input) != NULL) {
        PsodaPrinter::getInstance()->write("%s",buf);
        if(sscanf(buf,"  Average standard deviation of split frequencies: %f\n",&prob) > 0) {
          sprintf(buf,"Current Probability %f, Stopping Probability %f; iterations %d, max iterations %d\n",prob, STOPPINGPROB, count, bayes_iterations);
          PsodaPrinter::getInstance()->write("%s",buf);
          fprintf (output, "yes\n");
          fprintf (output, "100\n");
          break;
        }
      }
      count++;
      if(prob < STOPPINGPROB) {
        countbelow++;
      }
    }
    fprintf (output, "no\n");
    fprintf (output, "sumt contype=halfcompat burnin=%d\n", count-MINFORCONSENSUS);
    fprintf (output, "quit\n");
    fflush (output);
#ifdef notdef
// This doesnt seem to work, add something like it if mrbayes hangs.
    // Set an alarm so we dont wait forever
    signal(SIGALRM, timeout_catcher);
    alarm(10);
#endif
    while(fgets(buf, MAXLINE, input) != NULL) {
      PsodaPrinter::getInstance()->write("%s",buf);
    }
    // Wait for the child to exit
    int retpid=waitpid(pid, NULL, 0);
    fclose(output);
  } // End of Parent
#endif //WIN32

  // Now read the results out of the output file
  NewickTreeParser* ntp = new NewickTreeParser(&qtreeRepository);
  qtreeRepository.removeAll();
  int parserResult = ntp->parseFilename("mrbayes.nex.con");
  printf("parser returned %d\n",parserResult);

//Clean up after mrbayes
  unlink("mrbayes.nex");
  unlink("mrbayes.nex.con");
  unlink("mrbayes.nex.mcmc");
  unlink("mrbayes.nex.parts");
  unlink("mrbayes.nex.run1.p");
  unlink("mrbayes.nex.run1.t");
  unlink("mrbayes.nex.run2.p");
  unlink("mrbayes.nex.run2.t");
  unlink("mrbayes.nex.t");
  unlink("mrbayes.nex.p");
  unlink("mrbayes.nex.trprobs");
}
