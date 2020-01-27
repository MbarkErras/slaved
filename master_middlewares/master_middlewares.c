#include "master_middlewares.h"

int	init_cluster(char *configuration_file, t_cluster *cluster)
{
	int err;

	err = get_configuration(configuration_file, cluster);
	init_connection(cluster);	
}