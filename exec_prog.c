#include "exec_prog.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdio.h>

pid_t current_pid;

void sigint_handler(int signal)
{
    if (current_pid > 0)
        kill(current_pid, SIGINT);

    printf("\n");
}

int exec_prog(const char** argv)
{
    current_pid = fork();

    if (current_pid == -1)
    {
        perror("Forking for program execution");
    }

    if (current_pid)
    {
        signal(SIGINT, sigint_handler);

        int status;

        if (waitpid(current_pid, &status, 0) == -1)
        {
            perror("Waiting for program to finish");
            status = -1;
        }

        signal(SIGINT, SIG_DFL);
        return status;
    }
    else
    {
        if (execve(argv[0], (char**)argv, NULL) == -1)
            perror(argv[0]);

        return -1; //execve doesn't return on succes so we shouldn't reach that;
    }
}
