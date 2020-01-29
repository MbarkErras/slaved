#include "master_middlewares.h"

t_task  *create_task(t_task value)
{
    t_task *task;

    if (!(task = malloc(sizeof(t_task))))
        return (NULL);
    task->request = value.request;
    task->response = value.response;
    return (task);
}

void    init_computation(t_cluster *cluster)
{
    cluster->computation.tasks_queue = t_dstruct_list_init();
    cluster->computation.done_queue = t_dstruct_list_init();
    DEBUG("computation initiated!..\n");
}

void    queue_task(t_cluster *cluster, t_packet *request)
{
    queue_enqueue(&cluster->computation.tasks_queue,
        t_dstruct_create_node(create_task((t_task){request, NULL}), sizeof(t_task)));
}