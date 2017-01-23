#ifndef EVALUATOR_LikelihoodNODEDATA
#define EVALUATOR_LikelihoodNODEDATA

class LikelihoodNodeData
{
  public:
    LikelihoodNodeData();
  private:
    LikelihoodNodeData( const LikelihoodNodeData& Likelihood );
    LikelihoodNodeData& operator=( const LikelihoodNodeData& Likelihood );
  public:
    virtual ~LikelihoodNodeData();

    void init( const char* data, int length );
    void init( int length );


};
#endif

