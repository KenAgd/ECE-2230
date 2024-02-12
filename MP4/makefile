# makefile for MP4
# Version: 1
#
# -lm is used to link in the math library
#
# -Wall turns on all warning messages 
#
comp = gcc
comp_flags = -g -Wall 
comp_libs = -lm  

lab4 : linkedlist.o mem.o lab4.o
	$(comp) $(comp_flags) linkedlist.o mem.o lab4.o -o lab4 $(comp_libs)

linkedlist.o : linkedlist.c datatypes.h linkedlist.h
	$(comp) $(comp_flags) -c linkedlist.c

mem.o : mem.c mem.h
	$(comp) $(comp_flags) -c mem.c

lab4.o : lab4.c datatypes.h linkedlist.h mem.h
	$(comp) $(comp_flags) -c lab4.c

clean :
	rm -f *.o lab4 core

