#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
   
   // check for argument
   if (argc < 2) {   
      
      fprintf(stderr,"Error: Address argument required\n");
      exit(-1);
   } 
   printf("Argv[1] %s\n",argv[1]);
   unsigned long int address = atol(argv[1]); // convert string arg to int   
   unsigned long int pageNumber = address / 4096; // 4kb page size
   unsigned long int offset = address % 4096; // offset will be remainder
   
   printf("The address %d contains:\npage number = %ld\noffset = %ld\n", address, pageNumber, offset);  

}