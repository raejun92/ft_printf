NAME = libftprintf.a

OBJS = $(FILES:.c=.o)
B_OBJS = $(BONUS:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror
FILES = ft_printf.c

.PHONY : all clean fclean re

all : $(NAME)

$(NAME) : $(OBJS)
	ar rc $@ $^

clean :
	rm -f *.o

fclean : clean
	rm -f $(NAME)

re : fclean all