ECE 2230 MP2 test directory
Fall 2021

The scripts to run the test input files assumes that you have an mp2 folder.  In
this folder are two sub folders:
   mysol: a dirctory that contains your lab2 executable code
   tests: the directory with grading input and output files.

Then copy three files from the tests directory to your mysol directory: 
    run.sh, valrun.sh, check.pl.

Then move to your mysol directory and do:
    ./run.sh
    ./check.pl
And
    ./valrun.sh
    ./check.pl

Also do:
    make design

To delete output and executable files do
    make clean

