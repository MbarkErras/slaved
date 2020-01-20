#ifndef T_RESPONSE_H
# define T_RESPONSE_H

/*** temporary include until making it modular ***/
#include "slaved.h"

typedef struct  s_request
{
    uint8_t     type;
    uint32_t    size;
    void        *data;
}               t_request;

# define TYPE_T_REQUEST_INIT 0
# define TYPE_T_REQUEST_COMPUTATION 1
# define TYPE_T_REQUEST_FINISH 2
# define TYPE_T_REQUEST_DROP 3

int write_response(int fd, t_response response);

#endif