#include "master_middlewares.h"

int init_slaves(t_cluster *cluster)
{
    t_packet    request;
    int         err;

    err = 0;
    request.type = TYPE_T_REQUEST_INIT;
    //err = ERROR_WRAPPER((request.data = read_file(cluster->program, &request.size) != NULL);
    if (err)
        queue_enqueue(&cluster->computation.tasks_queue,
            t_dstruct_create_node(create_packet(request), sizeof(t_packet)));
    return (err);
}

/*
** slave_routine()
**  - loop over tasks queue and send requests.
**  - wait for response:
**      - if successful, update task accordingly.
**      - if failed, enqueue task again to the global task queue. 
*/

void    *slave_routine(void *slave)
{
    t_dstruct_node *task;

    while (1)
    {
        if (!CAST(slave, t_slave *)->tasks_queue.size)
            continue ;
        task = CAST(slave, t_slave *)->tasks_queue.head;
        write_packet(CAST(slave, t_slave *)->socket, *CAST(task->content, t_task *)->request);
        read_packet(CAST(slave, t_slave *)->socket, CAST(task->content, t_task *)->request);
        if (CAST(task->content, t_task *)->response->type == TYPE_T_RESPONSE_SUCCESS)
            queue_enqueue(&CAST(slave, t_slave *)->cluster->computation.done_queue, task);
        else
            queue_enqueue(&CAST(slave, t_slave *)->cluster->computation.tasks_queue, task);
        queue_dequeue(&CAST(slave, t_slave *)->tasks_queue, NULL);
        if (CAST(slave, t_slave *)->tasks_queue.size <
            CAST(slave, t_slave *)->cluster->least_used_slave->tasks_queue.size)
            CAST(slave, t_slave *)->cluster->least_used_slave = slave;
    }
}

/*
** connect_slaves()
**  - iterate over cluster slaves, and lunch a thread for every slave.
*/

int connect_slaves(t_cluster *cluster)
{
    int slave_socket;
    struct sockaddr_in  slave;
    int i;
    int err;
    pthread_t tid;

    err = 0;
    i = -1;
    while (++i < cluster->size)
    {
        err = ERROR_WRAPPER((slave_socket = socket(AF_INET, SOCK_STREAM, 0)) != -1);
        ft_bzero(&slave, sizeof(slave)); 
  
        slave.sin_family = AF_INET; 
        slave.sin_addr.s_addr = inet_addr("0.0.0.0"); 
        slave.sin_port = htons(PORT); 

        err = ERROR_WRAPPER(connect(slave_socket, (struct sockaddr *)&slave, sizeof(slave)) != 0);
        if (err)
        {
            DEBUG("error connecting a node..\n");
            continue ;
        }
        err = ERROR_WRAPPER(pthread_create(&tid, NULL, slave_routine, cluster->nodes[i]));
        cluster->nodes[i]->socket = err ? -1 : slave_socket;
    }
    return (0);
}

/*
** cluster_loadbalancer()
**
*/

void    *cluster_loadbalancer(void *cluster)
{
    t_dstruct_node    *task;
    int         i;

    while (1)
    {
        if (!CAST(cluster, t_cluster *)->computation.tasks_queue.size)
            continue ;
        task = CAST(cluster, t_cluster *)->computation.tasks_queue.head;
        if (CAST(task, t_task *)->request->type == TYPE_T_REQUEST_INIT)
        {
            i = -1;
            while (++i < CAST(cluster, t_cluster *)->size)
                queue_enqueue(&CAST(cluster, t_cluster *)->nodes[i]->tasks_queue, task);
        }
        else
            queue_enqueue(&CAST(cluster, t_cluster *)->least_used_slave->tasks_queue, task);
    }
}

/* init_cluster()
**  - parsing configuration file.
**  - initaiting connection with slaves.
*/

int init_cluster(char *configuration_file, t_cluster *cluster)
{
    int err;
    pthread_t   tid;

    err = 0;
    DEBUG("parsing configuration..\n");
    err = ERROR_WRAPPER(get_configuration(configuration_file, cluster) != 0);
    if (!cluster->size)
        return (1);
    DEBUG("connecting slaves..\n");
    err = ERROR_WRAPPER(connect_slaves(cluster));
    /*
    DEBUG("initiating computation..\n");
    if (!err)
        init_computation(cluster);
    DEBUG("initiating slaves..\n");
    err = ERROR_WRAPPER(init_slaves(cluster) != 0);
    DEBUG("running loadbalnacer routine..\n");
    err = ERROR_WRAPPER(pthread_create(&tid, NULL, cluster_loadbalancer, cluster));
    */
    return (err);
}

