# Name: Andrew Isaac
# Project: Lab4

# This is a Makefile for my lab assignment. It allows me to easily and 
# accurately rebuild the lab assignment.

# EXECUTABLES is a Make Variable (similar to an environment variable) that 
# lists all the "executables" associated with the lab. 

EXECUTABLES=dining-p launch-philosophers

SRC=dining-p.c launch-philosophers.c
OBJ=dining-p.o launch-philosophers.o

DINING=dining-p

DINING_SRC=dining-p.c 
DINING_OBJ=dining-p.o

LAUNCH=launch-philosophers

LAUNCH_SRC=launch-philosophers.c
LAUNCH_OBJ=launch-philosophers.o

# The CC and CFLAGS variables are defined to ensure I'm using the native 
# compiler and that the correct command-line options are set

CC=gcc
CFLAGS=-g -pthread -lrt

# Here is my default target for the Makefile. When I type make, this first 
# target will be created. I.e., all the executables

all: $(EXECUTABLES)
dining-p: $(DINING)
launch-philosophers: $(LAUNCH)

# On the following lines (which _must_ start with a tab), are the commands I 
# need to type -- each time my source file is modified.

#$(EXECUTABLES):
#	$(CC) $(CFLAGS) -c $(SRC)
#	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(DINING):
	$(CC) $(CFLAGS) -c $(DINING_SRC)
	$(CC) $(CFLAGS) -o $@ $(DINING_OBJ)

$(LAUNCH):
	$(CC) $(CFLAGS) -c $(LAUNCH_SRC)
	$(CC) $(CFLAGS) -o $@ $(LAUNCH_OBJ)

# By convention, I always have a target called
# "clean" that I used to define all temporary
# or intermediates.

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
