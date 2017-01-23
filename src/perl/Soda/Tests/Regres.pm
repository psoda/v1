#!/usr/bin/perl
package Soda::Tests::Regres;

use strict;

sub run
{
  ( my $SODA, my $DATAROOT ) = @_;

  my @DATAFILES = ( 
    "5_zilla_da_test.nex",
    "5_zilla_da.nex",
    "zilla_da.nex"
  );

  foreach my $X (@DATAFILES)
  {
    # this should be a popen thread in my opinion
    my $command = "$SODA $DATAROOT/$X";
    print $command . "\n";
    print `$command`;
  }
}

1;
