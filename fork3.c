#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const char* pipe_open_fail = "pipe cannot be opened\n";
const char*      fork_fail = "fork failed\n";
const char*     write_fail = "writing to pipe failed\n";
const char*     close_fail = "pipe closing failed\n";
const char*   wc_exec_fail = "wc app execution failed\n";
const char*       dup_fail = "dup error\n";

int
main(int argc, char** argv) {
    int pfd[2]; 
    int pid;
    if (pipe(pfd) < 0)
        perror(pipe_open_fail);

    pid = fork();
    if (pid < 0)
        perror(fork_fail);

    else if (pid == 0) { 
        if (close(pfd[1]))
            perror(close_fail);

        if (close(0))
            perror(close_fail);

        if (dup(pfd[0]))
            perror(dup_fail);

        char * const args[] = {"/bin/wc", 0};

        if (execv("/bin/wc", args))
            perror(wc_exec_fail);
    }
    else { 
        if (close(pfd[0]))
            perror(close_fail);
        
        for (int i = 0; i < argc; ++i) {
            int len = strlen(argv[i]);
            argv[i][len - 1] = '\n';

            int written = 0;
            while (written < len) {
                int added = write(pfd[1], argv[i] + written, len - written);

                if (added < 0)
                    perror(write_fail);

                written += added;
            }
        }
     
        if (close(pfd[1]))
            perror(close_fail);

        wait(0);
        exit(0);
    }
}