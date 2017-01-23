open(TEST_OUT,"../../src/psoda 9taxa.nex |");
use constant OUTSIDE => 1;
use constant BLOCKBEGIN => 2;
use constant BLOCKEND => 3;
$state = OUTSIDE;
use constant ERROR => 255;
use constant CORRECT => 0;
$rval = ERROR;

# Check that we do not have the incorrect output
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
#We are looking for the string memory exhausted
	if($line =~ /.*Error.*/)
	{
		exit(1);
	}
  
}

close(SUB_OUT);
exit(0);
