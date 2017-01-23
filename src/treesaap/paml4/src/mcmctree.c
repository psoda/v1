/* mcmctree.c 

   Markov chain Monte Carlo on trees (Bayesian phylogenetic analysis)
   
                   Ziheng YANG, December 2002

   cc -o mcmctree -march=pentiumpro -mcpu=pentiumpro -O4 -funroll-loops -fomit-frame-pointer -finline-functions mcmctree.c tools.c -lm
   cc -o mcmctree -m64 -march=opteron -mtune=opteron -ansi -O3 -funroll-loops -fomit-frame-pointer -finline-functions mcmctree.c tools.c -lm
   cc -o mcmctree -march=athlon -mcpu=athlon -O4 -funroll-loops -fomit-frame-pointer -finline-functions mcmctree.c tools.c -lm

   cc -o infinitesites -DINFINITESITES -march=athlon -mcpu=athlon -Wall -O2 -funroll-loops -fomit-frame-pointer -finline-functions mcmctree.c tools.c -lm

           cc -o mcmctree -fast mcmctree.c tools.c -lm
           cc -DHardBounds -o mcmctreeHB -fast mcmctree.c tools.c -lm
    
           cl -O2 -W3 mcmctree.c tools.c
           cl -DHardBounds -FemcmctreeHB.exe -O2 mcmctree.c tools.c
           mcmctree <ControlFileName>
*/

/*
#define INFINITESITES
*/


#include "paml.h"

#define NS            1200
#define NBRANCH      (NS*2-2)
#define NNODE        (NS*2-1)
#define MAXNSONS      3
#define NGENE         1000          /* used for gnodes[NGENE] */
#define LSPNAME       50
#define NCODE         4
#define NCATG         50
#define MaxNFossils   200


extern int noisy, NFunCall;
extern char BASEs[];

int GetOptions(char *ctlf);
int ReadTreeSeqs(FILE*fout);
int ReadBlengthGH (char infile[]);
int GenerateBlengthGH (char infile[]);
int GetMem(void);
void FreeMem(void);
int UseLocus(int locus, int copycondP, int setmodel, int setSeqName);
int AcceptRejectLocus(int locus, int accept);
void switchconPin(void);
int SetPGene(int igene, int _pi, int _UVRoot, int _alpha, double xcom[]);
int DownSptreeSetTime(int inode);
void getSinvDetS(double space[]);
int GetInitials(FILE *fout);
int GetGtree(int locus);
void printGtree(int printBlength);
int SetParameters(double x[]);
int ConditionalPNode(int inode, int igene, double x[]);
double lnpData(double lnpDi[]);
double lnpD_locus(int locus);
double lnpD_locus_Approx(int locus);
double lnptBD(void);
double lnptC(void);
int SetupPriorTimesFossilErrors(void);
double lnpriorTimes(void);
double lnpriorRates(void);
double logPriorRatio(double xnew, double xold, double a, double b, int distribution);
void copySptree(void);
void printSptree(void);
double Infinitesites (void);
double InfinitesitesClock (double *FixedDs);
int MCMC(FILE* fout);
int LabelOldCondP(int spnode);
double UpdateTimesOnly(double *lnL, double finetune);
double UpdateTimes(double *lnL, double finetune);
double UpdateRates(double *lnL, double finetune);
double UpdateRatesClock(double *lnL, double finetune);
double UpdateParameters(double *lnL, double finetune);
double UpdateParaRates(double finetune, double space[]);
double mixing(double finetune);
double UpdatePFossilErrors(double finetune);
int getPfossilerr (double postEFossil[], double nround);
int DescriptiveStatistics2Delete (FILE *fout, char infile[], int nbin, int nrho);

struct CommonInfo {
   char *z[NS], *spname[NS], seqf[256],outf[256],treef[256],daafile[96];
   char oldconP[NNODE];       /* update conP for node? (0 yes; 1 no) */
   int seqtype, ns, ls, ngene, posG[2],lgene[1], *pose, npatt, readpattern;
   int np, ncode, ntime, nrate, nrgene, nalpha, npi, ncatG, print, seed;
   int cleandata, ndata;
   int model, clock, fix_kappa, fix_omega, fix_alpha, fix_rgene, Mgene;
   int method, icode, codonf, aaDist, NSsites;
   double *fpatt, kappa, omega, alpha;
   double rgene[NGENE],piG[NGENE][NCODE];  /* not used */
   double (*plfun)(double x[],int np), freqK[NCATG], rK[NCATG], *conP, *fhK;
   double pi[NCODE];
   int    sconP, curconP;                    /* curconP = 0 or 1 */
   double *conPin[2], *conP0, space[10000];  /* fixed space unsafe */
   int conPSiteClass, NnodeScale;
   char *nodeScale;    /* nScale[ns-1] for interior nodes */
   double *nodeScaleF;       /* nScaleF[npatt] for scale factors */
}  com;

struct TREEB {
   int  nbranch, nnode, root, branches[NBRANCH][2];
}  tree;
struct TREEN { /* ipop is node number in species tree */
   int father, nson, sons[2], ibranch, ipop;
   double branch, age, label, *conP;
   char fossil;
}  *nodes, **gnodes, nodes_t[2*NS-1];

/* nodes_t[] is working space.  nodes is a pointer and moves around.  
   gnodes[] holds the gene trees, subtrees constructed from the master species 
   tree.  Each locus has a full set of rates (rates) for all branches on the 
   master tree, held in sptree.nodes[].rates.  Branch lengths in the gene 
   tree are calculated by using those rates and the divergence times.

   gnodes[][].label in the gene tree is used to store branch lengths estimated 
   under no clock when mcmc.usedata=2 (normal approximation to likelihood).
*/


struct SPECIESTREE {
   int nbranch, nnode, root, nspecies, nfossil;
   double RootAge[2], Pfossilerr, *CcomFossilErr;
   struct TREESPN {
      char name[LSPNAME+1], fossil, usefossil;  /* fossil: 0, 1(L), 2(U), 3(B), 4(G) */
      int father, nson, sons[2];
      double age, tfossil[2];     /* lower and upper bounds or alpha & beta */
      double *rates;              /* log rates for loci */
   } nodes[2*NS-1];
}  sptree;
/* all trees are binary & rooted, with ancestors unknown. */


struct DATA { /* locus-specific data and tree information */
   int ns[NGENE], ls[NGENE], npatt[NGENE], ngene, lgene[NGENE];
   int root[NGENE+1], conP_offset[NGENE], BlengthMethod;
   char *z[NGENE][NS], cleandata[NGENE];
   double *fpatt[NGENE], lnpT, lnpR, lnpDi[NGENE];
   double pi[NGENE][NCODE];
   double rgene[NGENE], kappa[NGENE], omega[NGENE], alpha[NGENE];
   double birth, death, sampling;
   double rgenegamma[2], kappagamma[2], omegagamma[2], alphagamma[2], sigma2gamma[2];
   double fossilerrorbeta[2];
   double sigma2[NGENE];  /* sigma2[g] are the variances */
   double *blMLE[NGENE], *Gradient[NGENE], *Hessian[NGENE];
}  data;

struct MCMCPARAMETERS {
   int burnin, nsample, sampfreq, usedata, covb0, saveconP, print;
   double finetune[6];
}  mcmc; /* control parameters */


char *models[]={"JC69","K80","F81","F84","HKY85","T92","TN93","REV"};
enum {JC69, K80, F81, F84, HKY85, T92, TN93, REV} MODELS;

int nR=4;
double PMat[16], Cijk[64], Root[4];
double _rateSite=1, OldAge=999;
int LASTROUND=0, BayesEB, debug=0, testlnL=0, NPMat=0; /* no use for this */

/* for sptree.nodes[].fossil: lower, upper, bounds, gamma, inverse-gamma */
enum {LOWER_F=1, UPPER_F, BOUND_F, GAMMA_F, IGAMMA_F} FOSSIL_FLAGS;
char *fossils[]={" ", "L", "U", "B", "G", "IG"}; 
enum {GAMMA, IGAMMA} DISTRIBUTIONS;
enum {ARITHMETIC, GEOMETRIC, BROWNIAN} BLengthMethods;
char *clockstr[]={"", "Global clock", "Independent rates", "Autocorrelated rates"};

#define MCMCTREE  1
#include "treesub.c"

int main (int argc, char *argv[])
{
   char ctlf[]="mcmctree.ctl";
   int i,j;
   FILE  *fout;

   noisy=3;
   com.alpha=0.;     com.ncatG=1;
   com.ncode=4;      com.clock=1;

   printf("MCMCTREE in %s\n", VerStr);
   if (argc>1) 
      { strcpy(ctlf, argv[1]); printf ("\nctlfile reset to %s.\n", ctlf); }

   data.birth=2;    data.death=1; data.sampling=0.05; 
   com.cleandata=0; mcmc.usedata=2;

   starttime();
   GetOptions(ctlf);
   fout=gfopen(com.outf,"w");

   fprintf(fout, "MCMCTREE (%s) %s\n", VerStr, com.seqf);
   fprintf(fout, "\nseed = %d\n", com.seed);

   ReadTreeSeqs(fout);

   if(mcmc.usedata==1) {
      if(com.seqtype!=0) error2("usedata = 1 for nucleotides only");
      if(com.alpha==0)
         com.plfun=lfun;
      else {
         if (com.ncatG<2 || com.ncatG>NCATG) error2("ncatG");
         com.plfun=lfundG;
      }
      if (com.model>HKY85)  error2("Only HKY or simpler models are allowed.");
      if (com.model==JC69 || com.model==F81) { com.fix_kappa=1; com.kappa=1; }
   }
   else if (mcmc.usedata==2) {
      com.model = 0;  com.alpha = 0;
   }
   else if(mcmc.usedata==3) {
      GenerateBlengthGH("out.BV");
      exit(0);
   }

   /* The error messages about RootAge are messy.  Tidy up. */
   if(sptree.RootAge[1]<=0 && sptree.nodes[sptree.root].fossil<=LOWER_F)
      error2("set RootAge in control file when there is no upper bound on root");

   if(data.fossilerrorbeta[0]==0 && !sptree.nodes[sptree.root].fossil) {
      if(sptree.RootAge[1] <= 0) 
         error2("set RootAge in control file when there is no upper bound on root");

      sptree.nodes[sptree.root].fossil = (sptree.RootAge[0]>0 ? BOUND_F : UPPER_F);
      for(i=0; i<2; i++) 
         sptree.nodes[sptree.root].tfossil[i] = sptree.RootAge[i];
   }
   printf("\nFossil calibration information read from the tree.\n");
   for(i=0; i<sptree.nspecies*2-1; i++) {
      if(j=sptree.nodes[i].fossil) 
         printf("Node %2d: %s (%9.4f, %9.4f)\n", i+1,fossils[j],sptree.nodes[i].tfossil[0],sptree.nodes[i].tfossil[1]);
   }
   
#if(defined INFINITESITES)
   Infinitesites();  
#else
   MCMC(fout);
#endif
   return(0);
}

int GetMem (void)
{
/* This allocates memory for conditional probabilities (conP).  
   gnodes[locus] is not allocated here but in GetGtree().

   conP0 is the space for the tips.  Only one copy is needed as the conditional 
   probabilities for tips are fixed according to the data and never change.  
   Space for conP0 is allocated for a locus only if the locus has unclean 
   (ambiguity) data, that is, if(data.cleandata[locus]==0).
   Conditional probabilities for internal nodes are com.conPin[2], allocated 
   according to data.ns[locus] and data.npatt[locus] at all loci.  Two copies 
   of the space are allocated, hence the [2].  The copy used for the current 
   gene trees is com.conPin[com.curconP] while the copy for proposed gene trees 
   is com.conPin[!com.curconP].  data.conP_offset[locus] marks the starting 
   position in conPin[] for each locus.

   Memory arrangement if(com.conPSiteClass=1):
   ncode*npatt for each node, by node, by iclass, by locus
*/
   /* sconP0: tips; sconP: internal nodes */
   int locus,j,k, sconP0, s1,sG=1, sfhK=0, g=data.ngene;
   double *conP0, *conP, *rates;

   /* get mem for conP (internal nodes), and conP0 (tips) */
   if(mcmc.usedata==1) {
      if(!com.fix_alpha && mcmc.saveconP) {
         com.conPSiteClass=1;  sG=com.ncatG;
      }
      data.conP_offset[0]=0;
      for(locus=0,com.sconP=sconP0=0; locus<g; locus++) {
         s1= com.ncode * data.npatt[locus];
         com.sconP += sG*s1*(data.ns[locus]-1)*sizeof(double);
         if(!data.cleandata[locus])
            sconP0 += s1*data.ns[locus]*sizeof(double);
         if(locus<g-1)
            data.conP_offset[locus+1]=data.conP_offset[locus]+sG*s1*(data.ns[locus]-1);
      }

      if((com.conPin[0]=(double*)malloc(2*com.sconP+sconP0))==NULL) 
         error2("oom conP");

      com.conPin[1]       =com.conPin[0]+com.sconP/sizeof(double);
      if(sconP0) com.conP0=com.conPin[1]+com.sconP/sizeof(double);
      printf("\n%d bytes for conP, %d bytes for conP0\n", 2*com.sconP,sconP0);

      /* set gnodes[locus][].conP for tips and internal nodes */
      com.curconP=0; conP0=com.conP0;
      for(locus=0; locus<g; locus++) {
         conP=com.conPin[0]+data.conP_offset[locus];
         for(j=data.ns[locus]; j<data.ns[locus]*2-1; j++,conP+=com.ncode*data.npatt[locus])
            gnodes[locus][j].conP = conP;
         if(!data.cleandata[locus]) {
            for(j=0; j<data.ns[locus]; j++,conP0+=com.ncode*data.npatt[locus])
               gnodes[locus][j].conP = conP0;
            UseLocus(locus, 0, mcmc.usedata, 0);
            InitConditionalPNode ();
         }
      }

      if(!com.fix_alpha) {
         for(locus=0; locus<g; locus++)
            sfhK = max2(sfhK, data.npatt[locus]);
         sfhK *= com.ncatG*sizeof(double);
         if((com.fhK=(double*)realloc(com.fhK,sfhK))==NULL) error2("oom");
      }

   }
   else if(mcmc.usedata==2) { /* allocate data.Gradient & data.Hessian */
      for(locus=0,k=0; locus<data.ngene; locus++)  
         k += (2*data.ns[locus]-1)*(2*data.ns[locus]-1+2);
      if((com.conPin[0]=(double*)malloc(k*sizeof(double)))==NULL)
         error2("oom varb");
      for(j=0; j<k; j++)  com.conPin[0][j]=-1;
      for(locus=0,j=0; locus<data.ngene; locus++) {
         data.blMLE[locus] = com.conPin[0]+j;
         data.Gradient[locus] = com.conPin[0]+j+(2*data.ns[locus]-1);
         data.Hessian[locus] = com.conPin[0]+j+(2*data.ns[locus]-1)*2;
         j += (2*data.ns[locus]-1)*(2*data.ns[locus]-1+2);
      }
   }

   if(com.clock>1) {  /* space for rates */
      s1=(sptree.nspecies*2-1)*g*sizeof(double);
      if(noisy) printf("%d bytes for rates.\n", s1);
      if((rates=(double*)malloc(s1))==NULL) error2("oom for rates");
      for(j=0; j<sptree.nspecies*2-1; j++) 
         sptree.nodes[j].rates = rates+g*j;
   }
   return(0);
}

