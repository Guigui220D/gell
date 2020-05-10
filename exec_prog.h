#ifndef EXEC_PROG_H
#define EXEC_PROG_H

void init_exec_manager();
void end_exec_manager();

int exec_prog(const char** argv);
int fork_prog(const char** argv);

#endif // EXEC_PROG_H

