NAME = push_swap
BNAME = checker
CC = cc 
CFLAGS = -Wall -Wextra -Werror
SRCS = 


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