void FreeMem (void)
{
   int locus, j;

   FOR(locus,data.ngene) free(gnodes[locus]);
   free(gnodes);
   if(mcmc.usedata) free(com.conPin[0]);
   if(mcmc.usedata==1) {
      for(locus=0; locus<data.ngene; locus++) {
         free(data.fpatt[locus]);
         for(j=0;j<data.ns[locus]; j++)
            free(data.z[locus][j]);
      }
   }
   if(com.clock>1)
      free(sptree.nodes[0].rates);

   if(mcmc.usedata==1 && com.alpha)
      free(com.fhK);
}


int UseLocus (int locus, int copyconP, int setModel, int setSeqName)
{
/* MCMCtree:
   This point nodes to the gene tree at locus gnodes[locus] and set com.z[] 
   etc. for likelihood calculation for the locus.  Note that the gene tree 
   topology (gnodes[]) is never copied, but nodes[].conP are repositioned in the 
   algorithm.  The pointer for root gnodes[][com.ns].conP is assumed to be the 
   start of the whole block for the locus.  
   If (copyconP && mcmc.useData), the conP for internal nodes point 
   to a fixed place (indicated by data.conP_offset[locus]) in the alternative 
   space com.conPin[!com.curconP].  Note that the conP for each locus uses the 
   correct space so that this routine can be used by all the proposal steps, 
   some of which operates on one locus and some change all loci.

   The conP for tips always point to com.conP0 (when com.cleandata=0), and are 
   not changed here.

   Try to replace this with UseLocus() for B&C.
*/
   int i, s1=com.ncode*data.npatt[locus], sG=(com.conPSiteClass?com.ncatG:1);
   double *conPt=com.conPin[!com.curconP]+data.conP_offset[locus];

   com.ns = data.ns[locus]; 
   com.ls = data.ls[locus];
   tree.root = data.root[locus]; 
   tree.nnode = 2*com.ns-1;
   nodes=gnodes[locus];
   if(copyconP && mcmc.usedata==1) { /* this preserves the old conP. */
      memcpy(conPt, gnodes[locus][com.ns].conP, sG*s1*(com.ns-1)*sizeof(double));
      for(i=com.ns; i<tree.nnode; i++)
         nodes[i].conP = conPt+(i-com.ns)*s1;
   }

   if(setModel && mcmc.usedata==1) {
      com.cleandata = data.cleandata[locus];
      for(i=0; i<com.ns; i++) 
         com.z[i] = data.z[locus][i];
      com.npatt = com.posG[1]=data.npatt[locus];  com.posG[0] = 0;
      com.fpatt = data.fpatt[locus];

      /* The following is model-dependent */
      com.kappa = data.kappa[locus];
      com.omega = data.omega[locus];
      com.alpha = data.alpha[locus];

      xtoy(data.pi[locus], com.pi, com.ncode);
      if(com.model<=TN93)
         EigenTN93(com.model, com.kappa, com.kappa, com.pi, &nR, Root, Cijk);

      if(com.alpha)
         DiscreteGamma (com.freqK,com.rK,com.alpha,com.alpha,com.ncatG,0);
/*
      com.NnodeScale = data.NnodeScale[locus];
      com.nodeScale = data.nodeScale[locus];
      nS = com.NnodeScale*com.npatt * (com.conPSiteClass?com.ncatG:1);
      for(i=0; i<nS; i++) 
         com.nodeScaleF[i]=0;
*/
   }
   if(setSeqName)
      for(i=0;i<com.ns;i++) com.spname[i] = sptree.nodes[nodes[i].ipop].name;
   return(0);
}



int AcceptRejectLocus (int locus, int accept)
{
/* This accepts or rejects the proposal at one locus.  
   This works for proposals that change one locus only.  
   After UseLocus(), gnodes[locus][ns].conP points to the alternative 
   conP space.  If the proposal is accepted, this copies the newly calculated 
   conP into gnodes[locus][ns].conP.  In either case, gnodes[].conP is 
   repositioned.
   Proposals that change all loci use switchconP() to accept the proposal.
*/
   int i, ns=data.ns[locus], s1=com.ncode*data.npatt[locus], sG=1;
   double *conP=com.conPin[com.curconP]+data.conP_offset[locus];

   if(mcmc.usedata==1) {
      if(com.conPSiteClass) sG=com.ncatG;
      if(accept)
         memcpy(conP, gnodes[locus][ns].conP, sG*s1*(ns-1)*sizeof(double));
      for(i=ns; i<ns*2-1; i++)
         gnodes[locus][i].conP = conP+(i-ns)*s1;
   }
   return(0);
}

void switchconPin(void)
{
/* This reposition pointers gnodes[locus].conP to the alternative com.conPin, 
   to avoid recalculation of conditional probabilities, when a proposal is 
   accepted in steps that change all loci in one go, such as UpdateTimes() 
   and UpdateParameters().
   Note that for site-class models (com.conPSiteClass), gnodes[].conP points 
   to the space for class 0, and the space for class 1 starts (ns-1)*ncode*npatt
   later.  Such repositioning for site classes is achieved in fx_r().
*/
   int i,locus;
   double *conP;

   com.curconP =! com.curconP;
   
   for(locus=0; locus<data.ngene; locus++) {
      conP=com.conPin[com.curconP]+data.conP_offset[locus];
      for(i=data.ns[locus]; i<data.ns[locus]*2-1; i++,conP+=com.ncode*data.npatt[locus])
         gnodes[locus][i].conP = conP;
   }
}



int SetPGene(int igene, int _pi, int _UVRoot, int _alpha, double xcom[])
{
/* This is not used. */
   if(com.ngene!=1) error2("com.ngene==1?");
   return (0);
}


int SetParameters (double x[])
{
/* This is not used. */
   return(0);
}

int GetPMatBranch2 (double PMat[], double t)
{
/* This calculates the transition probability matrix.
*/
   double Qrates[2], T,C,A,G,Y,R, mr;

   NPMat++;
   Qrates[0]=Qrates[1]=com.kappa;
   if(com.seqtype==0) {
      if (com.model<=K80)
         PMatK80(PMat, t, com.kappa);
      else if(com.model<=TN93) {
         T=com.pi[0]; C=com.pi[1]; A=com.pi[2]; G=com.pi[3]; Y=T+C; R=A+G;
         if (com.model==F84) { 
            Qrates[0]=1+com.kappa/Y;   /* kappa1 */
            Qrates[1]=1+com.kappa/R;   /* kappa2 */
         }
         else if (com.model<=HKY85) Qrates[1]=Qrates[0];
         mr=1/(2*T*C*Qrates[0] + 2*A*G*Qrates[1] + 2*Y*R);
         PMatTN93(PMat, t*mr*Qrates[0], t*mr*Qrates[1], t*mr, com.pi);
      }
   }
   return(0);
}


int ConditionalPNode (int inode, int igene, double x[])
{
   int n=com.ncode, i,j,k,h, ison;
   double t;

   for(i=0;i<nodes[inode].nson;i++) {
      ison=nodes[inode].sons[i];
      if (nodes[ison].nson>0  && !com.oldconP[ison])
         ConditionalPNode(ison, igene, x);
   }
   for(i=0;i<com.npatt*n;i++) nodes[inode].conP[i] = 1;

   for(i=0; i<nodes[inode].nson; i++) {
      ison = nodes[inode].sons[i];

      t = nodes[ison].branch*_rateSite;
      if(t < 0) {
         printf("\nt =%12.6f ratesite = %.6f", nodes[ison].branch, _rateSite);
         error2("blength < 0");
      }

      GetPMatBranch2(PMat, t);

      if(com.cleandata && nodes[ison].nson<1) /* tip */
         for(h=0; h<com.npatt; h++) 
            for(j=0;j<n;j++) 
               nodes[inode].conP[h*n+j] *= PMat[j*n+com.z[ison][h]];
      else {
         for(h=0; h<com.npatt; h++) 
            for(j=0;j<n;j++)  {
               for(k=0,t=0; k<n; k++)    /* t is used as temp */
                  t += PMat[j*n+k]*nodes[ison].conP[h*n+k];  /* expensive line! */
               nodes[inode].conP[h*n+j]*=t;
            }
      }
   }  /*  for (ison)  */

   /* node scaling.  Is this coded?  Please check.  */
   if(com.NnodeScale && com.nodeScale[inode])
      NodeScale(inode, 0, com.npatt);
   return (0);
}


double lnpD_locus (int locus)
{
/* This calculates ln{Di|Gi, Bi} using times in sptree.nodes[].age and rates.
   Rates are in data.rgene[] if (clock==1) and in sptree.nodes[].rates if 
   (clock==0).  Branch lengths in the gene tree, nodes[].branch, are calculated 
   in this routine but they may not be up-to-date before calling this routine.
   UseLocus() is called before this routine.
*/
   int  i,j, dad;
   double lnL=0, b,t;

   if (mcmc.usedata==0)  return(0);
   if(com.clock==1) {  /* clock */
      for(i=0; i<tree.nnode; i++)  /* age in gene tree */
         nodes[i].age = sptree.nodes[nodes[i].ipop].age;
      for(i=0; i<tree.nnode; i++) {
         if(i!=tree.root)
            nodes[i].branch = (nodes[nodes[i].father].age-nodes[i].age) * data.rgene[locus];
      }
   }
   else {              /* independent & correlated rates */
      for(i=0; i<tree.nnode; i++) {
         if(i==tree.root) continue;
         for(j=nodes[i].ipop,b=0; j!=nodes[nodes[i].father].ipop; j=dad) {
            dad = sptree.nodes[j].father;
            t = sptree.nodes[dad].age-sptree.nodes[j].age;
            b += t * sptree.nodes[j].rates[locus];
         }
         nodes[i].branch=b;
      }
   }
   if(mcmc.usedata==1)
      lnL = -com.plfun(NULL, -1);
   else if(mcmc.usedata==2)
      lnL = lnpD_locus_Approx(locus);

   return (lnL);
}

double lnpData (double lnpDi[])
{
/* This calculates the log likelihood, the log of the probability of the data 
   given gtree[] for each locus.
   This updates gnodes[locus][].conP for every node.
*/
   int j,locus;
   double lnL=0, y;

   if(mcmc.saveconP) 
      FOR(j,sptree.nspecies*2-1) com.oldconP[j]=0;
   for(locus=0; locus<data.ngene; locus++) {
      UseLocus(locus,0, mcmc.usedata, 1);
      y = lnpD_locus(locus);

      if(testlnL && fabs(lnpDi[locus]-y)>1e-5)
         printf("\tlnLi %.6f != %.6f at locus %d\n", lnpDi[locus],y,locus+1);
      lnpDi[locus]=y;
      lnL += y;
   }
   return(lnL);
}


double lnpD_locus_Approx (int locus)
{
/* This calculates the likelihood using the normal approxiamtion (Thorne et al. 
   1998).  The branch lengths on the unrooted tree estimated under no clock have 
   been read into nodes[][].label and the gradient and Hessian are in data.
   Gradient[] & data.Hessian[].  The branch lengths predicted from the rate-evolution 
   model (that is, products of rates and times) are in nodes[].branch.  
   The tree is rooted, and the two branch lengths around the root are summed up
   and treated as one branch length.  This is different from funSS_AHRS(), which 
   has the data branch lengths in nodes[].branch and calculate the predicted 
   likelihood by multiplying nodes[].age and nodes[].label (which holds the 
   rates).  Think about a redesign to avoid confusion.
*/
   int debug=0, i,j, ib, nb=tree.nnode-1-1;  /* don't use tree.nbranch, which is not up to date. */
   int son0=nodes[tree.root].sons[0], son1=nodes[tree.root].sons[1];
   double lnL, b[NS*2-1], *H=data.Hessian[locus];

   /* construct branch lengths */
   for(j=0; j<tree.nnode; j++) {
      if(j==son0 || j==son1) continue;
      ib = nodes[j].ibranch;
      if(j==tree.root)
         b[ib] = nodes[son0].branch+nodes[son1].branch - data.blMLE[locus][ib];
      else
         b[ib] = nodes[j].branch - data.blMLE[locus][ib];
   }

   if(debug) {
      OutaTreeN(F0,1,1);  FPN(F0);
      matout(F0, b, 1, nb);
      matout(F0, data.blMLE[locus], 1, nb);
   }

   if(mcmc.covb0==0) {  /* uses the Hessian matrix */
      for(i=0,lnL=0; i<nb; i++) {
         lnL -= b[i]*b[i]*H[i*nb+i]/2;
         for(j=0; j<i; j++)
            lnL -= b[i]*b[j]*H[i*nb+j];
      }
   }
   else {               /* this uses the variances only */
      for(i=0,lnL=0; i<nb; i++) {
         lnL -= b[i]*b[i]*H[i]/2;
      }
   }
   return (lnL);
}


int ReadBlengthGH (char infile[])
{
/* this reads the MLEs of branch lengths under no clock and their SEs, for 
   approximate calculation of sequence likelihood.  The MLEs of branch lengths 
   are stored in data.blMLE[locus] as well as nodes[].label, and the gradient 
   and Hessian in data.Gradient[locsu][] and data.Hessian[locus][].

   This also frees up memory for sequences.
*/
   FILE* fBGH = gfopen(infile,"r");
   char line[100];
   int locus, i,j, nb, son1,son2;
   double small=1e-20, *H, *root;
   int debug=0;

   if(noisy) printf("\nReading branch lengths, Gradient & Hessian from %s.\n", infile);
   j = (sptree.nspecies*2-2)*(sptree.nspecies*2-2 + 2);
   if((H=(double*)malloc(j*sizeof(double))) == NULL) error2("oom H");

   for(locus=0; locus<data.ngene; locus++) {
      printf("\n\nLocus %d\n", locus+1);
      UseLocus(locus, 0, 0, 1);
      nodes=nodes_t;
      fscanf(fBGH, "%d", &i);
      if(i!=com.ns) error2("ns not correct in ReadBlengthGH()");
      if(ReadaTreeN(fBGH, &i, &j, 0, 1)) 
         error2("error when reading gene tree");
      if(i==0) error2("gene tree should have branch lengths for error checking");
      if((nb=tree.nbranch) != com.ns*2-3) 
         error2("nb = ns * 2 -3 ?");

      if(debug) {
         FPN(F0); OutaTreeN(F0,1,1);  FPN(F0);
         OutaTreeB(F0);
         for(i=0; i<tree.nnode; i++) {
            printf("\nnode %2d: branch %2d (%9.5f) dad %2d  ", i+1, nodes[i].ibranch+1, nodes[i].branch, nodes[i].father+1);
            for(j=0; j<nodes[i].nson; j++) printf(" %2d", nodes[i].sons[j]+1);
         }
      }

      for(i=0; i<nb; i++)
         fscanf(fBGH, "%lf", &data.blMLE[locus][i]);
      for(i=0; i<nb; i++)
         fscanf(fBGH, "%lf", &data.Gradient[locus][i]);

      fscanf(fBGH, "%s", line);
      if(strstr(line,"HessianMatrix"))         mcmc.covb0 = 0;
      else if(strstr(line,"HessianDiagonal"))  mcmc.covb0 = 1;
      else                                     error2("expecting Hessian in in.BV");
      if(mcmc.covb0 == 0) {
         for(i=0; i<nb; i++)
            for(j=0; j<nb; j++)
               if(fscanf(fBGH, "%lf", &data.Hessian[locus][i*nb+j]) != 1)
                  error2("err when reading the hessian matrix in.BV");

         for(i=0,root=H+nb*nb; i<nb*nb; i++)  H[i] = data.Hessian[locus][i];
         eigenRealSym(H, nb, root, root+nb);
         for(i=0; i<nb; i++) 
            if(root[i]<0) break;
         if(i<nb) {
            printf("\nEigen values of H matrix\n");
            matout (F0, root, 1, nb);
         }
      }
      else {
         for(i=0; i<nb; i++)
            if (fscanf(fBGH, "%lf", &data.Hessian[locus][i]) != 1)
               error2("err when reading the hessian diagonal in.BV");
      }
      UseLocus(locus, 0, 0, 1);
      NodeToBranch();
      son1 = nodes[tree.root].sons[0];
      son2 = nodes[tree.root].sons[1];      
      nodes[tree.root].ibranch = (nodes[son1].nson==0 ? 0 : nodes[son2].ibranch-1);
      for(i=0; i<tree.nnode; i++) {
         if(i==tree.root || i==son1 || i==son2) continue;
         nodes[i].ibranch --;
      }
      nodes[son1].ibranch = nodes[son2].ibranch = -1;
      for(i=0; i<tree.nnode; i++) 
         nodes[i].branch = (nodes[i].ibranch==-1 ? 0 : data.blMLE[locus][nodes[i].ibranch]);

      if(debug) {
         FPN(F0);  FPN(F0);  OutaTreeN(F0,1,1);
         for(i=0; i<tree.nnode; i++) {
            printf("\nnode %2d: branch %2d (%9.5f) dad %2d  ", i+1, nodes[i].ibranch+1, nodes[i].branch, nodes[i].father+1);
            for(j=0; j<nodes[i].nson; j++) printf(" %2d", nodes[i].sons[j]+1);
         }
         FPN(F0);
      }
   }
   free(H);
   fclose(fBGH);
   /* free up memory for sequences */
   for(locus=0; locus<data.ngene; locus++) {
      free(data.fpatt[locus]);
      for(j=0;j<data.ns[locus]; j++)
         free(data.z[locus][j]);
   }

   return(0);
}


