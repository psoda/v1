open(TEST_OUT,"../../src/psoda step.nex |");
use constant OUTSIDE => 1;
use constant BLOCKBEGIN => 2;
use constant BLOCKEND => 3;
$state = OUTSIDE;
use constant ERROR => 255;
use constant CORRECT => 0;
$rval = ERROR;

$bscore = 16427.0;
$btime = 20;
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
