#!/usr/bin/perl -w
use strict;

use Soda;

#my @TESTFILE = ( 'testInput1.tree', 'testInput2.tree' );
my @TESTFILE = ( 'testInput2.tree' );

foreach my $x ( @TESTFILE )
{
  my $newickTreeParser = new Soda::NewickTreeParser;
  $newickTreeParser->parseFilename( $x );
  print "Between\n";
  my $tree = $newickTreeParser->swig_tree_get();
  #Soda::TreePrinter::PrintNewickTree($newickTreeParser->swig_tree_get());
  Soda::TreePrinter::PrintNewickTree($tree);
  Soda::TreePrinter::flush();
  Soda::TreePrinter::PrintTreeNodeList($tree);
  Soda::TreePrinter::flush();
  print "\n";
}
