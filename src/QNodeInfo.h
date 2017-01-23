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
#ifndef QSODA_NODEINFO 
#define QSODA_NODEINFO
#include "QNode.h"
#include "MultChar.h"
#include "Dataset.h"
#include "SSDataset.h"
#include "ParsimonyNodeData.h"
#include "FastParsimonyNodeData.h"
#include "LikelihoodNodeData.h"
#include <list>

class QNodeInfo;
class QNode;
class LikelihoodNodeData;
class ParsimonyNodeData;

class QNodeInfo
{
  public:
    QNodeInfo(bool leaf);
    QNodeInfo(const QNodeInfo& m);
    QNodeInfo(const QNodeInfo* m);
    const QNodeInfo& operator=(const QNodeInfo& m);
    ~QNodeInfo();

    QNodeInfo( int nodeNumber, bool leaf);

    QNode *& node();
    QNode * node() const;
    bool &leaf();
    bool leaf() const;
    const char* getSeqData() const;
    const char* getTaxonName() const;
	void setTaxonName(const char*);
    const char* getLabel() const;
	void setLabel(const char*);
    int & nodeIndex();
    int  nodeIndex() const;
    int & nchars();
    int nchars() const;
    int & nchildren();
    int nchildren() const;
    bool & visited();
    bool visited() const;
    ParsimonyNodeData *& nodeData();
    ParsimonyNodeData * nodeData() const;
    FastParsimonyNodeData *& nodeDataFP();
    FastParsimonyNodeData * nodeDataFP() const;
    LikelihoodNodeData *& nodeDataML();
    LikelihoodNodeData * nodeDataML() const;
    void print();
	void writeSequence();
    int & postChild1();
    int postChild1() const;
    int & postChild2();
    int postChild2() const;
    int & postDifferences();
    int postDifferences() const;
    bool & postChanged();
    bool postChanged() const;
    int & preChild1();
    int preChild1() const;
    int & preChild2();
    int preChild2() const;
    int & preDifferences();
    int preDifferences() const;
    bool & preChanged();
    bool preChanged() const;
	char & AncState(int);
	char AncState(int) const;
	const char* AncState() const;
	const char* AncState();

    list<MultChar *> *mMultChar;  /* for QAlign::multCharAlign() */
    Dataset* mDataset;            /* for QAlign::classicProgressiveAlign() */
    SSDataset* mSSDataset;        /* for QSSAlign::classicProgressiveSSAlign()*/

  protected:
	char SeqChar(char); //convert bit-vector to standard DNA notation
    bool    mLeaf;  // This Node is a leaf
    QNode *mNode; // points to Node associated with this information
    const char *mSeqData;    // Sequence Data associated with this node (Lazy allocation)
    char *mAncData;	//Ancestral Sequence associated with this node
    const char *mTaxonName;    // Points to the name of the taxon represented by this node (only valid for leaf nodes)
    const char *mLabel; // Label for posterior probabilities, consenses percentages, names, etc. Stored as a string
    ParsimonyNodeData *mNodeData; // Holds the data needed for fast parsimony scoring
    FastParsimonyNodeData *mNodeDataFP; // Holds the data needed for vectorized fast parsimony scoring
	LikelihoodNodeData *mNodeDataML; // Holds the data needed for fast likelihood scoring
    bool mVisited; // Flag indicating that this has been used as a split point
    int mIndex; // This is nasty, but we need it to get into the node data
    int mNchars; // The number of characters in the sequence
    int mNchildren; // The number of children of this node

	
	int mPostChild1, mPostChild2;
	int mPostDifferences;
	bool mPostChanged;
	
	int mPreChild1, mPreChild2;
	int mPreDifferences;
	bool mPreChanged;
	
};
#endif
