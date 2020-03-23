#include "aflsmart.h"

void update_input_structure(const string& filename, SmartInput* si) {

  pid_t pid = 0;
  int pipefd[2];
  FILE* output;
  char line[256];
  int status;
  u8* ifname;
  u8* ofname;

  if (pipe(pipefd) < 0) {
		cerr << "AFLSmart cannot create a pipe to communicate with Peach\n";
    abort();
  }

  pid = fork();
  if (pid == 0) {

    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    dup2(pipefd[1], STDERR_FILENO);
    ifname = alloc_printf("-inputFilePath=%s", fname);
    ofname = alloc_printf("-outputFilePath=%s/chunks/%s.repaired", out_dir,
                          basename(fname));
    execlp("peach", "peach", "-1", ifname, ofname, input_model_file, (char*) NULL);
    abort();

  } else {

    close(pipefd[1]);
    output = fdopen(pipefd[0], "r");

    while (fgets(line, sizeof(line), output)) {
      /* Extract validity percentage and update the current queue entry. */
      si->validity = 0;
      if (!strncmp(line, "ok", 2)) {
        si->validity = 100;
        break;
      } else if (!strncmp(line, "error", 5)) {
        char *s = line + 5;
        while (isspace(*s)) { s++; }
        char *start = s;
        while (isdigit(*s)) { s++; }
        *s = '\0';
        if (s != start) {
          si->validity = (u8) atoi(start);
        }
        break;
      }
    }

    waitpid(pid, &status, 0);

    string chunks_fname = alloc_printf("%s/chunks/%s.repaired.chunks", out_dir, basename(fname));
    struct chunk *chunk;

    get_chunks(chunks_fname, &chunk);
    si->chunk = chunk;
    si->cached_chunk = copy_chunks(chunk);

    fclose(output);
    delete chunks_fname;

  }

}
