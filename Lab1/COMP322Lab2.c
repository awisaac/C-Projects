#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>

int main() {

   int pid = fork();
   int status;
   int cReturnVal;
   struct tms buf;
   
   if (pid == 0) {
   
      printf("START: %d\n", time());
      printf("PID: %d, PPID: %d\n",getpid(),getppid());
      exit(0);
   }
   
   else {
   
      waitpid(pid,&status,0);
      cReturnVal = WIFEXITED(status);
      printf("PID: %d, PPID: %d, RETVAL: %d\n",getpid(),getppid(),cReturnVal);
      
      times(&buf);
      
      printf("USER: %jd, SYS: %jd\n", (int)(buf.tms_utime), (int)(buf.tms_stime));
      printf("CUSER: %jd, CSYS: %jd\n", (int)(buf.tms_cutime), (int)(buf.tms_cstime));
      printf("STOP: %d\n", time());
      
   }
}