#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <alloca.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "built_in_commands.h"
#include "exec_prog.h"
#include "which.h"

int run = 1;

void greetings();

void prompt();

void process_input(char* line, size_t len);

int main()
{
    errno = 0;

    greetings();

    char* line = malloc(0);
    size_t line_len = 0;

    size_t thing = 0;

    while (run)
    {
        prompt();

        line_len = getline(&line, &thing, stdin);

        if (line_len >= 0)
        {
            line[line_len - 1] = '\0';
            process_input(line, line_len);
        }
        else
        {
            perror("Getting user input");
        }
    }

    free (line);

    return 0;
}

void greetings()
{
    printf(
    "\n"
    "Gell (Guillaume's shell) 0.0.0.0.0.1\n"
    "Have fun !\n"
    "\n"
    );
}

void prompt()
{
    printf(" > ");
}

void process_input(char* line, size_t len)
{
    //Getting line words (args)
    int argc = 0;
    char* argv[50];

    {
        char* ptr = strtok(line, " ");

        while(ptr != NULL)
        {
            argv[argc++] = ptr;
            if (argc >= 50)
                return;
            ptr = strtok(NULL, " ");
        }
    }

    argv[argc] = NULL;

    assert(argc >= 1);

    int type;

    char* comment = which(argv[0], &type);

    switch (type)
    {
    case 1: //Executable
        argv[0] = comment;
        int return_code = exec_prog((const char**)argv);
        break;

    case 2: //Built-in command
        {
            int bic = find_built_in_command(argv[0], NULL);

            switch (bic)
            {
            case 0: //exit
                run = 0;
                break;
            case 1: //which
                if (argc >= 2)
                {
                    for (int i = 1; i < argc; i++)
                    {
                        int which_type = 0;
                        char* whichc = which(argv[i], &type);

                        printf("%s: %s\n", argv[i], whichc);

                        if (which_type == 1)
                            free(whichc);
                    }
                }
                else
                    printf("Please provide a name to check.\n");

                break;
            case 2: //cd
                if (argc >= 2)
                {
                    if (chdir(argv[1]) != 0)
                        perror("Couldn't change directory");
                }
                else
                    printf("Please provide a directory to go to.\n");
                break;
            }
        }
        break;

    default://Unknown command
        printf("%s: %s\n", argv[0], comment);
        break;
    }

}
