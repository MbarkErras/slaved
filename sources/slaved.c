#include "slaved.h"

int     handle_packets(t_slaved *slaved)
{
    t_packet response;

    DEBUG("handling packets..\n");
    while (1)
    {
        if (!slaved->packets_queue.size)
            continue ;
        else
        {
            DEBUG("executing a packet..\n");
            response = requests_dispatcher(
                slaved->packets_queue.head->content, slaved);
            DEBUG("writing response..\n");
            write_packet(slaved->connection_socket, response);
            queue_dequeue(&slaved->packets_queue, destroy_packet);
        }
    }
    return (0);
}

void    receive_packets(t_slaved *slaved)
{
    t_packet request;


    DEBUG("receiving packets..\n");
    while (1)
    {
        DEBUG("reading a packet..\n");
        if (read_packet(slaved->connection_socket, &request))
        {
            list_delete(slaved->packets_queue);
            return ;
        }
        queue_enqueue(&slaved->packets_queue,
            t_dstruct_create_node(create_packet(request, NULL), sizeof(t_packet)));
    }

}

void    *connect_master(void *slaved)
{
    struct sockaddr_in  slave_address;
    struct sockaddr_in  master_address;
    socklen_t           address_length;

    if ((CAST(slaved, t_slaved *)->server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        pthread_exit(NULL);
    ft_bzero(&slave_address, sizeof(slave_address));
    ft_bzero(&master_address, sizeof(master_address));
    slave_address.sin_family = AF_INET;
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY);
    slave_address.sin_port = htons(PORT);
    if (bind(CAST(slaved, t_slaved *)->server_socket, (struct sockaddr *)&slave_address,
        sizeof(slave_address)) ||
        listen(CAST(slaved, t_slaved *)->server_socket, 1))
        pthread_exit(NULL);
    while (1)
    {
        if ((CAST(slaved, t_slaved *)->connection_socket =
            accept(CAST(slaved, t_slaved *)->server_socket, (struct sockaddr *)
            &master_address, &address_length)) == -1)
            pthread_exit(NULL);
        receive_packets(slaved);
        close(CAST(slaved, t_slaved *)->connection_socket);
    }
}

int main(void)
{
    t_slaved    slaved;
    pthread_t   tid;
    int         err;

    err = 0;
    slaved.packets_queue = t_dstruct_list_init();

    err = ERROR_WRAPPER(pthread_create(&tid, NULL, connect_master, &slaved));
    err = ERROR_WRAPPER(pthread_create(&tid, NULL, handle_packets, &slaved));

    return (err);
}