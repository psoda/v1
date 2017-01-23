open(TEST_OUT,"../../src/psoda -v zilla.nex |");
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

$bscore = 16245.0;
$btime = 70;
# Check that we have the correct output
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
#Best Score: 16245 Time 59
  ($beststr, $scorestr, $score, $timestr, $time) = split(/\s+/, $line);
  if(($beststr eq "Best") &&
     ($scorestr eq "Score:") &&
     ($timestr eq "Time")) {
    print "expected $bscore, got $score\n";
    print "expected $btime, got $time\n";
    if(($score <= $bscore) && ($time <= $btime)) {
			print "Correct Score\n";
			$rval = CORRECT;
			last CHECKWHILE;
    }
  }
}

close(SUB_OUT);
exit($rval);
