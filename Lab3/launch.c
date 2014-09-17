#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

   int i,status, cReturnVal, pid = fork();      
   char *newarg[argc - 1];
      
   if (pid == 0) {
   
      for (i = 0; i < argc; i++) {
   
         newarg[i] = argv[i + 1]; 
      }         
      
      execv(argv[1],newarg);            
   }
   
   else {
   
      fprintf(stderr, "%s: $$ = %d\n", argv[1], pid);
      wait(&status);
      fprintf(stderr, "%s: $? = %d\n", argv[1], WEXITSTATUS(status));
   }
}