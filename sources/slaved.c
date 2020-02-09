#include "slaved.h"

void     *handle_packets(void *slaved)
{
    t_packet response;
    t_packet *request;

    while (1)
    {
        pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx);
        if (!CAST(slaved, t_slaved *)->packets_queue.size)
        {
            pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
            continue ;
        }
        else
        {
            request = CAST(slaved, t_slaved *)->packets_queue.head->content;
            queue_dequeue(&CAST(slaved, t_slaved *)->packets_queue, NULL);
            pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
            response = requests_dispatcher(request, slaved);
            destroy_packet(request);
            if (request->type == FINISH_PACKET_TYPE)
                pthread_exit(NULL);
            if (write_packet(CAST(slaved, t_slaved *)->connection_socket, response) ||
                response.type == TYPE_T_RESPONSE_FAILURE)
            {
                pthread_cancel(CAST(slaved, t_slaved *)->tid[0]);
                pthread_exit(NULL);
            }
        }
    }
    pthread_exit(NULL);
}

void    *receive_packets(void *slaved)
{
    t_packet request;

    DEBUG("receiving packets..\n");
    while (1)
    {
        if (read_packet(CAST(slaved, t_slaved *)->connection_socket, &request))
            pthread_exit(NULL);
        pthread_mutex_lock(&CAST(slaved, t_slaved *)->packets_queue_mx);
        queue_enqueue(&CAST(slaved, t_slaved *)->packets_queue,
            t_dstruct_create_node(create_packet(request, NULL), sizeof(t_packet)));
        if (request.type == FINISH_PACKET_TYPE)
            pthread_exit(NULL);
        pthread_mutex_unlock(&CAST(slaved, t_slaved *)->packets_queue_mx);
    }
}

int connect_master(t_slaved *slaved)
{
    struct sockaddr_in  slave_address;
    struct sockaddr_in  master_address;
    socklen_t           address_length;
    int                 err;

    err = 0;
    if ((slaved->server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return (1);
    ft_bzero(&slave_address, sizeof(slave_address));
    ft_bzero(&master_address, sizeof(master_address));
    slave_address.sin_family = AF_INET;
    slave_address.sin_addr.s_addr = htonl(INADDR_ANY);
    slave_address.sin_port = htons(PORT);
    if (bind(slaved->server_socket, (struct sockaddr *)&slave_address,
        sizeof(slave_address)) ||
        listen(CAST(slaved, t_slaved *)->server_socket, 1))
        return (1);
    while (1)
    {
        if ((slaved->connection_socket =
            accept(slaved->server_socket, (struct sockaddr *)
            &master_address, &address_length)) == -1)
            return (1);
        err = ERROR_WRAPPER(pthread_create(&slaved->tid[0], NULL, receive_packets, slaved));
        err = ERROR_WRAPPER(pthread_create(&slaved->tid[1], NULL, handle_packets, slaved));
        if (err)
            return (1);
        if (!pthread_join(slaved->tid[0], NULL))
        {
            pthread_join(slaved->tid[1], NULL);
            cleanup(slaved->computation);
            list_delete(&slaved->packets_queue, destroy_packet);
        }
        close(CAST(slaved, t_slaved *)->connection_socket);
    }
}


void main(void)
{
    t_slaved    slaved;

    slaved.packets_queue = t_dstruct_list_init();
    slaved.computation = init_computation();
    pthread_mutex_init(&slaved.packets_queue_mx, NULL);

    if (connect_master(&slaved))
        ft_putendl_fd("master connection error!", 2);
}