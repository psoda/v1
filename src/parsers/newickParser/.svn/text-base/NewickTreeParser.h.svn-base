#ifndef SODA_NEWICKTREEPARSER 
#define SODA_NEWICKTREEPARSER

#include <stdio.h>
#include "QTree.h"
#include <vector>
using namespace std;

class QTreeRepository;

class NewickTreeParser
{
  public:
    NewickTreeParser();
    NewickTreeParser(QTreeRepository* qTreeRepository);
  private:
    NewickTreeParser( const NewickTreeParser& newickTreeParser );
    NewickTreeParser& operator=( const NewickTreeParser& newickTreeParser );
    
    /**
     * Helper method for the other setBranchLength
     * This checks if the given node has an external node and if it does,
     * sets the branch length on it (if it hasn't been set yet),
     * or (otherwise) copies the branch length from the external node to the given node
     */
  public:
    virtual ~NewickTreeParser();

    //VIRTUAL INTERFACE FUNCTIONS
    //GETTERS & SETTERS
    //INSTANCE SPECIFIC

    int parseFilename( const char* filename );
    int parseFile( FILE* file );
    QTree* parseBuffer( const char* buffer, int length, bool storeTreeInRepository = true);
    void reset();
    void uploadAndResetTree();

    void pushInteriorNode();
    QNode *popInteriorNode();
    void nextNode();
    void addInteriorNode();
    QNode *addLeafNode(int intToken, double branchLength = -1);
    QNode *addLeafNode(char *leafName, double branchLength = -1);
    void setNodeBranchLength(double newBranchLength);
    void setBranchLength(QNode* node, double newBranchLength);
    char* buildLabel(double label);
    char* buildLabel(int label);
    char* buildLabel(char *label);
    void setLabel(char *&label);
    void finishTree();
    void biggestNodeNumber(int intToken)
    {
      mBiggestNodeNumber = (mBiggestNodeNumber>intToken)?mBiggestNodeNumber:intToken;
    }

    void setTranslation(int num, char *name);
    int translate(int token);

    QTreeRepository*& qtreeRepository() { return mQTreeRepository; }
    QTreeRepository* qtreeRepository() const { return mQTreeRepository; }
    
  public:
    //lexical analyzer state
    void* yyscanner;      //peer object to NewickTreeParser
    FILE* inFile;
    //parser state
    QTree* qtree;           //assumed to be cleaned up by NewickTreeParser owner
    QNode* qcurrentNode;    //no need to cleanup will be child of tree
    int mNumberOfInteriorNodes;
    int mNumberOfLeafNodes;
    bool mFirstNode;
    int mBiggestNodeNumber;
    QTreeRepository* mQTreeRepository;
    //token info
    int mIntToken;
    bool mStoreTreeInRepository;
  protected:
    int *mTranslationVector;
};

#endif

