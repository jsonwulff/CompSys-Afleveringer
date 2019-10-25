#include "alloc.c"
#include "io.c"

// Compare and merge
inline void merge_run(long size_a, long size_b, long from[], long to[]) {
  long* p_a = from;
  long* p_a_limit = from + size_a;
  long* p_b = p_a_limit;
  long* p_b_limit = p_a_limit + size_b;
  long* target = to;
  long* target_limit = target + size_a + size_b;
  while (target < target_limit) {
    if (p_a < p_a_limit) {
      if (p_b < p_b_limit) {
        if (*p_a < *p_b) {
          *target = *p_a++;
        } else {
          *target = *p_b++;
        }
      } else {
        *target = *p_a++;
      }
    } else {
      *target = *p_b++;
    }
    ++target;
  }
}

void merge_sort(long total_size, long run_size, long from[], long to[]) {
  long run_size_a = run_size;
  long run_size_b = run_size;
  if (total_size < (run_size_a + run_size_b)) {
    run_size_b = total_size - run_size_a;
  }
  if (total_size < 3) {
    merge_run(run_size_a, 0, from, to);
    merge_run(0, run_size_b, from + run_size_a, to + run_size_a);
    long* tmp = from;
    from = to;
    to = tmp;
    merge_run(run_size_a, run_size_b, from, to);

  } else {
    merge_sort(run_size_a, (run_size_a + 1) / 2, from, to);
    merge_sort(run_size_b, (run_size_b + 1) / 2, from + run_size_a,
               to + run_size_a);
    long* tmp = from;
    from = to;
    to = tmp;
    merge_run(run_size_a, run_size_b, from, to);
  }
}

void run_merge_sort(long num_elem, long array[]) {
  long* array_a = array;
  long* array_b = allocate(num_elem);
  merge_sort(num_elem, (num_elem + 1) / 2, array_a, array_b);

  if (array_b != array) {  // results in wrong array, must copy back
    for (long k = 0; k < num_elem; ++k) array[k] = array_b[k];
  }
}

long* run() {
  init_allocator();
  // Read it the program should print
  // 0: Silent mode
  // 1: Print sorted list
  // 2: Get input from stdin
  // 3: Test mode (print + input)
  long mode = read_long();
  long is_printing = mode & 1;
  long get_input = mode & 2;
  // Read number of elements in array
  long num_entries = read_long();
  long* p;
  // Run the algorithm
  if (get_input) {
    p = read_array(num_entries);
  } else {
    p = get_random_array(num_entries);
  }

  // Run the algorithm
  run_merge_sort(num_entries, p);

  if (is_printing) {
    print_array(num_entries, p);
  }
  return p;  // <-- prevent optimization
}

// #include <stdio.h>

// int main() {
//   long a = 2;
//   long b = 3;
//   long retval = b / a;
//   printf("%lu, %ld", retval, retval);
// }