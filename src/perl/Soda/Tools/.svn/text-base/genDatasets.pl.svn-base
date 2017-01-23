#!/usr/bin/perl -w
use strict;
use Soda::Tools::Parser;
use Soda::Tools::Dataset;
use Data::Dumper;

local $, = ",";
my $datafile;
my $ntaxa;
my $nchars;
#print "@ARGV\n";
if ( $#ARGV >= 2 ) 
{
  ( $datafile, $ntaxa, $nchars ) = @ARGV;
}
else
{
  ( $datafile, $ntaxa, $nchars ) = ( "../data/5_zilla.nex", 5, 758 );
}


my $dataset = new Soda::Tools::Dataset;
Soda::Tools::Parser->parseFile( $datafile, \$dataset );
$$dataset{currentTaxa} = undef;

#print Dumper( $dataset ) ;

my $matchCharacterLine = ${${$$dataset{data}}[0]}{data};
my @matchCharacterLine = unpack("C*", $matchCharacterLine);

my @matchChar = unpack("C*", ".");
my $matchChar = $matchChar[0];

my @newdataset;
for my $x ( @{$$dataset{data}} )
{
  my @temp = unpack("C*", $$x{data});
  my $temp = @temp;
  #print "Lines $temp $#temp\n";
  for ( my $i = 0; $i <= $#temp ; $i++ )
  {
    if ( $temp[$i] == $matchChar )
    {
      $temp[$i] = $matchCharacterLine[$i];
    }
  }
  push(@newdataset, \@temp);
}

for ( my $i = 0 ; $i < $ntaxa ; $i++ )
{
  print "${${$$dataset{data}}[$i]}{name}\n";
  #print @{$newdataset[$i]};
  my $end = @{$newdataset[$i]} - 1;
  #print "End $end \n";
  print pack("C*", @{$newdataset[$i]}[0..$end]);
  print "\n";
  print "\n";
}