int GenerateBlengthGH (char infile[])
{
/* This generates the sequence-alignment and tree files for calculating branch
   lengths, gradient, and hessian.
   This mimics Jeff Thorne's estbranches program.
*/
   FILE *fseq, *ftree, *fctl;
   FILE *fBGH = gfopen(infile, "w");
   char tmpseqf[32], tmptreef[32], ctlf[32];
   char line[10000];
   int i, locus;

   mcmc.usedata = 1;
   for(locus=0; locus<data.ngene; locus++) {
      sprintf(tmpseqf, "tmp%d.txt", locus+1);
      sprintf(tmptreef, "tmp%d.trees", locus+1);
      sprintf(ctlf, "tmp%d.ctl", locus+1);
      fseq  = gfopen(tmpseqf,"w");
      ftree = gfopen(tmptreef,"w");
      fctl  = gfopen(ctlf,"w");

      UseLocus(locus, 0, 0, 1);
      for(i=0; i<com.ns; i++) 
         com.z[i] = data.z[locus][i];
      com.npatt = com.posG[1]=data.npatt[locus];  com.posG[0] = 0;
      com.fpatt = data.fpatt[locus];

      DeRoot();
      printPatterns(fseq);
      fprintf(ftree, "  1\n\n");
      OutaTreeN(ftree, 1, 0);   FPN(ftree);

      fprintf(fctl, "seqfile = %s\n", tmpseqf);
      fprintf(fctl, "treefile = %s\n", tmptreef);
      fprintf(fctl, "outfile = out\nnoisy = 3\n");
      if(com.seqtype) {
         fprintf(fctl, "seqtype = %d\n", com.seqtype);
      }
      fprintf(fctl, "model = %d\n", com.model);
      if(com.seqtype==1) {
         fprintf(fctl, "icode = %d\n", com.icode);
         fprintf(fctl, "fix_kappa = 0\n kappa = 2\n");
         fprintf(fctl, "fix_omega = 0\n omega = 0.5\n");
      }
      if(com.seqtype==2) {
         fprintf(fctl, "aaRatefile = %s\n", com.daafile);
      }
      if(com.alpha) 
         fprintf(fctl, "fix_alpha = 0\nalpha = 0.5\nncatG = %d\n", com.ncatG);
      fprintf(fctl, "Small_Diff = 0.1e-6\ngetSE = 2\n");
      fprintf(fctl, "method = %d\n", (com.alpha||com.ns<10 ? 0 : 1));

      fclose(fseq);  fclose(ftree);  fclose(fctl);

      if(com.seqtype) sprintf(line, "codeml %s", ctlf);
      else            sprintf(line, "baseml %s", ctlf);
      system(line);

      appendfile(fBGH, "rst2");
   }
   fclose(fBGH);
   return(0);
}



int GetOptions (char *ctlf)
{
   int i, nopt=27, lline=4096;
   char line[4096],*pline, opt[20], *comment="*#";
   char *optstr[] = {"seed", "seqfile","treefile", "outfile","seqtype", 
        "aaRatefile", "icode", "usedata", "ndata", "model", "clock", 
        "RootAge", "fossilerror", "alpha", "ncatG", "cleandata", 
        "BlengthMethod", "BDparas", "kappa_gamma", "alpha_gamma", 
        "rgene_gamma", "sigma2_gamma", "print", "burnin", "sampfreq", 
        "nsample", "finetune"};
   double t=1, *eps=mcmc.finetune;
   FILE  *fctl=gfopen (ctlf, "r");

   if (fctl) {
      if (noisy) printf ("\nReading options from %s..\n", ctlf);
      for (;;) {
         if (fgets (line, lline, fctl) == NULL) break;
         for (i=0,t=0,pline=line; i<lline&&line[i]; i++)
            if (isalnum(line[i]))  { t=1; break; }
            else if (strchr(comment,line[i])) break;
         if (t==0) continue;
         sscanf (line, "%s%*s%lf", opt, &t);
         if ((pline=strstr(line, "="))==NULL) error2 ("option file.");

         for (i=0; i<nopt; i++) {
            if (strncmp(opt, optstr[i], 8)==0)  {
               if (noisy>=9)
                  printf ("\n%3d %15s | %-20s %6.2f", i+1,optstr[i],opt,t);
               switch (i) {
                  case ( 0):
                     com.seed=(int)t;
                     if(com.seed<=0) {
                        com.seed=abs((2*(int)time(NULL)+1));
                        printf("\nSeed used = %d\n", com.seed);
                     }
                     SetSeed(com.seed);
                     break;
                  case ( 1): sscanf(pline+1, "%s", com.seqf);    break;
                  case ( 2): sscanf(pline+1, "%s", com.treef);   break;
                  case ( 3): sscanf(pline+1, "%s", com.outf);    break;
                  case ( 4): com.seqtype=(int)t;    break;
                  case ( 5): sscanf(pline+2,"%s",com.daafile);   break;
                  case ( 6): com.icode=(int)t;      break;
                  case ( 7): mcmc.usedata=(int)t;   break;
                  case ( 8): com.ndata=(int)t;      break;
                  case ( 9): com.model=(int)t;      break;
                  case (10): com.clock=(int)t;      break;
                  case (11):
                     if((pline=strchr(line,'>')) !=NULL)
                        sscanf(pline+1, "%lf", &sptree.RootAge[0]);
                     if((pline=strchr(line,'<')) !=NULL)
                        sscanf(pline+1, "%lf", &sptree.RootAge[1]);
                     break;
                  case (12):
                     sscanf(pline+1,"%lf%lf", data.fossilerrorbeta,data.fossilerrorbeta+1);
                     break;
                  case (13): com.alpha=t;           break;
                  case (14): com.ncatG=(int)t;      break;
                  case (15): com.cleandata=(int)t;  break;
                  case (16): data.BlengthMethod=(int)t;  break;
                  case (17): 
                     sscanf(pline+1,"%lf%lf%lf", &data.birth,&data.death,&data.sampling);
                     break;
                  case (18): 
                     sscanf(pline+1,"%lf%lf", data.kappagamma, data.kappagamma+1); break;
                  case (19): 
                     sscanf(pline+1,"%lf%lf", data.alphagamma, data.alphagamma+1); break;
                  case (20): 
                     sscanf(pline+1,"%lf%lf", data.rgenegamma, data.rgenegamma+1); break;
                  case (21): 
                     sscanf(pline+1,"%lf%lf", data.sigma2gamma, data.sigma2gamma+1); break;
                  case (22): mcmc.print=(int)t;     break;
                  case (23): mcmc.burnin=(int)t;    break;
                  case (24): mcmc.sampfreq=(int)t;  break;
                  case (25): mcmc.nsample=(int)t;   break;
                  case (26):
                     sscanf(pline+1,"%lf%lf%lf%lf%lf%lf", eps,eps+1,eps+2,eps+3,eps+4,eps+5);
                     break;
               }
               break;
            }
         }
         if (i==nopt)
            { printf ("\noption %s in %s\n", opt, ctlf);  exit (-1); }
      }
      fclose(fctl);
   }
   else
      if (noisy) error2("\nno ctl file..");


   if(com.ndata>NGENE) error2("raise NGENE?");
   else if(com.ndata<=0) com.ndata=1;
   if(com.seqtype==0 || com.seqtype==2) 
      { com.fix_omega=1; com.omega=0; }

   if(com.seqtype==2) 
      com.ncode = 20;

   if(com.alpha==0)  { com.fix_alpha=1; com.nalpha=0; }
   if(com.clock<1 || com.clock>3) error2("clock should be 1, 2, 3?");

   return(0);
}


double lnpInfinitesitesClock (double t0, double FixedDs[])
{
/* This calculates the ln of the joint pdf, which is proportional to the 
   posterior for given root age, assuming infinite sites.  Fixed distances are 
   in FixedDs[]: d11, d12, ..., d(1,s-1), d21, d31, ..., d_g1.
   Note that the posterior is one dimensional, and the variable used is root age.
*/
   int s=sptree.nspecies, g=data.ngene, i,j;
   double lnp;
   
   sptree.nodes[sptree.root].age=t0;
   for(j=s; j<sptree.nnode; j++) 
      if(j!=sptree.root)
         sptree.nodes[j].age = t0*FixedDs[j-s]/FixedDs[0];

   lnp = lnpriorTimes();

   for(i=0; i<g; i++) {
      data.rgene[i] = (i==0 ? FixedDs[0]/t0 : FixedDs[s-1+i-1]/t0);
      lnp += (data.rgenegamma[0]-1)*log(data.rgene[i]) - data.rgenegamma[1]*data.rgene[i];
   }

   lnp += (2-s)*log(FixedDs[0]) + (s-g-2)*log(t0);  /* Jacobi */

   return (lnp);
}

double InfinitesitesClock (double *FixedDs)
{
/* This runs MCMC to calculate the posterior density of the root age 
   when there are infinite sites at each locus.  The clock is assumed, so that 
   the posterior is one-dimensional.
*/
   int i,j, ir, nround=0, nsaved=0;
   int s=sptree.nspecies, g=data.ngene;
   double t, tnew, naccept=0;
   double e=mcmc.finetune[0], lnp, lnpnew, lnacceptance, c, *x;
   double tmean, t025,t975;
   char timestr[32];

   matout2(F0, FixedDs, 1, s-1+g-1, 8, 4);
   printf("\nRunning MCMC to get %d samples for t0 (root age)\n", mcmc.nsample);
   t=sptree.nodes[sptree.root].age;
   lnp = lnpInfinitesitesClock(t, FixedDs);
   x=(double*)malloc(max2(mcmc.nsample,(s+g)*3)*sizeof(double));
   if(x==NULL) error2("oom x");

   for(ir=-mcmc.burnin,tmean=0; ir<mcmc.nsample*mcmc.sampfreq; ir++) {
      if(ir==0) { nround=0; naccept=0; tmean=0; }
      lnacceptance = e*rnd2normal8();
      c=exp(lnacceptance);
      tnew = t*c;
      lnpnew = lnpInfinitesitesClock(tnew, FixedDs);
      lnacceptance += lnpnew-lnp;

      if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
         t=tnew; lnp=lnpnew;  naccept++;
      }
      nround++;
      tmean = (tmean*(nround-1)+t)/nround;

      if(ir>=0 && (ir+1)%mcmc.sampfreq==0)
         x[nsaved++]=t;

      if((ir+1)%max2(mcmc.sampfreq, mcmc.sampfreq*mcmc.nsample/10000)==0)
         printf("\r%3.0f%%  %7.2f mean t0 = %9.6f", (ir+1.)/(mcmc.nsample*mcmc.sampfreq)*100, naccept/nround,tmean);
      if(mcmc.sampfreq*mcmc.nsample>20 && (ir+1)%(mcmc.sampfreq*mcmc.nsample/20)==0)
         printf(" %s\n", printtime(timestr));
   }

   qsort(x, (size_t)mcmc.nsample, sizeof(double), comparedouble);
   t025=x[(int)(mcmc.nsample*.025+.5)]; t975=x[(int)(mcmc.nsample*.975+.5)];

   /* Below x[] is used to collect the posterior means and 95% CIs */
   for(i=0; i<3; i++) {
      t = (i==0 ? tmean : (i==1 ? t025 : t975));
      lnpInfinitesitesClock(t, FixedDs);
      for(j=s; j<sptree.nnode; j++)
         x[i*(s+g)+(j-s)] = sptree.nodes[j].age;
      for(j=0; j<g; j++) 
         x[i*(s+g)+s-1+j]=data.rgene[j];
   }
   printf("\nmean & 95%% CI for times\n\n");
   for(j=s; j<sptree.nnode; j++)
      printf("Node %2d: %9.5f (%9.5f, %9.5f)\n", j+1,x[j-s],x[(s+g)+j-s],x[2*(s+g)+j-s]);
   printf("\nmean & 95%% CI for rates\n\n");
   for(j=0; j<g; j++)
      printf("gene %2d: %9.5f (%9.5f, %9.5f)\n", j+1,x[s-1+j], x[2*(s+g)+s-1+j], x[(s+g)+s-1+j]);

   printf("\nNote: the posterior has only one dimension.\n");
   free(x);
   exit(0);
}


