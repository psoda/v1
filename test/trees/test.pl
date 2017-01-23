open(TEST_OUT,"../../src/psoda trees.nex |");
use constant ERROR => 255;
use constant CORRECT => 0;
$rval = ERROR;

$cscore = 28.0;
# Check that we have the correct output
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
  ($scorestr, $score) = split(/\s+/, $line);
  print "expected $cscore, got $score\n";
  if($cscore == $score) {
    if($cscore == 29) {
      $rval = CORRECT;
      last CHECKWHILE;
    }
    if($cscore == 28.0) {
      $cscore = 27;
			next;
    }
    if($cscore == 27.0) {
      $cscore = 29;
			next;
    }
  } else {
    last CHECKWHILE;
  }
}

close(SUB_OUT);
exit($rval);
