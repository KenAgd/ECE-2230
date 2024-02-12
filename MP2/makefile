# The makefile for MP2.
#
# make          -- to build program lab2
# make design   -- to verify no illegal variables
# make driver   -- to compile testing program
# make clean    -- to delete object files, executable, and core
# make linkedlist.o  -- to compile only linkedlist.o
#
# You should not need to change this file.  
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

lab2 : linkedlist.o ens_support.o lab2.o
	gcc -Wall -g linkedlist.o ens_support.o lab2.o -o lab2

linkedlist.o : linkedlist.c datatypes.h linkedlist.h
	gcc -Wall -g -c linkedlist.c

ens_support.o : ens_support.c datatypes.h linkedlist.h ens_support.h
	gcc -Wall -g -c ens_support.c

lab2.o : lab2.c datatypes.h linkedlist.h ens_support.h
	gcc -Wall -g -c lab2.c

driver : driver.o linkedlist.o ens_support.o
	gcc -Wall -g linkedlist.o ens_support.o driver.o -o driver

driver.o : driver.c datatypes.h linkedlist.h ens_support.h
	gcc -Wall -g -c driver.c

#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
design :
	@grep -e "-> *list_head" lab2.c ens_support.c ||:
	@grep -e "-> *list_tail" lab2.c ens_support.c ||:
	@grep -e "-> *current_list_size" lab2.c ens_support.c ||:
	@grep -e "-> *list_sorted_state" lab2.c ens_support.c ||:
	@grep -e "-> *fwd_link" lab2.c ens_support.c ||:
	@grep -e "-> *back_link" lab2.c ens_support.c ||:
	@grep -e "-> *data_ptr" lab2.c ens_support.c ||:
	@grep "comp_proc" lab2.c ens_support.c ||:
	@grep "linked_elem_t" lab2.c ens_support.c ||:
	@grep "ep_id" linkedlist.c ||:
	@grep "rec_num" linkedlist.c ||:
	@grep "time_received" linkedlist.c ||:
	@grep "source_type" linkedlist.c ||:
	@grep "privacy" linkedlist.c ||:
	@grep "band" linkedlist.c ||:
	@grep "epid_rec_t" linkedlist.c ||:
	@grep "ens_" linkedlist.c ||:

clean :
	rm -f *.o lab2 driver core a.out

