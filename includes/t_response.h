
#ifndef T_REQUEST_H
# define T_REQUEST_H

/*** temporary include until making it modular ***/
#include "slaved.h"

typedef struct  s_response
{
   uint8_t      type;
   uint32_t     size;
   void         *data;
}               t_response;

# define TYPE_T_RESPONSE_SUCCESS 0
# define TYPE_T_RESPONSE_UNRECOGNIZED 1
# define TYPE_T_RESPONSE_SYSTEM 2


t_request   *create_request(t_request value);
int         read_request(int fd, t_request *request);
void        destroy_request(void *request);

t_response  execute_req_init(t_slave *slave, t_request *request);
t_response  execute_req_computation(t_program program, t_request *request, size_t *response_size);
t_response  execute_finish_req();
t_response  execute_drop_req();

t_response  requests_dispatcher(t_request *request, t_slave *slave);

# define FLAG_RESPONSE(err) (t_response){err, 0, NULL}

#endif