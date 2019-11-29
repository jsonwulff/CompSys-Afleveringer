#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "job_queue.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t job_full = PTHREAD_COND_INITIALIZER;

int job_queue_init(struct job_queue *job_queue, int capacity) {
  // Check that job_queue isn't initialized
  if(job_queue->init != 1 ){
    void** jobs = malloc(sizeof(void *) * capacity);
    // Check that malloc didn't fail
    if(jobs != NULL) {
      job_queue->jobs = jobs;
      job_queue->capacity = capacity;
      job_queue->cnt = 0;
      job_queue->init = 1;
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
  //pthread_cond_broadcast(&job_empty);
  printf("destroy\n");
  job_queue->init=0;
  free(job_queue->jobs);
  return 0;
}


// REPORT POINT ABOUT FIFO OR STACK PRINCIPLE
int job_queue_push(struct job_queue *job_queue, void *data) {

    pthread_mutex_lock(&lock);
    while (job_queue->cnt == job_queue->capacity) {
      pthread_cond_wait(&job_empty, &lock);
    }
    // printf("job queue pushed\n");
    job_queue->cnt++;
    job_queue->jobs[job_queue->cnt] = data;
    pthread_cond_broadcast(&job_full);
    pthread_mutex_unlock(&lock);
    return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {

  pthread_mutex_lock(&lock);
  while (job_queue->cnt == 0) {
    pthread_cond_wait(&job_full, &lock);
  }
  // printf("job queue popped\n");
  *data = job_queue->jobs[job_queue->cnt];
  job_queue->cnt--;
  pthread_mutex_unlock(&lock);
  return 0;

}
