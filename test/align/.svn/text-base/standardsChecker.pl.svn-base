#!/usr/bin/perl -w

my $FALSE = 0;
my $TRUE = 1;

use strict;

my $MAX_TAXON_LENGTH = 10;
my $NUCLEOTIDE_OK_CHARS = "abcdghkmnrstuvwyABCDGHKMNRSTUVWY";
my $AMINO_ACID_OK_CHARS = "abcdefghiklmnpqrstvwxyzABCDEFGHIKLMNPQRSTVWXYZ";  # following BioPerl's Bio::Tool:IUPAC.pm, "b", "x", and "z" are included in the ok list (see http://www.chem.qmul.ac.uk/iupac/AminoAcid/A2021.html#AA212 )
my $SECONDARY_STRUCTURE_OK_CHARS = "bceghistlBCEGHISTL";
my $SECONDARY_STRUCTURE_3_STATES_OK_CHARS = "ehlEHL";

my $SUCCESS = 0;
my $FAILURE = 1;

my $usage = "Usage $0: FILENAME(S) [options]\n".
    " -taxonlen=NUMBER                   (Set acceptable maximum taxon name length)\n".
    " -ignorestartswithnumber            (Do not check if the first character in the taxon name is a number)\n".
    " -ignorebars                        (Do not check for \"|\"s in the taxon name is a number)\n".
    " -p[rotein]                         (Use the protein alphabet ($AMINO_ACID_OK_CHARS) (defaults to IUPAC codes for nucleotides)\n".
    " -ss8                               (Use the secondary structure alphabet ($SECONDARY_STRUCTURE_OK_CHARS) (defaults to IUPAC codes for nucleotides)\n".
    " -ss3                               (Use the secondary structure alphabet ($SECONDARY_STRUCTURE_3_STATES_OK_CHARS) (defaults to IUPAC codes for nucleotides)\n".
    " -alphabet=<STRING>                 (Specify the exact alphabet to use (don't forget to include upper and lower case characters)\n".
    " -type={fasta,phylip,alignedfasta}  (Manually specify the file type)\n".
    " -sumOfPairs                        (Calculate the (self) sum of pairs score (matches = 1, mismatche = 0))\n".
    " -v                                 (Verbose)\n".
    " -d                                 (Debug)\n".
    "\n".
    " FILENAME(S) is one or more newick (.new), phylip (.phy) or fasta (.fa(s), .afa, .msda) files\n".
    "\n";


my %args;  # keys are: debug, taxonlen=<number>


for( my $argvIndex = 0; $argvIndex <= $#ARGV; $argvIndex++){
    if( $ARGV[ $argvIndex] =~ m/^-([^\s=]+)=(.*)/){
	$args{lc($1)} = $2; # lc = lowercase
	#print "$0 found arg $1 (=$args{$1})\n";
	splice @ARGV, $argvIndex, 1;
	$argvIndex--;
    }elsif( $ARGV[ $argvIndex] =~ m/^-([^\s]+)/){
	$args{lc($1)} = $FALSE; # lc = lowercase
	#print "$0 found arg $1 (=$args{$1})\n";
	splice @ARGV, $argvIndex, 1;
	$argvIndex--;
    }
}

my $VERBOSE = $FALSE;
if( exists($args{"v"}) ||
    exists($args{"verbose"})){
    $VERBOSE = $TRUE;
}

my $DEBUG = $FALSE;
if( exists($args{"d"}) ||
    exists($args{"debug"})){
    $DEBUG = $TRUE;
    $VERBOSE = $TRUE;
}

if( defined( $args{"taxonlen"}) &&
    $args{"taxonlen"} =~ m/^\d+$/ ){
    $MAX_TAXON_LENGTH = $args{"taxonlen"};;
}

my $ignoreStartsWithNumber = $FALSE;
if( exists( $args{"ignorestartswithnumber"})){
    $ignoreStartsWithNumber = $TRUE;
}

my $ignoreBars = $FALSE;
if( exists( $args{"ignorebars"})){
    $ignoreBars = $TRUE;
}


my $alphabet = $NUCLEOTIDE_OK_CHARS;
if( exists( $args{"p"}) ||
    exists( $args{"protein"})){
    $alphabet = $AMINO_ACID_OK_CHARS;
}elsif( exists( $args{"ss8"})){
    $alphabet = $SECONDARY_STRUCTURE_OK_CHARS;
}elsif( exists( $args{"ss3"})){
    $alphabet = $SECONDARY_STRUCTURE_3_STATES_OK_CHARS;
}

