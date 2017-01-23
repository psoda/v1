#!/usr/bin/perl
use Cwd;
use Soda::Tests::Regres;

print cwd();

$SODA = cwd() . "/soda";
$DATAROOT = cwd() . "/../data";


SodaTests::Regres::run( $SODA, $DATAROOT );
