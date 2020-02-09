
/*
** basically, a cleanup() function should be provided,
** and a requests_dispatcher() based on how many requests type you have.
*/

#include "slaved.h"
#include 

void    *init_computation()
{
    return (malloc(sizeof(t_program)));
}

void    cleanup(void *computation)
{
    kill(SIGKILL, CAST(computation, t_program *)->pid);
    close(CAST(computation, t_program *)->w_stdin);
    close(CAST(computation, t_program *)->r_stdout);
    free(computation);
}

t_packet  requests_dispatcher(t_packet *request, void *program)
{
    if (request->type == TYPE_T_REQUEST_INIT)
        return (execute_req_init(request, program));
    if (request->type == TYPE_T_REQUEST_COMPUTATION)
        return (execute_req_computation(request, program));
    return ((t_packet){NULL, 0, 0, NULL});
}