if( defined( $args{"alphabet"})){
    $alphabet = $args{"alphabet"};
}


my $DO_SUM_OF_PAIRS = $FALSE;
if( exists( $args{"sumofpairs"}) ||
    exists( $args{"sumOfPairs"})){
    $DO_SUM_OF_PAIRS = $TRUE;
}

my $fileType = "unknown";
my $isfileTypeSet = $FALSE;
if( defined( $args{"type"})){
    $fileType = $args{"type"};
    if( $fileType ne "phylip" &&
	$fileType ne "fasta" &&
	$fileType ne "alignedfasta"){
	die( "Error in type argument: $fileType is not valid\n". $usage);
    }
    $isfileTypeSet = $TRUE;
}

my @files = @ARGV;

if( $VERBOSE){
    print STDERR "Config:\n".
	"\t files                    @files\n".
	"\t MAX_TAXON_LENGTH         $MAX_TAXON_LENGTH\n".
	"\t ignoreStartsWithNumber   $ignoreStartsWithNumber\n".
	"\t ignoreBars               $ignoreBars\n".
	"\t alphabet                 $alphabet\n".
	"\t fileType                 $fileType\n".
	"\t DO_SUM_OF_PAIRS          $DO_SUM_OF_PAIRS\n".
	"";
}


if( $#files < 0){
    die( $usage );
}

#
# globals 
#
my $numSegs = -1;  # used as a global variable
my %taxonHash;

