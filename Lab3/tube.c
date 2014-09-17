#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

   int pid0, pid1, pFD[2], inFD, outFD, status1, status2;
   
   pipe(pFD);
   pid0 = fork();
   
   char *newargs1[2] = {argv[1],0};         
   char *newargs2[2] = {argv[2],0};
     
   if (pid0 == 0) { // 1st child process - writes      
      
      close(1);
      dup(pFD[1]);

      execv(newargs1[0],newargs1);
        
      exit(10);   
   }
   
   pid1 = fork();
   if (pid1 == 0) { // 2nd child process - reads
      
      close(0);
      dup(pFD[0]);

      execv(newargs2[0],newargs2);   
      
      close(inFD);      
      exit(10);
   }   
   
   else { // parent process
                                    
      close(pFD[0]);
      close(pFD[1]);
   
      fprintf(stderr,"CPID0: %d\n", pid0);
      fprintf(stderr,"CPID1: %d\n", pid1);
   
      waitpid(pid0,&status1,0);
      waitpid(pid1,&status2,0);
      
      fprintf(stderr,"%s: $? = %d\n",argv[1],WEXITSTATUS(status1));
      fprintf(stderr,"%s: $? = %d\n",argv[2],WEXITSTATUS(status2));

   }
      
   return 0;
}
   
   
   
   
   
   