double lnpInfinitesites (double FixedDs[])
{
/* This calculates the log joint pdf, which is proportional to the posterior 
   when there are infinite sites at each locus.  The variables in the posterior 
   are node ages, rate r0 for root son0, and mu & sigma2.  
   FixedDs holds fixed distances locus by locus.  The algorithm assumes clock=2.
*/
   int s=sptree.nspecies, g=data.ngene, locus,j;
   int root=sptree.root, *sons=sptree.nodes[root].sons;
   double lnJ, lnp, b, r0;  /* r0 is rate for root son0, fixed. */
   double t, t0=sptree.nodes[root].age - sptree.nodes[sons[0]].age;
   double t1=sptree.nodes[root].age - sptree.nodes[sons[1]].age;

   /* compute branch rates using times and branch lengths */
   for(locus=0; locus<g; locus++) {
      for(j=0; j<sptree.nnode; j++) {
         if(j==root || j==sons[0]) continue;  /* r0 is in the posterior */
         t = sptree.nodes[nodes[j].father].age - sptree.nodes[j].age;

         if(t<=0)
            error2("t<=0");
         if(j==sons[1]) {
            b=FixedDs[locus*sptree.nnode+sons[0]];
            r0=sptree.nodes[sons[0]].rates[locus];
            sptree.nodes[j].rates[locus] = (b-r0*t0)/t1;
            if(r0<=0 || t1<=0 || b-r0*t0<=0) {
               printf("\nr0 = %.6f t1 = %.6f b-t0*t0 = %.6f", r0, t1, b-r0*t0);
               error2("r<=0 || t1<=0 || b-r0*t0<=0...");
            }
         }
         else
            sptree.nodes[j].rates[locus] = FixedDs[locus*sptree.nnode+j]/t;
      }
   }
   if(debug==9) {
      printf("\n   (age tlength)        rates & branchlengths\n");
      for(j=0; j<sptree.nnode; j++,FPN(F0)) {
         t = (j==root? -1 : sptree.nodes[nodes[j].father].age - sptree.nodes[j].age);
         printf("%2d (%7.4f%9.4f)  ", j+1,sptree.nodes[j].age, t);
         for(locus=0; locus<g; locus++) printf(" %9.4f", sptree.nodes[j].rates[locus]);
         printf("  ");
         for(locus=0; locus<g; locus++) printf(" %9.4f", FixedDs[locus*sptree.nnode+j]);
      }
      sleep2(2);
   }

   lnp =  lnpriorTimes() + lnpriorRates();
   for(j=0,lnJ=-log(t1); j<sptree.nnode; j++) 
      if(j!=root && j!=sons[0] && j!=sons[1])
         lnJ -= log(sptree.nodes[nodes[j].father].age - sptree.nodes[j].age);
   lnp += g*lnJ;

   return (lnp);
}


double Infinitesites (void)
{
/* This reads the fixed branch lengths and runs MCMC to calculate the posterior
   of times and rates when there are infinite sites at each locus.  
   This is implemented for the global clock model (clock = 1) and the 
   independent-rates model (clock = 2), but not for the correlated-rates 
   model (clock = 3).
   This uses mcmc.finetune[] to propose changes.

   For clock=2, the MCMC samples the following variables: 
       s-1 times, rate r0 for left root branch at each locus, 
       mu at each locus & sigma2 at each locus.
*/
   int root=sptree.root, *sons=sptree.nodes[root].sons;
   int lline=10000, locus, nround, ir, i,j,k, ip,np, s=sptree.nspecies,g=data.ngene;
   int nxpr[2]={12,3};
   int MixingStep=1;
   char line[10000], timestr[36];
   double *e=mcmc.finetune, y, ynew, yb[2], naccept[4]={0};
   double lnL, lnLnew, lnacceptance, c,lnc;
   double *x,*mx, *FixedDs,*b, maxt0,tson[2];
   char mcmcout[32]="mcmc.out", *FidedDf[2]={"FixedDsClock1.txt", "FixedDsClock23.txt"};
   FILE *fdin=gfopen(FidedDf[com.clock>1],"r"), *fmcmcout=gfopen(mcmcout,"w");

   if(com.model!=0 || com.alpha!=0) error2("use JC69 for infinite data.");

   printSptree();
   GetMem();
   GetInitials(NULL);

   printf("\nInfinite sites\n");
   printf("Fixed branch lengths from %s (s = %d  g = %d):\n\n", FidedDf[com.clock>1], s,g);
   np = s-1 + g + g + g;   /* times, rates, mu & sigma2 */
   FixedDs=(double*)malloc((g*sptree.nnode+np*2)*sizeof(double));
   if(FixedDs==NULL) error2("oom");
   x=FixedDs+g*sptree.nnode;  
   mx=x+np;
   fscanf(fdin, "%d", &i);
   if(i!=s) error2("wrong number of species in FixedDs.txt");
   fgets(line, lline, fdin);

   if(data.fossilerrorbeta[0]) {
      puts("model of fossil errors for infinite data not tested yet.");
      getchar();
      SetupPriorTimesFossilErrors();
   }

   if(com.clock==1) { /* global clock: read FixedDs[] and close file. */
      for(i=0; i<s-1+g-1; i++) fscanf(fdin, "%lf", &FixedDs[i]);
      fclose(fdin);
      InfinitesitesClock(FixedDs);
     return(0);
   }
  
   for(locus=0,b=FixedDs,nodes=nodes_t; locus<g; locus++,b+=sptree.nnode) {
      ReadaTreeN(fdin,&i,&i,1,1);
      OutaTreeN(F0, 1, 1); FPN(F0);
      if(tree.nnode!=sptree.nnode) 
         error2("use species tree for each locus!");
      b[root]=-1;
      b[sons[0]]=nodes[sons[0]].branch+nodes[sons[1]].branch;
      b[sons[1]]=-1;
      for(j=0; j<sptree.nnode; j++) {
         if(j!=root && j!=sons[0] && j!=sons[1]) 
            b[j]=nodes[j].branch;
      }
   }
   fclose(fdin);

   printf("\nFixed distances at %d loci\n", g);
   for(j=0; j<sptree.nnode; j++,FPN(F0)) {
      printf("node %3d  ", j+1);
      for(locus=0; locus<g; locus++)
         printf(" %9.6f", FixedDs[locus*sptree.nnode+j]);
   }

   for(i=0; i<g; i++) { /* reset r0 so that r0*t0<b0.  GetInitial is unsafe. */
      y=FixedDs[i*sptree.nnode+sons[0]]/(sptree.nodes[root].age-sptree.nodes[sons[0]].age);
      sptree.nodes[sons[0]].rates[i] = y*rndu();
   }

   for(i=0; i<np; i++) mx[i]=0;
   for(i=0,k=0; i<sptree.nspecies-1; i++) x[k++]=sptree.nodes[s+i].age;
   for(i=0; i<g; i++) x[k++]=sptree.nodes[sons[0]].rates[i];
   for(i=0; i<g; i++) x[k++]=data.rgene[i];
   for(i=0; i<g; i++) x[k++]=data.sigma2[i];

   lnL = lnpInfinitesites(FixedDs);

   printf("\nStarting MCMC (np=%d) lnp = %9.3f\nInitials:            ", np, lnL);
   for(i=0; i<np; i++) printf(" %5.3f", x[i]);
   printf("\n\nparas: %d times, %d rates r0 (left daughter of root), %d mu, %d sigma2",s-1, g, g, g);
   printf("\nUsing finetune parameters from the control file\n");

   for(ir=-mcmc.burnin,nround=0; ir<mcmc.sampfreq*mcmc.nsample; ir++) {
      if(ir==0) { /* reset after burnin */
         nround=0; naccept[0]=naccept[1]=naccept[2]=naccept[3]=0;  zero(mx,com.np);
      }
      for(ip=0; ip<np; ip++) {
         lnacceptance = 0;
         if(ip<s-1) {  /* times */
            y = sptree.nodes[s+ip].age;
            for(i=0;i<2;i++) tson[i]=sptree.nodes[sptree.nodes[s+ip].sons[i]].age;
            yb[0]=max2(tson[0], tson[1]);
            yb[1]=(s+ip==root?OldAge:sptree.nodes[sptree.nodes[s+ip].father].age);
            if(s+ip==root) {
               for(i=0; i<g; i++) {
                  maxt0 = FixedDs[i*sptree.nnode+sons[0]]/sptree.nodes[sons[0]].rates[i];
                  yb[1] = min2(yb[1], tson[0]+maxt0);
               }
            }
            else if(s+ip==sons[0]) {
               for(i=0; i<g; i++) {
                  maxt0 = FixedDs[i*sptree.nnode+sons[0]]/sptree.nodes[sons[0]].rates[i];
                  yb[0] = max2(yb[0], sptree.nodes[root].age-maxt0);
               }
            }
            ynew = y + e[0]*rnd2normal8();
            ynew = sptree.nodes[s+ip].age = reflect(ynew,yb[0],yb[1]);
         }
         else if(ip-(s-1)<g) {    /* rate r0 for root son0 for loci (r0*t0<b0) */
            y = sptree.nodes[root].age-sptree.nodes[sons[0]].age;
            if(y<=0)
               printf("age error");
            yb[0] = 0;
            yb[1] = FixedDs[(ip-(s-1))*sptree.nnode+sons[0]]/y;
            y = sptree.nodes[sons[0]].rates[ip-(s-1)];
            ynew = y + e[1]*rnd2normal8();
            ynew = sptree.nodes[sons[0]].rates[ip-(s-1)] = reflect(ynew,yb[0],yb[1]);
         }
         else if (ip-(s-1+g)<g) { /* mu for loci */
            lnacceptance = e[3]*rnd2normal8();
            c=exp(lnacceptance);
            y = data.rgene[ip-(s-1+g)];
            ynew = data.rgene[ip-(s-1+g)] *= c;
            lnacceptance+=logPriorRatio(ynew, y, data.rgenegamma[0], data.rgenegamma[1], GAMMA);
         }
         else {                   /* sigma2 for loci */
            lnacceptance = e[3]*rnd2normal8();
            c=exp(lnacceptance);
            y = data.sigma2[ip-(s-1+g*2)];
            ynew = data.sigma2[ip-(s-1+g*2)] *= c;
            lnacceptance+=logPriorRatio(ynew, y, data.sigma2gamma[0], data.sigma2gamma[1], GAMMA);
         }

         lnLnew = lnpInfinitesites(FixedDs);
         lnacceptance += lnLnew-lnL;

         if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
            x[ip]=ynew;  lnL=lnLnew;
            if(ip<s-1)         naccept[0]++;   /* times */
            else if(ip<s-1+g)  naccept[1]++;   /* rates */
            else               naccept[3]++;   /* mu, sigma2 */
         }
         else {  /* reject */
            if(ip<s-1)                     /* times */
               sptree.nodes[s+ip].age = y;
            else if(ip-(s-1)<g)            /* rate r0 for root son0 */
               sptree.nodes[sons[0]].rates[ip-(s-1)] = y;
            else if (ip-(s-1+g)<g)         /* mu for loci */
               data.rgene[ip-(s-1+g)] = y;
            else                           /* sigma2 for loci */
               data.sigma2[ip-(s-1+g*2)] = y;
         }
      }

      if(MixingStep) {  /* this multiples times by c and divides r and mu by c. */
         lnc = e[2]*rnd2normal8();
         c = exp(lnc);
         lnacceptance = (s-1-g-g)*(lnc);
         for(j=s; j<sptree.nnode; j++)  sptree.nodes[j].age*=c;
         for(i=0; i<g; i++)             sptree.nodes[sons[0]].rates[i] /= c;
         for(i=0; i<g; i++) {
            y = data.rgene[i]; 
            ynew = data.rgene[i] /= c;
            lnacceptance+=logPriorRatio(ynew, y, data.rgenegamma[0], data.rgenegamma[1], GAMMA);
         }
         lnLnew = lnpInfinitesites(FixedDs);
         lnacceptance += lnLnew-lnL;
         if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
            for(j=s; j<sptree.nnode; j++)  x[j-s]=sptree.nodes[j].age;
            for(i=0; i<g; i++)             x[s-1+i]=sptree.nodes[sons[0]].rates[i];
            for(i=0; i<g; i++)             x[s-1+g+i]=data.rgene[i];
            lnL=lnLnew;
            naccept[2]++;
         }
         else {
            for(j=sptree.nspecies; j<sptree.nnode; j++)  sptree.nodes[j].age/=c;
            for(i=0; i<g; i++)  sptree.nodes[sons[0]].rates[i] *= c;
            for(i=0; i<g; i++)  data.rgene[i] *= c;
         }
      }
      nround++;
      for(i=0; i<np; i++) mx[i]=(mx[i]*(nround-1)+x[i])/nround;

      if((ir+1)%max2(mcmc.sampfreq, mcmc.sampfreq*mcmc.nsample/1000)==0) {
         printf("\r%3.0f%%", (ir+1.)/(mcmc.nsample*mcmc.sampfreq)*100.);
         printf(" %4.2f %4.2f %4.2f %4.2f  ", naccept[0]/((s-1)*nround),naccept[1]/(g*nround),naccept[2]/nround,naccept[3]/(g*2*nround));


         if(np<nxpr[0]+nxpr[1]) { nxpr[0]=com.np; nxpr[1]=0; }
         for(j=0; j<nxpr[0]; j++) printf(" %5.3f", mx[j]);
         if(np>nxpr[0]+nxpr[1] && nxpr[1]) printf(" -");
         for(j=0; j<nxpr[1]; j++) printf(" %5.3f", mx[com.np-nxpr[1]+j]);
         printf(" %5.2f", lnL);

         if(ir>0) {
            for(i=0; i<np; i++) fprintf(fmcmcout, " %7.5f", x[i]);  FPN(fmcmcout);
         }
      }
      if(mcmc.sampfreq*mcmc.nsample>20 && (ir+1)%(mcmc.sampfreq*mcmc.nsample/20)==0)
         printf(" %s\n", printtime(timestr));
   }
   free(FixedDs);

   printf("\nSummarizing MCMC results, time reset.\n");
   DescriptiveStatistics2Delete(NULL, mcmcout, 1, 1);

   exit(0);
}


int DownSptreeSetTime (int inode)
{
/* This goes down the species tree, from the root to the tips, to specify the 
   initial node ages.  If the age of inode is not set already, it will 
   initialize it.
   This is called by GetInitials().
*/
   int j,ison, correctionnews=0;

   for (j=0; j<sptree.nodes[inode].nson; j++) {
      ison = sptree.nodes[inode].sons[j];
      if(sptree.nodes[ison].nson) {   /* ison is not tip */
         if(sptree.nodes[ison].age == -1) {
            sptree.nodes[ison].age = sptree.nodes[inode].age *(.6+.4*rndu());
            correctionnews ++;
         }
         else if (sptree.nodes[ison].age > sptree.nodes[inode].age) {
            sptree.nodes[ison].age = sptree.nodes[inode].age *(0.95+0.5*rndu());
            correctionnews ++;
         }
         correctionnews += DownSptreeSetTime(ison);
      }
   }
   return(correctionnews);
}

