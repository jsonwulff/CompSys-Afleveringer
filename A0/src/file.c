#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <errno.h>  // errno
#include <string.h> // strerror

enum file_type {
  DATA = 0,
  EMPTY = 1,
  ASCII = 2,
};

enum file_type cur_type = 1;

enum file_type FindType(){
  return cur_type;
}

const char * const FILE_TYPE_STRINGS[] = {
  "data",
  "empty",
  "ASCII text",
};

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
      // Read file from start to end 
      while (1) {
        char b;
        int read = fread(&b, 1, 1, f);

        // Stop loop when file is read to the end
        if (read == 0) {
          break;
        }

        if (b == 0 && FindType() != DATA && FindType() != ASCII){
          cur_type = EMPTY;
        } else if (((b >= 0x07 && b <= 0x0D) || b == 0x1B || (b >= 0x20 && b <= 0x7E)) && FindType() != DATA) {
          cur_type = ASCII;
        } else {
          cur_type = DATA;
          break;
        }
        // Check chars
      }
      
    } else {
      print_error(argv[1],errno);
      retval = EXIT_FAILURE;
    }
    
    if(FindType() == DATA) {
      fprintf(stdout,"%s: data", filename);
    }else if (FindType() == EMPTY)
    {
      fprintf (stdout,"%s: empty", filename);
    } else {
      fprintf (stdout,"%s: ASCII text", filename);
    }
    fprintf(stdout,"\n");

    fclose(f);

  }
  

  // if (print_hello_world() <= 0) {
  //   retval = EXIT_FAILURE;
  // }

  return retval;
}
