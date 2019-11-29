struct job_queue {
  void** jobs;
  int capacity;
  int cnt;
  int init;
};