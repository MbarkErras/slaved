NAME = slaved

SRCS =	slaved.c \
		request_execution.c \
		t_request.c

OBJS = $(SRCS:.c=.o)

SRCS_DIR = srcs
OBJS_DIR = build
LIBS_DIR = libs
INCLUDES = includes $(LIBS_DIR)/centropy/includes $(LIBS_DIR)/cdata_structures/shared_includes

OBJS_PATH=$(addprefix $(OBJS_DIR)/, $(OBJS))
SRCS_PATH=$(addprefix $(SRCS_DIR)/, $(SRCS))

FLAGS = #-Wextra -Werror -Wall
INCLUDES_FLAGS = $(addprefix -I, $(INCLUDES))

all: $(NAME)

$(NAME): $(OBJS_PATH)
	make -C $(LIBS_DIR)/centropy
	make -C $(LIBS_DIR)/cdata_structures
	gcc $(OBJS_PATH) $(LIBS_DIR)/centropy/centropy.a \
		$(LIBS_DIR)/cdata_structures/cdstruct.a -o $(NAME)

$(OBJS_PATH): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(FLAGS) $(INCLUDES_FLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all