#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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
  assert(0);
}

int job_queue_destroy(struct job_queue *job_queue) {
  assert(0);
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  assert(0);
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(0);
}
