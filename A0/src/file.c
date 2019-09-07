#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <errno.h>  // errno
#include <string.h> // strerror

int print_hello_world() {
  return fprintf(stdout, "Hello, world!\n");
}

int print_error(char *path, int errnum) {
  return fprintf(stdout, "%s: Cannot determine (%s)\n", path, strerror(errnum));
}

int main(int argc, char* argv[]) {
  int retval = EXIT_SUCCESS;

  // Check correct number of arguments
  if (argc == 1){
    printf("Usage: file path\n");
    retval = EXIT_FAILURE;
  } else if (argc > 2) {
    printf("To many arguments given\n");
    retval = EXIT_FAILURE;
  } else {

    char* filename = argv[1];

    FILE* f = fopen(filename, "r");

    // Check that file exist
    if (f != NULL) {
      printf("Opening:%s\n", argv[1]);
      
      // Read file from start to end 
      while (1) {
        char b;
        int read = fread(&b, 1, 1, f);

        // Stop loop when file is read to the end
        if (read == 0) {
          break;
        }

        // Check chars
      }
      
    } else {
      print_error(argv[1],errno);
      retval = EXIT_FAILURE;
    }

    fclose(f);

  }
  

  // if (print_hello_world() <= 0) {
  //   retval = EXIT_FAILURE;
  // }

  return retval;
}
