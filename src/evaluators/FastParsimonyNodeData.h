#ifndef EVALUATOR_FAST_PARSIMONYNODEDATA
#define EVALUATOR_FAST_PARSIMONYNODEDATA

#include "ParsimonyNodeData.h"

typedef char v16qi __attribute__ ((vector_size (16)));
typedef char v8qi __attribute__ ((vector_size (8)));
typedef float v4sf __attribute__ ((vector_size (16)));

union c16vector
{
	v16qi v;
	v4sf sf;
	v8qi half[2];
	char c[16];
};

class FastParsimonyNodeData
{
  public:
    FastParsimonyNodeData();
	FastParsimonyNodeData( const FastParsimonyNodeData* parsimony );
  private:
    FastParsimonyNodeData( const FastParsimonyNodeData& parsimony );
    FastParsimonyNodeData& operator=( const FastParsimonyNodeData& parsimony );
  public:
    virtual ~FastParsimonyNodeData();

    void init( char* data, int length );
    void init( int length );

    void initSiteData( ParsimonySiteDataIndex, int length );
    void initSiteData( ParsimonySiteDataIndex, char *src, int length );

	void copySiteData( ParsimonySiteDataIndex, c16vector *src, int length );

    c16vector*& siteData( ParsimonySiteDataIndex index ) { return mSiteData[ index ]; }
    double& score( ParsimonySiteDataIndex index ) { return mScore[ index ]; }

    c16vector* siteData( ParsimonySiteDataIndex index ) const { return mSiteData[ index ]; }
    double score( ParsimonySiteDataIndex index ) const { return mScore[ index ]; }

  protected:
    c16vector* mSiteData[3];
    double mScore[3];
	int mLength;
    bool mLeaf;
};
#endif

