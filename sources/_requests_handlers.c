#include "slaved.h"

/*** check if child process is healthy and report a response ***/
t_packet  execute_req_init(t_slaved *slaved, t_packet *request)
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SYSTEM));
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SYSTEM));
    slaved->program.w_stdin = stdin_pipe[1];
    slaved->program.r_stdout = stdout_pipe[0];
    write(fd, request->data, request->size);
    if (!(slaved->program.pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        close(stdin_pipe[0]);
        dup2(stdin_pipe[1], 1);
        close(stdin_pipe[1]);
        if (execve(SLAVED_PROGRAM_NAME, NULL, NULL) == -1)
            exit (0);
    }
    else if (slaved->program.pid == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SYSTEM));
    F_SET(slaved->flags, F_SLAVE_PROGRAM_RECEIVED);
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

/*** protect sys calls ***/
t_packet  execute_req_computation(t_packet *request, t_program program)
{
    t_packet response;
    void    *result;

    write(program.w_stdin, request->data, request->size);
    read(program.r_stdout, &response.size, sizeof(uint32_t));
    response.data = malloc(sizeof(char) * response.size);
    read(program.r_stdout, response.data, response.size);
    return (response);
}

t_packet  execute_req_finish()
{
    t_packet response;

    return (response);
}

t_packet  execute_req_drop()
{
    t_packet response;

    return (response);
}

t_packet  requests_dispatcher(t_packet *request, t_slaved *slaved)
{
    if (request->type == TYPE_T_REQUEST_INIT)
        return (execute_req_init(slaved, request));
    if (request->type == TYPE_T_REQUEST_COMPUTATION)
        return (execute_req_computation(request, slaved->program));
    if (request->type == TYPE_T_REQUEST_FINISH)
        return (execute_req_finish());
    if (request->type == TYPE_T_REQUEST_DROP)
        return (execute_req_drop());
    return ((FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_UNRECOGNIZED)));
}