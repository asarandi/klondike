#ifndef KLONDIKE_H
# define KLONDIKE_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_board {
    char desc[256];
    char waste[24 + 1];
    char stock[24 + 1];
    char f[4][13 + 1];
    char t[7][19 + 1];
} t_board;

int     stoi(char *s);
char    *itos(int i);
t_board *stob(char *s);
void    print(t_board *b);
int     num_moves(t_board *b);
t_board **make_moves(t_board *b);

#endif
