# Name: Andrew Isaac
# Project: Lab6

# This is a Makefile for my lab assignment. It allows me to easily and 
# accurately rebuild the lab assignment.

# EXECUTABLES is a Make Variable (similar to an environment variable) that 
# lists all the "executables" associated with the lab. 

EXECUTABLES=spiritd mole

SRC=spiritd.c mole.c
OBJ=spiritd.o mole.o

SPIRITD=spiritd

SPIRITD_SRC=spiritd.c 
SPIRITD_OBJ=spiritd.o

MOLE=mole

MOLE_SRC=mole.c
MOLE_OBJ=mole.o

# The CC and CFLAGS variables are defined to ensure I'm using the native 
# compiler and that the correct command-line options are set

CC=gcc
CFLAGS=-g

# Here is my default target for the Makefile. When I type make, this first 
# target will be created. I.e., all the executables

all: $(EXECUTABLES)
spiritd: $(SPIRITD)
mole: $(mole)

# On the following lines (which _must_ start with a tab), are the commands I 
# need to type -- each time my source file is modified.

#$(EXECUTABLES):
#	$(CC) $(CFLAGS) -c $(SRC)
#	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(SPIRITD):
	$(CC) $(CFLAGS) -c $(SPIRITD_SRC)
	$(CC) $(CFLAGS) -o $@ $(SPIRITD_OBJ)

$(MOLE):
	$(CC) $(CFLAGS) -c $(MOLE_SRC)
	$(CC) $(CFLAGS) -o $@ $(MOLE_OBJ)

# By convention, I always have a target called
# "clean" that I used to define all temporary
# or intermediates.

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
