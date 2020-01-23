#ifndef PACKET_UTILITIES_H
# define PACKET_UTILITIES_H

# include <stdint.h>
# include <unistd.h>

# define "error_wrapper.h"

typedef struct  s_packet
{
   uint8_t      type;
   uint32_t     size;
   void         *data;
}               t_packet;

# define TYPE_T_REQUEST_INIT 0
# define TYPE_T_REQUEST_COMPUTATION 1
# define TYPE_T_REQUEST_FINISH 2
# define TYPE_T_REQUEST_DROP 3

# define TYPE_T_RESPONSE_SUCCESS 0
# define TYPE_T_RESPONSE_UNRECOGNIZED 1
# define TYPE_T_RESPONSE_SYSTEM 2

t_packet    *create_packet(t_packet value);
void        destroy_packet(void *packet);

/*
** write_packet() and read_packet() return 0 upon successful execution, 1 otherwise.
*/

int         write_packet(int fd, t_packet packet);
int         read_packet(int fd, t_packet *packet);

# define ERROR_RESPONSE_PACKET(err) (t_response){err, 0, NULL}

#endif