int GetInitials (FILE *fout)
{
/* This sets the initial values for starting the MCMC, and returns np, the 
   number of parameters in the MCMC, to be collected in collectx().
   The routine guarantees that each node age is younger than its ancestor's age.
   It does not check the consistency of divergence times against the fossil 
   constraints.  As the model assumes soft bounds, any divergence times are 
   possible, even though this means that the chain might start from a poor 
   place.
*/
   int np=0, i,j, g=data.ngene;
   double maxlower=0; /* rought age for root */
   double a_r=data.rgenegamma[0], b_r=data.rgenegamma[1], a,b;

   com.rgene[0]=-1;  /* com.rgene[] is not used.  -1 to force crash */
   puts("\ngetting initial values to start MCMC.");

   /* set up rough time unit by looking at the fossil info */
   for(j=sptree.nspecies; j<sptree.nnode; j++)  {
      sptree.nodes[j].age=-1;

      if(sptree.nodes[j].fossil == BOUND_F) {
         sptree.nodes[j].age = 
            (sptree.nodes[j].tfossil[0]+sptree.nodes[j].tfossil[1])/2*(0.9+rndu()*0.2);
         maxlower = max2(maxlower, sptree.nodes[j].tfossil[0]);
      }
      else if(sptree.nodes[j].fossil == GAMMA_F) {
         sptree.nodes[j].age = 
            (sptree.nodes[j].tfossil[0]-1)/sptree.nodes[j].tfossil[1]*(0.9+rndu()*0.2);
         maxlower = max2(maxlower, PointGamma(0.025, sptree.nodes[j].tfossil[0], sptree.nodes[j].tfossil[1]));
      }
      else if(sptree.nodes[j].fossil == LOWER_F) {
         sptree.nodes[j].age = sptree.nodes[j].tfossil[0] * (1.1+0.2*rndu());
         maxlower = max2(maxlower, sptree.nodes[j].tfossil[0]);
      }
      else if(sptree.nodes[j].fossil == UPPER_F)
         sptree.nodes[j].age = sptree.nodes[j].tfossil[1] * (0.6+0.4*rndu());
   }

   if(sptree.nodes[sptree.root].age == -1) {
      maxlower = max2(maxlower, sptree.RootAge[0]);
      sptree.nodes[sptree.root].age = min2(maxlower*1.5, sptree.RootAge[1]) * (.8+.4*rndu());
   }
   for(i=0; i<1000; i++) {
      if(DownSptreeSetTime(sptree.root) == 0)
         break;
   }
   if(i==1000) 
      error2("Starting times are unfeasible even if I have tried a thousand times!\nTry again.");

   /* initial mu (mean rates) for genes */
   np = sptree.nspecies-1 + g;
   for(i=0; i<g; i++)
      data.rgene[i] = 0.01+rndgamma(a_r)/b_r;   /* mu */

   if(com.clock>1) {               /* sigma2, rates for nodes or branches */
      np += g + g*(sptree.nnode-1);

      /* sigma2 in lnrates among loci */
      for(i=0; i<g; i++)
         data.sigma2[i] = rndgamma(data.sigma2gamma[0])/data.sigma2gamma[1]+0.01;

      /* rates at nodes */
      for(j=0; j<sptree.nnode; j++) {
         if(j==sptree.root) {
            for(i=0; i<g; i++)  sptree.nodes[j].rates[i] = -99;
         }
         else {
            for(i=0; i<g; i++)  sptree.nodes[j].rates[i] = .01+rndgamma(a_r)/b_r;
         }
      }
   }

   /* set up substitution parameters */
   if(mcmc.usedata==1)
      for(i=0; i<g; i++) {
         if(com.model>=K80 && !com.fix_kappa) { data.kappa[i] = 1+2*rndu();  np++; }
         if(!com.fix_omega) {  data.omega[i] = .1+.2*rndu();  np++;  }
         if(!com.fix_alpha) {  data.alpha[i] = .5+.2*rndu();  np++;  }
      }

   if((a=data.fossilerrorbeta[0]) != 0) {
      b = data.fossilerrorbeta[1];
      sptree.Pfossilerr = a/(a+b)*(0.4+0.6*rndu());
      np ++;
   }
   return(np);
}

int collectx (double x[])
{
/* this collects parameters into x[] for printing and summarizing.
   It returns the number of parameters.

     clock=1: times, rates for genes, kappa, alpha
     clock=0: times, rates or rates by node by gene, sigma2, rho_ij, kappa, alpha
*/
   int printrates=1, i,j, np=0, g=data.ngene;

   for(i=sptree.nspecies; i<sptree.nspecies*2-1; i++) 
      x[np++] = sptree.nodes[i].age;
   for(i=0; i<g; i++)
      x[np++] = data.rgene[i];
   if(com.clock>1) {
      for(i=0; i<g; i++)
         x[np++] = data.sigma2[i];
      for(i=0; i<g; i++)
         for(j=0; j<sptree.nnode; j++) {
            if(j!=sptree.root) {
               if(printrates)  x[np++] = sptree.nodes[j].rates[i];
               else            x[np++] = log(sptree.nodes[j].rates[i]);
            }
         }
   }
   if(mcmc.usedata==1) {
      if(!com.fix_kappa)
         for(i=0; i<g; i++)
            x[np++]=data.kappa[i];

      if(!com.fix_alpha)
         for(i=0; i<g; i++)
            x[np++]=data.alpha[i];
   }
   if(data.fossilerrorbeta[0])
      x[np++] = sptree.Pfossilerr;

   if(np!=com.np) {
      printf("np in collectx is %d != %d\n", np, com.np);
      if(!mcmc.usedata && (com.model || com.alpha)) printf("\nUse JC69 for no data");
      error2("");
   }
   return(0);
}


#define P0t_BD(expmlt) (rho*(lambda-mu)/(rho*lambda +(lambda*(1-rho)-mu)*expmlt))

double PDFkernelBD(double t, double t1, double vt1, double lambda, double mu, double rho)
{
/* this calculates the kernel density from the birth-death process with 
   species sampling.
*/
   double pdf, expmlt, small=1e-20;

   if(fabs(lambda-mu)<small)
      pdf = (1+rho*lambda*t1)/(t1*square(1+rho*lambda*t));
   else {
      expmlt=exp((mu-lambda)*t);
      pdf = P0t_BD(expmlt);
      pdf = pdf*pdf * lambda/(vt1*rho) * expmlt;
   }
   return(pdf);
}

double CDFkernelBD(double t, double t1, double vt1, double lambda, double mu, double rho)
{
/* this calculates the CDF for the kernel density from the birth-death process with 
   species sampling.
*/
   double cdf, expmlt, small=1e-20;

   if(fabs(lambda-mu)<small)
      cdf = (1+rho*lambda*t1)*t/(t1*(1+rho*lambda*t));
   else {
      expmlt=exp((mu-lambda)*t);
      cdf = rho*lambda/vt1 * (1-expmlt)/(rho*lambda +(lambda*(1-rho)-mu)*expmlt);
   }
   return(cdf);
}



double lnptBD (void)
{
/* This calculates f_BD(t-C|tC), the conditional probability of other times given 
   the calibration times, that is, the first term in equation 3 in Yang & Rannala 
   (2006).  This is called by lnpriorTimes().  

   The routine uses sptree.nodes[].tfossil, sptree.nodes[].fossil,
   and lamdba, mu, rho from the birth-death process with species sampling 
   (data.birth, data.death, data.sampling).     
   The routine sorts the node ages in the species tree and then uses the 
   birth-death prior conditional on the calibration points.  
   t[0] is t1 in Yang & Rannala (2006).

   rankt[3]=1 means that the 3rd yougest age is node number ns+1.  This is set up
   for the calibration nodes only, = -1 for non-calibration nodes.  First we collect 
   all times into t[] and sort them. Next we collect all calibration times into tc[]
   and sort them.  Third, we find the ranks of calibration times in tc[], that is, 
   i1, i2, ..., ic in YB06.

   The term for root is not in this routine.  The root is excluded from the ranking.
*/
   int  i,j,k, n1=sptree.nspecies-1, rankprev, nfossil=0;
   int  ranktc[MaxNFossils]; /* ranks of calibration nodes */
   double t[NS-1], tc[MaxNFossils], t1=sptree.nodes[sptree.root].age;
   double lnpt, Scale, expmlt=1, vt1, P0t1, cdf, cdfprev, small=1e-20;
   double lambda=data.birth, mu=data.death, rho=data.sampling;
   int debug=0;

   /* (A) Calculate f_BD(t_{_C)}, joint of the non-calibration times */
   /* vt1 is needed only if (lambda != mu) */
   if(fabs(lambda-mu)>small) {
      expmlt= exp((mu-lambda)*t1);
      P0t1  = P0t_BD(expmlt);
      vt1   = 1-P0t1/rho*expmlt;
   }
   else {
      P0t1 = rho/(1+rho*mu*t1);
      vt1  = mu*t1*P0t1;
   }

   /* root (j=0) is excluded from the ranking. */
   for(j=1,lnpt=1,Scale=0; j<n1; j++) {
      k = sptree.nspecies+j;
      if(!sptree.nodes[k].usefossil)
         lnpt *= PDFkernelBD(sptree.nodes[k].age, t1, vt1, lambda, mu, rho);
      if(j%50==0) { Scale += log(lnpt); lnpt=1; }
   }
   lnpt = Scale + log(lnpt);

   /* (B) Calculate f_BD(t_C), marginal for calibration nodes.  This goes through the 
      calibration nodes in the order of their ages, with tc[] sorted beforehand.
   */
   for(j=sptree.nspecies; j<sptree.nnode; j++) {
      t[j-sptree.nspecies] = sptree.nodes[j].age;
      if(j!=sptree.root && sptree.nodes[j].usefossil) 
         tc[nfossil++] = sptree.nodes[j].age;
   }
   if(nfossil>MaxNFossils) error2("raise MaxNFossils?");

   if(nfossil) {
      /* The only reason for sorting t[] is to construct ranktc[].  */
      qsort(t,  (size_t)n1, sizeof(double), comparedouble);
      qsort(tc, (size_t)nfossil, sizeof(double), comparedouble);

      for(i=j=0; i<nfossil; i++) {
         if(i) j = ranktc[i-1]+1;
         for( ; j<n1; j++)
            if(tc[i]<t[j]) break;
         ranktc[i] = j;
      }
      if(debug==1) {
         matout2(F0, t, 1, n1, 9, 5);
         matout2(F0, tc, 1, nfossil, 9, 5);
         for(i=0; i<nfossil; i++) 
            printf("%9d", ranktc[i]);  
         FPN(F0);
      }
   }

   for(i=0,rankprev=0,cdfprev=0; i<nfossil+1; i++) {
      if(i < nfossil) {
         cdf = CDFkernelBD(tc[i], t1, vt1, lambda, mu, rho);
         k = ranktc[i]-rankprev-1;
      }
      else {
         cdf = 1;
         k = n1-rankprev-1;
      }
      if(k > 0)
         lnpt += LnGamma(k+1.0) - k*log(cdf-cdfprev);
      rankprev=ranktc[i];  cdfprev=cdf;
   }
   if(debug==1) printf("\npdf = %.12f\n", exp(lnpt));

   return (lnpt);
}


#ifndef HardBounds
   double tailL=0.025,  tailR=0.025;
#else
   double tailL=1e-299, tailR=1e-299;
#endif

double lnptC (void)
{
/* This calculates the prior density of times at calibration nodes as specified 
   by the fossil calibration information, the second term in equation 3 in 
   Yang & Rannala (2006).  
   a=tL, b=tU.

   The term for root is always calculated in this routine.  
   If there is a fossil at root and if it is a lower bound, it is re-set to a 
   pair of bounds.
*/
   int j, nfossil=0, fossil;
   double t, lnpt=0, a,b, theta,thetaL,thetaR;

   if(sptree.nfossil==0) return(0);

   for(j=sptree.nspecies; j<sptree.nnode; j++) {
      if(j!=sptree.root && !sptree.nodes[j].usefossil) 
         continue;
      nfossil++;
      t = sptree.nodes[j].age;
      a = sptree.nodes[j].tfossil[0]; 
      b = sptree.nodes[j].tfossil[1];
      fossil = sptree.nodes[j].fossil;

      if(j==sptree.root) {
         if(!sptree.nodes[j].usefossil) {  /* root fossil absent or deleted */
            a = sptree.RootAge[0];
            b = sptree.RootAge[1];
            fossil = (sptree.RootAge[0]>0 ? BOUND_F : UPPER_F);
         }
         else if (fossil==LOWER_F) {   /* root fossil is lower bound */
            b = sptree.RootAge[1];
            fossil = BOUND_F;
         }
      }

      switch(fossil) {
      case (LOWER_F):
         theta = log(0.1)/log(0.9);
         lnpt += log(tailL*theta/a);
         if(t<a) lnpt += (theta-1)*log(t/a);
         break;
      case (UPPER_F):
         thetaR = (1-tailR)/(tailR*b);
         lnpt += (t<b ? log((1-tailR)/b) : log(tailR*thetaR)-thetaR*(t-b));
         break;
      case (BOUND_F): 
         if(t>a && t<b)
            lnpt += log((1-tailL-tailR)/(b-a));
         else if(t<a) {
            thetaL = (1-tailL-tailR)*a/(tailL*(b-a));
            lnpt += log(tailL*thetaL/a) + (thetaL-1)*log(t/a);
         }
         else {
            thetaR = (1-tailL-tailR)/(tailR*(b-a));
            lnpt += log(tailR*thetaR) - thetaR*(t-b);
         }
         break;
      case (GAMMA_F):
         lnpt += a*log(b)-b*t+(a-1)*log(t)-LnGamma(a);
         break;
      }
   }
   return(lnpt);
}


double SampleFossil(int fossil, double a, double b, double *importance);
double getScaleFossilCombination (void);

double SampleFossil (int fossil, double a, double b, double *importance)
{
/* this routine is used by getScaleFossilCombination().  It samples a time from 
   the marginal fossil distribution with parameters a and b.

   LOWER_F is the same as BOUND_F except that it requires importance correction.
   It is used for non-root only, in which case b = RootAge[1], set up 
   before calling this routine.
   LOWER_F is the only case for which importance correction is needed.
*/
   double t, r=rndu(), theta, thetaL, thetaR;

   *importance = 1;
   switch(fossil) {
   case(LOWER_F):  
      theta = log(0.1)/log(0.9);
      thetaL = (1-tailL-tailR)*a/(tailL*(b-a));
      thetaR = (1-tailL-tailR)/(tailR*(b-a));
      if(r > tailL + tailR) { /* flat part */
         t = a + (b - a) * rndu();
         *importance = (theta/a) / thetaR;
      }
      else if (r < tailL) {  /* left tail */
         t = a * pow(rndu(), 1/thetaL);
         *importance = theta/thetaL * pow(t/a, theta-thetaL);
      }
      else {                 /* right tail */
         t = b - log(rndu())/thetaR;
         *importance = theta/a / (thetaR*exp(-thetaR*(t-b)));
      }
      break;
   case(UPPER_F):
      if(r > tailR)  { /* flat part */
         t = b*rndu();
      }
      else {  /* right tail */
         thetaR = (1-tailR)/(tailR*b);
         t = b - log(rndu())/thetaR;
      }
      break;
   case (BOUND_F):
      if(r > tailL + tailR)  /* flat part */
         t = a + (b - a) * rndu();
      else if (r < tailL) {  /* left tail */
         thetaL = (1-tailL-tailR)*a/(tailL*(b-a));
         t = a * pow(rndu(), 1/thetaL);
      }
      else {                 /* right tail */
         thetaR = (1-tailL-tailR)/(tailR*(b-a));
         t = b - log(rndu())/thetaR;
      }
      break;
   case(GAMMA_F):
      t = rndgamma(a)/b;
      break;
   default: 
      printf("fossil = %d in SampleFossil().", fossil);
      exit(-1);
   }
   return(t);
}


