NAME = libftprintf.a

OBJS = $(FILES:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror
FILES = ft_printf.c \
		flags_func.c \
		utils_func.c \
		output_func.c \
		toa_func.c \
		character_func.c \
		decimal_func.c \
		hexadecimal_func.c

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJS)
	ar rc $@ $^

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)

re : fclean all