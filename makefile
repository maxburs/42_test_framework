NAME = framework.a

SRCS = framework.c

OBJS = $(SRCS:.c=.o)

LIBFTPRINTF = ft_printf/libftprintf.a

HEADERS = -I . -I ./ft_printf -I ./ft_printf/libft

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(LIBFTPRINTF)
	cp $(LIBFTPRINTF) $@
	ar -rc $@ $(OBJS)
	ranlib $@

$(LIBFTPRINTF): force
	cd ft_printf && $(MAKE)

force:
	@true

%.o: %.c
	gcc $(CFLAGS) $(HEADERS) -c -o $@ $<

clean:
	rm -f $(OBJS)
	cd libft && $(MAKE) fclean
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: clean fclean re all