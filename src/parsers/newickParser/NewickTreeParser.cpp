#include "NewickTreeParser.h"
#include "QNode.h"
#include "QNodeInfo.h"
#include "QTree.h"
#include "QTreeRepository.h"
#include "NewickLexicalAnalyzer.h"
#include "PsodaException.h"
#include <string>
#include <stdio.h>
#include <assert.h>
#include "Interpreter.h"


NewickTreeParser::NewickTreeParser() :
  yyscanner(NULL),
  qtree(NULL),
  qcurrentNode(NULL),
  mNumberOfInteriorNodes(0),
  mNumberOfLeafNodes(0),
  mFirstNode(true),
  mBiggestNodeNumber(0),
  mIntToken(0),
  mStoreTreeInRepository(true),
  mTranslationVector(NULL)
{
  return;
}

NewickTreeParser::NewickTreeParser(QTreeRepository* qTreeRepository) :
  yyscanner(NULL),
  qtree(NULL),
  qcurrentNode(NULL),
  mNumberOfInteriorNodes(0),
  mNumberOfLeafNodes(0),
  mFirstNode(true),
  mBiggestNodeNumber(0),
  mIntToken(0),
  mStoreTreeInRepository(true),
  mTranslationVector(NULL)
{
	qtreeRepository() = qTreeRepository;
}

NewickTreeParser::NewickTreeParser( const NewickTreeParser& orig __attribute__((unused)) ) :
  yyscanner(NULL),
  qtree(NULL),
  qcurrentNode(NULL),
  mNumberOfInteriorNodes(0),
  mNumberOfLeafNodes(0),
  mFirstNode(true),
  mBiggestNodeNumber(0),
  mIntToken(0),
  mStoreTreeInRepository(true),
  mTranslationVector(NULL)
{
}

