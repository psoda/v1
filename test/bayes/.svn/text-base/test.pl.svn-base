use constant ERROR => 255;
use constant CORRECT => 0;

$rval = ERROR;
system("rm bayesout.tre");
$ENV{PSODA_HOME} = '../../src/';
system("echo home is \$PSODA_HOME\n");
open(TEST_OUT,"../../src/psoda bayes.nex |");

# Check that we have the correct output
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
	if($line =~ /99/) {
		print("found output line\n"); 
    $rval = CORRECT;
	}
}

exit($rval);
