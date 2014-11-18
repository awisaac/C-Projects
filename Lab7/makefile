# Name: Andrew Isaac
# Project: Lab7

# This is a Makefile for my lab assignment. It allows me to easily and 
# accurately rebuild the lab assignment.

# EXECUTABLES is a Make Variable (similar to an environment variable) that 
# lists all the "executables" associated with the lab. 

EXECUTABLES=matrix_gen matrix_add reader

SRC=matrix_gen.c matrix_add.c reader.c
OBJ=matrix_gen.o matrix_add.o reader.o

MATRIX_GEN=matrix_gen

MATRIX_GEN_SRC=matrix_gen.c 
MATRIX_GEN_OBJ=matrix_gen.o

MATRIX_ADD=matrix_add

MATRIX_ADD_SRC=matrix_add.c
MATRIX_ADD_OBJ=matrix_add.o

READER=reader

READER_SRC=reader.c
READER_OBJ=reader.o

# The CC and CFLAGS variables are defined to ensure I'm using the native 
# compiler and that the correct command-line options are set

CC=gcc
CFLAGS=-g -lrt

# Here is my default target for the Makefile. When I type make, this first 
# target will be created. I.e., all the executables

all: $(EXECUTABLES)
matrix_gen: $(MATRIX_GEN)
matrix_add: $(MATRIX_ADD)
reader: $(READER)

# On the following lines (which _must_ start with a tab), are the commands I 
# need to type -- each time my source file is modified.

#$(EXECUTABLES):
#	$(CC) $(CFLAGS) -c $(SRC)
#	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(MATRIX_GEN):
	$(CC) $(CFLAGS) -c $(MATRIX_GEN_SRC)
	$(CC) $(CFLAGS) -o $@ $(MATRIX_GEN_OBJ)

$(MATRIX_ADD):
	$(CC) $(CFLAGS) -c $(MATRIX_ADD_SRC)
	$(CC) $(CFLAGS) -o $@ $(MATRIX_ADD_OBJ)

$(READER):
	$(CC) $(CFLAGS) -c $(READER_SRC)
	$(CC) $(CFLAGS) -o $@ $(READER_OBJ)

# By convention, I always have a target called
# "clean" that I used to define all temporary
# or intermediates.

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~