NewickTreeParser& NewickTreeParser::operator=( const NewickTreeParser& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

NewickTreeParser::~NewickTreeParser()
{
  if (mTranslationVector) {
    free(mTranslationVector);
    mTranslationVector = NULL;
  }

#if 0
	if (tree)
	{
		delete tree;
	}
#endif
}

int newickparse (void *YYPARSE_PARAM);
void newickset_scanner (void* scanner);
void newickset_filename (string newFilename);

int NewickTreeParser::parseFilename( const char* filename )
{
  FILE* file = fopen( filename, "r" ); 
  int returnCode = -1;
  if ( file )
  {
    newickset_filename( filename );
    returnCode = parseFile( file ); 
    fclose( file );
  } else {
    string errorMessage = "Could not open file: ";
    errorMessage += filename;
    throw PsodaException(errorMessage);
  }
  return returnCode;
}

int NewickTreeParser::parseFile( FILE* file )
{
  NewickTreeParser& thisRef = *this;
  newicklex_init( &thisRef.yyscanner );
  newickset_scanner( &thisRef.yyscanner );
  newickset_in( file, thisRef.yyscanner );
  newickset_extra( this, thisRef.yyscanner);
  int parserResultCode = newickparse( this );

  newicklex_destroy( thisRef.yyscanner );
  thisRef.yyscanner = NULL;
  return parserResultCode;
}

QTree* NewickTreeParser::parseBuffer( const char* buffer, int length, bool storeTreeInRepository )
{
  mStoreTreeInRepository = storeTreeInRepository;
  NewickTreeParser& thisRef = *this;
  newicklex_init( &thisRef.yyscanner );
  newickset_scanner( &thisRef.yyscanner );
  // We have to have 2 '\0' characters at the end for the scan_buffer command
  char *buff = new char[length+2];
  for (int i = 0; i < length+2; i++)
      buff[i] = '\0';
  strcpy(buff, buffer);
  newick_scan_buffer( const_cast<char*>(buff), length+2, thisRef.yyscanner );
  //newick_scan_string( const_cast<char*>(buffer), thisRef.yyscanner );
  newickset_extra( this, thisRef.yyscanner);
  int parserResultCode = newickparse( this ); 
  newicklex_destroy( thisRef.yyscanner ); 
  thisRef.yyscanner = NULL; 
  delete[] buff;
  return qtree; 
}

/** reset
 * assumes that the heap allocate tree created during a previous parse has been stored off or freed by the instance of NewickTreeParsers parent
 */
void NewickTreeParser::reset()
{
  yyscanner = NULL;
  qtree = NULL;
  mIntToken = 0;
  mNumberOfInteriorNodes = 0;
  mNumberOfLeafNodes = 0;
  mFirstNode = true;
}

void NewickTreeParser::uploadAndResetTree()
{
  if (mStoreTreeInRepository)
  {
      qtreeRepository()->addTree(qtree);
      //printf("NTP1: added tree %s\n", qtree->treeStr());
      //  qtreeRepository()->print(cout);
      qtree = NULL;
  }
  mIntToken = 0;
  mNumberOfInteriorNodes = 0;
  mNumberOfLeafNodes = 0;
  mFirstNode = true;
  mBiggestNodeNumber = 0;
}
QNode* NewickTreeParser::popInteriorNode()
{
  //fprintf(stderr,"Popping: Current node is %d\n", qcurrentNode->nodeInfo()->nodeIndex());
  qcurrentNode->internal1() = qcurrentNode->nodeInfo()->node(); /* I am finished. Connect up the list */
  qcurrentNode = qcurrentNode->internal1()->external();
  return qcurrentNode;
}

void NewickTreeParser::nextNode()
{
    qcurrentNode->internal1() = new QNode(*(qcurrentNode->nodeInfo()), ISLEAF); /* Add to the current list */
    qcurrentNode->nodeInfo()->nchildren()++;
    qcurrentNode = qcurrentNode->internal1();
    //fprintf(stderr, "Moved current internally\n");
}

void NewickTreeParser::setBranchLength(QNode* node, double newBranchLength) 
{
    assert(node->external() != NULL);

    if (node->branchLength() < 0)
    {
        node->external()->branchLength() = newBranchLength;
        node->branchLength() = newBranchLength;
    }
}

void NewickTreeParser::setNodeBranchLength(double newBranchLength) 
{
  if (newBranchLength < 0) 
  {
    // don't bother if the length is not valid
    return;
  }
  // we need to find out which node of the three doesn't have an external that has been given a branch length
  setBranchLength(qcurrentNode, newBranchLength);
  //setBranchLength(qcurrentNode->internal1(), newBranchLength);
  //setBranchLength(qcurrentNode->internal2(), newBranchLength);
}

char *NewickTreeParser::buildLabel(double label)
{
    char *l = (char *)malloc(256);
    snprintf(l, 255, "%lf", label);
    return l;
}

char *NewickTreeParser::buildLabel(int label)
{
    char *l = (char *)malloc(256);
    snprintf(l, 255, "%d", label);
    return l;
}

char *NewickTreeParser::buildLabel(char* label)
{
    char *l = (char *)malloc(256);
    snprintf(l, 255, "%s", label);
    return l;
}

void NewickTreeParser::setLabel(char *&label)
{
    if (label)
    {
        qcurrentNode->nodeInfo()->setLabel(label);
        free(label);
        label = NULL;
    }
}

void NewickTreeParser::finishTree()
{
    if (qtree->root()->nodeInfo()->nchildren() == 2)
    {
        //fprintf(stderr,"Unrooting tree\n");
        QNode* qoldRoot = qtree->root();
        //link the two children of the root node together to create a rootless tree
        qoldRoot->child1()->connect(qoldRoot->child2());
        qtree->root() = qoldRoot->child1();
        //fprintf(stderr,"Eliminated traditional root\n");

        qtree->nnodes() = mNumberOfInteriorNodes+ mNumberOfLeafNodes;
        qtree->removeNodeInfo(qoldRoot->nodeInfo());
        //fprintf(stderr,"Finished tree\n");
    }
    else
    {
        //fprintf(stderr, "This node needs slimmed [%d]\n", qtree->root()->nodeInfo()->nchildren());
        // Start at the beginning which is pointed to by nodeInfo structure
        QNode *trailer = qtree->root()->nodeInfo()->node();
        QNode *tmp     = qtree->root()->nodeInfo()->node()->internal1();
        for (int i = 0; i < qtree->root()->nodeInfo()->nchildren()-1; i++)
        {
            trailer = tmp;
            tmp = tmp->internal1();
            //fprintf(stderr, "Moved one location\n");
        }
        // I should now be on the last one added. 
        // I need to move its child to the first one
        // and then delete this one.
        qtree->root()->external() = tmp->external();
        qtree->root()->external()->external() = qtree->root();
        trailer->internal1() = qtree->root();
        free(tmp);
        //fprintf(stderr, "Node has been slimmed\n");
    }
}

void NewickTreeParser::addInteriorNode()
{
  //create node

    QNodeInfo *qinternalNodeInfo = new QNodeInfo(mBiggestNodeNumber+mNumberOfInteriorNodes+1, ISNTLEAF);
    qtree->insertNodeInfo(qinternalNodeInfo); // Insert the internal node on the tree list
    QNode *qinteriorNode = new QNode(*qinternalNodeInfo, ISLEAF);  /* Create as a leaf. We will add to it later */
    qinternalNodeInfo->node() = qinteriorNode;

    mNumberOfInteriorNodes++;
    //fprintf(stderr,"Created internal node %d\n", qinternalNodeInfo->nodeIndex());
  
    //attach node
    if ( ! mFirstNode )
    {
        qcurrentNode->connect( qinteriorNode );
        qcurrentNode = qinteriorNode;
        nextNode();
    }
    else
    {
        //we are going to throw away the root node hence we will duplicate its number.
        mBiggestNodeNumber--;
        qtree->root() = qinteriorNode;
        qcurrentNode = qinteriorNode;
        mFirstNode = false;
        nextNode();
    }
}

void NewickTreeParser::setTranslation(int num, char *name)
{
    Dataset *dataset = Interpreter::getInstance()->dataset();
    
    /* hack */
    //    cerr << "\nNewickTreeParser::setTranslation()\n\n";
    //    UnalignedData *unalignedData = Interpreter::getInstance()->unalignedData();
    //    cerr << unalignedData->toString() << endl;
    /* END hack */
    
    if (mTranslationVector == NULL) {
      //      printf("ntaxa(): %d\n", dataset->ntaxa());
      //      printf("ntaxa(): %d\n", dataset->getSequenceData()->getNumTaxa());
      //      printf("sequence data: %s\n", dataset->getSequenceData()->toString().c_str());
        mTranslationVector = (int *)malloc(dataset->ntaxa() * sizeof(int));
    }
    int taxonNumber = dataset->getTaxonNumber(name);
    if (taxonNumber == -1)
    {
      /* hack */
      //      taxonNumber = unalignedData->getNameIndex(name);
      if (taxonNumber == -1)
      {
	/* END hack */
      
	  fprintf(stderr, "Taxon \"%s\" does not match any from the dataset\n", name);
	  exit(1);
	}
    }
    if (num - 1 < 0 || num - 1 >= dataset->ntaxa()) {
      printf("out of bounds\n");
    }
    //    printf("size: %d\n", dataset->ntaxa());
    //    printf("num: %d\n", num - 1);
    mTranslationVector[num - 1] = taxonNumber;
}

int NewickTreeParser::translate(int token)
{
    if (mTranslationVector)
    {
        return mTranslationVector[token - 1];
    }
    else
    {
        return token-1;
    }
}


QNode *NewickTreeParser::addLeafNode(char *leafName, double branchLength)
{
    // Convert the name to a number and then just call addLeafNode with the number
    Dataset *dataset = Interpreter::getInstance()->dataset();
    int leafNumber = dataset->getTaxonNumber(leafName);
    //fprintf(stderr, "Leaf %s is mapped to number %d\n", leafName, leafNumber);

    // When adding this leaf to the tree, it needs to be a 1 based number, 
    // but we get a 0 based number from getTaxonNumber
    return addLeafNode(leafNumber+1, branchLength);
    //free(leafName);
}

QNode *NewickTreeParser::addLeafNode(int intToken, double branchLength)
{
    //create node
    mNumberOfLeafNodes++;
  
    int id = translate(intToken);
    //fprintf(stderr, "Leaf %d is mapped to number %d\n", intToken, id);
    QNodeInfo *qnodeInfo = new QNodeInfo(id, ISLEAF);
    qtree->insertNodeInfo(qnodeInfo); // Insert the internal node on the tree list
    QNode *qleaf = new QNode(*qnodeInfo,ISLEAF);
    qleaf->branchLength() = branchLength;
    qnodeInfo->node() = qleaf;

    //attach leaf node
    if ( ! mFirstNode )
    {
        qcurrentNode->connect( qleaf );
    }
    else
    {
        qtree->root() = qleaf;
        qcurrentNode = qleaf;
        mFirstNode = false;
    }

    //fprintf(stderr,"Added node %d\n", id);
    return qleaf;
}
