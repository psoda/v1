#!/usr/local/bin/perl -w

use strict;

opendir( DIR, ".");
my @files = readdir( DIR);
close(DIR);
my $fileName = "";
for( my $i = 0; $i <= $#files; $i++){
    if( $files[$i] =~ m/\.nex$/){
	$fileName = $files[$i];
	print "File: $fileName\n";

	my $fileBase = $fileName;
	$fileBase =~ s/\.nex$//;
	
	my $cmd = "../../src/psoda $fileName > $fileBase.out";
	print "$cmd\n";
	my $rc = system( $cmd);
	if( $rc ){
	    die( "\nERROR: cmd \"$cmd\" failed ");
	}

	my $ALIGNMENT_FILE = "$fileBase.afa";
	if( ! -f $ALIGNMENT_FILE){
	    die( "\nERROR: output alignment \"$ALIGNMENT_FILE\" not created: $! ");
	}

	$cmd = "./standardsChecker.pl $ALIGNMENT_FILE";
	print "$cmd\n";
	$rc = `$cmd`;
	if( $rc ne ""){
	    die( "\nERROR: cmd \"$cmd\" failed (output = $rc) ");
	}

	if( -f "$fileBase.expected.afa"){
	    $cmd = "diff -bd $fileBase.expected.afa $ALIGNMENT_FILE";
	    print "$cmd\n";
	    $rc = `$cmd`;
	    if( $rc ne ""){
		die( "\nERROR: cmd \"$cmd\" failed (output = $rc) ");
	    }
	}
    }
}

	
exit( 0); # CORRECT
