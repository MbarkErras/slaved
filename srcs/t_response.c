#include "t_response.h"

/*** protect write calls ***/
int write_response(int fd, t_response response)
{
    int err;

    err = 0;
    write(fd, &response.flags, sizeof(uint8_t));
    if (!response.flags)
    {
        write(fd, &response.size, sizeof(uint32_t));
        write (fd, response.data, response.size);
    }
    return (err);
}