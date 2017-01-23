#!/usr/local/bin/perl -w

use strict;

my $DEBUG = 1;

opendir( DIR, ".");
my @files = readdir( DIR);
close(DIR);
my $fileName = "";
for( my $i = 0; $i <= $#files; $i++){
  if( $files[$i] =~ m/\.nex$/){
	$fileName = $files[$i];
	print "File: $fileName\n" if $DEBUG;

	my $fileBase = $fileName;
	$fileBase =~ s/\.nex$//;
	
	my $cmd = "../../src/psoda $fileName > $fileBase.out";
	print "$cmd\n" if $DEBUG;
	my $rc = system( $cmd);
	if( $rc ){
	    die( "\nERROR: cmd \"$cmd\" failed ");
	}

	#my $ALIGNMENT_FILE = "$fileBase.afa";
	#if( ! -f $ALIGNMENT_FILE){
	#    die( "\nERROR: output alignment \"$ALIGNMENT_FILE\" not created: $! ");
	#}

	#$cmd = "./standardsChecker.pl $ALIGNMENT_FILE";
	#print "$cmd\n" if $DEBUG;
	#$rc = `$cmd`;
	#if( $rc ne ""){
	#    die( "\nERROR: cmd \"$cmd\" failed (output = $rc) ");
	#}

	my $outputFile = "$fileBase.out";
  
    # TODO: Verify output
	if( -f "$fileBase.expected.out"){
	    $cmd = "diff -bd $fileBase.expected.out $outputFile";
	    print "$cmd\n" if $DEBUG;
	    $rc = `$cmd`;
	    if( $rc ne ""){
			die( "\nERROR: cmd \"$cmd\" failed (output = $rc) ");
	    }
	}
  }
}

	
exit( 0); # CORRECT
