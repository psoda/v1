#!/usr/bin/perl

$Directory = shift ;
$ProjectName = shift ;
#print $Directory;

$replaceMap{"PROJECTNAME"} = uc($ProjectName);
#$replaceMap{"Namespace"} = $Namespace;
#$replaceMap{"CLASSNAME"} = uc($Classname);
#$replaceMap{"NAMESPACE"} = uc($Namespace);
#$replaceMap{"classname"} = lcfirst($Classname);

$templateSconscriptBody = do { local( @ARGV, $/ ) = "TemplateSConscript" ; <> } ;

for $key ( keys %replaceMap )
{
  $templateSconscriptBody =~ s/$key/$replaceMap{$key}/g;
}

open( $fh, ">$Directory\/SConscript" ) ;
print $fh $templateSconscriptBody ;
close( $fh ) ;
