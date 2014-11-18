#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
   
   int buffer[1024];
   int i;
   
   if (argc < 2) {
      fprintf(stderr,"File path argument required.");   
   }
     
   int fd = open(argv[1], O_RDONLY);
      
   while (read(fd, buffer, sizeof(int) * 1024) > 0) {
      
      for (i = 0; i < 1024; i++) {
         fprintf(stdout,"%d ",buffer[i]);
      }   
   }
}