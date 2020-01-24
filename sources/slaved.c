#include "slaved.h"

void    *handle_packets(void *slaved)
{
    t_packet response;

    while (1)
    {
        if (!CAST(slaved, t_slaved *)->packets_queue.size)
            continue ;
        else
        {
            response = requests_dispatcher(
                CAST(slaved, t_slaved *)->packets_queue.head->content, slaved);
            write_packet(CAST(slaved, t_slaved *)->connection_socket, response);
            queue_dequeue(&CAST(slaved, t_slaved *)->packets_queue, destroy_packet);
        }
    }
    return (NULL);
}

void    receive_packets(t_slaved *slaved)
{
    t_packet request;

    while (1)
    {
        if (read_packet(slaved->connection_socket, &request))
            continue ;
        queue_enqueue(&slaved->packets_queue, 
            t_dstruct_create_node(create_packet(request), sizeof(t_packet)));
    }

}

int connect_master(t_slaved *slaved)
{
    int                 socket_fd;
    struct sockaddr_in  slave_address;
    struct sockaddr_in  master_address;
    socklen_t           address_length;
    pthread_t           tid;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return (1);
    ft_bzero(&slave_address, sizeof(slave_address));
    ft_bzero(&master_address, sizeof(slave_address));
    slave_address.sin_family = AF_INET; 
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    slave_address.sin_port = htons(PORT);
    if ((bind(socket_fd, (struct sockaddr *)&slave_address, sizeof(slave_address))) != 0)
        return (1);
    if (listen(socket_fd, 1) != 0)
        return (1);
    while (1)
    {
        if ((slaved->connection_socket =
            accept(socket_fd, (struct sockaddr *)&master_address, &address_length)) == -1)
            return (1);
        receive_packets(slaved);
    }
    close(socket_fd);
}

int main(void)
{
    t_slaved    slaved;
    pthread_t   tid;

    slaved.packets_queue = t_dstruct_list_init();
    pthread_create(&tid, NULL, handle_packets, &slaved);
    if (connect_master(&slaved))
        return (ft_perror(EXEC_NAME, NULL, CONNECTION_ERROR));

}