#include "master_middlewares.h"

int init_slaves(t_cluster *cluster)
{
    
}

int connect_slaves(t_cluster *cluster)
{
    int socket;
    struct sockaddr_in  slave;
    int i;
    int err;

    err = 0;
    i = -1;
    while (++i < cluster->size)
    {
        err = ERROR_WRAPPER((socket = socket(AF_INET, SOCK_STREAM, 0)) != -1);
        ft_bzero(&slave, sizeof(slave)); 
  
        slave.sin_family = AF_INET; 
        slave.sin_addr.s_addr = inet_addr("0.0.0.0"); 
        slave.sin_port = htons(PORT); 

        err = ERROR_WRAPPER(connect(socket, (struct sockaddr *)&slave, sizeof(slave)) != 0);
        if (err)
            DEBUG("error connecting node: %d\n", i);
        cluster->nodes[i]->socket = err ? -1 : socket;
    }

}

/* init_cluster()
**  - parsing configuration file.
**  - initaiting connection with slaves.
*/

int init_cluster(char *configuration_file, t_cluster *cluster)
{
    int err;

    err = 0;
    err = ERROR_WRAPPER(get_configuration(configuration_file, cluster) != 0);
    err = ERROR_WRAPPER(connect_slaves(cluster));
    err = ERROR_WRAPPER(init_slaves(cluster) != 0);
    return (err);
}

