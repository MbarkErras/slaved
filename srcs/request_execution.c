#include "slaved.h"

void    execute_program_request(t_slave *slave, t_request *request)
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVE_PROGRAM_NAME, O_CREAT | O_WRONLY)) == -1)
        return ;
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
        return ;
    slave->program.w_stdin = stdin_pipe[1];
    slave->program.r_stdout = stdout_pipe[0];
    write(fd, request->data, request->size);
    if (!(slave->program.pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        dup2(stdin_pipe[1], 1);
        if (execve(SLAVE_PROGRAM_NAME, NULL, NULL) == -1)
            exit (0);
    }
    else if (slave->program.pid == -1)
        return ;
    F_SET(slave->flags, PROGRAM_RECEIVED);
}

void    *execute_computation_request(t_slave *slave, t_request *request, size_t *response_size)
{
    void    *result;

    if (!F_GET(slave->flags, PROGRAM_RECEIVED))
        return (NULL);
    write(slave->program.w_stdin, request->data, request->size);
    sleep(1);
    read(slave->program.r_stdout, response_size, sizeof(size_t));
    result = malloc(sizeof(char) * *response_size);
    read(slave->program.r_stdout, result, *response_size);
    return (result);
}