NAME        = codexion

OBJ_DIR     = obj
INC_DIR     = includes

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3
INCLUDES    = -I. -Iutils

SRCS        = main.c \
              parsing.c \
			  utils.c \
			  coder.c \
			  queue.c \


OBJS        = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

DEPS        = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJ_DIR)

run: all
	@./$(NAME)  5 200 200 100 100 500 200 fifo

# number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re