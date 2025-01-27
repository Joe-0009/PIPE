NAME = pipex
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = pipex.c \
       pipex_helpers.c \
       pipex_utils.c \
       pipex_utils2.c \
       pipex_execution.c \
       ft_split.c


OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	
clean : 
	rm -rf $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJS)