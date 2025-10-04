NAME = philo

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3

OBJ_DIR = ./executables

SRCS :=    helpers.c \
			init_data.c \
			log_time.c \
			main.c \
			overall_routine.c \
			routine.c \

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -lpthread -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re