#!/usr/bin/sh
#
# MP1 place in student directory
valgrind --leak-check=yes ./lab2 < ../tests/t00testinput > gradingout_t00testinput 2> gradingout_valt00testinput
valgrind --leak-check=yes ./lab2 < ../tests/t01nolist > gradingout_t01nolist 2> gradingout_valt01nolist
valgrind --leak-check=yes ./lab2 < ../tests/t02destruct > gradingout_t02destruct 2> gradingout_valt02destruct
valgrind --leak-check=yes ./lab2 < ../tests/t03oneadd > gradingout_t03oneadd 2> gradingout_valt03oneadd
valgrind --leak-check=yes ./lab2 < ../tests/t04oneaddtrim > gradingout_t04oneaddtrim 2> gradingout_valt04oneaddtrim
valgrind --leak-check=yes ./lab2 < ../tests/t05oneaddclear > gradingout_t05oneaddclear 2> gradingout_valt05oneaddclear
valgrind --leak-check=yes ./lab2 < ../tests/t06oneaddmatch > gradingout_t06oneaddmatch 2> gradingout_valt06oneaddmatch
valgrind --leak-check=yes ./lab2 < ../tests/t07sort > gradingout_t07sort 2> gradingout_valt07sort
valgrind --leak-check=yes ./lab2 < ../tests/t08sortreplace > gradingout_t08sortreplace 2> gradingout_valt08sortreplace
valgrind --leak-check=yes ./lab2 < ../tests/t09notfullreplace > gradingout_t09notfullreplace 2> gradingout_valt09notfullreplace
valgrind --leak-check=yes ./lab2 < ../tests/t10trim > gradingout_t10trim 2> gradingout_valt10trim
valgrind --leak-check=yes ./lab2 < ../tests/t11clearold > gradingout_t11clearold 2> gradingout_valt11clearold
valgrind --leak-check=yes ./lab2 < ../tests/t12match > gradingout_t12match 2> gradingout_valt12match
valgrind --leak-check=yes ./lab2 < ../tests/t13dblcreate > gradingout_t13dblcreate 2> gradingout_valt13dblcreate
valgrind --leak-check=yes ./lab2 < ../tests/t14emptylist > gradingout_t14emptylist 2> gradingout_valt14emptylist
valgrind --leak-check=yes ./lab2 < ../tests/t15emptytrim > gradingout_t15emptytrim 2> gradingout_valt15emptytrim
