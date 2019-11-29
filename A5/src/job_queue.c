#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "job_queue.h"

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
      pthread_mutex_init(&job_queue->lock, NULL);
      pthread_cond_init(&job_queue->job_empty, NULL);
      pthread_cond_init(&job_queue->job_full, NULL);
      return 0;
    } else {
      return 2;
    }
  } else {
    return 1;
  }
}

int job_queue_destroy(struct job_queue *job_queue) {
  pthread_mutex_lock(&job_queue->lock);
  while(job_queue->cnt != 0) {
    pthread_cond_wait(&job_queue->job_empty, &job_queue->lock);
  }
  job_queue->init=0;
  free(job_queue->jobs);
  pthread_cond_broadcast(&job_queue->job_full);
  pthread_mutex_unlock(&job_queue->lock);
  return 0;
}


// REPORT POINT ABOUT FIFO OR STACK PRINCIPLE
int job_queue_push(struct job_queue *job_queue, void *data) {
  pthread_mutex_lock(&job_queue->lock);
  while (job_queue->cnt == job_queue->capacity) {
    pthread_cond_wait(&job_queue->job_empty, &job_queue->lock);
    }
    job_queue->cnt++;
    job_queue->jobs[job_queue->cnt] = data;
    pthread_cond_signal(&job_queue->job_full);
    pthread_mutex_unlock(&job_queue->lock);
    return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  pthread_mutex_lock(&job_queue->lock);
  while (job_queue->cnt == 0 && job_queue->init == 1 ) {
    pthread_cond_wait(&job_queue->job_full, &job_queue->lock);
  }
  if (job_queue->init == 1){
    *data = job_queue->jobs[job_queue->cnt];
    job_queue->cnt--;
    pthread_cond_signal(&job_queue->job_empty);
    pthread_mutex_unlock(&job_queue->lock);
    return 0;
  } else {
    pthread_mutex_unlock(&job_queue->lock);
    return -1;
  }


}
