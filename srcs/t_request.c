#include "t_request.h"

/*** protect read calls ***/
int read_request(int fd, t_request *request)
{
    int i;

    if (read(fd, &request->type, sizeof(request->type)) < 1)
        return (1);
    read(fd, &request->size, sizeof(request->size));
    request->data = malloc(sizeof(char) * request->size);
    read(fd, request->data, request->size);
}

t_request *create_request(t_request value)
{
    t_request *request;

    if (!(request = malloc(sizeof(t_request))))
        return (NULL);
    request->type = value.type;
    request->size = value.size;
    request->data = value.data;
    return (request);
}

void    destroy_request(void *request)
{
    free(CAST(request, t_request *)->data);
    free(request);
}