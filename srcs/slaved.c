#include "slaved.h"

void     *execute_requests(void *slave)
{
    t_request   *request;
    t_response  response;

    while (1)
    {
        if (!CAST(slave, t_slave *)->requests_queue.size)
            continue ;
        else
        {
            request = CAST(CAST(slave, t_slave *)->requests_queue.head->content, t_request *);
            response = requests_dispatcher(request);
            write_response(CAST(slave, t_slave *)->connection_fd, response);
            queue_dequeue(&CAST(slave, t_slave *)->requests_queue, destroy_request);
        }
    }
    return (NULL);
}

void    *receive_requests(void *slave)
{
    t_request   request;
    int         err;

    while (1)
    {
        if (read_request(CAST(slave, t_slave *)->connection_fd, &request))
            continue ;
        queue_enqueue(&CAST(slave, t_slave *)->requests_queue, 
            t_dstruct_create_node(create_request(request), sizeof(t_request)));
    }
    return (NULL);
}

int connect_master(t_slave *slave)
{
    int                 socket_fd;
    struct sockaddr_in  slave_address;
    struct sockaddr_in  master_address;
    socklen_t           address_length;
    pthread_t           tid;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return (CONNECTION_ERR);
    ft_bzero(&slave_address, sizeof(slave_address));
    ft_bzero(&master_address, sizeof(slave_address));
    slave_address.sin_family = AF_INET; 
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    slave_address.sin_port = htons(PORT);
    if ((bind(socket_fd, (struct sockaddr *)&slave_address, sizeof(slave_address))) != 0)
        return (CONNECTION_ERR);
    if (listen(socket_fd, LISTEN_QUEUE) != 0)
        return (CONNECTION_ERR);
    while (1)
    {
        if ((slave->connection_fd = accept(socket_fd, (struct sockaddr *)&master_address, &address_length)) == -1)
            return (CONNECTION_ERR);
        pthread_create(&tid, NULL, receive_requests, slave);
        pthread_create(&tid, NULL, execute_requests, slave);
    }
    close(socket_fd);
}

int main()
{
    t_slave slave;

    slave.requests_queue = t_dstruct_list_init();
    connect_master(&slave);
    return (0);
}

/* a todo:
    - freeing not used memory after executing requests and sending response.
    - cleanup functions for TYPE_T_REQUEST_FINISH/DROP.
    - protecting all syscalls and reporting errors.
    - creating raytracing program.
    - writing raytracing middlewares (de/serializers) for io.
    - deployment and testings.
*/