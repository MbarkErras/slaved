#include "slaved.h"

/*
** execute_requests() routine runs in a thread and executes requests.
*/

void     *execute_requests(void *slave)
{
    size_t  response_size;
    void    *response;
    t_request   *request;

    while (1)
    {
        if (!CAST(slave, t_slave *)->requests_queue.size)
            continue ;
        else
        {
            request = CAST(CAST(slave, t_slave *)->requests_queue.head->content, t_request *);
            if (request->type == PROGRAM)
                execute_program_request(slave, request);
            else if (request->type == COMPUTATION)
            {
                response = execute_computation_request(slave, request, &response_size);
                write(CAST(slave, t_slave *)->connection_fd, &response_size, sizeof(response_size));
                write(CAST(slave, t_slave *)->connection_fd, response, response_size);
            }
            queue_dequeue(&CAST(slave, t_slave *)->requests_queue, destroy_request);
        }
    }
    return (NULL);
}

/*
** execute_requests() routine runs in a thread, it first reads request,
** deserialize it using specified middleware then enqueue the request to
** the request_queue.
*/

void    *receive_requests(void *slave)
{
    t_request   request;
    int         i;

    i = -1;
    while (1)
    {
        read(CAST(slave, t_slave *)->connection_fd, &request.type, sizeof(request.type));
        read(CAST(slave, t_slave *)->connection_fd, &request.size, sizeof(request.size));
        request.data = malloc(sizeof(char) * request.size);
        while ((size_t)++i < request.size)
            read(CAST(slave, t_slave *)->connection_fd, request.data + i, 1);
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
    slave_address.sin_family = AF_INET; 
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    slave_address.sin_port = htons(PORT);
    if ((bind(socket_fd, (struct sockaddr *)&slave_address, sizeof(slave_address))) != 0)
        return (CONNECTION_ERR);
    if (listen(socket_fd, LISTEN_QUEUE) != 0)
        return (CONNECTION_ERR);
    while (1)
    {
        if ((slave->connection_fd = accept(socket_fd, (struct sockaddr *)&master_address, &address_length)) != -1)
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