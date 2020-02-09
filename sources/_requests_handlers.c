#include "slaved.h"

int     write_program(int fd, void *data, int size)
{
    int ret;
    int remaining;

    remaining = size;
    while (1)
    {
        if (!remaining)
            break ;
        if ((ret = write(fd, data, remaining)) == -1)
            return (1);
        remaining -= ret;
    }
    return (0);
}

t_packet  execute_req_init(t_packet *request, t_program *program)
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY, S_IWUSR)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    slaved->program.w_stdin = stdin_pipe[1];
    slaved->program.r_stdout = stdout_pipe[0];
    if (write_program(fd, request->data, request->size))
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (!(slaved->program.pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        close(stdin_pipe[0]);
        dup2(stdout_pipe[1], 1);
        close(stdout_pipe[0]);
        if (execv(SLAVED_PROGRAM_NAME, (char *[2]){SLAVED_PROGRAM_NAME, NULL}) == -1)
            exit (0);
    }
    else if (slaved->program.pid == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    F_SET(slaved->flags, F_SLAVE_PROGRAM_RECEIVED);
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet  execute_req_computation(t_packet *request, t_program *program)
{
    t_packet response;
    void    *result;
    int ret;

    ret = write(program.w_stdin, &request->size, sizeof(request->size));
    ret = write(program.w_stdin, request->data, request->size);
    ret = read(program.r_stdout, &response.size, sizeof(uint32_t));
    response.data = malloc(sizeof(char) * response.size);
    ret = read(program.r_stdout, response.data, response.size);
    response.type = TYPE_T_RESPONSE_SUCCESS;
    return (response);
}