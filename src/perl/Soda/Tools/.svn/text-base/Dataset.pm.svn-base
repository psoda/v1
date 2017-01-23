#!/usr/bin/perl -w
use strict;
use Parse::RecDescent;

package Soda::Tools::Taxon;
sub new()
{
  my $this = shift;
  my $class = ref($this) || $this;
  my $self = { name=>shift, data=>undef };
  bless $self, $class;
  return $self;
}

package Soda::Tools::Dataset;
sub new()
{
  my $this = shift;
  my $class = ref($this) || $this;
  my $self = { data=>[] };
  bless $self, $class;
  return $self;
}

sub setIsMatrix()
{
  my $self = shift;
  $$self{isMatrix} = 1;
}

sub setIsSequences()
{
  my $self = shift;
  $$self{isSequences} = 1;
}

sub addName()
{
  my $self = shift;
  $$self{currentTaxa} = Soda::Tools::Taxon->new( shift );
}

sub addCharacters()
{
  my $self = shift;
  my $chars = shift;
  my $charsLength = length( $chars );
  $$self{minCharacterLength} = $charsLength if $$self{data} or $charsLength < $$self{minCharacterLength};
  #print $$self{minCharacterLength} . " " .  $charsLength . "\n";
  $$self{currentTaxa}{data} = $chars;
  push @{$$self{data}},  $$self{currentTaxa};
}

1;
