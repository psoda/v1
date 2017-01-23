open(TEST_OUT,"../../src/psoda ShowTrees.nex |");
open(GOAL_IN,"ShowTrees.goal");

use constant ERROR => 255;
use constant CORRECT => 0;



$rval = CORRECT;
# Check that we have the correct output
while ($line = <TEST_OUT>) {
  $target = <GOAL_IN>;
  if(!($line eq $target))
  {
	$rval = ERROR;
  }
}

close(TEST_OUT);
close(GOAL_IN);
exit($rval);