FILE_LOOP: for( my $i = 0; $i <= $#files; $i++){
    my @taxonNames;
    my @seqs;
    my $line;

    my $numTaxa = -1;
    my $numChars = -1;

    if( $VERBOSE){ print STDERR "file = $files[$i]\n"; }
    if( ! open( FILE, $files[$i])){ print STDERR "Error $files[$i] FAILED to open file: $! (in $0)\n"; next FILE_LOOP; }
    my $isAligned = $FALSE;
    my $counter = 0;


    if( $isfileTypeSet == $FALSE){
	$fileType = "unknown";
    }

    %taxonHash = ();
    
    if( $fileType eq "unknown"){
	if( $files[$i] =~ m/\.phy$/ ){
	    $fileType = "phylip";
	}elsif( $files[$i] =~ m/\.msda$/ ||
		$files[$i] =~ m/\.afa$/){
	    $fileType = "alignedfasta";
	}elsif(	$files[$i] =~ m/\.fas?$/ ){
	    $fileType = "fasta";
	}else{	
	    # unknown file extention (look at first line and guess)
	    if( ! defined( $line = <FILE>)){
		print STDERR "ERROR: File \"$files[$i]\" appears to be empty\n";
		next FILE_LOOP;
	    }
	    
	    if( $line =~ m/^\s*(\d+)\s+(\d+)\s*$/){
		$fileType = "phylip";
	    }elsif( $line =~ m/^\s*>/){
		$fileType = "fasta";
		while( <FILE>){
		    if( $_ !~ m/^\s*>/ &&
			$_ =~ m/-/){
			$fileType = "alignedfasta";
			last;
		    }
		}
	    }

	    close( FILE);
	    if( ! open( FILE, $files[$i])){ print STDERR "Error $files[$i] FAILED to open file: $! (in $0)\n"; next FILE_LOOP; }
	    
	}
	if( $DEBUG){ print STDERR "DEBUG: First line indicates that the file type is $fileType\n"; }
    } # END if( $fileType eq "unknown") 
    
    if( $fileType eq "phylip"){
	$numTaxa = -1;
	$numChars = -1;

	$isAligned = $TRUE;
	
	if( ! defined( $line = <FILE>)){ print STDERR "Error reading first line of the phylip file \"$files[$i]\": $! ($files[$i] FAILED)\n"; next FILE_LOOP; }
	if( $line !~ m/\s*(\d+)\s+(\d+)/){ print STDERR "Error num of seqs and characters from line \"$line\" ($files[$i] FAILED)\n"; next FILE_LOOP; }
	$numTaxa = $1;
	$numChars = $2;

	while( $line = <FILE>){
	    if( $line =~ m/^\s*$/){
		while( $line = <FILE>){
		    if( $line !~ m/^\s*$/){ print STDERR "Error, found a non blank line after one or more blank lines (line = \"$line\") ($files[$i] FAILED)\n"; next; }
		}
		next;
	    }
	    $counter++;
	    if( $counter > $numTaxa){ print STDERR "Error, found more taxa than previously declared ($numTaxa) with line \"$line\" (skipping $files[$i])\n"; next FILE_LOOP; }

	    if( $line !~ m/^(\S+)\s+(\S+)$/){ print STDERR "Error in format ($files[$i] FAILED)\n"; next; }
	    $taxonNames[ $counter - 1] = $1;
	    my $seq = $2;
	    chomp $seq;
	    $seqs[ $counter - 1] = $seq;
	}

	for( $counter = 0; $counter <= $#seqs; $counter++){
	    if( length( $seqs[ $counter]) != $numChars){ print STDERR "ERROR, found more characters (".length( $seqs[$counter]).") then specified ($numChars) for taxon $taxonNames[$counter] (skipping $files[$i])\n"; next FILE_LOOP; }
	}
	
    }elsif( $fileType =~ m/fasta/){
	
	if( $fileType eq "alignedfasta"){
	    $isAligned = $TRUE;
	}

	while( $line = <FILE>){
	    if( $line =~ m/^\s*$/){
		while( $line = <FILE>){
		    if( $line !~ m/^\s*$/){ print STDERR "Error, found a non blank line after one or more blank lines (line = \"$line\") ($files[$i] FAILED)\n"; next; }
		}
		next;
	    }

	    if( $line =~ m/^\s*>\s*(.*)/){
		$counter++;
		$taxonNames[ $counter - 1] = $1;
		$seqs[ $counter - 1] = "";
	    }else{
		chomp $line;
		$seqs[ $counter - 1] .= $line;
	    }
	}
    }elsif( $fileType eq "newick"){
	warn "ALERT: support for newick files not yet implemented\n";
	next FILE_LOOP;
	
	my $cmd = "wc -l $files[$i]";
	my $result = `$cmd`;
	if( $result !~ /^\s*(\d+)\s/){ print STDERR "Error running $cmd ($files[$i] FAILED)\n\n"; next FILE_LOOP; }
    }else{
	print STDERR "ERROR, file \"$files[$i]\" is of unknown type (skipping)\n";
	next FILE_LOOP;
    }
    close FILE;
    
    $numTaxa = $#taxonNames + 1;
    $numChars = length( $seqs[0]);
    if( $numTaxa != $#seqs + 1){  print STDERR "Found a different number of taxon names and sequences ($files[$i] FAILED)\n\n"; next; }
    my $alignment = [[]];  # 2D array
    $numSegs = -1;

    for( my $k = 0; $k < $numTaxa; $k++){
	my $taxon = $taxonNames[$k];
	my $seq = $seqs[$k];
	my $localAlphabet = $alphabet . "\#";
	if( $isAligned == $TRUE){
	    $localAlphabet .= "-";
	}
	
	checkTaxonAndSeq( $taxon, $seq, $files[$i], $localAlphabet);
	
	if( $isAligned == $TRUE &&
	    (length( $seq) - $numSegs) != $numChars){ print STDERR "Error, found a different number of characters (".(length($seq) - $numSegs).") for $taxon than previously declared ($numChars) ($numSegs segments) (skipping $files[$i])\n"; }

	if( $isAligned == $TRUE){
	    my @seqArray = split( //, $seq);
	    for( my $j = 0; $j <= $#seqArray; $j++){
		$alignment->[$k][$j] = $seqArray[$j];
	    }
	}
    }
    if( $isAligned == $TRUE){
	(checkForGappedColumn( $alignment, $numTaxa, $numChars, $files[$i]) == $SUCCESS) or next FILE_LOOP;
	if( $DO_SUM_OF_PAIRS == $TRUE){
	    sumOfPairs($alignment, $numTaxa, $numChars, $files[$i]);
	}
    }
}



# NOTE: checkTaxonAndSeq() uses (reads and writes) global variable $numSegs

sub checkTaxonAndSeq{
    my ($taxon, $seq, $fileName, $okChars) = @_;
    my $seqCopy = $seq;
    my $status = $SUCCESS;

    #
    # taxon name
    #
    # MUSCLE can not associate tree taxon with sequence taxon if there are spaces after the names	    
    if( $taxon =~ m/\s/){ print STDERR "Error, found a white space character in taxon label \"$taxon\" ($fileName FAILED)\n"; $status = $FAILURE; }
    if( $ignoreStartsWithNumber == $FALSE && $taxon =~ m/^\d/){ print STDERR "Taxon label \"$taxon\" starts with a number.  Certain packages don't like this (e.g., TNT?)  ($fileName FAILED)\n"; $status = $FAILURE; }
    if( $ignoreBars == $FALSE && $taxon =~ m/\|/){ print STDERR "Error, found the \"|\" character in taxon label \"$taxon\" ($fileName FAILED)\n"; $status = $FAILURE; }
    if( length($taxon) > $MAX_TAXON_LENGTH){ print STDERR "Taxon label \"$taxon\" is longer than $MAX_TAXON_LENGTH ($fileName FAILED)\n"; $status = $FAILURE; }
    my $taxonStd = $taxon;
    $taxonStd =~ s/[^\w\d\s]/_/g;
    if( exists( $taxonHash{$taxonStd})){
	print STDERR "Taxon label \"$taxon\" ($taxonStd) already found ($fileName FAILED)";
	if( $taxonHash{$taxonStd} eq $seq){
	    print STDERR " (by the way, the sequences are identical)";
	}
	print STDERR "\n";
	$status = $FAILURE;
    }
    #$taxonHash{$taxonStd} = 1;    
    $taxonHash{$taxonStd} = $seq;
    
    #
    # sequence data
    #
    if( $seq =~ m/\?/){
	print STDERR "Found a \"?\" in the seq data";
	if( $DEBUG){ print STDERR "\"$seq\""; }
	print STDERR "($fileName FAILED)\n"; $status = $FAILURE;
    }
    if( $seq =~ m/\s/){ print STDERR "Found a space character in the seq data \"$seq\" ($fileName FAILED)\n"; $status = $FAILURE; }

    $seq =~ s/\-//g;
    if( length( $seq) == 0){
	print STDERR "Error, sequence ONLY contains gap characters (seq = \"$seqCopy\") (taxa = $taxon) ($fileName FAILED)\n";
	$status = $FAILURE;
    }
    $seq =~ s/[$okChars]+//g;
    if( $seq ne ""){
	print STDERR "Error, the following chars are not found in the specificed alphabet \"$seq\" (taxa = $taxon) (alphabet = $alphabet) ($fileName FAILED)\n";
	$status = $FAILURE;
    }
    
    # segment support
    $seqCopy =~ s/[^\#]//g;
    if( length( $seqCopy) != $numSegs){
	if( $numSegs == -1){
	    $numSegs = length( $seqCopy);
	}else{
	    print STDERR "Error, found " . length($seqCopy) ." segments, but I was expecting $numSegs (taxon = $taxon) ($fileName FAILED)\n"; $status = $FAILURE;
	}
    }
    
    return $status;
    
} # END checkTaxonAndSeq()


sub checkForGappedColumn{
    my ( $alignment, $numTaxa, $numChars, $fileName) = @_;

    if( $numTaxa == 1){ return $SUCCESS; }
    
    # check alignment for columns with only gaps
  COL: for( my $col = 0; $col < $numChars; $col++){
      #print STDERR "alignment->[0][$col] = ". $alignment->[0][$col] ."\n";
      for( my $seqI = 0; $seqI < $numTaxa; $seqI++){
	  if( $alignment->[$seqI][$col] ne "-"){
	      next COL;
	  }
      }
      print STDERR "Error, column $col has only gaps in it (skipping $fileName)\n"; return $FAILURE;
  }
    
    return $SUCCESS;
} # END checkForGappedColumn()


sub sumOfPairs{
    my ( $alignment, $numTaxa, $numChars, $fileName) = @_;

    if( $numTaxa == 1){ return $FAILURE; }
    
    my $sum = 0;
    my $numPairwises = 0;

    for( my $seqI = 0; $seqI < $numTaxa; $seqI++){
	for( my $seqJ = 0; $seqJ < $numTaxa; $seqJ++){
	    if($seqI == $seqJ){
		next;
	    }
	    for( my $col = 0; $col < $numChars; $col++){
		if( $alignment->[$seqI][$col] eq $alignment->[$seqJ][$col]){
		    $sum++;
		}
	    }
	    $numPairwises++;
	}
    }

    $sum /= $numPairwises * $numChars;
    $sum *= 100.0;
    print "Sum Of Pairs Score $fileName: $sum\%\n";
} # END sumOfPairs()


