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
#include "QNodeInfo.h"
#include "Interpreter.h"
#include <stdio.h>
#include "PsodaPrinter.h"
#include "PsodaWarning.h"

QNodeInfo::QNodeInfo(bool leaf)
:
	mMultChar(NULL),
	mDataset(NULL),
	mSSDataset(NULL),
	mLeaf(leaf),
	mNode(),
	mSeqData(NULL),
	mAncData(NULL),
	mTaxonName(NULL),
    mLabel(NULL),
	mNodeData(NULL),
	mNodeDataFP(NULL),
	mNodeDataML(NULL),
	mVisited(false),
	mIndex(0), // This should be set with the assignment operator later
	mNchars(),
    mNchildren(0),
	mPostChild1(-1),
	mPostChild2(-1),
	mPostDifferences(0),
	mPostChanged( !leaf),
	mPreChild1(-1),
	mPreChild2(-1),
	mPreDifferences(0),
	mPreChanged(true)
{
}

/**
 * Copy Constructor for QNodeInfo
 */
QNodeInfo::QNodeInfo(const QNodeInfo& orig)  
:
	mMultChar(orig.mMultChar),
	mDataset(orig.mDataset),
	mSSDataset(orig.mSSDataset),
	mLeaf(orig.mLeaf),
	mNode(orig.mNode),
	mSeqData(orig.mSeqData),
	mAncData(NULL),
	mTaxonName(orig.mTaxonName),
	mLabel(orig.mLabel),
	mNodeData(NULL),
	mNodeDataFP(NULL),
	mNodeDataML(NULL),
	mVisited(false),
	mIndex(orig.mIndex),
	mNchars(),
    mNchildren(0),
	mPostChild1(orig.mPostChild1),
	mPostChild2(orig.mPostChild2),
	mPostDifferences(orig.mPostDifferences),
	mPostChanged(orig.mPostChanged),
	mPreChild1(orig.mPreChild1),
	mPreChild2(orig.mPreChild2),
	mPreDifferences(orig.mPreDifferences),
	mPreChanged(orig.mPreChanged)  
{
	 mPostChild1 = -1;
	 mPostChild2 = -1;
	 mPostChanged = ! orig.leaf();
	 mPostDifferences = 0;	
	 
	 mPreChild1 = -1;
	 mPreChild2 = -1;
	 mPreChanged = true;
	 mPreDifferences = 0;		 

	/*Make a deep copy of mAncData*/
	if(orig.mAncData)
	{
	mAncData = new char [mNchars];
		for(int i=0;i<mNchars;i++)
			mAncData[i] = orig.mAncData[i];
	} 
}

QNodeInfo::QNodeInfo(const QNodeInfo* orig)
:
	mMultChar(orig->mMultChar),
	mDataset(orig->mDataset),
	mSSDataset(orig->mSSDataset),
	mLeaf(orig->mLeaf),
	mNode(orig->mNode),
	mSeqData(orig->mSeqData),
	mAncData(NULL),
	mTaxonName(orig->mTaxonName),
	mLabel(orig->mLabel),
	mNodeData(NULL),
	mNodeDataFP(NULL),
	mNodeDataML(NULL),
	mVisited(false),
	mIndex(orig->mIndex),
	mNchars(orig->mNchars),
    mNchildren(orig->mNchildren),
	mPostChild1(orig->mPostChild1),
	mPostChild2(orig->mPostChild2),
	mPostDifferences(orig->mPostDifferences),
	mPostChanged(orig->mPostChanged),
	mPreChild1(orig->mPreChild1),
	mPreChild2(orig->mPreChild2),
	mPreDifferences(orig->mPreDifferences),
	mPreChanged(orig->mPreChanged)  
{

	/*Make a deep copy of mAncData*/
	if(orig->mAncData)
	{
	mAncData = new char [mNchars];
		for(int i=0;i<mNchars;i++)
			mAncData[i] = orig->mAncData[i];
	} 

#ifdef QUINN
	mNodeData = new ParsimonyNodeData( orig->nodeData() );
#else
	mNodeData = NULL;
	mPostChild1 = -1;
	mPostChild2 = -1;
	mPostChanged = ! orig->leaf();
	mPostDifferences = 0;	
	
	mPreChild1 = -1;
	mPreChild2 = -1;
	mPreChanged = true;
	mPreDifferences = 0;	
	
#endif
}

