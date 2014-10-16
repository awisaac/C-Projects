// COMP 282 Lab 4 - dining_p.c
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <semaphore.h>
#include <signal.h>
#include <string.h>

sig_atomic_t volatile stop = 0;

void eat(int seat) {
   printf("Philosopher #%d is eating\n",seat);
   usleep(rand() % 1000003); // sleep random time less than 1 sec
}

void think(int seat) {
   printf("Philosopher #%d is thinking\n",seat);
   usleep(rand() % 1000003);
}

void handler(int signum) {
   stop = 1;  
}

void getChopstick(char *filename, int philosophers, int seat, int side) {   
   
   // can't use seat itself in sprintf or it gets converted to char value   
   int seat2 = seat; 
      
   if (side == 1 && seat + 1 > philosophers) {
      sprintf(filename, "/awi93796%d\0", 1); // last left chop stick
   }   
   else if (side == 1 && seat + 1 <= philosophers){
      sprintf(filename, "/awi93796%d\0", seat2 + 1); // left chop stick
   }    
   else {      
      sprintf(filename, "/awi93796%d\0", seat2); // right chop stick
   }   
}

int main(int argc, char *argv[]) {

   signal(SIGTERM,handler);
   printf("PID: %d\n",getpid());      
   
   if (argc < 3) {
   
      fprintf(stderr, "Error: Not enough arguments.\n");
      exit(1);
   }
   
   char leftFilename[10 + strlen(argv[1])]; // allocs /awi93796 + # of digits in N + \0
   char rightFilename[10 + strlen(argv[1])];
   sem_t *rightChop;
   sem_t *leftChop;
   sem_t *mutex;
      
   int philosophers = strtol(argv[1], NULL, 10);
   int seat = strtol(argv[2], NULL, 10);
   int count = 0;
   
   getChopstick(rightFilename, philosophers, seat, 0);        
   getChopstick(leftFilename, philosophers, seat, 1);
      
   mutex = sem_open("/awi93796", O_CREAT|O_EXCL, 0666, 1);
   if (mutex == SEM_FAILED ) {
      mutex = sem_open("/awi93796", O_CREAT);
   }
   
   leftChop = sem_open(leftFilename, O_CREAT|O_EXCL, 0666, 1);
   if (leftChop == SEM_FAILED ) {
      leftChop = sem_open(leftFilename, O_CREAT);
   }
      
   rightChop = sem_open(rightFilename, O_CREAT|O_EXCL, 0666, 1);
   if (rightChop == SEM_FAILED ) {
      rightChop = sem_open(rightFilename, O_CREAT); 
   }   
      
   if (rightChop == SEM_FAILED || leftChop == SEM_FAILED || mutex == SEM_FAILED) {
   
      fprintf(stderr,"Error: Semaphore open failed.\n");
      exit(1);
   }
     
   while (!stop) {         
            
      sem_wait(mutex); // removes hold and wait - can have only 0 or 2 chopsticks
      if (!stop) sem_wait(rightChop);         
      if (!stop) sem_wait(leftChop);
      sem_post(mutex);
      eat(seat);
      sem_post(leftChop);
      sem_post(rightChop);
      think(seat);
      count++;         
   }
   
   sem_close(leftChop);
   sem_close(rightChop);
   sem_close(mutex);
   
   sem_unlink(rightFilename);   
   sem_unlink(leftFilename);
   sem_unlink("/awi93796");
   
   fprintf(stderr,"Philosopher #%d completed %d cycles\n",seat,count);
   return 0;
} 

