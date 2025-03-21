#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char* pipe_open_fail = "pipe cannot be opened\n";
const char*      fork_fail = "fork failed\n";
const char*     write_fail = "writing to pipe failed\n";
const char*     close_fail = "pipe closing failed\n";
const char*   wc_exec_fail = "wc app execution failed\n";
const char*       dup_fail = "dup fail\n";

int  PIPE_FAIL = 1;
int  FORK_FAIL = 2;
int WRITE_FAIL = 3;
int CLOSE_FAIL = 4;
int    WC_FAIL = 5;
int   DUP_FAIL = 6;

int
main(int argc, char** argv) {
    int pfd[2]; 
    int pid;
    if (pipe(pfd) < 0)
        fprintf(2, "%s", pipe_open_fail), exit(PIPE_FAIL);

    pid = fork();
    if (pid < 0)
        fprintf(2, "%s", fork_fail), exit(FORK_FAIL);

    else if (pid == 0) { 
        if (close(pfd[1]))
            fprintf(2, "%s", close_fail), exit(CLOSE_FAIL);

        if (close(0))
            fprintf(2, "%s", close_fail), exit(CLOSE_FAIL);

        if (dup(pfd[0]))
            fprintf(2, "%s", dup_fail), exit(DUP_FAIL);

        char *args[] = {"/wc", 0};
        if (exec("/wc", args))
            fprintf(2, "%s", wc_exec_fail), close(pfd[0]), exit(WC_FAIL);
    }
    else { 
        if (close(pfd[0]))
            fprintf(2, "%s", close_fail), exit(CLOSE_FAIL);
        
        for (int i = 0; i < argc; ++i) {
            int len = strlen(argv[i]);
            argv[i][len - 1] = '\n';

            int written = 0;
            while (written < len) {
                int added = write(pfd[1], argv[i] + written, len - written);
                if (added < 0)
                    fprintf(2, "%s", write_fail),  exit(WRITE_FAIL);
                
                written += added;
            }   
        }
     
        if (close(pfd[1]))
            fprintf(2, "%s", close_fail),  exit(CLOSE_FAIL);

        wait(0);
        exit(0);
    }
}