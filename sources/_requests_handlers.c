#include "slaved.h"

int     write_program(int fd, void *data, int size)
{
    int ret;
    int remaining;

    remaining = size;
    while (1)
    {
        if (!remaining)
            break ;
        if ((ret = write(fd, data, remaining)) == -1)
            return (1);
        remaining -= ret;
    }
    return (0);
}

/*
t_packet  execute_req_init(t_packet *request, t_program *program)
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY, S_IWUSR)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    slaved->program.w_stdin = stdin_pipe[1];
    slaved->program.r_stdout = stdout_pipe[0];
    if (write_program(fd, request->data, request->size))
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (!(slaved->program.pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        close(stdin_pipe[0]);
        dup2(stdout_pipe[1], 1);
        close(stdout_pipe[0]);
        if (execv(SLAVED_PROGRAM_NAME, (char *[2]){SLAVED_PROGRAM_NAME, NULL}) == -1)
            exit (0);
    }
    else if (slaved->program.pid == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    F_SET(slaved->flags, F_SLAVE_PROGRAM_RECEIVED);
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet  execute_req_computation(t_packet *request, t_program *program)
{
    t_packet response;
    void    *result;
    int ret;

    ret = write(program.w_stdin, &request->size, sizeof(request->size));
    ret = write(program.w_stdin, request->data, request->size);
    ret = read(program.r_stdout, &response.size, sizeof(uint32_t));
    response.data = malloc(sizeof(char) * response.size);
    ret = read(program.r_stdout, response.data, response.size);
    response.type = TYPE_T_RESPONSE_SUCCESS;
    return (response);
}
*/

t_packet    execute_req_init(t_packet *request, t_program *program)
{
    int fd;
    int stdin_pipe[2];
    int stdout_pipe[2];

    if ((fd = open(SLAVED_PROGRAM_NAME, O_CREAT | O_WRONLY, S_IWUSR)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (mkdir(TEXTURES_DIR, 0644) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    program->w_stdin = stdin_pipe[1];
    program->r_stdout = stdout_pipe[0];
    if (write_program(fd, request->data, request->size))
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    if (!(program->pid = fork()))
    {
        dup2(stdin_pipe[0], 0);
        close(stdin_pipe[0]);
        dup2(stdout_pipe[1], 1);
        close(stdout_pipe[0]);
        if (execv(SLAVED_PROGRAM_NAME, (char *[2]){SLAVED_PROGRAM_NAME, NULL}) == -1)
            exit (0);
    }
    else if (program->pid == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_scene(t_packet *request, t_program *program)
{
    int ret;

    ret = 0;
    ret = write(program->w_stdin, &request->data, request->size);
    ret = write(program->w_stdin, "?", 1);
    if (ret)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_texture(t_packet *request, t_program *program)
{
    int ret;
    int     texture_fd;
    char    texture_filename[TEXTURES_FILENAME_SIZE + 2];
    char    *texture_path;

    ft_bzero(texture_filename, TEXTURES_FILENAME_SIZE + 2);
    texture_filename[0] = '/';

    ret = 0;
    ret = write(request->data, texture_filename + 1, TEXTURES_FILENAME_SIZE);
    texture_path = ft_strjoin_free(TEXTURES_DIR, texture_filename);
    if ((texture_fd = open(texture_path, 0644)) == -1)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    ret = write(texture_fd, request->data + TEXTURES_FILENAME_SIZE,
        request->size - TEXTURES_FILENAME_SIZE);
    if (ret)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}

t_packet    execute_req_computation(t_packet *request, t_program *program)
{
    t_packet    response;
    int         ret;

    ret = 0;
    ret = write(program->w_stdin, request->data, request->size);
    ret = read(program->r_stdout, &response.size, sizeof(response.size));
    if (!ret && !(response.data = malloc(response.size)))
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    ret = read(program->r_stdout, response.data, response.size);
    if (ret)
        return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_FAILURE));
    return (FLAG_RESPONSE_PACKET(TYPE_T_RESPONSE_SUCCESS));
}