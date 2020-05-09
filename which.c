#include "which.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "built_in_commands.h"

char* which(char const* name, int* type)
{
    *type = 0;

    if (find_built_in_command(name, NULL) >= 0)
    {
        *type = 2;
        return "Built-in command";
    }

    const char* env = getenv("PATH");
    const char* delim = ":";

    char env_copy[strlen(env) + 1];
    strcpy(env_copy, env);

    char *ptr = strtok(env_copy, delim);

	while(ptr != NULL)
	{
	    char* path = malloc(strlen(ptr) + strlen(name) + 2);
	    strcpy(path, ptr);
	    path[strlen(ptr)] = '/';
	    strcpy(path + strlen(ptr) + 1, name);

	    if (!access(path, F_OK | X_OK))
        {
            *type = 1;
            return path;
        }

        free(path);

		ptr = strtok(NULL, delim);
	}

	return "Name not found";
}
