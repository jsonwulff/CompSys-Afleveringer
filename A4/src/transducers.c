#include "transducers.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct stream {
  FILE *read_end;
  int flag;  // 1 for in-use, 0 for available
};

void transducers_free_stream(stream *s) {
  fclose(s->read_end);
  s->flag = 0;
  free(s);
}

static int file_pipe(FILE *files[2]) {
  int fds[2];
  int r = pipe(fds);

  if (r == 0) {
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");
    if (files[0] && files[1]) {
      return 0;
    } else {
      return 1;
    }

  } else {
    return r;
  }
}

int transducers_link_source(stream **out, transducers_source s,
                            const void *arg) {
  FILE *files[2];
  if(file_pipe(files) == 0) {
    pid_t pid = fork();

    if (pid == 0) {
      fclose(files[0]);
      s(arg, files[1]);
      fclose(files[1]);
      exit(0);
    } else {             // Parent
      fclose(files[1]);  // close write end
      struct stream *source_stream = malloc(sizeof(struct stream));
      source_stream->flag = 0;
      source_stream->read_end = files[0];

      *out = source_stream;
    }

    return 0;

  } else {
    return 1;
  }

}

int transducers_link_sink(transducers_sink s, void *arg, stream *in) {
  if (in->flag == 0) {
    in->flag = 1;
    s(arg, in->read_end);
    return 0;
  } else {
    return 1;
  }
}

int transducers_link_1(stream **out, transducers_1 t, const void *arg,
                       stream *in) {
  if(in->flag == 0) {
    FILE *files[2];
    if (file_pipe(files) == 0) {
      pid_t pid = fork();
      if(pid == 0) {
        fclose(files[0]);
        t(arg, files[1], in->read_end);
        fclose(files[1]);
        exit(0);
      } else {
        fclose(files[1]);
        struct stream *source_stream = malloc(sizeof(struct stream));
        source_stream->flag = 0;
        source_stream->read_end = files[0];
        *out = source_stream;
      }
      in->flag = 1;
      return 0;
    } else {
      return 1;
    }
  } else {
    return 1;
  }

}

int transducers_link_2(stream **out, transducers_2 t, const void *arg,
                       stream *in1, stream *in2) {
  if(in1->flag == 0 || in2->flag == 0) {
    FILE *files[2];
    if (file_pipe(files) == 0) {
      pid_t pid = fork();
      if (pid == 0) {
        fclose(files[0]);
        t(arg, files[1], in1->read_end, in2->read_end);
        fclose(files[1]);
        exit(0);
      } else {
        fclose(files[1]);
        struct stream *source_stream = malloc(sizeof(struct stream));
        source_stream->flag = 0;
        source_stream->read_end = files[0];
        *out = source_stream;
      }
      in1->flag = 1;
      in2->flag = 1;
      return 0;
    } else {
      return 1;
    }
  } else {
    return 1;
  }
}

int transducers_dup(stream **out1, stream **out2, stream *in) {
  if(in->flag == 0) {
    FILE *f1[2];
    FILE *f2[2];
    if (file_pipe(f1) == 0 && file_pipe(f2) == 0) {
      pid_t pid = fork();

      if (pid == 0) {
        fclose(f1[0]);
        fclose(f2[0]);

        char x;
        while (fread(&x, sizeof(char), 1, in->read_end) == 1) {
          char y = x;
          fwrite(&y, sizeof(char), 1, f1[1]);
          fwrite(&y, sizeof(char), 1, f2[1]);
        }
        fclose(f1[1]);
        fclose(f2[1]);
        exit(0);
      } else {
        fclose(f1[1]);
        fclose(f2[1]);
        struct stream *source_stream1 = malloc(sizeof(struct stream));
        source_stream1->flag = 0;
        source_stream1->read_end = f1[0];
        *out1 = source_stream1;
        struct stream *source_stream2 = malloc(sizeof(struct stream));
        source_stream2->flag = 0;
        source_stream2->read_end = f2[0];
        *out2 = source_stream2;
      }
      in->flag = 1;
      return 0;
    } else {
      return 1;
    }
  } else {
    return 1;
  }
}