/**
 * Assignment operator for QNodeInfo
 *
 * \alert{The Assignment operator is only partially implemented}
 */
const QNodeInfo& QNodeInfo::operator=(const QNodeInfo& orig)
{
	FILE* ferr;
	
	ferr = fdopen(2,"w");

	fprintf(ferr,"WARNING: QNodeInfo::operator= called but not fully implemented\n");

	/*Make a deep copy of mAncData*/
	if(orig.mAncData)
	{
	mAncData = new char [mNchars];
		for(int i=0;i<mNchars;i++)
			mAncData[i] = orig.mAncData[i];
	} 

  return *this;
}

/**
 * Create a new NodeInfo
 */
QNodeInfo::QNodeInfo( int nodeNumber, bool leaf) 
:
	mMultChar(NULL),
	mDataset(NULL),
	mSSDataset(NULL),
	mLeaf(leaf),
	mNode(NULL),
	mSeqData(NULL),
	mAncData(NULL),
	mTaxonName(NULL),
	mLabel(NULL),
	mNodeData(NULL),
	mNodeDataFP(NULL),
	mNodeDataML(NULL),
	mVisited(false),
	mIndex(nodeNumber),
	mNchars(0),
    mNchildren(0),
	mPostChild1(-1),
	mPostChild2(-1),
	mPostDifferences(0),
	mPostChanged(!leaf),
	mPreChild1(-1),
	mPreChild2(-1),
	mPreDifferences(0),
	mPreChanged(true) 
{
    //    static int myVar = 0;
    Dataset* dataset = Interpreter::getInstance()->dataset();
    if (leaf) {
      mTaxonName = dataset->getTaxonName(nodeNumber);
      mSeqData = dataset->getCharacters(nodeNumber,true);
      mNchars = dataset->nchars();
    } else {
      mSeqData = NULL;
      mTaxonName = NULL;
      mNchars = dataset->nchars();
    }
		
}

/**
 * Delete everything that was allocated
 */
QNodeInfo::~QNodeInfo()
{
	if(!leaf())
		if(mTaxonName)
			delete mTaxonName;

    if(mNodeData)
        delete mNodeData;
	if(mNodeDataFP)
		delete mNodeDataFP;
    if(mNodeDataML)
        delete mNodeDataML;
	if(mAncData)
		delete mAncData;
    // Dataset will delete the sequence data and taxon names

}

QNode*& QNodeInfo::node() { return mNode; }
QNode* QNodeInfo::node() const { return mNode; }
bool& QNodeInfo::leaf()           { return mLeaf; } 
bool QNodeInfo::leaf() const     { return mLeaf; } 
const char* QNodeInfo::getSeqData() const { return mSeqData; }

const char* QNodeInfo::getTaxonName() const { return mTaxonName; }
void QNodeInfo::setTaxonName(const char* name)
{
	if(leaf()) return; //this is only valid for internal nodes
	if(mTaxonName)
		delete mTaxonName;
	mTaxonName = strdup(name);
}

const char* QNodeInfo::getLabel() const { return mLabel; }
void QNodeInfo::setLabel(const char *label) 
{ 
	if(leaf()) return; //this is only valid for internal nodes
	if(mLabel)
		delete mLabel;
	mLabel = strdup(label);
}


int& QNodeInfo::nodeIndex() { return mIndex; }
int QNodeInfo::nodeIndex() const { return mIndex; }
int& QNodeInfo::nchars() { return mNchars; }
int QNodeInfo::nchars() const { return mNchars; }
int& QNodeInfo::nchildren() { return mNchildren; }
int QNodeInfo::nchildren() const { return mNchildren; }
bool& QNodeInfo::visited() { return mVisited; }
bool QNodeInfo::visited() const { return mVisited; }
ParsimonyNodeData*& QNodeInfo::nodeData() { return mNodeData; }
ParsimonyNodeData* QNodeInfo::nodeData() const { return mNodeData; }
FastParsimonyNodeData*& QNodeInfo::nodeDataFP() { return mNodeDataFP; }
FastParsimonyNodeData* QNodeInfo::nodeDataFP() const { return mNodeDataFP; }
LikelihoodNodeData*& QNodeInfo::nodeDataML() { return mNodeDataML; }
LikelihoodNodeData* QNodeInfo::nodeDataML() const { return mNodeDataML; }

/**
 * Print yourself out
 */
