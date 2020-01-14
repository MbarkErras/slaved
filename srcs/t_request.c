#include "slaved.h"

t_request *create_request(t_request value)
{
    t_request *request;

    if ((request = malloc(sizeof(t_request))))
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