#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "job_queue.h"

// Relevant files about threading:
// 191108/
// sumbytes.c
// parcat.c
// goodcnt.c

// 191118:
// fibs-threadpool-spin.c
// fibs-threadpool.c
// fibs-mt.c

int job_queue_init(struct job_queue *job_queue, int capacity) {
  // Error handeling for init cheack that we dont intialize a queue that is already initialluzed
  // job_queue->jobs = malloc(sizeof(void *) * capacity);

  if(job_queue->init != 1 ){ // Check that job_queue isn't initialized
    void** jobs = malloc(sizeof(void *) * capacity);
    if(jobs != NULL) { // Check that malloc didn't fail
      printf("job queue init\n");
      job_queue->jobs = jobs;
      job_queue->capacity = capacity;
      job_queue->cnt = 0;
      job_queue->init = 1; // REPORT POINT
      return 0;
    } else {
      return 2;
    }
  } else {
    return 1;
  }
}

int job_queue_destroy(struct job_queue *job_queue) {
  while(job_queue->cnt != 0) {

  }
  printf("destroy\n");
  job_queue->init=0;
  free(job_queue->jobs);
  return 0;
}


// REPORT POINT ABOUT FIFO OR STACK PRINCIPLE
int job_queue_push(struct job_queue *job_queue, void *data) {
    // Does not take race condition into account and doesn't wait
    if (job_queue->cnt < job_queue->capacity) {
      printf("job queue pushed\n");
      job_queue->cnt++;
      job_queue->jobs[job_queue->cnt] = data;
      return 0;
    } else {
      return 1;
    }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  while (job_queue->cnt == 0) {
  }
  if (job_queue->cnt != 0) {
    printf("job queue popped\n");
    *data = job_queue->jobs[job_queue->cnt];
    job_queue->cnt--;
    return 0;
  } else {
    return 1;
  }

  // if (job_queue->init == -1){return -1;}
}