void QNodeInfo::print()
{
    printf("nodeData[%d]=%p, ",nodeIndex(), mNodeData);
    if(node() == NULL) 
    {
        printf("QnodeInfo:print() null node\n");
        return;
    }
        
    node()->print();
    if(!leaf())
    {
        if(node()->internal1())
        {
            node()->internal1()->print();
        }
        if(node()->internal2())
        {
            node()->internal2()->print();
        }
    }
    printf("\n");
}

void QNodeInfo::writeSequence()
{
	PsodaPrinter* p;
	int i;
	p=PsodaPrinter::getInstance();
	if(nchars()==0)
	{
		throw PsodaWarning("A zero length sequence can not be written\n");
	}
	
	if(leaf())
	{
		for(i=0;i<nchars();i++) {
			p->write("%c",SeqChar(mSeqData[i]));
		}
	}
	else
	{
		if(mAncData)
			for(i=0;i<nchars();i++)
				p->write("%c",SeqChar(mAncData[i]));
		else
			for(i=0;i<nchars();i++)
				p->write(" ");
	}
}

#define BASE_A 0x1<<0
#define BASE_C 0x1<<1
#define BASE_G 0x1<<2
#define BASE_T 0x1<<3

char QNodeInfo::SeqChar(char base)
{
	switch(base)
	{
	case BASE_A:
		return 'A';
	case BASE_C:
		return 'C';
	case BASE_G:
		return 'G';
	case BASE_T:
		return 'T';
		
	case BASE_A | BASE_G:
		return 'R';
	case BASE_C | BASE_T:
		return 'Y';
	case BASE_A | BASE_C:
		return 'M';
	case BASE_G | BASE_T:
		return 'K';
	case BASE_C | BASE_G:
		return 'S';
	case BASE_A | BASE_T:
		return 'W';
		
	case BASE_A | BASE_C | BASE_T:
		return 'H';
	case BASE_C | BASE_G | BASE_T:
		return 'B';
	case BASE_A | BASE_G | BASE_T:
		return 'D';
	case BASE_A | BASE_C | BASE_G:
		return 'V';
	case BASE_A | BASE_C | BASE_G | BASE_T:
		return 'N';
	case 0x1f:
		return '-';
	}
	return '!';
}

int& QNodeInfo::postChild1() { return mPostChild1; }
int QNodeInfo::postChild1() const { return mPostChild1; }
int& QNodeInfo::postChild2() { return mPostChild2; }
int QNodeInfo::postChild2() const { return mPostChild2; }
int& QNodeInfo::postDifferences() { return mPostDifferences; }
int QNodeInfo::postDifferences() const { return mPostDifferences; }
bool& QNodeInfo::postChanged() { return mPostChanged; }
bool QNodeInfo::postChanged() const { return mPostChanged; }

int& QNodeInfo::preChild1() { return mPreChild1; }
int QNodeInfo::preChild1() const { return mPreChild1; }
int& QNodeInfo::preChild2() { return mPreChild2; }
int QNodeInfo::preChild2() const { return mPreChild2; }
int& QNodeInfo::preDifferences() { return mPreDifferences; }
int QNodeInfo::preDifferences() const { return mPreDifferences; }
bool& QNodeInfo::preChanged() { return mPreChanged; }
bool QNodeInfo::preChanged() const { return mPreChanged; }


char & QNodeInfo::AncState(int index)
{
	if(index < 0)
		throw PsodaWarning("Bad Ancestral State Index (index < 0)");
	if(index >= mNchars)
		throw PsodaWarning("Bad Ancestral State Index (index >- nchars)");
	if(!mAncData)
		mAncData = new char[mNchars];
	return mAncData[index];
}

char QNodeInfo::AncState(int index) const
{
	if(index < 0)
		throw PsodaWarning("Bad Ancestral State Index (index < 0)");
	if(index >= mNchars)
		throw PsodaWarning("Bad Ancestral State Index (index >= nchars)");
	if(!mAncData)
		throw PsodaWarning("Can not index NULL Ancestral State");
	return mAncData[index];
}
	
const char* QNodeInfo::AncState() const {
	if (!mAncData)
		throw PsodaWarning(string() + __FILE__ + " Ancestral State is NULL");
	return mAncData;
}
const char* QNodeInfo::AncState() {
	if (!mAncData)
		throw PsodaWarning(string() + __FILE__ + " Ancestral State is NULL");
	return mAncData; 
}
