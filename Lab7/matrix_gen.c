// COMP 282 Lab 7 matrix_gen.c
// Andrew Isaac 11/15/14

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
   
   if (argc < 2) {
   
      fprintf(stderr,"No argument provided.");
      exit(-1);
   }     
   
   int i, j, size = atoi(argv[1]);
   int int_buf[size];
      
   srand(time(NULL));
   
   for (i = 0; i < size; i++) {
      for (j = 0; j < size; j++) {
         int_buf[j] = rand() % 200 - 100;
      }      
      
      write(1,int_buf, sizeof(int)* size);      
   }      
     

   return(0);
}