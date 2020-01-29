#ifndef MASTER_MIDDLEWARES_H
# define MASTER_MIDDLEWARES_H

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
# include "packet_utilities.h"
# include "lists_wrappers.h"
# include "queue.h"

# include "error_wrapper.h"

# include "_requests_handlers.h"

// /!\ D E V
# include <stdio.h>
# define DEBUG_P 1
# define DEBUG(x) printf(DEBUG_P ? x : "")
// /!\ D E V

# define BUFFER_SIZE 69
# define PORT 1337

typedef struct s_cluster t_cluster;

typedef struct	s_slave
{
	char		*ip;
	int			socket;
	t_dstruct_list	tasks_queue;
	t_cluster		*cluster;
}				t_slave;

typedef struct	s_computation
{
	t_dstruct_list	tasks_queue;
	t_dstruct_list	done_queue;
}				t_computation;

typedef struct	s_cluster
{
	t_slave		*nodes;
	t_slave		*least_used_slave;
	size_t		size;
	int			program;
	t_computation	computation;
}				t_cluster;

typedef struct	s_task
{
	t_packet	*request;
	t_packet	*response;
}				t_task;

int init_cluster(char *configuration_file, t_cluster *cluster);
void    init_computation(t_cluster *cluster);
void    queue_task(t_cluster *cluster, t_packet *request);
t_task  *create_task(t_task value);

/*
** CONFIGURATION UTILITITES
*/

# define BYTE_BLOCK_LOCK 0
# define BYTE_BLOCK 1
# define BYTE_BLOCK_OFFSET 2
# define DOTS 3

int         get_configuration(char *configuration_file, t_cluster *cluster);
char		*read_file(int fd, uint32_t *size);

/*
** CLUSTER LOADBALANCER
*/

t_slave *get_available_slave(t_cluster *cluster);

/*
** ERROR HANDLING
*/

# define BAD_CONFIG 1

#endif