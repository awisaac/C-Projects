// COMP 322 Lab 6 - spiritd.c
// Program is a daemon that spawns two mole children.  When mole children are killed
// by the USR1 and USR2 signals sent to the parent, they may randomly respawn.  Sending
// a TERM signal to the parent kills the children and the parent.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <syslog.h>

// mole pids
int mole1, mole2;
char *mole_loc;

// creates a new mole
void newMole(int mole_num) {
   
   int pid = fork();
      
   if (pid == 0) { // child process           
      
      // mole 1
      if (mole_num == 1) {        
         if (execl(mole_loc, "mole", "mole1", 0) == -1) {            
            syslog(LOG_ERR | LOG_LPR, "PID: %d: %m", getpid()); // log errors to syslog daemon
            exit(-1);  
         }
      }
      
      // mole 2
      else {               
         if (execl(mole_loc,"mole", "mole2", 0) == -1) {
            syslog(LOG_ERR | LOG_LPR, "PID: %d: %m", getpid()); // log errors to syslog daemon           
            exit(-1);          
         }      
      }
   }
   
   // child pid gets saved
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
   
   static int moles_dead[2]; // tracks the alive/dead state of the moles
   int i, mole_selected, signals[] = {SIGTERM, SIGUSR1, SIGUSR2};
         
   //reregister handler   
   for (i = 0; i < 3; i++) {
   
      if (signal(signals[i],handler) == SIG_ERR) {      
         syslog(LOG_ERR | LOG_LPR, "PID: %d: Signum is invalid.", getpid()); // log errors to syslog daemon        
         exit(-1);
      }   
   }  
   
   // term kills both moles and parent
   if (signum == SIGTERM) {
   
      kill(getpid() * -1,SIGTERM);
      exit(0);   
   }
   
   else if (signum == SIGUSR1) {
      
      moles_dead[0] = 1;    
      // if mole1 is killed, 3/4 chance of randomly respawning a new mole
      if (kill(mole1,SIGTERM) == 0 && rand() > RAND_MAX / 4) {         
         
         // if both are dead, pick one to randomly respawn
         if (moles_dead[0] == 1 && moles_dead[1] == 1) {
            mole_selected = rand() % 2;         
            newMole(mole_selected + 1);
            moles_dead[mole_selected] = 0;               
         }
         
         // else respawn mole1, since mole2 is alive
         else {
            newMole(1);
            moles_dead[0] = 0;
         }         
      }   
   }
   
   else if (signum == SIGUSR2) {
      
      moles_dead[1] = 1;      
      // if mole2 is killed, 3/4 chance of randomly respawning a new mole
      if (kill(mole2,SIGTERM) == 0 && rand() > RAND_MAX / 4) {
      
         // if both are dead, pick one to randomly respawn
         if (moles_dead[0] == 1 && moles_dead[1] == 1) {
            mole_selected = rand() % 2;         
            newMole(mole_selected + 1);
            moles_dead[mole_selected] = 0;               
         }
         
         // else respawn mole2, since mole1 is alive
         else {      
            newMole(2);
            moles_dead[1] = 0;
         }
      }   
   }    
}

int main(int argc, char **argv) {   
         
   int i, fd0, fd1, fd2;   
   struct rlimit limits;
   int signals[] = {SIGTERM, SIGUSR1, SIGUSR2};
      
   // get current working directory to find mole program
   // location after daemon changes working directory to /
   mole_loc = strcat(getenv("PWD"),"/mole");
  
   // new seed for random mole respawn;
   srand(time(NULL));
      
   // set all created file permissions to 0
   umask(0);
   
   // fork off a child process and die
   if (fork() != 0) {   
      exit(0);
   }
  
   // create a new session
   if (setsid() == -1) {
      syslog(LOG_ERR | LOG_LPR, "PID: %d: %m", getpid()); // log errors to syslog daemon
      exit(-1);    
   }
   
   // change working directory to /
   if (chdir("/") == -1) {
      syslog(LOG_ERR | LOG_LPR, "PID: %d: %m", getpid());
      exit(-1);      
   }         
   
   // get file desc. limits
   if (getrlimit(RLIMIT_NOFILE, &limits) == -1) {
      syslog(LOG_ERR | LOG_LPR, "PID: %d: %m", getpid());
      exit(-1);       
   }
   
   // if no limit, make limit 1024
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
   
   if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
   
      syslog(LOG_ERR | LOG_LPR, "PID: %d: STDIN, STDOUT, STDERR map to /dev/null failed", getpid());
      exit(-1);
   }
   
   //create 2 moles   
   newMole(1); 
   newMole(2);   
   
   // create handlers for signals   
   for (i = 0; i < 3; i++) {
   
      if (signal(signals[i],handler) == SIG_ERR) {      
         syslog(LOG_ERR | LOG_LPR, "PID: %d: Signum is invalid.", getpid());
         exit(-1);
      }   
   } 
   
   // wait for signals
   while(1) {      
      wait();
   }  
}