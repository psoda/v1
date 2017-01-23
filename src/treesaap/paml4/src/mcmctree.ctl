          seed = -1

       seqfile = 9s200K.txt
      treefile = 9s.trees

       outfile = out

       usedata = 1    * 0: no data; 1:seq like; 2:read in.BV; 3: write out.BV
         ndata = 1
       seqtype = 0  * 0: nucleotides; 1:codons; 2:AAs

         clock = 1    * 1: global clock; 2: independent rates; 3: correlated rates
   fossilerror = 1 10   * fossil error beta(p,q), with p=0 for no errors
       RootAge = '>3.2<3.8' * root age constraints, used if no fossil for root.

         model = 0    * 0:JC69, 1:K80, 2:F81, 3:F84, 4:HKY85
         alpha = 0.    * alpha for gamma rates at sites
         ncatG = 5    * No. categories in discrete gamma

     cleandata = 0    * remove sites with ambiguity data (1:yes, 0:no)?
 BlengthMethod = 0    * 0: arithmetic; 1: geometric; 2: Brownian

       BDparas = 2 2 0.1   * birth, death, sampling
   kappa_gamma = 6 2      * gamma prior for kappa
   alpha_gamma = 1 1      * gamma prior for alpha

   rgene_gamma = 2 2     * gamma prior for rate for genes
  sigma2_gamma = 1 2     * gamma prior for sigma^2  (for clock=2 or 3)

      finetune = 0.02 0.02 0.5 0.5 .9 1.5 * times, rates, mixing, paras, RateParas, FossilErr

         print = 1
        burnin = 10000
      sampfreq = 2
       nsample = 50000
