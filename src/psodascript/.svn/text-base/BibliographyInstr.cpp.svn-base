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
#include "BibliographyInstr.h"
#include "Interpreter.h"
#include "PsodaPrinter.h"


using namespace std;

BibliographyInstr::BibliographyInstr() : BuiltInCommand() {
  setDescription("Print out a bibliography of references for programs included in PSODA.");
  return;
}

BibliographyInstr::~BibliographyInstr() {
  return;
}

void BibliographyInstr::execute(Environment* baseEnv, Literal*& returnVal __attribute__((unused)) )
{
	execute(baseEnv);
}

void BibliographyInstr::execute(Environment* baseEnv __attribute__((unused)) ) {
    PsodaPrinter *psodaPrinter = PsodaPrinter::getInstance();
    psodaPrinter->write("\n\n");
    psodaPrinter->write("###  PSODA is an open-source project built from the following components. ###\n");
    psodaPrinter->write("###  Following are references to be used in publications.                 ###\n");
    psodaPrinter->write("\n");
    psodaPrinter->write("# PSODA\n");
    psodaPrinter->write("Carroll, H., Teichert, A., Krein, J.,Sundberg, K.,Snell, Q.,Clement, M., An open source phylogenetic search and alignment package, International Journal of Bioinformatics Research and Applications (IJBRA), Vol. 5, No. 3, 2009");
    psodaPrinter->write("\n");
    psodaPrinter->write("Woolley, S., Johnson, J., Smith, M., Crandall, K., McClellan, D. TreeSAAP: Selection on Amino Acid Properties using phylogenetic trees, Bioinformatics, 19, 671-672");
    psodaPrinter->write("\n");
    psodaPrinter->write("Clamp, M., Cuff, J., Searle, S. M. and Barton, G. J. (2004), The Jalview Java Alignment Editor. Bioinformatics, 20, 426-7.");
    psodaPrinter->write("\n");
    psodaPrinter->write("Ronquist, F. and J. P. Huelsenbeck. 2003. MRBAYES 3: Bayesian phylogenetic inference under mixed models. Bioinformatics 19:1572-1574. ");
    psodaPrinter->write("\n");
    psodaPrinter->write("Alexandros Stamatakis, RAxML-VI-HPC: maximum likelihood-based phylogenetic analyses with thousands of taxa and mixed models, Bioinformatics 2006; doi: 10.1093/bioinformatics/btl446 ");
    psodaPrinter->write("\n");
    psodaPrinter->write("Zmasek, C. M. and Eddy, S. R. (2001). ATV: display and manipulation of annotated phylogenetic trees. Bioinformatics, 17(4), 383-384.");
    psodaPrinter->write("\n");
#ifdef GUI
        PsodaPrinter::getInstance()->write("## Bibliography Completed Successfully\n");
#endif
}

string BibliographyInstr::getName() const {
  return "bibliography";
}
