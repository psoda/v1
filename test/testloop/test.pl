open(TEST_OUT,"../../src/psoda test.nex |");
use constant ERROR => 255;
use constant CORRECT => 0;
$rval = ERROR;

$count = 10;
CHECKWHILE: while ($line = <TEST_OUT>) {
  print "Output Line was: $line\n";
  if ($line =~ /^(\s)*$/) { # Blank Line
		next;
  }
  ($rcount) = split(/\s+/, $line);
  print "expected $count, got $rcount\n";
  if($rcount == $count) {
    if($count == 0) {
      $rval = CORRECT;
      last CHECKWHILE;
    }
    $count --;
    next;
  } else {
    last CHECKWHILE;
  }
}

close(SUB_OUT);
exit($rval);
