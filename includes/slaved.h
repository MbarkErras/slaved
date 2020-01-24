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

# define EXEC_NAME "slaved: "
# define SLAVED_PROGRAM_NAME "slaved_program"
# define PORT 1337

# include "centropy.h"
# include "lists_wrappers.h"
# include "queue.h"
# include "packet_utilities.h"
# include "error_wrapper.h"

# include "_requests_handlers.h"

typedef struct  s_program
{
    pid_t       pid;
    int         w_stdin;
    int         r_stdout;
}               t_program;

typedef struct  s_slaved
{
    int             connection_socket;
    t_dstruct_list  packets_queue;
    t_program       program;
    uint8_t         flags;
}               t_slaved;

t_packet  requests_dispatcher(t_packet *request, t_slaved *slaved);

/*
** SLAVED FLAGS
*/

# define F_SLAVE_PROGRAM_RECEIVED 0

# define F_GET(x, f) (x & (1 << f))
# define F_BGET(x, f) (x & f)
# define F_SET(x, f) (x |= (1 << f))
# define F_BSET(x, f) (x |= f)
# define F_UNSET(x, f) (x &= ~(1 << f))
# define F_BUNSET(x, f) (x &= ~f)

#endif