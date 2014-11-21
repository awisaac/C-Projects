#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
   
   int buffer[1024];
   int i, bytes_read;
   
   bytes_read = read(0, buffer, sizeof(int) * 1024);
      
   while (bytes_read > 0) {
      
      for (i = 0; i < bytes_read / sizeof(int); i++) {
         fprintf(stdout,"%d ",buffer[i]);
      }
             
      bytes_read = read(0, buffer, sizeof(int) * 1024);       
   }
}