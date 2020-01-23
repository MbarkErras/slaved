NAME = slaved

SRCS_DIR = srcs
OBJS_DIR = build
INCLUDES = includes libs/centropy/includes libs/cdata_structures/shared_includes
LIBS_DIR = libs

SRCS =	slaved.c \
		request_execution.c \
		t_request.c \
		t_response.c
OBJS = $(NAME_SRCS:.c=.o)

SRCS_PATHS = $(addprefix $(SRCS_DIR)/, $(SRCS))
OBJS_PATHS = $(addprefix $(OBJS_DIR)/, $(OBJS))

FLAGS = #-Wall -Wextra -Werror
INCLUDES_FLAGS = $(addprefix -I, $(INCLUDES))

all: $(NAME)

$(NAME): $(OBJS_PATHS)
	make -C $(LIBS_DIR)/centropy
	make -C $(LIBS_DIR)/cdata_structures
	gcc $(OBJS_PATHS) -o $(NAME)

$(OBJS_PATHS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(INCLUDES_FLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean:
	rm -f $(NAME)

re: fclean all