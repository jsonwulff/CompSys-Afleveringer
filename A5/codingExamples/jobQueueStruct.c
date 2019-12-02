struct job_queue {
  void** jobs;
  int capacity;
  int cnt;
  int init;
  pthread_mutex_t lock;
  pthread_cond_t job_empty;
  pthread_cond_t job_full;
};