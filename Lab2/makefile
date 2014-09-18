# Name: Andrew Isaac
# Project: Lab2

# This is a Makefile for my lab assignment. It allows me to easily and 
# accurately rebuild the lab assignment.

# EXECUTABLES is a Make Variable (similar to an environment variable) that 
# lists all the "executables" associated with the lab. 

EXECUTABLES=launch tube

SRC=launch.c tube.c
OBJ=launch.o tube.o

LAUNCH=launch

LAUNCHSRC=launch.c
LAUNCHOBJ=launch.o

TUBE=tube

TUBESRC=tube.c
TUBEOBJ=tube.o

# The CC and CFLAGS variables are defined to ensure I'm using the native 
# compiler and that the correct command-line options are set

CC=gcc
CFLAGS=-g

# Here is my default target for the Makefile. When I type make, this first 
# target will be created. I.e., all the executables

all: $(EXECUTABLES)
launch: $(LAUNCH)
tube: $(TUBE)

# On the following lines (which _must_ start with a tab), are the commands I 
# need to type -- each time my source file is modified.

$(EXECUTABLES):
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
$(LAUNCH):
	$(CC) $(CFLAGS) -c $(LAUNCHSRC)
	$(CC) $(CFLAGS) -o $@ $(LAUNCHOBJ)
$(TUBE):
	$(CC) $(CFLAGS) -c $(TUBESRC)
	$(CC) $(CFLAGS) -o $@ $(TUBEOBJ)

# By convention, I always have a target called
# "clean" that I used to define all temporary
# or intermediates.

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
