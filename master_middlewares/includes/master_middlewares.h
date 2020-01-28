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

# include "error_wrapper.h"

// /!\ D E V
# include <stdio.h>
# define DEBUG_P 1
# define DEBUG(x) printf(DEBUG_P ? x : "")
// /!\ D E V

# define BUFFER_SIZE 69
# define PORT 1337

typedef struct	s_slave
{
	char		*ip;
	int			socket;
	t_dstruct_list	tasks_queue;
}				t_slave;

typedef struct	s_cluster
{
	t_slave		**nodes;
	size_t		size;
	int			program;
}				t_cluster;

typedef struct	s_task
{
	void		*input;
	void		*output;
	int			flags;
}				t_task;

typedef struct	s_computation
{
	t_dstruct_list	tasks_queue;
	int			done;
}				t_computation;

/*
** CONFIGURATION UTILITITES
*/

# define BYTE_BLOCK_LOCK 0
# define BYTE_BLOCK 1
# define BYTE_BLOCK_OFFSET 2
# define DOTS 3

int         get_configuration(char *configuration_file, t_cluster *cluster);

/*
** ERROR HANDLING
*/

# define BAD_CONFIG 1

#endif