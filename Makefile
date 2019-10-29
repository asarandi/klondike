NAME = klondike
SRC = ht.c main.c moves.c print.c pq.c utils.c
OBJ = $(SRC:.c=.o)

CFLAGS += -g #-Wall -Werror -Wextra
LDFLAGS += -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
