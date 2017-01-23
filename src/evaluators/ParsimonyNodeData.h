#ifndef EVALUATOR_PARSIMONYNODEDATA
#define EVALUATOR_PARSIMONYNODEDATA


typedef enum ParsimonySiteDataIndex { PSDI_POST = 0, PSDI_PRE = 1, PSDI_FINAL = 2 } ParsimonySiteDataIndex;
class ParsimonyNodeData
{
  public:
    ParsimonyNodeData();
	ParsimonyNodeData( const ParsimonyNodeData* parsimony );
  private:
    ParsimonyNodeData( const ParsimonyNodeData& parsimony );
    ParsimonyNodeData& operator=( const ParsimonyNodeData& parsimony );
  public:
    virtual ~ParsimonyNodeData();

    void init( char* data, int length );
    void init( int length );

    void initSiteData( ParsimonySiteDataIndex, int length );
    void initSiteData( ParsimonySiteDataIndex, char *src, int length );

    char*& siteData( ParsimonySiteDataIndex index ) { return mSiteData[ index ]; }
    double& score( ParsimonySiteDataIndex index ) { return mScore[ index ]; }

    char* siteData( ParsimonySiteDataIndex index ) const { return mSiteData[ index ]; }
    double score( ParsimonySiteDataIndex index ) const { return mScore[ index ]; }

  protected:
    char* mSiteData[3];
    double mScore[3];
	int mLength;
    bool mLeaf;
};
#endif

