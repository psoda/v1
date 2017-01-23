open(TEST_OUT,"../../src/psoda ../../data/test/test.nex |");
use constant ERROR => 255;
use constant CORRECT => 0;
my $rval = ERROR;

my $lastLine = "";
while (my $line = <TEST_OUT>) {
    print "Output Line was: $line\n";
    # only advance the line if the line isn't only space
    if (!($line =~ /^\s$/)) {
	$lastLine = $line;
    }
}
if ($lastLine =~ m/Passed All Tests/i) {
    $rval = CORRECT;
}
close(TEST_OUT);
exit($rval);
