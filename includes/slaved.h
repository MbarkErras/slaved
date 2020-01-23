#ifndef SLAVED_H
# define SLAVED_H

# include "centropy.h"
# include "queue.h"
# include "packet_utilities.h"

typedef struct  s_slaved
{
    int         connection_socket;
    
    t_cdstruct  *tasks_queue;
}               t_slaved;


#endif