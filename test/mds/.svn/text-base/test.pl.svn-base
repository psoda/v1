open(TEST_OUT,"../../src/psoda MDS.nex |");
open(GOAL_IN,"MDS.goal");

use constant ERROR => 255;
use constant CORRECT => 0;



$rval = CORRECT;
# Check that we have the correct output
while ($line = <TEST_OUT>) {
  $target = <GOAL_IN>;
  if(!($line eq $target))
  {
	$rval = ERROR;
	print($line);
	print($target);
  }
}

close(TEST_OUT);
close(GOAL_IN);
exit($rval);
