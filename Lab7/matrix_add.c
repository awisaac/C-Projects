// COMP 282 Lab 7 matrix_add.c
// Andrew Isaac 11/15/14

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void matrix_add(int *current, int size, int scalar) {
   
   int i;
   
   for (i = 0; i < size; i++) {      
      current[i] += scalar;   
   }
}

int main(int argc, char **argv) {
   
   if (argc < 3) {
   
      fprintf(stderr,"Two arguments required.\n");
      exit(-1);
   }
   
   srand(time(NULL));      
   
   int i, j;
   int size = atoi(argv[1]); // side length of matrix
   size *= size; // square side to get total size
   int blocks = atoi(argv[2]); // number of blocks
   
   if (size % blocks != 0) {
   
      fprintf(stderr,"Matrix size can not be evenly divided into %d blocks.\n",blocks);
      exit(-1); 
   }
   
   
   int block_size = size / blocks; // size of a block
   int scalar = rand() % 200 - 100; // random scalar
   
   int *next = malloc(sizeof(int) * block_size); // read block 
   int *current = malloc(sizeof(int) * block_size); // add block 
   int *prev = malloc(sizeof(int) * block_size); // write block
   
   struct aiocb async_cb_r;
      
      async_cb_r.aio_fildes = STDIN_FILENO;
      async_cb_r.aio_buf = next;
      async_cb_r.aio_nbytes = sizeof(int) * block_size;
      async_cb_r.aio_reqprio = 0;
      
   struct aiocb async_cb_w;
      
      async_cb_w.aio_fildes = STDOUT_FILENO;
      async_cb_w.aio_buf = prev;
      async_cb_w.aio_nbytes = sizeof(int) * block_size;
      async_cb_w.aio_reqprio = 0;
      
   int start_time = time(NULL); // start time   
   
   // prime the pump
   async_cb_r.aio_offset = 0;
   aio_read(&async_cb_r);
   while (aio_error(&async_cb_r) == EINPROGRESS) {} // complete the first async request     
   aio_return(&async_cb_r);         
   memcpy(current, next, sizeof(int) * block_size); // copy next to current
             
   for (i = 1; i < blocks; i++) {
      
      async_cb_r.aio_offset = i * sizeof(int) * block_size;
      aio_read(&async_cb_r); // read stdin to next    
                 
      matrix_add(current, block_size, scalar); // do add on current while reading next 
      memcpy(prev, current, sizeof(int) * block_size);
                  
      async_cb_w.aio_offset = (i - 1) * sizeof(int) * block_size;                             
      aio_write(&async_cb_w);      
      while (aio_error(&async_cb_w) == EINPROGRESS) {}
      aio_return(&async_cb_w); // wait until write to prev is complete

      while (aio_error(&async_cb_r) == EINPROGRESS) {}
      aio_return(&async_cb_r); // wait until read is complete
      
      memcpy(current, next, sizeof(int) * block_size); // copy next to current                                 
   }   
   
   matrix_add(current, block_size, scalar); // do final add   
   memcpy(prev, current, sizeof(int) * block_size);
   
   async_cb_w.aio_offset = (i - 1) * sizeof(int) * block_size;                             
   aio_write(&async_cb_w);      
   while (aio_error(&async_cb_w) == EINPROGRESS) {}
   aio_return(&async_cb_w); // last write complete   
   
   int end_time = time(NULL); // end time
   
   fprintf(stderr,"Start time: %d, End time: %d, Total time: %d\n", start_time, end_time, end_time - start_time);
   
   free(next);
   free(current);
   free(prev);       
   return(0);
}