double getScaleFossilCombination (void)
{
/* This uses Monte Carlo integration to calculate the scale constant for the joint 
   prior on fossil times, when some fossils are assumed to be in error and not used.
   The scale constant is the integral of the density over the feasible region, where
   the time satisfy the age constraints on the tree.
   It is assumed that the ancestral nodes have smaller node numbers than descendent 
   nodes, as is the case if the node numbers are assigned by ReadaTreeN().
   nfs = nfossilused or nfs = nfossilused + 1, depending on whether 
   or not a fossil is used for the root.
*/
   signed char ConstraintTab[MaxNFossils][MaxNFossils]={0};  /* 1: row>col; 0: irrelevant */
   int nr=2000000, ir, i,j,k, nfs,ifs[MaxNFossils]={0}, feasible;
   int root=sptree.root, rootfossil = sptree.nodes[root].fossil;
   double C, mt[MaxNFossils]={0}, t[MaxNFossils], ab[MaxNFossils][2]={0}, a,b, y, impt, naccept;
   double theta=log(0.1)/log(0.9);
   int debug=1;

   if(sptree.nfossil > MaxNFossils) error2("raise MaxNFossils");
   /* construct constraint table */
   for(i=sptree.nspecies,nfs=0; i<sptree.nnode; i++) {
      if(i==root || sptree.nodes[i].usefossil) 
         ifs[nfs++] = i;
   }
   for(i=1; i<nfs; i++) {
      for(j=0; j<i; j++) {
         for(k=ifs[i]; k!=-1; k=sptree.nodes[k].father) {
            if(k == ifs[j]) { ConstraintTab[i][j] = 1; break; }
         }
      }
   }
   if(debug) {
      for(i=0; i<nfs; i++) {
         printf("\n%d (%2d %s): ", i+1, ifs[i]+1, fossils[sptree.nodes[ifs[i]].fossil]);
         for(j=0; j<i; j++)
            printf("%4d", ConstraintTab[i][j]);
      }
      FPN(F0);
   }

   /* set up fossil calibration info, edit lower bounds to use importance sampling. */
   for(i=0; i<nfs; i++) {
      j = ifs[i];
      ab[i][0] = sptree.nodes[j].tfossil[0]; 
      ab[i][1] = sptree.nodes[j].tfossil[1]; 

      if(j == root) { /* Importance correction is never used for root */
         if(!sptree.nodes[j].usefossil) {  /* root fossil absent or excluded */
            ab[i][0] = sptree.RootAge[0];
            ab[i][1] = sptree.RootAge[1];
            rootfossil = (sptree.RootAge[0]>0 ? BOUND_F : UPPER_F);
         }
         else if (sptree.nodes[root].fossil==LOWER_F) {   /* root fossil is lower bound */
            ab[i][1] = sptree.RootAge[1];
            rootfossil = BOUND_F;
         }
      }
      else if(sptree.nodes[j].fossil==LOWER_F) {  /* nonroot fossil is lower bound */
         /* this is the only case that need importance correction */ 
         ab[i][1] = sptree.RootAge[1];
         for(k=0; k<i; k++) {
            if(ConstraintTab[i][k] && (ifs[k]!=root || sptree.nodes[ifs[k]].usefossil)) {
               if(sptree.nodes[ifs[k]].fossil==UPPER_F || sptree.nodes[ifs[k]].fossil==BOUND_F)
                  ab[i][1] = min2(ab[i][1], sptree.nodes[ifs[k]].tfossil[1]);
               else if(sptree.nodes[ifs[k]].fossil==GAMMA_F) {
                  a=sptree.nodes[ifs[k]].tfossil[0];
                  b=sptree.nodes[ifs[k]].tfossil[1];
                  ab[i][1] = min2(ab[i][1], a/b+2*sqrt(a)/b);
               }
            }
         }
      }
   }

   /* sample from the marginal fossil densities and truncate by rejection */
   for(ir=0,naccept=0,C=0; ir<nr; ir++) {
      for(i=0,y=1; i<nfs; i++) {
         j = ifs[i];
         t[i] = SampleFossil((j==root?rootfossil:sptree.nodes[j].fossil), ab[i][0], ab[i][1], &impt);
         y *= impt;
      }

      feasible = 1;
      for(i=1; i<nfs; i++) {
         for(j=0; j<i; j++)
            if(ConstraintTab[i][j] && t[i]>t[j]) 
               { feasible=0; y=0; break; }
         if(!feasible) break;
      }

      if(feasible) naccept++;
      C = (ir*C + y)/(ir+1);
      for(i=0; i<nfs; i++) 
         mt[i] = (ir*mt[i] + t[i]*y)/(ir+1);
      if((ir+1)%500000 == 0) {
         printf("\r%7d %5.1f%% C %8.5f mt", ir+1, naccept/(ir+1)*100, C);
         for(i=0; i<nfs; i++) 
            printf(" %7.4f", mt[i]/C);
      }
   }
   naccept /= nr;
   if(naccept < 0.01) 
      puts("\aMC integration to calculate C is inefficient due to low acceptance rate");
   return(C);
}


int SetupPriorTimesFossilErrors (void)
{
/* This prepares for lnpriorTimes() under models of fossil errors.  It calculates 
   the scaling factor for the probability density of times for a given combination
   of the indicator variables, indicating which fossil is in error and not used.
   The combination in which all fossils are wrong is excluded.
*/
   int  is,i, ncomFerr=(1<<sptree.nfossil)-1, nused=0, it;
   int debug=1;

   if(data.fossilerrorbeta[0] == 0) error2("should not be here."); 
   sptree.CcomFossilErr = (double*)malloc(ncomFerr*sizeof(double));
   if(sptree.CcomFossilErr == NULL) error2("oom for CcomFossilErr");

   /* cycle through combinations of indicators, excluding that of all 0s. */
   for (i=0; i<ncomFerr; i++) {
      if(debug) printf("\n\n********* Com %2d/%2d: ", i+1, ncomFerr);

      it = i;
      for (is=sptree.nspecies, nused=0; is<sptree.nnode; is++) {
         if(sptree.nodes[is].fossil) {
            sptree.nodes[is].usefossil = 1 - it%2;
            nused += sptree.nodes[is].usefossil;
            it /= 2;
            if(debug) printf("%2d (%2d)", sptree.nodes[is].usefossil, is+1);
         }
      }

      if(debug) printf("  %2d fossils used", nused);

      sptree.CcomFossilErr[i] = getScaleFossilCombination();
   }

   return(0);
}


double lnpriorTimes (void)
{
/* This calculates the prior density of node times in the master species tree:
   sptree.nodes[].age. 
*/
   int  is,i, ncomFerr=(1<<sptree.nfossil)-1, nused, it;
   double pE = sptree.Pfossilerr, ln1pE=log(1-pow(pE,(double)sptree.nfossil));
   double lnpt=0, scaleF=-1e300, y;

   if(data.fossilerrorbeta[0]==0)  /* no fossil errors in model */
      lnpt = lnptC() + lnptBD();
   else {  /* cycle through combinations of indicators, excluding that of all 0s. */
      for(i=0; i<ncomFerr; i++) {
         it = i;
         for(is=sptree.nspecies, nused=0; is<sptree.nnode; is++) {
            if(sptree.nodes[is].fossil) {
               sptree.nodes[is].usefossil = 1 - it%2;
               nused += sptree.nodes[is].usefossil;
               it/=2;
            }
         }

         y = nused*log(1-pE)+(sptree.nfossil-nused)*log(pE)-ln1pE
           - log(sptree.CcomFossilErr[i]) + lnptC() + lnptBD();

         if(y < scaleF + 100)
            lnpt += exp(y-scaleF);
         else {         
            lnpt = lnpt*exp(scaleF-y) + 1;
            scaleF = y;
         }
         lnpt += exp(y-scaleF);
      }
      lnpt = scaleF + log(lnpt);
   }
   return (lnpt);
}

int getPfossilerr (double postEFossil[], double nround)
{
/* This is modified from lnpriorTimes(), and prints out the conditonal Perror 
   for each fossil given the times and pE.
*/
   int  is,i,k, nf=sptree.nfossil, ncomFerr=(1<<nf)-1, nused, it;
   double pE = sptree.Pfossilerr, ln1pE=log(1-pow(pE,(double)nf));
   double pt, pEf[MaxNFossils]={0}, scaleF=-1e300, y;
   char Ef[MaxNFossils];  /* indicators of fossil errors */

   for(i=0,pt=0; i<ncomFerr; i++) {
      it = i;
      for(is=sptree.nspecies, k=0, nused=0; is<sptree.nnode; is++) {
         if(sptree.nodes[is].fossil) {
            sptree.nodes[is].usefossil = 1 - it%2;
            nused += sptree.nodes[is].usefossil;
            Ef[k++] = !sptree.nodes[is].usefossil;
            it /= 2;
         }
      }
      if(k != nf) error2("k == nf?");

      y = nused*log(1-pE)+(nf-nused)*log(pE)-ln1pE
        - log(sptree.CcomFossilErr[i]) + lnptC() + lnptBD();

      if(y < scaleF + 200) {
         pt += y = exp(y-scaleF);
      }
      else {         /* change scaleF */
         pt = pt*exp(scaleF-y) + 1;
         for(k=0; k<nf; k++)
            pEf[k] *= exp(scaleF-y);
         scaleF = y;
         y = 1;
      }
      for(k=0; k<nf; k++)
         if(Ef[k]) pEf[k] += y;
   }
   for(k=0; k<nf; k++)
      pEf[k] /= pt;

   for(k=0; k<nf; k++)
      postEFossil[k] = (postEFossil[k]*(nround-1)+pEf[k])/nround;
   
   return (0);
}



int LabelOldCondP (int spnode)
{
/* This sets com.oldconP[j]=0 if node j in the gene tree needs updating, after 
   either rates or times have changed for spnode in the species tree.  This is to 
   avoid duplicated computation of conditional probabilities.  The routine workes 
   on the current gene tree and accounts for the fact that some species may be 
   missing at some loci.
   The routine first finds spnode or its first ancestor that is present in the 
   gene tree, then identifies that node in the genetree.  This reveals the 
   oldest node j in the gene tree that is descendent of spnode.  Node j and all 
   its ancestors in the gene tree need updating.

   Before calling this routine, set com.oldconP[]=1.  This routine changes some 
   com.oldconP[] into 0 but do not change any 0 to 1.

   The gene tree is in nodes[], as UseLocus has been called prior to this.
   This is called by UpdateTimes and UpdateRates.
*/
   int i, j=spnode;

   if(j>=tree.nnode || j!=nodes[j].ipop) {

      /* From among spnode and its ancestors in the species tree, find the 
         first node, i, that is in genetree.
      */
      for(i=spnode; i!=-1; i=sptree.nodes[i].father) {

         /* Find that node in genetree that is node i in species tree.  
            Its descendent, node j, is the oldest node in gene tree that is 
            descendent of spnode.
         */
         for(j=0; j<tree.nnode && nodes[j].ipop!=i; j++) ;

         if(j<tree.nnode) break;
      }
   }

   if(j<tree.nnode)
      for( ; ; j=nodes[j].father) {
         com.oldconP[j]=0;
         if(j==tree.root) break;
      }

   return(0);
}

double UpdateTimesOnly (double *lnL, double finetune)
{
/* This updates the node ages in the master species tree sptree.nodes[].age, 
   one by one for the clock model.  
*/
   int  locus, is, i, *sons, dad;
   double c, naccept=0, t,tnew, tson[2], tb[2],y,ynew;
   double lnacceptance, lnLd, lnpDinew[NGENE], lnpTnew;

   for(is=sptree.nspecies; is<sptree.nnode; is++) {
      t = sptree.nodes[is].age;
      sons = sptree.nodes[is].sons;
      dad = sptree.nodes[is].father;
      tson[0] = sptree.nodes[sons[0]].age;
      tson[1] = sptree.nodes[sons[1]].age;
      tb[0] = max2(tson[0], tson[1]);
      if(dad != -1)    /* used for non-root */
         tb[1] = sptree.nodes[dad].age;      
      lnacceptance = 0;
      lnLd = 0;

      if(is == sptree.root) {
         lnacceptance = finetune*rnd2normal8();
         c = exp(lnacceptance);
         tnew = tb[0]+(t-tb[0])*c;
         sptree.nodes[is].age = tnew;
      }
      else {  /* this slides a window on y=(t-te)/(tp-te), with 0<y<1. */
         y = (t-tb[0])/(tb[1]-tb[0]); /* linear transform */
         ynew = y + finetune*rnd2normal8();
         ynew = reflect(ynew,0,1);
         sptree.nodes[is].age = tnew = tb[0] + ynew*(tb[1]-tb[0]);
      }

      lnpTnew = lnpriorTimes();
      lnacceptance += lnpTnew-data.lnpT;

      for(locus=0,lnLd=0; locus<data.ngene; locus++) {
         UseLocus(locus, 1, mcmc.usedata, 0);

         if(mcmc.saveconP) {
            for(i=0;i<sptree.nnode;i++) com.oldconP[i]=1;
            LabelOldCondP(is);
         }
         if(com.oldconP[tree.root]) 
            lnpDinew[locus] = data.lnpDi[locus];
         else
            lnpDinew[locus] = lnpD_locus(locus);
         lnLd += lnpDinew[locus]-data.lnpDi[locus];
      }
      lnacceptance += lnLd;

      if(debug==2) printf("species %2d tb: %8.5f %8.5f t: %8.5f%8.5f %9.2f", is,tb[0],tb[1],t,tnew,lnLd);

      if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
         naccept++;
         data.lnpT = lnpTnew;
         for(i=0;i<data.ngene;i++) 
            data.lnpDi[i] = lnpDinew[i];
         *lnL+=lnLd;
         if(mcmc.usedata==1) switchconPin();
         if(debug==2) printf(" Y (%4d)\n", NPMat);
      }
      else {
         sptree.nodes[is].age = t;
         if(debug==2) printf(" N (%4d)\n", NPMat);
         for(locus=0; locus<data.ngene; locus++)
            AcceptRejectLocus(locus, 0);  /* reposition conP */
      }
   }  /* for(is) */
   return(naccept/(sptree.nspecies-1.));
}

double UpdateTimes (double *lnL, double finetune)
{
/* This updates the node ages in the master species tree sptree.nodes[].age, 
   one by one.  It simultaneously changes the rates at the three branches 
   around the node so that the branch lengths remain the same, and there is 
   no need to update the lnL.
*/
   int  is, i, *sons, dad;
   double c, naccept=0, t,tnew, tson[2], tb[2],y,ynew, rateratio[3];
   double lnacceptance, lnLd, lnpTnew,lnpRnew;

   if(debug==2) puts("\nUpdateTimes ");
   if(com.clock==1) 
      return UpdateTimesOnly(lnL, finetune);

   for(is=sptree.nspecies; is<sptree.nnode; is++) {
      t=sptree.nodes[is].age;
      sons=sptree.nodes[is].sons;
      tson[0] = sptree.nodes[sons[0]].age;
      tson[1] = sptree.nodes[sons[1]].age;
      dad = sptree.nodes[is].father;
      tb[0] = max2(tson[0], tson[1]);
      if(dad != -1)    /* used for non-root */
         tb[1] = sptree.nodes[dad].age;      
      lnacceptance=0;
      lnLd=0;

      if(is == sptree.root) {
         lnacceptance = finetune*rnd2normal8();
         c = exp(lnacceptance);
         sptree.nodes[is].age = tnew = tb[0]+(t-tb[0])*c;
      }
      else {  /* this slides a window on y=(t-te)/(tp-te), with 0<y<1. */
         y = (t-tb[0])/(tb[1]-tb[0]); /* linear transform */
         ynew = y + finetune*rnd2normal8();
         ynew = reflect(ynew,0,1);
         sptree.nodes[is].age = tnew = tb[0] + ynew*(tb[1]-tb[0]);
      }

      /* Thorne et al. (1998) equation 9. */
      rateratio[0]=(t-tson[0])/(tnew-tson[0]);
      rateratio[1]=(t-tson[1])/(tnew-tson[1]);
      for(i=0; i<data.ngene; i++) {
         sptree.nodes[sons[0]].rates[i] *= rateratio[0];
         sptree.nodes[sons[1]].rates[i] *= rateratio[1];
      }
      lnacceptance += data.ngene*log(rateratio[0]*rateratio[1]);
      if(is != sptree.root) {
         rateratio[2]=(t-tb[1])/(tnew-tb[1]);
         for(i=0; i<data.ngene; i++)
            sptree.nodes[is].rates[i] *= rateratio[2];
         lnacceptance += data.ngene*log(rateratio[2]);
      }

      lnpTnew = lnpriorTimes();
      lnacceptance += lnpTnew-data.lnpT;
      lnpRnew = lnpriorRates();
      lnacceptance += lnpRnew-data.lnpR;

      if(debug==2) printf("species %2d tb: %8.5f %8.5f t: %8.5f%8.5f", is,tb[0],tb[1],t,tnew);

      if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
         naccept++;
         data.lnpT = lnpTnew;
         data.lnpR = lnpRnew;
         if(debug==2) printf(" Y (%4d)\n", NPMat);
      }
      else {
         sptree.nodes[is].age = t;
         for(i=0; i<data.ngene; i++) {
            sptree.nodes[sons[0]].rates[i] /= rateratio[0];
            sptree.nodes[sons[1]].rates[i] /= rateratio[1];
            if(is!=sptree.root)  sptree.nodes[is].rates[i] /= rateratio[2];
         }
         if(debug==2) printf(" N (%4d)\n", NPMat);
      }
   }  /* for(is) */
   return(naccept/(sptree.nspecies-1.));
}




