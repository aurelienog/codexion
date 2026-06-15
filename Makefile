NAME = codexion.a


CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread


SRC = putnbr_hex.c ft_putnbr_fd.c ft_putstr_fd.c ft_strchr.c ft_putchar_fd.c ft_printf.c ft_unsigned_putnbr_fd.c print_address.c
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)


fclean: clean
	rm -f $(NAME)


re: fclean all

.PHONY: all clean fclean re