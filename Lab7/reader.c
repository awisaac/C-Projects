#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
   
   int buffer[1024];
   int i, bytes_read;
   
   if (argc < 2) {
      fprintf(stderr,"File path argument required.");
      exit(-1);   
   }
     
   int fd = open(argv[1], O_RDONLY);
   
   bytes_read = read(fd, buffer, sizeof(int) * 1024);
      
   while (bytes_read > 0) {
      
      if (bytes_read == sizeof(int) * 1024) {      
         for (i = 0; i < 1024; i++) {
            fprintf(stdout,"%d ",buffer[i]);
         } 
      }      
      else {
         for (i = 0; i < bytes_read / sizeof(int); i++) {
            fprintf(stdout,"%d ",buffer[i]);
         }       
      }
      
      bytes_read = read(fd, buffer, sizeof(int) * 1024);       
   }
}