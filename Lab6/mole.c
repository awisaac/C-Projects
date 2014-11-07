#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
   
   // place lab6 in home directory
   char *home = getenv("HOME");
   char *fullpath = strcat(home,"/lab6.log");
   
   if (argc < 2) {
      
      fprintf(stderr,"No arguments provided.\n");
      exit(-1);      
   }
   
   // open lab6 for write only
   int fd = open(fullpath, O_WRONLY | O_APPEND | O_CREAT);
   
   if (fd == -1) {   
      
      perror(sys_errlist[errno]);
      exit(-1);  
   }
   
   // write specified string to file 
   if (strcmp(argv[1], "mole1") == 0) {
   
      write(fd, "Pop mole1\n",11); 
   }
   else if (strcmp(argv[1], "mole2") == 0) {  
     
      write(fd, "Pop mole2\n",11); 
   }
   
   close(fd);
   
   // hang around until terminated
   while (1) {
      wait();
   }
}