#ifndef MASTER_MIDDLEWARES_H
# define MASTER_MIDDLEWARES_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

# define BUFFER_SIZE 69

typedef struct	s_slave
{
	char		ip[16];
	int			socket;
}				t_slave;

typedef struct	s_cluster
{
	t_slave		**nodes;
	size_t		size;
	int			program;
}				t_cluster;

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