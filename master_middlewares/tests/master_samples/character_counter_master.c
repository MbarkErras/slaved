#include "master_middlewares.h"
#include <errno.h>

# define COMPUTATION_SIZE 1687552

char    *file_partial_read()
{

}

int main(int argc, char **argv)
{
    t_cluster   cluster;
    int         fd;
    struct stat input_state;
    int         err;
    int         i;

    if (argc != 3)
        exit (1);

    if (stat(argv[1], &input_state) == -1)
        exit (2);

    if (init_cluster(argv[2], &cluster))
        exit (3);
    printf("success: init_cluster()..\n");
    i = -1;
    int quota = COMPUTATION_SIZE / cluster.size;
    while (++i < cluster.size)
    {


    }
    while (1)
        continue;
}
