#include "exec_prog.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdio.h>

#define MAX_FORKS 256

pid_t fork_pids[MAX_FORKS];
pid_t current_pid;

void sigchld_handler(int signal)
{
    int stat = -1;
    pid_t pid;

    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        if (pid == current_pid)
            current_pid = 0;

        for (int i = 0; i < MAX_FORKS; i++)
            if (fork_pids[i] == pid)
            {
                printf("Fork #%d terminated.\n", pid);
                fork_pids[i] = 0;
            }
    }
}

void sigint_handler(int signal)
{
    if (current_pid > 0)
        kill(current_pid, SIGINT);

    printf("\n");
}

void init_exec_manager()
{
    signal(SIGCHLD, sigchld_handler);

    for (int i = 0; i < MAX_FORKS; i++)
        fork_pids[i] = 0;
}

void end_exec_manager()
{
    for (int i = 0; i < MAX_FORKS; i++)
        if (fork_pids[i])
            kill(fork_pids[i], SIGKILL);
}

int exec_prog(const char** argv)
{
    current_pid = fork();

    if (current_pid == -1)
    {
        perror("Forking for program execution");
        return -1;
    }

    if (current_pid)
    {
        signal(SIGINT, sigint_handler);

        while (current_pid) { pause(); }

        signal(SIGINT, SIG_DFL);

        return 0;
    }
    else
    {
        if (execve(argv[0], (char**)argv, NULL) == -1)
            perror(argv[0]);

        return -1; //execve doesn't return on succes so we shouldn't reach that;
    }
}

int fork_prog(const char** argv)
{
    int id = -1;
    for (int i = 0; i < MAX_FORKS; i++)
        if (!fork_pids[i])
        {
            id = i;
            break;
        }

    if (id < 0)
    {
        printf("Reached max fork count, couldn't fork.\n");
        return -1;
    }

    current_pid = fork();

    if (current_pid == -1)
    {
        perror("Forking for program execution");
        return -1;
    }

    if (current_pid == 0)
    {
        if (execve(argv[0], (char**)argv, NULL) == -1)
            perror(argv[0]);

        return -1; //execve doesn't return on succes so we shouldn't reach that;
    }
    else
        fork_pids[id] = current_pid;

    return 0;
}
