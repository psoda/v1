#!/usr/bin/perl -w
use strict;

use Soda;

my $interpreter = new Soda::Interpreter;
my $dataset = new Soda::Dataset;

$interpreter->setDataset( $dataset );
$dataset->DISOWN();
$dataset->setntaxa(2);
$dataset->createSequences(); 
$dataset->setDataformat( $Soda::Dataset::SEQUENCES_DATAFORMAT );

$dataset->addName( "one" );
$dataset->addCharacters( "AAAACCCCGGGG" );
$dataset->addName( "one" );
$dataset->addCharacters( "AAAATTCCCCTTGGGG" );

$interpreter->doDirectAlignmentSearch();