#if (0)
void getSinvDetS (double space[])
{
/* This uses the variance-correlation matrix data.correl[g*g] to constructs 
   Sinv (inverse of S) and detS (|S|).  It also copies the variances into 
   data.sigma2[g].  This is called every time data.correl is updated.

   What restrictions should be placed on data.correl[]???

   space[data.ngene]
*/
   int i, j, g=data.ngene;
   int debug=0;

   for(i=0; i<g; i++)
      data.Sinv[i*g+i] = data.sigma2[i] = data.correl[i*g+i];
   for(i=0; i<g; i++) {
      for(j=0; j<i; j++)
         data.Sinv[i*g+j] = data.Sinv[j*g+i]
            = data.correl[i*g+j]*sqrt(data.sigma2[i]*data.sigma2[j]);
   }

   if(debug) {
      printf("\ncorrel & S & Sinv ");
      matout(F0, data.correl, g, g);
      matout(F0, data.Sinv, g, g);
   }

   matinv (data.Sinv, g, g, space);
   data.detS = fabs(space[0]);

   if(debug) {
      matout(F0, data.Sinv, g, g);
      printf("|S| = %.6g\n", data.detS);
   }
   if(data.detS<0) 
      error2("detS < 0");
}
#endif


double lnpriorRates (void)
{
/* This calculates the log of the prior of rates under the two rate-drift models:
   the independent rates (clock=2) and the geometric Brownian motion model (clock=3).

   clock=2: the algorithm cycyles through the branches, and add up the log 
   probabilities.
   clock=3: the root rate is mu or data.rgene[].  The algorithm cycles through 
   the ancestral nodes and deals with the two daughter branches.
*/
   int i, inode, dad=-1, g=data.ngene, s=sptree.nspecies, sons[2], root=sptree.root;
   double lnpR=-log(2*Pi)/2.0*(2*s-2)*g, t,tA,t1,t2,Tinv[4], detT;
   double zz, r=-1, rA,r1,r2, y1,y2;

   if(com.clock==2) {
      for(i=0; i<g; i++)
         lnpR -= log(data.sigma2[i])/2.*(2*s-2);
      for(inode=0; inode<sptree.nnode; inode++) {
         if(inode==root) continue;
         for(i=0; i<g; i++) {
            r = sptree.nodes[inode].rates[i];
            zz=log(r/data.rgene[i]) + data.sigma2[i]/2;
            lnpR += -zz*zz/(2*data.sigma2[i]) - log(r);
         }
      }
   }
   else { /* clock=3 */
      for(inode=0; inode<sptree.nnode; inode++) {
         if(sptree.nodes[inode].nson==0) continue; /* skip the tips */
         dad = sptree.nodes[inode].father;
         for(i=0; i<2; i++) sons[i] = sptree.nodes[inode].sons[i];
         t = sptree.nodes[inode].age;
         if(inode==root) tA = 0;
         else            tA = (sptree.nodes[dad].age - t)/2;
         t1 = (t-sptree.nodes[sons[0]].age)/2;
         t2 = (t-sptree.nodes[sons[1]].age)/2;
         detT = t1*t2+tA*(t1+t2);
         Tinv[0] = (tA+t2)/detT; 
         Tinv[1] = Tinv[2] = -tA/detT; 
         Tinv[3] = (tA+t1)/detT;
         for(i=0; i<g; i++) {
            rA = (inode==root||dad==root ? data.rgene[i] : sptree.nodes[dad].rates[i]);
            r1 = sptree.nodes[sons[0]].rates[i];
            r2 = sptree.nodes[sons[1]].rates[i];
            y1 = log(r1/rA)+(tA+t1)*data.sigma2[i]/2;
            y2 = log(r2/rA)+(tA+t2)*data.sigma2[i]/2;
            zz = (y1*y1*Tinv[0]+2*y1*y2*Tinv[1]+y2*y2*Tinv[3]);
            lnpR -= zz/(2*data.sigma2[i]) + log(detT*square(data.sigma2[i]))/2 + log(r1*r2);
         }
      }
   }
   return lnpR;
}

double UpdateRates (double *lnL, double finetune)
{
/* This updates rates under the rate-drift models (clock=2 or 3).
   It cycles through nodes in the species tree at each locus.

   Slight waste of computation: For every proposal to change rates, lnpriorRates() 
   is called to calculate the prior for all rates at all loci on the whole 
   tree, thus wasting computation, if rates are not correlated across loci.
*/
   int locus, inode, j, g=data.ngene;
   double naccept=0, lnpRnew, lnpDinew, lnacceptance, lnLd, rold, c;

   if(com.clock==1)
      return UpdateRatesClock(lnL, finetune);

   if(debug==3) puts("\nUpdateRates");
   for(locus=0; locus<g; locus++) {
      for(inode=0; inode<sptree.nnode; inode++) {
         if(inode==sptree.root) continue;
         rold = sptree.nodes[inode].rates[locus];
         c = exp(finetune*rnd2normal8());
         sptree.nodes[inode].rates[locus] *= c;
         UseLocus(locus, 1, mcmc.usedata, 0);  /* copyconP=1 */

         if(mcmc.saveconP) {
            FOR(j,sptree.nspecies*2-1) com.oldconP[j]=1;
            LabelOldCondP(inode);
         }

         lnpRnew=lnpriorRates();
         lnacceptance = lnpRnew-data.lnpR;
         lnpDinew=lnpD_locus(locus);
         lnLd = lnpDinew - data.lnpDi[locus];
         lnacceptance +=  lnLd + log(c);

         if(lnacceptance>0 || rndu()<exp(lnacceptance)) {
            naccept++;
            if(mcmc.usedata==1) AcceptRejectLocus(locus,1);
   
            data.lnpR=lnpRnew;
            data.lnpDi[locus]=lnpDinew;
            *lnL += lnLd;
         }
         else {
            if(mcmc.usedata==1) AcceptRejectLocus(locus,0);
            sptree.nodes[inode].rates[locus]=rold;
         }
      }
   }
   return(naccept/(g*(sptree.nnode-1)));
}


double logPriorRatio(double xnew, double xold, double a, double b, int distribution)
{
/* This calculates the log of prior ratio when x is updated from xold to xnew.
   x has distribution with parameters a and b.
*/
   double lnr;

   if(distribution==GAMMA)
      lnr = (a-1)*log(xnew/xold) - b*(xnew-xold);
   else if(distribution==IGAMMA)
      lnr = -9999;
   return(lnr);
}

double UpdateRatesClock (double *lnL, double finetune)
{
/* This updates rates data.rgene[] under the clock by cycling through the loci.
   The proposal affects all branch lengths, so com.oldconP[]=0.
*/
   int locus, j, g=data.ngene;
   double naccept=0, rgene0, lnLd, lnpDinew, lnacceptance, c=1;

   if(debug==3) puts("\nUpdateRatesClock");
   if(mcmc.saveconP) FOR(j,sptree.nspecies*2-1) com.oldconP[j]=0;
   for(locus=0; locus<g; locus++) {
      rgene0=data.rgene[locus];
      c = exp(finetune*rnd2normal8());
      data.rgene[locus] = rgene0 * c;
  
      UseLocus(locus, 1, mcmc.usedata, 0);
      lnpDinew=lnpD_locus(locus);
      lnLd = lnpDinew - data.lnpDi[locus];   /* likelihood ratio */
      lnacceptance =  lnLd + log(c);         /* proposal ratio */
      /* prior ratio */
      lnacceptance += logPriorRatio(data.rgene[locus],rgene0,data.rgenegamma[0],data.rgenegamma[1],GAMMA);

      if(debug==3)
         printf("\nLocus %2d rgene %9.4f%9.4f %10.5f", locus+1, rgene0, data.rgene[locus], lnLd);

      if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
         naccept++;
         *lnL += lnLd;
         data.lnpDi[locus]=lnpDinew;
         if(mcmc.usedata==1) AcceptRejectLocus(locus,1);
         if(debug==3) printf(" Y\n");
      }
      else {
         data.rgene[locus]=rgene0;
         if(mcmc.usedata==1) AcceptRejectLocus(locus,0); /* reposition conP */

         if(debug==3) printf(" N\n");
      }
   }

   return(naccept/g);
}

double UpdateParameters (double *lnL, double finetune)
{
/* This updates parameters in the substitution model such as the ts/tv 
   rate ratio for each locus.

   Should we update the birth-death process parameters here as well?

*/
   int locus, j, ip, np=!com.fix_kappa+!com.fix_alpha;
   double naccept=0, lnLd,lnpDinew, lnacceptance, c=1;
   double pold, pnew, *gammaprior;

   if(debug==4) puts("\nUpdateParameters");
   if(np==0) return(0);

   if(mcmc.saveconP) FOR(j,sptree.nspecies*2-1) com.oldconP[j]=0;
   for(locus=0; locus<data.ngene; locus++) {
      for(ip=0; ip<np; ip++) {
         if(ip==0 && !com.fix_kappa) {  /* kappa */
            pold=data.kappa[locus];
            c = exp(finetune*rnd2normal8());
            data.kappa[locus] = pnew = pold * c;
            gammaprior=data.kappagamma;
         }
         else {  /* alpha */
            pold=data.alpha[locus];
            c = exp(finetune*rnd2normal8());
            data.alpha[locus] = pnew = pold * c;
            gammaprior=data.alphagamma;
         }

         UseLocus(locus, 1, mcmc.usedata, 0); /* this copies parameter from data.[] to com. */

         lnpDinew=lnpD_locus(locus);
         lnLd = lnpDinew - data.lnpDi[locus];
         lnacceptance =  lnLd + log(c);
         lnacceptance += logPriorRatio(pnew,pold,gammaprior[0],gammaprior[1],GAMMA);

         if(debug==4)
            printf("\nLocus %2d para%d %9.4f%9.4f %10.5f", locus+1,ip+1,pold,pnew,lnLd);

         if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
            naccept++;
            *lnL += lnLd;
            data.lnpDi[locus]=lnpDinew;
            if(mcmc.usedata==1) AcceptRejectLocus(locus,1);
            if(debug==4) printf(" Y\n");
         }
         else {
            if(ip==0 && !com.fix_kappa)
               data.kappa[locus]=pold;
            else 
               data.alpha[locus]=pold;
            
            if(mcmc.usedata==1) AcceptRejectLocus(locus,0);

            if(debug==4) printf(" N\n");
         }
      }
   }
   return(naccept/(data.ngene*np));
}


double UpdateParaRates (double finetune, double space[])
{
/* This updates mu (mean rate) and sigma2 (variance of lnrates) at each locus.
   The gamma hyperpriors are assumed to be the same across loci.  
   The proposals in this routine do not change the likelihood.
*/
   int i, ip, g=data.ngene;
   double lnacceptance, lnpRnew, naccept=0, pold, pnew, *gammaprior, c;
   char *parastr[2]={"mu", "sigma2"};

   if(com.clock==1) return(0);
   if(debug==5) puts("\nUpdateParaRates (rgene & sigma2)");

   for(i=0; i<g; i++) {
      for(ip=0; ip<2; ip++) {  /* this loops through mu (rgene) and sigma2 */
         if(ip==0) {  /* rgene (mu) */
            pold=data.rgene[i];
            c = exp(finetune*rnd2normal8());
            data.rgene[i] = pnew = pold * c;
            gammaprior=data.rgenegamma;
         }
         else {         /* sigma2 */
            pold=data.sigma2[i];
            c = exp(finetune*rnd2normal8());
            data.sigma2[i] = pnew = pold * c;
            gammaprior=data.sigma2gamma;
         }

         lnacceptance = logPriorRatio(pnew,pold,gammaprior[0],gammaprior[1],GAMMA);
         if(debug==5) printf("%-7s %2d %9.5f -> %9.5f ", parastr[ip], i, pold,pnew);

         lnpRnew = lnpriorRates();
         lnacceptance += lnpRnew-data.lnpR;
         lnacceptance += log(c);

         if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
            naccept++;
            data.lnpR = lnpRnew;
         }
         else {
            if(ip==0)  data.rgene[i]  = pold;
            else       data.sigma2[i] = pold;
         }
      }
   }
   return(naccept/(g*2));
}



double mixing (double finetune)
{
/* If com.clock==1, this multiplies all times by c and divides all rates 
   (mu or data.rgene[]) by c, so that the likelihood does not change.  

   If com.clock=2 or 3, this multiplies all times by c and divide by c all rates: 
   sptree.nodes[].rates and mu (data.rgene[]) at all loci.  The likelihood is 
   not changed.
*/
   int i,j, ndivide = data.ngene;
   double naccept=0, a=data.rgenegamma[0], b=data.rgenegamma[1], c, lnc;
   double lnacceptance=0, lnpTnew, lnpRnew=-1;

   /*
   lnc = finetune*(rndu()-.5);
   lnc = finetune*rndnormal();
   */
   lnc = finetune*rnd2normal8();
   
   c = exp(lnc);
   for(j=sptree.nspecies; j<sptree.nnode; j++) 
      sptree.nodes[j].age *= c;

   for(j=0; j<data.ngene; j++) {  /* mu */
      lnacceptance += (a-1)*(-lnc) - b*(data.rgene[j]/c-data.rgene[j]);
      data.rgene[j] /= c;
   }
   if(com.clock>1) {              /* rate-drift models */
      ndivide += data.ngene*(sptree.nspecies*2-2);
      for(i=0; i<sptree.nnode; i++) 
         if(i != sptree.root)
            for(j=0; j<data.ngene; j++)
               sptree.nodes[i].rates[j] /= c;
      lnpRnew = lnpriorRates();
      lnacceptance += lnpRnew-data.lnpR;
   }

   lnpTnew = lnpriorTimes();
   lnacceptance += lnpTnew-data.lnpT + ((sptree.nspecies-1) - ndivide)*lnc;

   if(lnacceptance>0 || rndu()<exp(lnacceptance)) { /* accept */
      naccept = 1;
      data.lnpT = lnpTnew;
      data.lnpR = lnpRnew;
   }
   else {   /* reject */
      for(j=sptree.nspecies; j<sptree.nnode; j++) 
         sptree.nodes[j].age /= c;
      for(j=0; j<data.ngene; j++) data.rgene[j] *= c;
      if(com.clock > 1) {
         for(i=0; i<sptree.nnode; i++) 
            if(i != sptree.root)
               for(j=0; j<data.ngene; j++)
                  sptree.nodes[i].rates[j] *= c;
      }
   }
   return(naccept);
}



