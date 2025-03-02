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

int  PIPE_FAIL = 1;
int  FORK_FAIL = 2;
int WRITE_FAIL = 3;
int CLOSE_FAIL = 4;
int    WC_FAIL = 5;

int
main(int argc, char** argv) {
    int pfd[2]; 
    int pid;
    if (pipe(pfd) < 0)
        fprintf(stderr, "%s", pipe_open_fail), exit(PIPE_FAIL);

    pid = fork();
    if (pid < 0)
        fprintf(stderr, "%s", fork_fail), exit(FORK_FAIL);

    else if (pid == 0) { 
        close(pfd[1]);
        close(0);
        dup(pfd[0]);

        char * const args[] = {"/bin/wc", 0};

        if (execv("/bin/wc", args))
            fprintf(stderr, "%s", wc_exec_fail), close(pfd[0]), exit(WC_FAIL);
    }
    else { 
        close(pfd[0]);
        
        for (int i = 0; i < argc; ++i) {
            int len = strlen(argv[i]);
            argv[i][len - 1] = '\n';

            if (len != write(pfd[1], argv[i], len))
                fprintf(stderr, "%s", write_fail),  exit(WRITE_FAIL);
        }
     
        int ret = close(pfd[1]);

        if (ret < 0)
            fprintf(stderr, "%s", close_fail),  exit(CLOSE_FAIL);
        else
            wait(0);

        exit(0);
    }
}