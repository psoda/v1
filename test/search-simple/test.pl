open(TEST_OUT,"../../src/psoda -v search.nex |");
use constant OUTSIDE => 1;
use constant BLOCKBEGIN => 2;
use constant BLOCKEND => 3;
$state = OUTSIDE;
use constant ERROR => 255;
use constant CORRECT => 0;
$rval = ERROR;

# Skip past the printout of the paup block
MAINWHILE: while ($line = <TEST_OUT>) {
  print "Line was: $line\n";
  if ($line =~ /\*\*\*\*\*\*/) { # A set of stars at the beginning or end of the code block
    if($state == OUTSIDE) {
			$state = BLOCKBEGIN;
      next;
    }
    if($state == BLOCKBEGIN) {
			$state = BLOCKEND;
      last MAINWHILE;
    }
  }
}
if($state != BLOCKEND) {
  exit(ERROR);
}

$cscore = 21.0;
# Check that we have the correct output
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
  ($scorestr, $score) = split(/\s+/, $line);
  print "expected $cscore, got $score\n";
  if(($scorestr eq "score") && ($cscore == $score)) {
    print "Correct Score\n";
    $rval = CORRECT;
    last CHECKWHILE;
  }
}

close(SUB_OUT);
exit($rval);
