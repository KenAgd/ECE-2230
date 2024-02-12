#!/usr/bin/perl
use strict;
use warnings;
# put in students directory and run to check against correct output file
#

# loop through each file and directory at current location

    # backtick executes a command and collects stdout 
    # stderr is not collected but goes to output

# @ denotes an array
my @alltest = qw(
    t00testinput
    t01nolist 
    t02destruct 
    t03oneadd 
    t04oneaddtrim 
    t05oneaddclear 
    t06oneaddmatch 
    t07sort 
    t08sortreplace 
    t09notfullreplace 
    t10trim
    t11clearold 
    t12match 
    t13dblcreate 
    t14emptylist 
    t15emptytrim
);

foreach my $test (@alltest) {
    my $gradefile = 'gradingout_'.$test;
    my $valgrindfile = 'gradingout_val'.$test;
    my $answerfile = 'out_'.$test;

    my $info = `diff -w $gradefile ../tests/$answerfile`;
    my $found = 0;
    if ($? > 0) {
        print "$test failed\n";
        #print "the info is: $info\n";
        $found = 1;
    }
    if (-f $valgrindfile) {
        $info = `grep "no leaks" $valgrindfile`;
        if ($info eq "") {
            $info = `grep "total heap usag" $valgrindfile`;
            print "\t$test found leaks: $info";
        }
        $info = `grep "0 errors" $valgrindfile`;
        if ($info eq "") {
            $info = `grep "ERROR SUMMARY" $valgrindfile`;
            print "\t$test $info\n";
        }
    }
    if ($found == 1) {
        print "\n";
    }
}

    #$info = `cd $user; grep "winning amount is" gradingoutput; cd ..`;
    #print "the info is: $info\n";

    # for example, if we want to see only the errors and
    # warnings we can ignore the stdout.  To see stdout:
    # print "the info is: $info\n";
    # print "the status is $?\n";

    # system executes a command but only gets final return value
    # all output just goes to stdout
    #
    # $make = "cd $user; make clean; make; cd ..";
    # $rc = 0xffff & system $make;
    # if ($rc != 0) {
    #     die "system $make failed: $?";
    # }


#    a more elaborate way to test return value
#    $rc = 0xffff & system $make;
#    if ($rc == 0) {
#        print "ran with normal exit\n";
#    }
#    elsif ($rc == 0xff00) {
#        print "command failed: $!\n";
#    }
#    elsif (($rc & 0xff) == 0) {
#        $rc >>= 8;
#        print "ran with non-zero exit status $rc\n";
#    }
#    else {
#        print "ran with ";
#        if ($rc & 0x80) {
#            $rc &= ~0x80;
#            print "coredump from ";
#        }
#        print "signal $rc\n";
#    }

# reg expression  (upper case negates the following)
#   \d [0-9]
#   \w [a-zA-Z0-9_]
#   \s [ \r\t\n\f]
#   * zero or more
#   + one or more
#   ? zero or one

# File tests
#     -x file or directory is executable
#     -e file or directory exists
#     -z file exists and has zero size
#     -s non zero size
#     -f a file
#     -d a directory
#