double UpdatePFossilErrors (double finetune)
{
/* This updates the probability of fossil errors sptree.Pfossilerr.  
   The proposal do not change the likelihood.
*/
   double lnacceptance, lnpTnew, naccept=0, pold, pnew;
   double p = data.fossilerrorbeta[0], q = data.fossilerrorbeta[1];

   pold = sptree.Pfossilerr;
   pnew = pold + finetune*rnd2normal8();
   sptree.Pfossilerr = pnew = reflect(pnew,0,1);
   lnacceptance = (p-1)*log(pnew/pold) + (q-1)*log((1-pnew)/(1-pold));
   lnpTnew = lnpriorTimes();
   lnacceptance += lnpTnew-data.lnpT;

   if(lnacceptance>=0 || rndu()<exp(lnacceptance)) {
      naccept++;
      data.lnpT = lnpTnew;
   }
   else {
      sptree.Pfossilerr = pold;
   }
   return(naccept);
}


int DescriptiveStatistics2Delete (FILE *fout, char infile[], int nbin, int nrho)
{
   FILE *fin=gfopen(infile,"r");
   int  n, p, i,j, jj;
   char *fmt=" %9.6f", timestr[36];
   double *x, *mean, *median, *minx, *maxx, *x005,*x995,*x025,*x975,*x25,*x75;
   double *y;

   printf("\nscanning the file %s once..", infile);
   scanfile(fin, &n, &p);
   printf("\nAha, i've seen %d observations, each with %d variables\n", n,p);
   if(n<=2) return(1);

   x = (double*)malloc((p*12+p*p + p*nrho)*sizeof(double));
   if (x==NULL) { puts("did not get enough space."); exit(-1); }
   for(j=0;j<p*12+p*p + p*nrho; j++) x[j]=0;
   mean=x+p; median=mean+p; minx=median+p; maxx=minx+p; 
   x005=maxx+p; x995=x005+p; x025=x995+p; x975=x025+p; x25=x975+p; x75=x25+p;

   printf("collecting mean and sorting to get percentiles\n");
   for(i=0; i<n; i++) {
      for(j=0;j<p;j++) fscanf(fin,"%lf", &x[j]);
      for(j=0;j<p;j++) mean[j]+=x[j]/n;
   }

   y=(double*)malloc(n*sizeof(double));
   if(y==NULL) { printf("not enough mem for %d variables\n",n); exit(-1); }
   for(jj=0;jj<p;jj++) {
      rewind(fin);
      for(i=0;i<n;i++) {
         for(j=0;j<p;j++) fscanf(fin,"%lf", &x[j]);
         y[i]=x[jj];
      }
      qsort(y, (size_t)n, sizeof(double), comparedouble);
      if(n%2==0)  median[jj]=(y[n/2]+y[n/2+1])/2;
      else        median[jj]=y[(n+1)/2];
      x005[jj]=y[(int)(n*.005)];  x995[jj]=y[(int)(n*.995)];
      x025[jj]=y[(int)(n*.025)];  x975[jj]=y[(int)(n*.975)];
      x25[jj]=y[(int)(n*.25)];    x75[jj]=y[(int)(n*.75)];
      printf("\t%d/%d  %10s\r", jj+1,p, printtime(timestr));
   }

   printf("\n\nposterior mean & 95%% credibility interval\n\n");
   for(j=0;j<p;j++) {
      printf("%.4f (%.4f, %.4f) ",mean[j], x025[j],x975[j]);
      if(j<sptree.nspecies-1)
         printf("  age of Anode %2d (jeffnode %2d)", sptree.nspecies+1+j,2*sptree.nspecies-1-1-j);
      FPN(F0);
   }

   if(fout) {
      fprintf(fout, "\n\nposterior mean & 95%% credibility interval\n\n");
      for(j=0;j<p;j++) {
         fprintf(fout, "%.4f (%.4f, %.4f) ",mean[j], x025[j],x975[j]);
         if(j<sptree.nspecies-1)
            fprintf(fout, "  age of Anode %2d (jeffnode %2d)", sptree.nspecies+1+j, 2*sptree.nspecies-1-1-j);
         FPN(fout);
      }
   }
   free(x); free(y);  return(0);
}


int MCMC (FILE* fout)
{
   char mcmcout[32]="mcmc.out";
   FILE *fmcmcout=NULL;
   int nsteps=4+(mcmc.usedata>0)+(data.fossilerrorbeta[0]>0), nxpr[2]={6,2};
   int i,j,k, ir, g=data.ngene;
   double Paccept[6]={0}, lnL=0, nround=0, *x, *mx, *vx, *px, postEFossil[MaxNFossils]={0};
   char timestr[36];

   noisy = 2;
   mcmc.saveconP = 1;
   if(mcmc.usedata!=1) mcmc.saveconP = 0;
   for(j=0; j<sptree.nspecies*2-1; j++) 
      com.oldconP[j] = 0;
   GetMem();
   if(mcmc.usedata==2)
      ReadBlengthGH("in.BV");
   if(mcmc.print) fmcmcout=gfopen(mcmcout,"w");

   printf("\n%d burnin, sampled every %d, %d samples\n", 
           mcmc.burnin, mcmc.sampfreq, mcmc.nsample);
   if(mcmc.usedata) puts("Approximating posterior");
   else             puts("Approximating prior");

   printf("(Settings: cleandata=%d.  print=%d  saveconP=%d.)\n", 
          com.cleandata, mcmc.print, mcmc.saveconP);

   com.np = GetInitials(fout);

   x=(double*)malloc(com.np*(com.np+2)*sizeof(double));
   if(x==NULL) error2("oom in mcmc");
   mx=x+com.np;  vx=mx+com.np;

   
/*
   printf("\aresetting times");
   {
      int k;
      double x0[]={1.1, 0.99, 0.88, 0.77, 0.55, 0.44, 0.22, 0.11};
      for(i=sptree.nspecies,k=0; i<sptree.nnode; i++)
         sptree.nodes[i].age=x0[k++];
   }
   printSptree();
*/

   collectx(x);

   if(!com.fix_kappa && !com.fix_alpha && data.ngene==2) { nxpr[0]=6; nxpr[1]=4; }

   puts("\npriors: ");
   if(mcmc.usedata==1) {
      if(!com.fix_kappa) printf("\tG(%.4f, %.4f) for kappa\n", data.kappagamma[0], data.kappagamma[1]);
      if(!com.fix_omega) printf("\tG(%.4f, %.4f) for omega\n", data.omegagamma[0], data.omegagamma[1]);
      if(!com.fix_alpha) printf("\tG(%.4f, %.4f) for alpha\n", data.alphagamma[0], data.alphagamma[1]);
   }
   printf("\tG(%.4f, %.4f) for rgene\n", data.rgenegamma[0], data.rgenegamma[1]);
   if(com.clock>1) printf("\tG(%.4f, %.4f) for sigma2\n", data.sigma2gamma[0], data.sigma2gamma[1]);

   /* calculates prior for times and likelihood for each locus */
   if(data.fossilerrorbeta[0]) 
      SetupPriorTimesFossilErrors();
   data.lnpT = lnpriorTimes();
   for(j=0; j<data.ngene; j++) com.rgene[j]=-1;  /* com.rgene[] is not used. */
   if(com.clock>1)
      data.lnpR = lnpriorRates();
   printf("\nInitial parameters (np = %d):\n", com.np);
   for(j=0;j<com.np;j++) printf(" %9.6f",x[j]); FPN(F0);
   lnL = lnpData(data.lnpDi);
   printf("\nlnL0 = %9.2f\n", lnL);

   printf("\nStarting MCMC (np = %d) . . .\n", com.np);
   printf("finetune steps (time rate mixing para RatePara?):");
   for(j=0; j<nsteps; j++) 
      printf(" %7.4f",mcmc.finetune[j]);
   if(com.clock==1) 
      printf("\n  paras: %d times, %d mu, (& kappa, alpha)\n", 
             sptree.nspecies-1, data.ngene);
   else
      printf("\n  paras: %d times, %d mu, %d sigma2 (& rates, kappa, alpha)\n", 
             sptree.nspecies-1, data.ngene, data.ngene);

   zero(mx,com.np);
   zero(vx,com.np*com.np);
   if(com.np<nxpr[0]+nxpr[1]) { nxpr[0]=com.np; nxpr[1]=0; }
   for(ir=-mcmc.burnin; ir<mcmc.sampfreq*mcmc.nsample; ir++) {
      if(ir==0) { /* reset after burnin */
         nround=0; zero(Paccept,nsteps);
         zero(mx,com.np); zero(vx,com.np*com.np); 
#if 0
         if(mcmc.burnin>10) {  /* reset finetune parameters.  Do we want this? */
            for(j=0;j<nsteps;j++) 
               if(Paccept[j]<.1 || Paccept[j]>.8) {
                  mcmc.finetune[j] *= Paccept[j]/0.3;
                  printf("finetune #%d reset to %.5f\n", j+1,mcmc.finetune[j]);
               }
         }
#endif
      }

      nround++;

      Paccept[0] = (Paccept[0]*(nround-1) + UpdateTimes(&lnL, mcmc.finetune[0]))/nround;
      Paccept[1] = (Paccept[1]*(nround-1) + UpdateRates(&lnL, mcmc.finetune[1]))/nround;
      Paccept[2] = (Paccept[2]*(nround-1) + mixing(mcmc.finetune[2]))/nround;
      if(mcmc.usedata==1)
         Paccept[3] = (Paccept[3]*(nround-1) + UpdateParameters(&lnL, mcmc.finetune[3]))/nround;
      if(com.clock>1)
         Paccept[4] = (Paccept[4]*(nround-1) + UpdateParaRates(mcmc.finetune[4],com.space))/nround;
      if(data.fossilerrorbeta[0])
         Paccept[5] = (Paccept[5]*(nround-1) + UpdatePFossilErrors(mcmc.finetune[5]))/nround;

      collectx(x);

      for(j=0; j<com.np; j++) mx[j]=(mx[j]*(nround-1)+x[j])/nround;
      for(i=0; i<com.np; i++) for(j=0; j<com.np; j++)
         vx[i*com.np+j] += (x[i]-mx[i])*(x[j]-mx[j]) * (nround-1.)/nround;
      if(data.fossilerrorbeta[0])
         getPfossilerr(postEFossil, nround);

      if(mcmc.print && ir>=0 && (ir+1)%mcmc.sampfreq==0) {
         for(j=0;j<com.np; j++) fprintf(fmcmcout," %7.5f",x[j]);
         if(mcmc.usedata) fprintf(fmcmcout," %10.3f",lnL);
         FPN(fmcmcout);
      }
      if((ir+1)%max2(mcmc.sampfreq, mcmc.sampfreq*mcmc.nsample/10000)==0) {
         printf("\r%3.0f%%", (ir+1.)/(mcmc.nsample*mcmc.sampfreq)*100.);

         for(j=0; j<nsteps; j++) 
            printf(" %4.2f", Paccept[j]); 
         printf(" ");

         px = (ir >= 0 ? mx : x);
         px = mx;

         FOR(j,nxpr[0]) printf(" %5.3f", px[j]);
         if(com.np>nxpr[0]+nxpr[1] && nxpr[1]) printf(" -");
         FOR(j,nxpr[1]) printf(" %5.3f", px[com.np-nxpr[1]+j]);
         if(mcmc.usedata) printf(" %4.1f",lnL);
      }

      if(mcmc.sampfreq*mcmc.nsample>20 && (ir+1)%(mcmc.sampfreq*mcmc.nsample/20)==0) {
         printf(" %s\n", printtime(timestr));
         testlnL=1;
         if(fabs(lnL-lnpData(data.lnpDi))>1e-5) {
            printf("\n%12.6f = %12.6f?\n", lnL, lnpData(data.lnpDi));
            puts("lnL not right?");
         }
         testlnL=0;
      }
   }
   if(mcmc.print) fclose(fmcmcout);

   printf("\nTime used: %s", printtime(timestr));
   fprintf(fout,"\nTime used: %s", printtime(timestr));

   fprintf(fout, "\n\nmean and S.D. of parameters using all iterations\n\n");
   fprintf(fout, "mean  ");
   for(i=0; i<com.np*com.np; i++)  
      vx[i] /= nround;
   for(i=0; i<com.np; i++) 
      fprintf(fout, " %9.5f", mx[i]);
   fprintf(fout, "\nS.D.  ");
   for(i=0; i<com.np; i++) 
      fprintf(fout, " %9.5f", sqrt(vx[i*com.np+i])); 
   FPN(fout);

   puts("\nSpecies tree with node labels and ages, for TreeView to produce a tree like fig. 1 in Y&R06.");
   copySptree();
   for(j=0; j<sptree.nspecies-1; j++) 
      nodes[sptree.nspecies+j].age=mx[j];
   for(j=0; j<sptree.nnode; j++)
      if(j!=sptree.root)  
         nodes[j].branch = nodes[nodes[j].father].age-nodes[j].age;
   FPN(F0); OutaTreeN(F0,1,1); FPN(F0);
   fputs("\nSpecies tree with node labels and ages, for TreeView to produce a tree like fig. 1 in Y&R06.",fout);
   FPN(fout); OutaTreeN(fout,1,PrNodeNum); FPN(fout);
   FPN(fout); OutaTreeN(fout,1,1); FPN(fout);

   free(x); 
   if(data.fossilerrorbeta[0]) {
      free(sptree.CcomFossilErr);
      printf("\nPosterior probabilities that each fossil is in error.\n");
      for(i=k=0; i<sptree.nspecies*2-1; i++) {
         if((j=sptree.nodes[i].fossil) == 0) continue;
         printf("Node %2d: %s (%9.4f, %9.4f)", i+1,fossils[j],sptree.nodes[i].tfossil[0],sptree.nodes[i].tfossil[1]);
         printf(" %8.3f\n", postEFossil[k++]);
      }
   }

   if(mcmc.print) {
      printf("\nSummarizing MCMC output.");
      printf("\nParameters are in the order of times, mu's, sigma2 & rates (if clock23), kappa, alpha, fossilErrors\n");
      fprintf(fout,"\n\nSummary of MCMC results\n");
      fprintf(fout,"\nParameters are in the order of times, mu's, sigma2 & rates (if clock23), kappa, alpha, fossilErrors\n");

      DescriptiveStatistics2Delete(fout, mcmcout, 1, 1);
      /* DescriptiveStatistics(fout, mcmcout, 20, 20); */
      printf("\nTime used: %s\n", printtime(timestr));
   }
   FreeMem();
   return(0);
}
