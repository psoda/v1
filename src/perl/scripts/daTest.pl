#!/usr/bin/perl -w
use strict;

use Soda;
use Soda::Paup::Parser;

my $interpreter = new Soda::Interpreter;

open INFILE, "< ../../data/5_zilla_da.nex" ;
local ($/);
my $text;
$text = <INFILE>;

Soda::Paup::Parser->parse( $text, \$interpreter );
