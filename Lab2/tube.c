#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int parseArgs(int argc, char *argv[]) {
   
   int i, comma;
      
   for (i = 1; i < argc; i++) {
   
      if (strcmp(argv[i], ",") == 0) {
      
         comma = i;
      }
   }
   
   return comma;
}

int main(int argc, char *argv[]) {

   int i, comma, pid0, pid1, pFD[2], inFD, outFD, status1, status2;
   comma = parseArgs(argc, argv);
   
   char *newargv0[comma];
   char *newargv1[argc - comma]; 
      
   for (i = 0; i < comma - 1; i++) {
      
      newargv0[i] = argv[i + 1];
   }
   
   newargv0[i] = 0; // char ** terminator
            
   for (i = comma + 1; i < argc; i++) {
      
      newargv1[i - (comma + 1)] = argv[i];
   }
   
   newargv1[i - comma - 1] = 0; // char ** terminator
         
   pipe(pFD);
   pid0 = fork();   
     
   if (pid0 == 0) { // 1st child process - writes      

      dup2(pFD[1],1);

      execv(newargv0[0],newargv0);
        
      exit(-1); // this should only call if execv fails   
   }
   
   pid1 = fork();
   if (pid1 == 0) { // 2nd child process - reads

      dup2(pFD[0],0);

      execv(newargv1[0],newargv1);   
            
      exit(-1); // thos should only call if execv fails
   }   
   
   else { // parent process
                                       
      fprintf(stderr,"CPID0: %d\n", pid0); // child processes
      fprintf(stderr,"CPID1: %d\n", pid1);
   
      waitpid(pid0,&status1,0); // wait child processes finish
      waitpid(pid1,&status2,0);
            
      close(pFD[0]); // close pipe
      close(pFD[1]);
      
      fprintf(stderr,"%s: $? = %d\n",newargv0[0],WEXITSTATUS(status1)); // child processes exit status
      fprintf(stderr,"%s: $? = %d\n",newargv1[0],WEXITSTATUS(status2));

   }
      
   return 0;
}
   
   
   
   
   
   

