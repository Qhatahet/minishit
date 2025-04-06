CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
ARFLAGS = rcs
SRCS = ft_toupper.c ft_tolower.c ft_isalpha.c ft_memset.c ft_bzero.c ft_atoi.c ft_isdigit.c \
	ft_strchr.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strncmp.c ft_strlen.c ft_strrchr.c ft_memcpy.c ft_strnstr.c ft_strlcpy.c\
	ft_strlcat.c ft_strdup.c ft_memcmp.c ft_memchr.c ft_calloc.c ft_memmove.c\
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_substr.c\
	ft_strjoin.c ft_striteri.c ft_strmapi.c ft_itoa.c ft_strtrim.c ft_split.c\
	get_next_line/get_next_line_utils.c get_next_line/get_next_line.c\
	ft_printf/ft_printf.c ft_printf/ft_ptr.c ft_printf/ft_putchar.c ft_printf/ft_putnbr.c\
	ft_printf/ft_putstr.c ft_printf/ft_tohex.c ft_printf/ft_unsignednb.c\
	ft_strcmp.c

BSRS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c\
       ft_lstclear.c ft_lstiter.c ft_lstmap.c\

OBJS = $(SRCS:.c=.o)
B_OBJS = $(BSRS:.c=.o)
all: $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(NAME): $(OBJS)
	ar $(ARFLAGS) $@ $^
bonus: $(B_OBJS) $(NAME)
	ar $(ARFLAGS) $(NAME) $(B_OBJS)
clean:
	rm -f $(OBJS) $(B_OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all bonus clean fclean re
