// COMP 282 Lab 4 - dining_p.c
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <semaphore.h>
#include <signal.h>

volatile sig_atomic_t stop;

void handler(int signum) {
   stop = 1;
}

void chop(char *filename, int philosophers, int seat, int side) {   
    
   printf("Philosophers=%d Seat=%d\n",philosophers,seat);
   int file = seat; // can't use seat itself in sprintf or it gets converted to char value - weird!
      
   if (side == 1 && seat + 1 >= philosophers) {
      sprintf(filename, "/chopstick%d", 0); // last left chop stick
   }
   
   else if (side == 1 && seat + 1 < philosophers){
      sprintf(filename, "/chopstick%d", file + 1); // left chop stick
   }   
    
   else {      
      sprintf(filename, "/chopstick%d", file); // right chop stick
   }   
}

int main(int argc, char *argv[]) {

   signal(SIGTERM,handler);
   printf("PID: %d\n",getpid());      
   
   if (argc < 3) {
   
      fprintf(stderr, "Error: Not enough arguments.\n");
      exit(1);
   }
      
   int philosophers = strtol(argv[1], NULL, 10);
   int seat = strtol(argv[2], NULL, 10);
     
   char leftFilename[15];
   char rightFilename[15];
        
   chop(leftFilename, philosophers, seat, 0);
   printf("Right: %s\n", leftFilename);
   sem_t *rightChop = sem_open(leftFilename,O_CREAT,666,1);
   
   chop(rightFilename, philosophers, seat, 1);
   printf("Left: %s\n", rightFilename); 
   sem_t *leftChop = sem_open(rightFilename,O_CREAT,666,1);
   
   if (rightChop == SEM_FAILED || leftChop == SEM_FAILED) {
   
      fprintf(stderr,"Error: Semaphore open failed.\n");
      exit(1);
   }
   
   else {
      
      while(!stop) {
         
         printf("Waiting for right chopstick: %s...\n",rightFilename);
         sem_wait(rightChop);
         printf("Waiting for left chopstick: %s...\n",leftFilename);
         sem_wait(leftChop);
         printf("Eating with both chopsticks...\n");
         sleep(10); // eat
         printf("Done eating. Putting chop sticks down.\n");
         sem_post(leftChop);
         sem_post(rightChop);
         printf("Thinking...\n");
         sleep(10); // think    
         
      }  
   }
   
   sem_close(leftChop);
   sem_close(rightChop);
   
   sem_unlink(rightFilename);   
   sem_unlink(leftFilename);
} 

