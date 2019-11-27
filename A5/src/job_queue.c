#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "job_queue.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_pushed = PTHREAD_COND_INITIALIZER;

int job_queue_init(struct job_queue *job_queue, int capacity) {
  if(job_queue->init != 1 ){ // Check that job_queue isn't initialized
    void** jobs = malloc(sizeof(void *) * capacity);
    if(jobs != NULL) { // Check that malloc didn't fail
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
  // printf("destroy\n");
  job_queue->init=0;
  free(job_queue->jobs);
  return 0;
}


// REPORT POINT ABOUT FIFO OR STACK PRINCIPLE
int job_queue_push(struct job_queue *job_queue, void *data) {
    // Does not take race condition into account and doesn't wait
    if (job_queue->cnt < job_queue->capacity) {
      // printf("job queue pushed\n");
      job_queue->cnt++;
      job_queue->jobs[job_queue->cnt] = data;
      pthread_cond_broadcast(&job_pushed);
      return 0;
    } else {
      return 1;
    }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {

  pthread_mutex_lock(&lock);
  if (job_queue->cnt == 0) {
    pthread_cond_wait(&job_pushed, &lock);
  }
  // printf("job queue popped\n");
  *data = job_queue->jobs[job_queue->cnt];
  job_queue->cnt--;
  pthread_mutex_unlock(&lock);
  return 0;

}
