#include "packet_utilities.h"

int read_packet(int fd, t_packet *packet)
{
    int err;

    err = 0;
    err = ERROR_WRAPPER(read(fd, &packet->type, sizeof(packet->type)) > 0);
    err = ERROR_WRAPPER(read(fd, &packet->size, sizeof(packet->size)));
    packet->data = malloc(sizeof(char) * packet->size);
    err = ERROR_WRAPPER(packet->data != NULL);
    err = ERROR_WRAPPER(read(fd, packet->data, packet->size) > 0);
    return (err);
}

int write_packet(int fd, t_packet packet)
{
    int err;

    err = 0;
    err = ERROR_WRAPPER(write(fd, &packet.type, sizeof(uint8_t)) != -1);
    if (!err && !packet.type)
    {
        err = ERROR_WRAPPER(write(fd, &packet.size, sizeof(uint32_t)) != -1);
        err = ERROR_WRAPPER(write(fd, packet.data, packet.size) != -1);
    }
    return (err);
}

t_packet *create_packet(t_packet value)
{
    t_packet *packet;

    if (!(packet = malloc(sizeof(t_packet))))
        return (NULL);
    packet->type = value.type;
    packet->size = value.size;
    packet->data = value.data;
    return (packet);
}

void    destroy_packet(void *packet)
{
    free(((t_packet *)packet)->data);
    free(packet);
}