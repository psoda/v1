#!/usr/bin/perl

$Classname = shift ;
$Namespace = shift ;
$Template = shift ;

$HFileName = "Template.h" ;
$CPPFileName = "Template.cpp" ;

if ( defined $Template ) 
{
  $HFileName = "TemplateTemplateSingle.h" ;
  $CPPFileName = "TemplateTemplateSingle.cpp" ;
}

#print $Classname, $Namespace ;

$replaceMap{"Classname"} = $Classname;
$replaceMap{"Namespace"} = $Namespace;
$replaceMap{"CLASSNAME"} = uc($Classname);
$replaceMap{"NAMESPACE"} = uc($Namespace);
$replaceMap{"classname"} = lcfirst($Classname);

$templatehBody = do { local( @ARGV, $/ ) = $HFileName ; <> } ;
$templatecppBody = do { local( @ARGV, $/ ) = $CPPFileName ; <> } ;

for $key ( keys %replaceMap )
{
  $templatehBody =~ s/$key/$replaceMap{$key}/g;
  $templatecppBody =~ s/$key/$replaceMap{$key}/g;
}

open( $fh, ">$Classname.h" ) ;
print $fh $templatehBody ;
close( $fh ) ;

if ( not defined $Template )
{
  open( $fh, ">$Classname.cpp" ) ;
  print $fh $templatecppBody ;
  close( $fh ) ;
}
