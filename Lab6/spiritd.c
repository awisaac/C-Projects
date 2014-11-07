#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

// mole pids
int mole1, mole2;

// creates a new mole
void newMole(int mole_num) {
   
   int pid = fork();
   
   if (pid == 0) {      
      
      if (mole_num == 1) {
      
         if (execl("/root/mole", "mole", "mole1") == -1) {
            perror(sys_errlist[errno]);
            exit(-1);  
         }
      }
      
      else {      
            
         if (execl("/root/mole","mole", "mole2") == -1) {
            perror(sys_errlist[errno]);
            exit(-1);          
         }      
      }
   }
   
   else {
   
      if (mole_num == 1) {
         mole1 = pid; 
      }
      
      else {
         mole2 = pid;
      }   
   }   
}

void handler(int signum) {
      
   //reregister handler
   int i, signals[] = {SIGTERM, SIGUSR1, SIGUSR2};
   
   for (i = 0; i < 3; i++) {
   
      if (signal(signals[i],handler) == SIG_ERR) {
      
         fprintf(stderr,"Signal handler error.");
         exit(-1);
      }   
   }  
   
   // term kills both moles and parent
   if (signum == SIGTERM) {
   
      kill(getpid() * -1,SIGTERM);
      exit(0);   
   }
   
   else if (signum == SIGUSR1) {
          
      // if mole1 is killed, randomly respawn mole1
      if (kill(mole1,SIGTERM) == 0 && rand() > RAND_MAX / 10) {         
         newMole(1);         
      }   
   }
   
   else if (signum == SIGUSR2) {
         
      // if mole2 is killed, randomly respawn mole2
      if (kill(mole2,SIGTERM) == 0 && rand() > RAND_MAX / 10) {      
         newMole(2);
      }   
   } 
}

int main(int argc, char **argv) {   
         
   // new seed for random mole respawn;
   srand(time(NULL));
   
   int i, fd0, fd1, fd2;   
   struct rlimit limits;
   int signals[] = {SIGTERM, SIGUSR1, SIGUSR2};   
      
   // set all created file permissions to 0
   umask(0);
   
   // fork off a child process and die
   if (fork() != 0) {   
      exit(0);
   }
  
   // create a new session
   if (setsid() == -1) {
      perror(sys_errlist[errno]);
      exit(-1);    
   }
   
   // change working directory to /
   if (chdir("/") == -1) {
      perror(sys_errlist[errno]);
      exit(-1);      
   }         
   
   // get file desc. limits
   if (getrlimit(RLIMIT_NOFILE, &limits) == -1) {
      perror(sys_errlist[errno]);
      exit(-1);       
   }
   
   // if no limit, make a limit
   if (limits.rlim_max == RLIM_INFINITY) {   
      limits.rlim_max = 1024;
   } 
   
   // close all files
   for (i = 0; i < limits.rlim_max; i++) {   
      close(i);
   }
   
   // remap stdin, stdout, and stderr to /dev/null
   fd0 = open("/dev/null",O_RDWR);
   fd1 = dup(0);
   fd2 = dup(0);
   
   //create 2 moles   
   newMole(1); 
   newMole(2);   
   
   // create handlers for signals   
   for (i = 0; i < 3; i++) {
   
      if (signal(signals[i],handler) == SIG_ERR) {      
         fprintf(stderr,"Signal handler error.");
         exit(-1);
      }   
   } 
   
   // wait for signals
   while(1) {      
      wait();
   }  
}