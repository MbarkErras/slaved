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
# define SLAVE_PROGRAM_NAME "slave_program"

typedef struct  s_program
{
    pid_t       pid;
    int         w_stdin;
    int         r_stdout;
}               t_program;

typedef struct  s_slave
{
    int             connection_fd;
    t_dstruct_list  requests_queue;
    t_program       program;
    char            flags;
}               t_slave;

typedef struct  s_request
{
    int         type;
    size_t      size;
    void        *data;
}               t_request; 

# define PROGRAM 0
void    execute_program_request(t_slave *slave, t_request *request);

# define COMPUTATION 1
void    *execute_computation_request(t_slave *slave, t_request *request, size_t *response_size);

# define PROGRAM_RECEIVED 0
# define ACCEPT_CONNECTION 1

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
# define F_GET(x, f) (x & (1 << f))
# define F_BGET(x, f) (x & f)
# define F_SET(x, f) (x |= (1 << f))
# define F_BSET(x, f) (x |= f)
# define F_UNSET(x, f) (x &= ~(1 << f))
# define F_BUNSET(x, f) (x &= ~f)

#endif