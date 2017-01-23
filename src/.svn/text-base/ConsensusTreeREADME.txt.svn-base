Consensus Tree README

Consensus Tree functionality was added to PSODA in December of 2007 by David Packard. Much of the code used was taken from the Phylip Consensus program. The ConsensusTree.cpp and ConsensusTree.h file contain the class definition in which the Consensus Tree functions are implemented. A sample nexus file called consensus.nex can be found in the data directory or you can view the example below. Please reference the commands section to see what options have been implemented.

Command

Use the CONTREE command to request computation of strict and/or majority-rule consensus trees .

CONTREE [tree-list][/options];
The tree list specifies which trees to include in the consensus; the default is
"ALL."

Options
	. STRICT = YES|NO
	. MAJRULE = YES|NO
	. PERCENT = integer-value
	. SHOWTREE = YES|NO

Description of options

STRICT = YES |NO
By default, a strict consensus tree is computed. Use STRICT = NO to
suppress this computation.

MAJRULE = YES|NO
Specify MAJRULE = YES to request computation of a semistrict
(combinable component) consensus tree. MAJRULE = NO reverses
the effect of a previous MAJRULE specification.
The following options apply only if MAJRULE is in effect:

PERCENT = integer-value
The integer-value specifies the percentage of the trees on which
a group must appear in order to be retained in the majority

SHOWTREE = YES|NO
Unless SHOWTREE = NO, the consensus tree (or trees if more than one
consensus method is specified) is printed to the display buffer.

INDICES = YES|NO
Specify INDICES to request calculation of a variety of consensus
indices. The following indices are computed by PSODA and are
described in Rohlf (1982) and Swofford (1991):
Component information (consensus fork)

Example

gettrees file=tfile1.out;
gettrees file=tfile2.out;
gettrees file=tfile3.out;
contree all/strict=no majrule=yes percent=60;

Rohlf, F. J. 1982. Consensus indices for comparing classifications. Mathematical
Biosciences 59:131-144.

Swofford, D. L. 1991. When are phylogeny estimates from morphological and molecular
data incongruent? Pages 295-333 in M. M. Miyamoto and J. Cracraft
(ed.), Phylogenetic Analysis of DNA Sequences (Oxford University
Press: New York, N. Y.).

