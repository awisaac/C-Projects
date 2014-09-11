// COMP 322 Lab 1 "Time and Forking Children"
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>

void printPIDInfo(int pid, int status) {
   
   int cReturnVal, i;
   struct tms buf;
      
   if (pid == 0) {
      printf("PPID: %d, PID: %d\n",getppid(),getpid());
   }
   
   else {
      cReturnVal = WEXITSTATUS(status);
      printf("PPID: %d, PID: %d, CPID: %d, RETVAL: %d\n",getppid(),getpid(),pid,cReturnVal);
      
      i = times(&buf);
      
      // printf("Number of ticks since arbitrary time: %ld\n", i);
      // printf("Number of ticks per second: %ld\n",sysconf(_SC_CLK_TCK));      
      printf("USER: %ld SYS: %ld\n", buf.tms_utime, buf.tms_stime);
      printf("CUSER: %ld CSYS: %ld\n", buf.tms_cutime, buf.tms_cstime);     
   }
}

int main() {
  
   time_t * t = NULL;
   int status, pid;
   
   printf("START: %d\n", time(t)); // seconds since UNIX epoch
   pid = fork();
      
   if (pid == 0) { // child process
      
      printPIDInfo(pid, 0);
      exit(27); // arbitrary return value
   }
   
   else { // parent process
      
      waitpid(pid,&status,0);
      printPIDInfo(pid, status);
   }
   
   printf("STOP: %d\n", time(t));  
   return 0;
}