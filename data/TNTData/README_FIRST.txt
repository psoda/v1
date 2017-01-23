
To read the entire data set, enter TNT (available
at www.zmuc.dk/public/phylogeny/tnt), and type: 

      tnt*>proc EukaData.tnt ; <enter>

Due to memory requirements, the entire data set can 
only be read on 64-bit Linux systems. 

On 32-bit systems, the taxon names (contained in 
the file Taxon_Names_Only.tnt) can be read; this 
will allow plotting and processing the trees. 

Note! 

The *.fin files contain the sequences; these are 
kept separate so that all GenBank accession numbers 
used in the study are recorded.  For a given taxon, 
TNT will store the first accession number it finds, 
and ignore the others. 

Please refer to the documentation of TNT for options  
to save memory (e.g. by deactivating uninformative 
characters and creating a new data set with only the 
informative characters). 

The two files with computer code, gb2tnt.c and fas2fas.c, 
contain code to process GenBank files and produce FASTA
files (to be used in alignment programs).  If using Linux, 
make sure you define a "LINUX" flag for compilation, e.g. 

         gcc -DLINUX -o gb2tnt gb2tnt.c







