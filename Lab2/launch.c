// COMP 282 Lab 2 - launch.c
// Andrew Isaac 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

   int status, pid = fork();      
      
   if (pid == 0) { // child process
      
      execvp(argv[1],argv + 1); // execute arguments
      
      exit(-1); // should only call when execvp fails            
   }
   
   else { // parent process
   
      fprintf(stderr, "%s: $$ = %d\n", argv[1], pid); // child pid
      wait(&status);
      fprintf(stderr, "%s: $? = %d\n", argv[1], WEXITSTATUS(status)); // child exit status
   }
   
   return 0;
}