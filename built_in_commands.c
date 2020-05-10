#include "built_in_commands.h"

#include <stddef.h>

char const * const built_in_commands[] =
{
    "exit",
    "which",
    "cd",
    "pwd"
};

int find_built_in_command(char const * name, char const * * most_fitting_name)
{
    size_t best_match = 0;

    for (size_t i = 0; i < sizeof(built_in_commands) / sizeof(char*); i++)
    {
        size_t j = 0;
        size_t match = 0;

        while (built_in_commands[i][j] == name[j])
        {
            if (!built_in_commands[i][j])
            {
                if (!name[j])
                {
                    if (most_fitting_name)
                        *most_fitting_name = built_in_commands[i];
                    return i;
                }
                else
                    break;
            }

            if (!name[j])
                break;

            match++;
            j++;
        }

        if (match > best_match)
        {
            best_match = match;
            if (most_fitting_name)
                *most_fitting_name = built_in_commands[i];
        }
    }

    return -1;
}
