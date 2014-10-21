// COMP 282 Lab 4 - launch-philosophers.c
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

sig_atomic_t volatile stop = 0;
sig_atomic_t volatile pStop = 0;

void handler(int signum) {
   
   signal(SIGTERM,handler);
   
   if (getpgid(getpid()) == getpid()) {                 
      stop = 1;
   }   
   else {
      pStop = 1;       
   }     
}

void eat(int seat) {
   printf("Philosopher #%d is eating\n",seat);
   usleep(rand() % 1000003); // sleep random time less than 1 sec
}

void think(int seat) {
   printf("Philosopher #%d is thinking\n",seat);
   usleep(rand() % 1000003);
}

sem_t *getChopstick(int seat, int philosophers, int side, sem_t *mutex) {
   
   sem_t *chopstick;   
   
   if (side == 1 && seat + 1 > philosophers) {
      chopstick = mutex + sizeof(sem_t); // last left chop stick
   }   
   else if (side == 1 && seat + 1 <= philosophers){
      chopstick = mutex + seat * sizeof(sem_t) + sizeof(sem_t); // left chop stick
   }    
   else {      
      chopstick = mutex + seat * sizeof(sem_t); // right chop stick
   }
   
   return chopstick;    
}

void philosophize(int seat, int philosophers, sem_t *mutex) {
   
   sem_t *rightChop = getChopstick(seat, philosophers, 0, mutex);
   sem_t *leftChop = getChopstick(seat, philosophers, 1, mutex);
   int count = 0;
   
   printf("Philosopher %d with pid %d created.\n", seat, getpid()); 
   
   while (!pStop) {
      
      sem_wait(mutex); // removes hold and wait - can have only 0 or 2 chopsticks at a time
      if (!pStop) sem_wait(rightChop);         
      if (!pStop) sem_wait(leftChop);
      sem_post(mutex);
      eat(seat);
      sem_post(leftChop);
      sem_post(rightChop);
      think(seat);
      count++;  
   }
   
   printf("Philosopher %d completed %d cycles.\n", seat, count); 
   exit(0);
}

int main(int argc, char* argv[]) {
   
   void (*pHandler)(int) = signal(SIGTERM,handler);
   
   if (pHandler == SIG_ERR) {
      
      fprintf(stderr,"Error: Signal handler not established.\n");
      perror(sys_errlist[errno]);
      exit(-1);   
   }
   
   // check for seats argument
   if (argc < 2) {   
      
      fprintf(stderr,"Error: Number of seats argument required\n");
      exit(-1);
   } 
   
   int seats = strtol(argv[1],NULL, 10);
   int sem_mem_size = sizeof(sem_t) * (seats + 1);
   
   // Create a new gpid based on pid   
   int gpidSuccess = setpgid(0,0);
  
   if (gpidSuccess != 0) {
   
      fprintf(stderr,"Error: GPID not set.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   }
   
   // open shared memory to store semaphores
   int shm_fd = shm_open("shared_memory", O_RDWR|O_CREAT,0666);
   
   if (shm_fd == -1) {
   
      fprintf(stderr,"Error: shared memory not opened.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   }   
   
   // set size of shared memory to hold N semaphores
   int ftruncateSuccess = ftruncate(shm_fd, sem_mem_size);
   
   if (ftruncateSuccess == -1) {
   
      fprintf(stderr,"Error: size of shared memory not set.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   }
   
   // map memory to fd and get ptr to mem loc    
   void *mem_ptr = mmap(0,sem_mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0); 
   
   if (mem_ptr == MAP_FAILED) {   
      
      fprintf(stderr,"Error: shared memory was not mapped.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   }
   
   // create semaphores   
   int i = 0;
   sem_t *sem_mem_loc = mem_ptr;
   
   while (i < seats + 1) {
            
      int initSuccess = sem_init(sem_mem_loc, 1, 1);
      
      if (initSuccess != 0) {   
      
         fprintf(stderr,"Error: Unnamed semaphore %d could not be initialized.\n", i);
         perror(sys_errlist[errno]);
         exit(-1);
      }
      
      sem_mem_loc = sem_mem_loc + sizeof(sem_t);
      i++;   
   }   
 
   // fork off philosopher children
      
   fprintf(stderr, "PPID: %d ",getpid());
      
   int pid;
   i = 1;
   
   while (i <= seats) {
      
      pid = fork();
      
      if (pid == 0) {
      
         philosophize(i, seats, mem_ptr);
      }
      
      i++;  
   }
     
   // wait for signal
   while (!stop) {
      pause();
   } 
   
   // kill child processes
   pid = getpid() * -1;
   int killSuccess = kill(pid, SIGTERM);
   if (killSuccess == -1) {
         
         fprintf(stderr,"Error: Children processes could not be killed.\n");
         perror(sys_errlist[errno]);     
   }  
      
   // destroy semaphores
   
   i = 0;
   sem_mem_loc = mem_ptr;
   
   while (i < seats + 1) {
            
      int destSuccess = sem_destroy(sem_mem_loc);
      
      if (destSuccess == -1) {   
      
         fprintf(stderr,"Error: Unnamed semaphore %d could not be destroyed.\n", i);
         perror(sys_errlist[errno]);         
      }
      
      sem_mem_loc = sem_mem_loc + sizeof(sem_t);
      i++;   
   }
       
   // unmap memory
   int munmapSuccess = munmap(mem_ptr, sem_mem_size);
   
   if (munmapSuccess == -1) {   
      
      fprintf(stderr,"Error: shared memory was not unmapped.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   }
   
   // unlink shared memory
   int unlinkSuccess = shm_unlink("shared_memory");
   
   if (unlinkSuccess == -1) {   
      
      fprintf(stderr,"Error: shared memory was not unlinked.\n");
      perror(sys_errlist[errno]);
      exit(-1);
   } 
   
   return 0;     
}
