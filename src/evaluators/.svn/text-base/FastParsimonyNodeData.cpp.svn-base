#include "FastParsimonyNodeData.h"
#include "Interpreter.h"

#include <string.h>

FastParsimonyNodeData::FastParsimonyNodeData()
{
  memset( mSiteData, 0, sizeof ( c16vector* ) * 3 );
  memset( mScore, 0, sizeof ( double ) * 3 );
  //mSiteData = { NULL, NULL, NULL };
  //mScore = { 0, 0, 0 };
}

FastParsimonyNodeData::FastParsimonyNodeData( const FastParsimonyNodeData& orig )
:
	mLength(orig.mLength),
	mLeaf(orig.mLeaf)
{
  memcpy( mSiteData, orig.mSiteData, sizeof ( c16vector* ) * 3 );
  memcpy( mScore, orig.mScore, sizeof ( double ) * 3 );
  
}

FastParsimonyNodeData::FastParsimonyNodeData( const FastParsimonyNodeData* orig )
{
	memset( mSiteData, 0, sizeof ( c16vector* ) * 3 );
	memset( mScore, 0, sizeof ( double ) * 3 );
	
	for(int i = 0; i < 3; i++)
	{
		copySiteData((ParsimonySiteDataIndex)i, orig->mSiteData[i], orig->mLength);
		mScore[i] = orig->mScore[i];
	}
}


FastParsimonyNodeData& FastParsimonyNodeData::operator=( const FastParsimonyNodeData& orig )
{
  if ( this != &orig )
  {
  }
  return *this;
}

void FastParsimonyNodeData::init( char* data, int length )
{
  initSiteData( PSDI_POST, data, length );
  initSiteData( PSDI_PRE, length );
  initSiteData( PSDI_FINAL, length );
  mLength = length;
  mLeaf = true;
}

void FastParsimonyNodeData::init( int length )
{
  initSiteData( PSDI_POST, length );
  initSiteData( PSDI_PRE, length );
  initSiteData( PSDI_FINAL, length );
  mLength = length;
  mLeaf = false;
}

void FastParsimonyNodeData::initSiteData( ParsimonySiteDataIndex index, int length )
{
  //mSiteData[ (int) index] = static_cast<char*>( operator new ( sizeof ( char ) * length ) );
  mSiteData[ (int) index] = new c16vector[length];
  mScore[ index ] = 0 ;
  mLength = length;
}

void FastParsimonyNodeData::initSiteData( ParsimonySiteDataIndex index, char *src, int length )
{
  //mSiteData[ (int) index] = static_cast<char*>( operator new ( sizeof ( char ) * length ) );
  mSiteData[ (int) index] = new c16vector[length];
  for(int i=0;i<length;i++)
	for(int j=0;j<16;j++)
		if(i*16+j < Interpreter::getInstance()->dataset()->nchars())
			mSiteData[(int) index][i].c[j] = src[i*16+j];
		else
			mSiteData[(int) index][i].c[j] = 0;
  //mSiteData[ (int) index] = new char[length];
  mScore[ index ] = 0 ;
  mLength = length;
}

void FastParsimonyNodeData::copySiteData( ParsimonySiteDataIndex index, c16vector *src, int length )
{
  //mSiteData[ (int) index] = static_cast<char*>( operator new ( sizeof ( char ) * length ) );
  mSiteData[ (int) index] = new c16vector[length];
  for(int i=0;i<length;i++)
			mSiteData[(int) index][i].v = src[i].v;
  mScore[ index ] = 0 ;
  mLength = length;
}

FastParsimonyNodeData::~FastParsimonyNodeData()
{

    if (!mLeaf && mSiteData[PSDI_POST])
        delete[] mSiteData[PSDI_POST];
    if (mSiteData[PSDI_PRE])
        delete[] mSiteData[PSDI_PRE];
    if (mSiteData[PSDI_FINAL])
        delete[] mSiteData[PSDI_FINAL];
}

