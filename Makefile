NAME        = codexion

OBJ_DIR     = obj
INC_DIR     = includes

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g3
INCLUDES    = -I. -Iutils

SRCS        = main.c \
              parsing.c \
			  utils.c \

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
	@./$(NAME) 10 10 200 200 200 214 74 edf

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re