#!/usr/bin/perl
use strict;

# perverse script to get rid of unwanted jar signatures
use Cwd qw(abs_path);
use File::Temp qw(tempdir);

my $tempdir = tempdir( CLEANUP => 1);

my $jarfile;

my @jarfiles;

while (scalar @ARGV) {
    my $jarfile = shift @ARGV;
    ((-f $jarfile) and $jarfile=~/.+\.jar/) 
        and push @jarfiles, abs_path($jarfile);
}
my $pwdir = `pwd`;
chdir($tempdir);

while (scalar @jarfiles) {
    $jarfile = shift @jarfiles;
    system("rm -Rf *");
    system("jar xf $jarfile");
    system("mv $jarfile $jarfile.bak");
    system("find META-INF \\( -name \"*.SF\" \\) -exec rm \\{\\} \\;");
    system("find META-INF \\( -name \"*.RSA\" \\) -exec rm \\{\\} \\;");
    system("jar cf $jarfile *");
}

chdir($pwdir);
