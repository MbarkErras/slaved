#include "master_middlewares.h"
#include <errno.h>

int main(int argc, char **argv)
{
    t_cluster   cluster;
    int         fd;
    struct stat input_state;
    int         err;

    if (argc != 3)
        exit (1);

    if (stat(argv[1], &input_state) == -1)
        exit (2);

    if (init_cluster(argv[2], &cluster))
        exit (3);
    printf("success: init_cluster()..\n");




    while (1)
        continue;
}