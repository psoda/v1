#!/usr/local/bin/perl

opendir(DIR,"./") || die("Cannot open directory !\n");

# Get contents of directory

@dir_contents= readdir(DIR);

# Close the directory

closedir(DIR);

# Now loop through array and run tests in each directory

foreach $dir (@dir_contents)
 {
   if( $dir !~ m/^\./ &&
       $dir ne "test.pl")
    {
      print "$dir -- ";
      chdir $dir;
      if( -e "test.pl") {
        system("perl test.pl > test.out 2>&1");
        $exit_value = $? >> 8;
        print "Return value: $exit_value - ";
        if($exit_value == 0) {
          print "PASSED\n";
        } else {
          print "FAILED\n";
        }
      } else {
          print "MISSING\n";
      }
      chdir "..";
    }
 }
