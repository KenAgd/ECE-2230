# makefile for MP5
# Version: 2
#
# -lm is used to link in the math library
#
# -Wall turns on all warning messages 
#
#
CC = gcc
CPPFLAGS = -DVALIDATE
CFLAGS = -g -Wall
LDLIBS = -lm  

# the default target is to build the lab5 executable
lab5 : tree.o lab5.o

bst.o : tree.c tree.h

lab5.o : lab5.c tree.h

clean :
	rm -f tree.o lab5.o lab5

