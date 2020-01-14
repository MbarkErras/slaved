#ifndef SLAVED_H
# define SLAVED_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/socket.h>
# include <netinet/in.h> 
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/time.h>

# include "centropy.h"
# include "queue.h"

# define PORT 1337
# define LISTEN_QUEUE 1

typedef struct  s_request
{
    int         type;
    size_t      size;
    void        *data;
}               t_request;

# define PROGRAM 0
void    execute_program_request(t_request *request);

# define COMPUTATION 1
void    *execute_computation_request(t_request *request, size_t *response_size);

typedef struct  s_slave
{
    int             connection_fd;
    t_dstruct_list  requests_queue;
    char            flags;
}               t_slave;

# define ACCEPT_CONNECTION 0

/*
** CONSTRUCTORS/DECONSTRUCTORS
*/

t_request *create_request(t_request value);
void    destroy_request(void *request);

/*
** ERROR HANDLING
*/

# define ERROR_WRAPPER(err, x) err ? err : x
# define BAD_CONFIG 1
# define CONNECTION_ERR 2

/*
** UTILITY MACROS
*/

# define CAST(v, t) ((t)v)

#endif