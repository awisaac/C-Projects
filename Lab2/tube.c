// COMP 282 Lab 2 - tube.c
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int parseArgs(int argc, char *argv[]) { // Finds comma delimiter and converts to null
   
   int i, comma = 1;
      
   for (i = 1; i < argc; i++) {
   
      if (strcmp(argv[i], ",") == 0) {
      
         comma = i;
         argv[i] = 0;
      }
   }
   
   return comma;
}

int main(int argc, char *argv[]) {

   int i, comma, pid0, pid1, pFD[2], status1, status2;
   comma = parseArgs(argc, argv);
         
   pipe(pFD);
   pid0 = fork();   
     
   if (pid0 == 0) { // 1st child process - writes      

      dup2(pFD[1],1); // out end of pipe becomes stdout
      
      if (comma == 1) { // no args entered for first process
         
         char * noargs[2] = {argv[1], 0}; // insert null terminator 
         execvp(argv[1],noargs); // exec no args process
      }
      
      else { // args provided
         execvp(argv[1],argv + 1); // exec process with args
      }
        
      exit(-1); // this should only call if execv fails   
   }
   
   pid1 = fork();
   if (pid1 == 0) { // 2nd child process - reads

      dup2(pFD[0],0); // in end of pipe becomes stdin
      
      execvp(argv[comma + 1],argv + comma + 1); // exex 2nd process     
                  
      exit(-1); // this should only call if execv fails
   }   
   
   else { // parent process
                                       
      fprintf(stderr,"CPID0: %d\n", pid0); // child processes
      fprintf(stderr,"CPID1: %d\n", pid1);
   
      waitpid(pid0,&status1,0); // wait until child processes finish
      waitpid(pid1,&status2,0);
            
      close(pFD[0]); // close pipe
      close(pFD[1]);
      
      fprintf(stderr,"%s: $? = %d\n",argv[1],WEXITSTATUS(status1)); // child processes exit status
      fprintf(stderr,"%s: $? = %d\n",argv[comma + 1],WEXITSTATUS(status2));      
   }
      
   return 0;
}
   
   
   
   
   
   

