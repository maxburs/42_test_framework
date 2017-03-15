NAME = framework.a

SRCS = framework.c

OBJS = $(SRCS:.c=.o)

LIBFT = libft/libft.a

HEADERS = -I . -I libft

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) ./libft/libft.a
	cp ./libft/libft.a $@
	ar -rc $@ $(OBJS)
	ranlib $@

$(LIBFT): libft/
	cd libft && $(MAKE)

%.o: %.c
	gcc $(CFLAGS) $(HEADERS) -c -o $@ $<

clean:
	rm -f $(OBJS)
	cd libft && $(MAKE) fclean
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: clean fclean re