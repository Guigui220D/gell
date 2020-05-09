#include "exec_prog.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int exec_prog(const char** argv)
{
    pid_t f = fork();

    if (f == -1)
    {
        perror("Forking for program execution");
    }

    if (f)
    {
        int status;

        if (waitpid(f, &status, 0) == -1)
        {
            perror("Waiting for program to finish");
            return -1;
        }

        return status;
    }
    else
    {
        if (execve(argv[0], (char**)argv, NULL) == -1)
            perror(argv[0]);

        return -1; //execve doesn't return on succes so we shouldn't reach that;
    }
}
