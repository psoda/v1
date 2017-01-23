#!/usr/bin/perl -w
use strict;
use FileHandle;
use Parse::RecDescent;

package Soda::Tools::Parser;
use vars qw( @ISA );
@ISA = ( "Parse::RecDescent" );

local $/;
my $grammar = <DATA>;
$::RD_HINT = 1;
#$::RD_TRACE = 1;
sub new
{
  my $this = shift;
  my $class = ref($this) || $this;
  my $self = new Parser::RecDescent( $grammar );
  bless $self, $class;
  return $self;
}

sub parse
{
  ( my $class, my $text, my $interpreter ) = @_;
  my $self = Parse::RecDescent->new( $grammar );
  $self->{"interpreter"} = $interpreter;
  $self->nexus($text);
}

sub parseFile
{
  my ( $class, $filename, $interpreter ) = @_;
  my $self = Parse::RecDescent->new( $grammar );
  $self->{"interpreter"} = $interpreter;
  local ($/);
  my $fh = new FileHandle "<$filename";
  $self->nexus(<$fh>);
}

__DATA__

startrule: nexus

nexus: NEXUSSY blocks
blocks: block blocks |
block:  BEGINSY blockDecleration ENDSY SEMICOLONSY
blockDecleration: blockData | blockPaup
blockData:  DATASY SEMICOLONSY dataBlockHeaders dataBlockType
dataBlockType: matrix | sequences
dataBlockHeaders: DIMENSIONSSY dimensions SEMICOLONSY formatLine optionsLine eliminateLine taxlabelsLine charstateLine 
dimensions: ntax nchar | nchar ntax 
ntax: NTAXASY EQSY NUMBERSY 
nchar: NCHARSY EQSY NUMBERSY
formatLine: FORMATSY formatList SEMICOLONSY |
formatList: formatItem formatList |

formatItem:
DATATYPESY EQSY format_item_datatype
| GAPSY EQSY CHAR
| MISSINGSY EQSY CHAR
| MATCHCHARSY EQSY CHAR
| INTERLEAVESY

format_item_datatype:
NUCLEOTIDESY
|DNASY
|STANDARDSY
|RNASY
|PROTEINSY

optionsLine:
eliminateLine:
taxlabelsLine:
charstateLine:

matrix:
MATRIXSY { ${$thisparser->{"interpreter"}}->setIsMatrix();  } matrixEntryList SEMICOLONSY 

matrixEntryList:
matrixEntry matrixEntryList
|

matrixEntry:
NAMESY { ${$thisparser->{"interpreter"}}->addName( $item{NAMESY} ); } DNASEQSY { ${$thisparser->{"interpreter"}}->addCharacters( $item{DNASEQSY} ); } 

sequences:
SEQUENCESSY { ${$thisparser->{"interpreter"}}->setIsSequences(); } sequencesEntryList SEMICOLONSY

sequencesEntryList:
sequencesEntry sequencesEntryList
|

sequencesEntry:
NAMESY { ${$thisparser->{"interpreter"}}->addName( $item{NAMESY} ); } DNASEQSY { ${$thisparser->{"interpreter"}}->addCharacters( $item{DNASEQSY} ); }

blockPaup:

NEXUSSY: /#NEXUS/
BEGINSY: /BEGIN/i
ENDSY: /END/i
SEMICOLONSY: /\;/
DATASY: /data/i
DIMENSIONSSY: /dimensions/i
NTAXASY: /ntaxa/i | /ntax/i
NCHARSY: /nchar/i
EQSY: /\=/
NUMBERSY: /[0-9]+/
FORMATSY: /format/i
DATATYPESY: /datatype/i
GAPSY: /gap/i
MISSINGSY: /missing/i
MATCHCHARSY: /matchchar/i
INTERLEAVESY: /interleave/i
NUCLEOTIDESY: /nucleotide/i
DNASY: /dna/i
STANDARDSY: /standard/i
RNASY: /rna/i
PROTEINSY: /protein/i
MATRIXSY: /matrix/i
NAMESY: /[a-z][.\w]*/i
SEQUENCESSY: /sequences/i
DNASEQSY: /[a-z?.-]*/i
CHAR: /./i

