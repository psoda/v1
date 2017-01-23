#include "ParsimonyNodeData.h"

#include <string.h>

ParsimonyNodeData::ParsimonyNodeData()
{
  memset( mSiteData, 0, sizeof ( char* ) * 3 );
  memset( mScore, 0, sizeof ( double ) * 3 );
  //mSiteData = { NULL, NULL, NULL };
  //mScore = { 0, 0, 0 };
}

ParsimonyNodeData::ParsimonyNodeData( const ParsimonyNodeData& orig )
:
	mLength(orig.mLength),
	mLeaf(orig.mLeaf)
{
  memcpy( mSiteData, orig.mSiteData, sizeof ( char* ) * 3 );
  memcpy( mScore, orig.mScore, sizeof ( double ) * 3 );
  
}

ParsimonyNodeData::ParsimonyNodeData( const ParsimonyNodeData* orig )
{
	memset( mSiteData, 0, sizeof ( char* ) * 3 );
	memset( mScore, 0, sizeof ( double ) * 3 );
	
	for(int i = 0; i < 3; i++)
	{
		initSiteData((ParsimonySiteDataIndex)i, orig->mSiteData[i], orig->mLength);
		mScore[i] = orig->mScore[i];
	}
}


ParsimonyNodeData& ParsimonyNodeData::operator=( const ParsimonyNodeData& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

void ParsimonyNodeData::init( char* data, int length )
{
  initSiteData( PSDI_POST, data, length );
  initSiteData( PSDI_PRE, length );
  initSiteData( PSDI_FINAL, length );
  mLength = length;
  mLeaf = true;
}

void ParsimonyNodeData::init( int length )
{
  initSiteData( PSDI_POST, length );
  initSiteData( PSDI_PRE, length );
  initSiteData( PSDI_FINAL, length );
  mLength = length;
  mLeaf = false;
}

void ParsimonyNodeData::initSiteData( ParsimonySiteDataIndex index, int length )
{
  //mSiteData[ (int) index] = static_cast<char*>( operator new ( sizeof ( char ) * length ) );
  mSiteData[ (int) index] = new char[length];
  mScore[ index ] = 0 ;
  mLength = length;
}

void ParsimonyNodeData::initSiteData( ParsimonySiteDataIndex index, char *src, int length )
{
  //mSiteData[ (int) index] = static_cast<char*>( operator new ( sizeof ( char ) * length ) );
  mSiteData[ (int) index] = src;
  //mSiteData[ (int) index] = new char[length];
  mScore[ index ] = 0 ;
  mLength = length;
}

ParsimonyNodeData::~ParsimonyNodeData()
{

    if (!mLeaf && mSiteData[PSDI_POST])
        delete[] mSiteData[PSDI_POST];
    if (mSiteData[PSDI_PRE])
        delete[] mSiteData[PSDI_PRE];
    if (mSiteData[PSDI_FINAL])
        delete[] mSiteData[PSDI_FINAL];
}

